#include <pthread.h>
#include <stdlib.h>
#include <stdint.h>

typedef enum { READERS, WRITERS, N_WAY } PRIORITY;

typedef struct rwlock {
    int active_reader;
    int active_writer;
    int waiting_readers;
    int waiting_writers;
    int n_way;
    int reader_pass;
    PRIORITY priority;
    pthread_mutex_t mutex;
    pthread_cond_t readers_ok;
    pthread_cond_t writers_ok;
} rwlock_t;

rwlock_t *rwlock_new(PRIORITY p, int n) {
    rwlock_t *rw = malloc(sizeof(rwlock_t));
    if (!rw)
        return NULL;

    rw->active_reader = 0;
    rw->active_writer = 0;
    rw->waiting_readers = 0;
    rw->waiting_writers = 0;
    rw->reader_pass = 1;
    rw->n_way = n;
    rw->priority = p;
    pthread_mutex_init(&rw->mutex, NULL);
    pthread_cond_init(&rw->readers_ok, NULL);
    pthread_cond_init(&rw->writers_ok, NULL);

    return rw;
}

void rwlock_delete(rwlock_t **rw) {
    if (rw && *rw) {
        pthread_mutex_destroy(&(*rw)->mutex);
        pthread_cond_destroy(&(*rw)->readers_ok);
        pthread_cond_destroy(&(*rw)->writers_ok);
        free(*rw);
        *rw = NULL;
    }
}

void reader_lock(rwlock_t *rw) {
    pthread_mutex_lock(&rw->mutex);
    rw->waiting_readers++;

    while ((rw->priority == WRITERS && rw->waiting_writers > 0) || rw->active_writer > 0
           || (rw->priority == N_WAY && rw->reader_pass > rw->n_way)) {
        pthread_cond_wait(&rw->readers_ok, &rw->mutex);
    }

    rw->waiting_readers--;
    rw->active_reader++;
    rw->reader_pass++;
    pthread_cond_signal(&rw->readers_ok);

    pthread_mutex_unlock(&rw->mutex);
}

void reader_unlock(rwlock_t *rw) {
    pthread_mutex_lock(&rw->mutex);
    rw->active_reader--;
    if (rw->active_reader == 0 && rw->waiting_writers > 0) {
        pthread_cond_signal(&rw->writers_ok);
    }
    pthread_mutex_unlock(&rw->mutex);
}

void writer_lock(rwlock_t *rw) {
    pthread_mutex_lock(&rw->mutex);
    rw->waiting_writers++;

    while ((rw->priority == READERS && rw->waiting_readers > 0) || rw->active_reader > 0
           || rw->active_writer > 0) {
        pthread_cond_wait(&rw->writers_ok, &rw->mutex);
    }

    rw->waiting_writers--;
    rw->active_writer++;
    pthread_mutex_unlock(&rw->mutex);
}

void writer_unlock(rwlock_t *rw) {
    pthread_mutex_lock(&rw->mutex);
    rw->active_writer--;

    if (rw->priority == WRITERS && rw->waiting_writers > 0) {
        pthread_cond_signal(&rw->writers_ok);
    } else if (rw->priority == N_WAY && rw->waiting_readers > 0) {
        rw->reader_pass = 1;
        for (int i = 0; i < rw->n_way && 0 < rw->waiting_readers; i++) {
            pthread_cond_signal(&rw->readers_ok);
        }
    } else if (rw->waiting_readers > 0) {
        pthread_cond_broadcast(&rw->readers_ok);
    } else {
        pthread_cond_signal(&rw->writers_ok);
    }

    pthread_mutex_unlock(&rw->mutex);
}

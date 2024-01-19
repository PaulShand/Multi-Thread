#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>

typedef struct queue {
    void **item;
    int size;
    int out;
    int in;
    int counter;
    pthread_mutex_t mute;
    pthread_cond_t empty, full;
} queue_t;

queue_t *queue_new(int size) {
    queue_t *q = malloc(sizeof(queue_t));
    q->size = size;
    q->out = 0;
    q->in = -1;
    q->counter = 0;
    q->item = malloc(sizeof(void *) * size);
    pthread_mutex_init(&q->mute, NULL);
    pthread_cond_init(&q->empty, NULL);
    pthread_cond_init(&q->full, NULL);
    return q;
}

void queue_delete(queue_t **q) {
    pthread_mutex_destroy(&(*q)->mute);
    pthread_cond_destroy(&(*q)->empty);
    pthread_cond_destroy(&(*q)->full);
    free((*q)->item);
    free(*q);
    *q = NULL;
}

bool queue_push(queue_t *q, void *item) {
    if (q == NULL)
        return false;
    pthread_mutex_lock(&q->mute);
    while (q->counter == q->size) {
        pthread_cond_wait(&q->empty, &q->mute);
    }
    q->in = (q->in + 1) % q->size;
    q->item[q->in] = item;
    q->counter += 1;
    pthread_cond_signal(&q->full);
    pthread_mutex_unlock(&q->mute);
    return true;
}

bool queue_pop(queue_t *q, void **item) {
    if (q == NULL)
        return false;
    pthread_mutex_lock(&q->mute);
    while (q->counter == 0) {
        pthread_cond_wait(&q->full, &q->mute);
    }
    *item = q->item[q->out];
    q->out = (q->out + 1) % q->size;
    q->counter--;
    pthread_cond_signal(&q->empty);
    pthread_mutex_unlock(&q->mute);
    return true;
}

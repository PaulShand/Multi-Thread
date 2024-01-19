CFLAGS=-Wall -Wextra -Werror -pedantic

all: queue.o rwlock.o

queue.o: queue.c
	clang $(CFLAGS) -c queue.c

rwlock.o: rwlock.c
	clang $(CFLAGS) -c rwlock.c

clean:
	rm -f *.o rwlock queue

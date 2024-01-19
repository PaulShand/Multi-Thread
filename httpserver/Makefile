CFLAGS=-Wall -Wextra -Werror -pedantic

all: httpserver

httpserver: httpserver.o
	clang $(CFLAGS) -o httpserver httpserver.c asgn4_helper_funcs.a


httpserver.o: httpserver.c
	clang $(CFLAGS) -c httpserver.c

clean:
	rm -f *.o httpserver

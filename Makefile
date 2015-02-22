CC=gcc
FLAGS=-std=c99
LIBS=-lpthread

all:
	${CC} ${FLAGS} handle_http.c server.c ${LIBS}

clean:
	rm a.out

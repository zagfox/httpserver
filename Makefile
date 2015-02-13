CC=gcc
FLAGS="-std=c99"

all:
	${CC} ${FLAGS} server.c
clean:
	rm a.out

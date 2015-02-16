CC=gcc
FLAGS="-std=c99"

all:
	${CC} ${FLAGS} server.c
dbg:
	${CC} ${FLAGS} -g server.c
clean:
	rm a.out

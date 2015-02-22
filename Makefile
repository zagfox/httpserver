CC=gcc
FLAGS=-std=c99
LIBS=-lpthread
INCLUDE=-I./

all: handle_http.o
	${CC} ${FLAGS} ${INCLUDE} handle_http.o server.c ${LIBS} -o server

handle_http.o: 
	${CC} ${FLAGS} ${INCLUDE} -c http/handle_http.c 

clean:
	rm *.o
	rm server

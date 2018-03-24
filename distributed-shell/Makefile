all: server

server.o: server.c server.h
	gcc -c server.c -o server.o

server: server.o
	gcc server.o -o server

clean: 
	rm server server.o 
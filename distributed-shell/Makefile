all: server client

server.o: server.c server.h shared.h
	gcc -c server.c -o server.o

server: server.o
	gcc server.o -lcrypt -o server

client.o: client.c client.h shared.h
	gcc -c client.c -o client.o

client: client.o
	gcc client.o -lcrypt -o client

clean: 
	rm server server.o client client.o

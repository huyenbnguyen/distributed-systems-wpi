all: rm

rm.o: rm.c args.h
	gcc -c rm.c -o rm.o

rm: rm.o
	 gcc rm.o -o rm

clean:
	-rm -f rm.o rm
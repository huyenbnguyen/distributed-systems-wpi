all: rm dump 

shared.o: shared.c shared.h
	gcc -c shared.c -o shared.o

shared: shared.o
	gcc shared.o -o shared

dump.o: dump.c shared.h
	gcc -c dump.c -o dump.o

dump: dump.o
	gcc dump.o shared.o -o dump

rm.o: rm.c shared.h
	gcc -c rm.c -o rm.o

rm: rm.o
	 gcc rm.o shared.o -o rm

clean:
	-rm -f rm.o rm dump.o dumpm
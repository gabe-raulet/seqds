all: main

main: main.o dseq.o
	gcc-11 -g -o main main.o dseq.o

main.o: main.c
	gcc-11 -g -c -o main.o main.c

dseq.o: dseq.c dseq.h
	gcc-11 -g -c -o dseq.o dseq.c

clean:
	rm -f *.o main

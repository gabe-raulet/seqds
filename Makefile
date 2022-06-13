all: main

main: main.o dseq.o seqstream.o
	gcc-11 -g -o main main.o dseq.o seqstream.o

main.o: main.c
	gcc-11 -g -c -o main.o main.c

dseq.o: dseq.c dseq.h
	gcc-11 -g -c -o dseq.o dseq.c

seqstream.o: seqstream.c seqstream.h
	gcc-11 -g -c -o seqstream.o seqstream.c

clean:
	rm -f *.o main

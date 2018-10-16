CC=clang
CFLAGS=-c -Wall -std=gnu11
LDFLAGS=-lrt -pthread -lcomedi -lm

all: main

run: main
	@(clear && ./ex5)

main: main.o io.o
	$(CC) $(LDFLAGS) main.o io.o -o ex5

main.o: main.c
	$(CC) $(CFLAGS) main.c

io.o: io.c
	$(CC) $(CFLAGS) io.c

clean:
	@(rm main.o io.o ex5)

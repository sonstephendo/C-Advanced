#declare the variable
CC=gcc
# -c = compilation -Wall = giving warning
CFLAGS=-c -Wall -g

HEADERS = jval.h

all: main

main: main.o sortsearch.o
	$(CC) -o main main.o sortsearch.o

main.o: main.c $(HEADERS)
	$(CC) $(CFLAGS) main.c

sortsearch.o: sortsearch.c $(HEADERS)
	$(CC) $(CFLAGS) sortsearch.c

clean:
	rm -rf *o main


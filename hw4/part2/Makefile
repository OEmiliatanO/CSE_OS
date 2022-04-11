CC:=gcc

all: main.o mm.o
	$(CC) -o main main.o mm.o
dep: main.c mm.c
	$(CC) -Wall -c main.c
	$(CC) -Wall -c mm.c
clean:
	-rm *.o

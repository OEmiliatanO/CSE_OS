CC:=gcc

all: main.o mm.o
	$(CC) -o main main.o mm.o
dep: main.c mm.c
	$(CC) -Wall -O3 -c main.c
	$(CC) -Wall -O3 -c mm.c
debug: main.c mm.c
	$(CC) -Wall -c main.c
	$(CC) -Wall -D_DEBUG_ -c mm.c
clean:
	-rm *.o

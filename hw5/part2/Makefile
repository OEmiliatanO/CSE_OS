CC:=g++

all: writer.o reader.o
	$(CC) -o writer.o
	$(CC) -o reader.o
dep: writer.c reader.c
	$(CC) -Wall -O3 -c writer.c
	$(CC) -Wall -O3 -c reader.c
clean:
	-rm *.o

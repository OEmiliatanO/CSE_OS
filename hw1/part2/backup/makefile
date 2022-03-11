CC:=g++

all: Pack.o parser.o
	$(CC) -o Pack Pack.o parser.o
dep: Pack.cpp parser.cpp
	$(CC) -Wall -c Pack.cpp
	$(CC) -Wall -c parser.cpp
clean:
	rm *.o

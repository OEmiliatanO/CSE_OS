CC:=g++

all: myshell.o parser.o
	$(CC) -o myshell myshell.o parser.o
dep: myshell.cpp parser.cpp
	$(CC) -Wall -c myshell.cpp
	$(CC) -Wall -c parser.cpp
clean:
	rm *.o

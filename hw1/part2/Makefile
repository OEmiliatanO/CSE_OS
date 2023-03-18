CC:=g++

all: Pack.o parser.o
	$(CC) -o Pack.elf Pack.o parser.o
dep: Pack.cpp parser.cpp
	$(CC) -Wextra -Werror -Wall -O3 -c Pack.cpp
	$(CC) -Wextra -Werror -Wall -O3 -c parser.cpp
clean:
	-rm *.o

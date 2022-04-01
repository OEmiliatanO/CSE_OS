CC:=g++

all: main.o philo.o monitor.o
	$(CC) -o main main.o philo.o monitor.o -lpthread
dep: main.cpp philo.cpp monitor.cpp
	$(CC) -Wall -c main.cpp
	$(CC) -Wall -c philo.cpp
	$(CC) -Wall -c monitor.cpp
clean:
	-rm *.o

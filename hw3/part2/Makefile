CC:=g++

all: main.o philo.o monitor.o semaphore.o
	$(CC) -o main main.o philo.o monitor.o semaphore.o -lpthread
dep: main.cpp philo.cpp monitor.cpp semaphore.cpp
	$(CC) -Wall -c main.cpp
	$(CC) -Wall -c philo.cpp
	$(CC) -Wall -c monitor.cpp
	$(CC) -Wall -c semaphore.cpp
clean:
	-rm *.o

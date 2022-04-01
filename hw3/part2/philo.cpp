#include "philo.h"
#include <iostream>
#include <unistd.h>
#include <chrono>
#include <thread>

void philo::think()
{
	printf("%d is thinking...\n", this->id);
	std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 100));
	//usleep(rand() % 100000);
}

void philo::eat()
{
	printf("%d is eating...\n", this->id);
	std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 100));
	//usleep(rand() % 100000);
}

void philo::take_chops()
{
	//printf("%d try to take chops\n", this->id);
	this->master->lock(this->id);
	this->master->lock((this->id + 1) % this->master->getn());
	//printf("%d takes chops\n", this->id);
}

void philo::put_chops()
{
	//printf("%d try to put chops\n", this->id);
	this->master->unlock(this->id);
	this->master->unlock((this->id + 1) % this->master->getn());
	//printf("%d put chops\n", this->id);
}

int philo::getid()
{
	return this->id;
}

/*
void philo::work()
{
	//printf("%d works\n", this->id);
	while(true)
	{
		think();
		//printf("%d after thinking\n", this->id);
		take_chops();
		eat();
		put_chops();
	}
}
*/

#include "philo.h"
#include <iostream>
#include <unistd.h>
#include <chrono>
#include <thread>

void philo::think()
{
	printf("%d is thinking...\n", this->id);
	std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 100));
}

void philo::eat()
{
	printf("%d is eating...\n", this->id);
	std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 100));
}

void philo::take_chops()
{
	this->master->lock(this->id);
	this->master->lock((this->id + 1) % this->master->getn());
}

void philo::put_chops()
{
	this->master->unlock(this->id);
	this->master->unlock((this->id + 1) % this->master->getn());
}

int philo::getid()
{
	return this->id;
}

void philo::work()
{
	while(true)
	{
		think();
		take_chops();
		eat();
		put_chops();
	}
}	

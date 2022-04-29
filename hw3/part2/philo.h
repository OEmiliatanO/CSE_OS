#ifndef __PHILO_H__
#define __PHILO_H__
#include "monitor.h"

class philo
{
	int id;
	monitor* master;
	
	void think();
	void eat();

	void take_chops();
	void put_chops();

public:
	philo() = default;
	philo(int id, monitor* m) : id(id), master(m) {}

	void work();
	int getid();
};

#endif

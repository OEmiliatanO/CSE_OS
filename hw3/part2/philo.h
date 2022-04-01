#ifndef __PHILO_H__
#define __PHILO_H__
#include <iostream>
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

	void work()
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
	int getid();
};

#endif

#ifndef __MONITOR_H__
#define __MONITOR_H__
#include <iostream>
#include <vector>
#include <pthread.h>
#include <unistd.h>
#include "semaphore.h"

class monitor
{
	int MAXN;
	std::vector<semaphore_t*> arr;

public:
	monitor(int n) : MAXN(n) { for (int i = 0; i < MAXN; ++i) arr.push_back(new semaphore_t(1)); }
	~monitor() { for (int i = 0; i < MAXN; ++i) delete arr[i]; }

	void lock(int i);
	void unlock(int i);
	int getn();
};


#endif

#ifndef __MONITOR_H__
#define __MONITOR_H__
#include <iostream>
#include <vector>
#include <mutex>

class monitor
{
	int MAXN;
	std::vector<std::mutex*> arr;

public:
	monitor(int n) : MAXN(n) { for (int i = 0; i < MAXN; ++i) arr.push_back(new std::mutex()); }
	~monitor() { for (int i = 0; i < MAXN; ++i) delete arr[i]; }

	void lock(int i);
	void unlock(int i);
	int getn();
};


#endif

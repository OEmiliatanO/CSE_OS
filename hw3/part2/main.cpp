#include <iostream>
#include <vector>
#include <thread>
#include "monitor.h"
#include "philo.h"

constexpr int MAXN = 5;

int main()
{
	monitor m(MAXN);
	std::vector<philo> arr;
	std::vector<std::thread> threadPool;

	for (int i = 0; i < MAXN; ++i)
		arr.emplace_back(i, &m);

	for (auto& it : arr)
		threadPool.emplace_back(&philo::work, std::ref(it));

	for (auto& it : threadPool)
		it.join();
	
	return 0;
}

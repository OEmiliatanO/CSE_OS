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
	for (int i = 0; i < MAXN; ++i)
		arr.push_back(philo(i, &m));
	for (auto& it : arr)
	{
		std::thread t(&philo::work, std::ref(it));
		t.detach();
	}

	while(true);
	
	return 0;
}

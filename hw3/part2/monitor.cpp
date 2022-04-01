#include "monitor.h"

void monitor::lock(int i)
{
	this->arr[i]->lock();
}

void monitor::unlock(int i)
{
	this->arr[i]->unlock();
}

int monitor::getn()
{
	return this->MAXN;
}

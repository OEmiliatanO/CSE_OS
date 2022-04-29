#include "monitor.h"

void monitor::lock(int i)
{
	this->arr[i]->down();
}

void monitor::unlock(int i)
{
	this->arr[i]->up();
}

int monitor::getn()
{
	return this->MAXN;
}

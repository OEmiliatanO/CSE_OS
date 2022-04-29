#ifndef __SEMAPHORE_H__
#define __SEMAPHORE_H__
#include <unistd.h>
#include <pthread.h>

class semaphore_t
{
	pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
	pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
	int val;
public:
	semaphore_t(int v): val(v) {}
	int getvalue();
	void up();
	void down();
};

#endif

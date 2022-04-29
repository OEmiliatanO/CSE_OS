#include "semaphore.h"

void semaphore_t::up()
{
	pthread_mutex_lock(&this->mutex);
	++val;
	pthread_cond_signal(&this->cond);
	pthread_mutex_unlock(&this->mutex);
}

void semaphore_t::down()
{
	pthread_mutex_lock(&this->mutex);
	while (this->val == 0)
	{
		pthread_cond_wait(&this->cond, &this->mutex);
	}
	--this->val;
	pthread_mutex_unlock(&this->mutex);
}

int semaphore_t::getvalue()
{
	return this->val;
}

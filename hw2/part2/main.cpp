#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <cstdlib>

constexpr int MAXN = 5;

enum STATE { THINK, HUNGRY, EAT };

struct semaphore
{
	pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
	pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
	int val = 0;
};
void down(semaphore &s)
{
	pthread_mutex_lock(&s.mutex);
	while (s.val == 0)
	{
		pthread_cond_wait(&s.cond, &s.mutex);
	}
	--s.val;
	pthread_mutex_unlock(&s.mutex);
}
void up(semaphore &s)
{
	pthread_mutex_lock(&s.mutex);
	++s.val;
	pthread_cond_signal(&s.cond);
	pthread_mutex_unlock(&s.mutex);
}

int state[MAXN]{};
semaphore s[MAXN]{};
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void think(int i)
{
	printf("%d thinking...\n", i);
	sleep(1); //sleep 1 sec
}

void eat(int i)
{
	printf("%d eating...\n", i);
	sleep(3); // eat 3 sec
}

#define LEFT(i)  (((i) - 1) % MAXN + MAXN) % MAXN
#define RIGHT(i) (((i) + 1) % MAXN)
void test(int i)
{
	if (state[i] == STATE::HUNGRY && state[LEFT(i)] != STATE::EAT && state[RIGHT(i)] != STATE::EAT)
	{
		state[i] = STATE::EAT;
		up(s[i]);
	}
}

void take_chops(int i)
{
	pthread_mutex_lock(&mutex);
	state[i] = STATE::HUNGRY;
	test(i);
	pthread_mutex_unlock(&mutex);
	down(s[i]);
}

void put_chops(int i)
{
	pthread_mutex_lock(&mutex);
	state[i] = STATE::THINK;
	test(LEFT(i));
	test(RIGHT(i));
	pthread_mutex_unlock(&mutex);
}

void* philosopher(void* data)
{
	int i = *((int *)data);
	while(true)
	{
		think(i);
		take_chops(i);
		eat(i);
		put_chops(i);
	}
	pthread_exit(NULL);
}

int main()
{
	pthread_t phors[MAXN];
	int* arg[MAXN];
	for (int i = 0; i < MAXN; ++i)
	{
		arg[i] = (int *)malloc(sizeof(int));
		*arg[i] = i;
		pthread_create(phors + i, NULL, philosopher, (void *)arg[i]);
	}

	for (int i = 0; i < MAXN; ++i)
	{
		pthread_join(phors[i], NULL);
		free(arg[i]);
	}

	return 0;
}

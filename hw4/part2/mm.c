#include "mm.h"
#include <unistd.h>

struct header
{
	size_t size = 0;
	node *nex = nullptr;
}*now = nullptr;


size_t align(size_t require)
{
	return (require + (unit - 1)) & ~(unit - 1);
}

void *mymalloc(size_t raw_size)
{
	size_t size = align(sizeof(struct header) + raw_size);

	if (now == nullptr)
	{
		now = (header *)sbrk(0);
		sbrk(size);
		now->size = size;
		now->nex = now;
		return (void *)(now + 1);
	}
	
	header *p = now;
	for (; p->nex != now; p = p->nex)
	{
		if (p->size - sizeof(struct header) >= size)
			return (void *)(p + 1);
	}
	p = (header *)sbrk(0);
	sbrk(size);
	return (void *)(p + 1);
}

void myfree(void *ptr)
{
	return;
}

void *myrealloc(void *ptr, size_t size)
{
	return NULL;
}

void *mycalloc(size_t nmemb, size_t size)
{
	return NULL;
}


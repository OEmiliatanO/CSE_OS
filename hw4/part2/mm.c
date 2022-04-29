#include "mm.h"
#include <unistd.h>
#include <stdio.h>

#define MEM_t void *
#define perr(...) fprintf(stderr, __VA_ARGS__)
#define SIZE(h) h->head.size
#define NEXT(h) h->head.nex

#define nullptr NULL
#define constexpr const

typedef union header
{
	struct major
	{
		// member "size" represents the size of header plus the actual free space.
		size_t size;
		union header *nex;
	}head;

	// for alignment
	long _;
}header;

constexpr size_t unit = sizeof(header);

header base, *free_list;

size_t align(size_t require)
{
	return (require + (unit - 1)) & ~(unit - 1);
}

void printinfo(void *adr)
{
	header *p = (header *)adr - 1;
	perr("! position: %p\n", p);
	perr("! this size: %ld\n", SIZE(p));
	perr("! -> %p\n", NEXT(p));
}

void printFree_list()
{
	perr("====free_list====\n");
	perr("! free_list points to %p\n", free_list);
	header *p = free_list;
	do
	{
		perr("!! position: %p\n", p);
		perr("!! this size: %ld\n", SIZE(p));
		perr("!! -> %p\n", NEXT(p));
		p = NEXT(p);
	}while(p && p != free_list);
	perr("=================\n");
}

void init()
{
	perr("! unit: %ld\n", unit);
	perr("! base: %p\n", &base);
	perr("! sizeof(size_t): %ld\n", sizeof(size_t));
}

MEM_t mymalloc(size_t raw_size)
{
	perr("! call mymalloc(%ld)\n\n", raw_size);
	header *pre, *mem;
	size_t size = align(unit + raw_size);
	perr("! size really need: %ld\n", size);

	if (free_list == nullptr)
	{
		perr("!! first call of myalloc(%ld).\n", raw_size);
		free_list = &base;
		base.head.nex = &base;
		base.head.size = 0;
	}
	
	perr("! myalloc(%ld)\n", raw_size);
	printFree_list();
	
	perr("! for loop to find the fit node.\n");
	for (pre = free_list, mem = NEXT(free_list); ; pre = mem, mem = NEXT(mem))
	{
		perr("!! for loop:\n");
		perr("!! pre = %p\n", pre);
		perr("!! -> %p\n", NEXT(pre));
		perr("!! mem = %p\n", mem);
		perr("!! -> %p\n", NEXT(mem));
		if (SIZE(mem) >= size)
		{
			perr("!!! inside block: if (SIZE(mem) >= size)\n");
			perr("!!! find node in %p, whose size is %ld\n", mem, SIZE(mem));
			if (SIZE(mem) - size < (unit << 1))
			{
				perr("!!!! size of mem(%ld) - requested mem(%ld) < 2*unit(%ld)\n", SIZE(mem), size, unit << 1);
				free_list = pre;
				NEXT(pre) = NEXT(mem);
			}
			else
			{
				perr("!!!! size of mem(%ld) - requested mem(%ld) >= 2*unit(%ld)\n", SIZE(mem), size, unit << 1);
				free_list = pre;
				header *memoffset = mem + size / unit;
				NEXT(pre) = memoffset;
				SIZE(memoffset) = SIZE(mem) - size;
				NEXT(memoffset) = NEXT(mem);
				SIZE(mem) = size;
			}
			perr("!!! mem: %p\n", mem);
			perr("!!! return %p\n", mem + 1);
			perr("\n\n");
			return (MEM_t)(mem + 1);
		}

		if (mem == free_list)
		{
			perr("!!! inside block: if (mem == free_list)\n");
			mem = sbrk(0);
			sbrk(size);
			SIZE(mem) = size;
			perr("!!! mem: %p\n", mem);
			perr("!!! return %p\n", mem + 1);
			perr("\n\n");
			return (MEM_t)(mem + 1);
		}
	}

	perr("! error occurs in myalloc(%ld)\n", raw_size);
	perr("\n\n");
	//printFree_list();

	return nullptr;
}

void myfree(void *blocks)
{
	perr("! call myfree(%p)\n\n", blocks);

	header *blkhead = (header *)blocks - 1;
	header *pos;

	perr("! the block to release: %p\n", blkhead);

	for (pos = &base; ; pos = NEXT(pos))
	{
		if ((MEM_t)pos < (MEM_t)blkhead && (MEM_t)blkhead < (MEM_t)NEXT(pos))
			break;
		if ((MEM_t)pos >= (MEM_t)NEXT(pos) && ((MEM_t)blkhead > (MEM_t)pos || (MEM_t)blkhead < (MEM_t)NEXT(pos)))
			break;
	}

	perr("! blkhead: %p\n", blkhead);
	perr("! blkhead.size: %ld\n", SIZE(blkhead));
	perr("! blkhead + blkhead.size: %p\n", blkhead + SIZE(blkhead)/ unit);

	perr("! pos: %p\n", pos);
	perr("! pos.size: %ld\n", SIZE(pos));
	perr("! pos + pos.size: %p\n", pos + SIZE(pos)/ unit);
	perr("! -> %p\n", NEXT(pos));
	

	perr("! pos.next: %p\n", NEXT(pos));
	perr("! pos.nex.size: %ld\n", SIZE(NEXT(pos)));
	perr("! pos.nex + pos.nex.size: %p\n", NEXT(pos) + SIZE(NEXT(pos))/ unit);
	perr("! -> %p\n", NEXT(NEXT(pos)));

	if (blkhead + SIZE(blkhead) / unit == NEXT(pos))
	{
		perr("!! right of block can be merged.\n");
		SIZE(blkhead) += SIZE(NEXT(pos));
		NEXT(blkhead) = NEXT(NEXT(pos));
		perr("!! after merged:\n");
	}
	else
	{
		perr("!! right of block can't be merged.\n");
		NEXT(blkhead) = NEXT(pos);
	}

	perr("! blkhead: %p\n", blkhead);
	perr("! blkhead.size: %ld\n", SIZE(blkhead));
	perr("! -> %p\n", NEXT(blkhead));
	
	if (pos + SIZE(pos) / unit == blkhead)
	{
		perr("!! left of block can be merged.\n");
		SIZE(pos) += SIZE(blkhead);
		NEXT(pos) = NEXT(blkhead);
		perr("!! after merged:\n");
	}
	else 
	{
		perr("!! left of block can't be merged.\n");
		NEXT(pos) = blkhead;
	}

	perr("!! pos: %p\n", pos);
	perr("!! pos.size: %ld\n", SIZE(pos));
	perr("!! -> %p\n", NEXT(pos));

	perr("\n\n");
	
	free_list = pos;
	printFree_list();
}

void *myrealloc(void *ptr, size_t size)
{
	return NULL;
}

void *mycalloc(size_t nmemb, size_t size)
{
	return NULL;
}


#include "mm.h"
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#ifdef _DEBUG_
#define perr(...) fprintf(stderr, __VA_ARGS__)
#else
#define perr(...) 
#endif

// memory space type
#define MEM_t void *
#define PRIV static

// for use c++ keyword and function
#define nullptr NULL
#define constexpr const
#define min(a, b) ((a)>(b) ? (b):(a))

// block header, the number of bytes occupied by header shall be fixed.
// use union for alignment, 
// e.g. if sizeof(size_t) + sizeof(header *) < 8, then sizeof(header) is still 8.
typedef union header
{
	// the header
	struct major
	{
		// .size: the size of header plus the actual free space.
		size_t size;
		// .nex : the next free node.
		union header *nex;
	}head;

// for convenient usage
#define SIZE(h) h->head.size
#define NEXT(h) h->head.nex

	// for alignment
	long _;
}header;

// unit: the smallest unit for allocating a space
PRIV constexpr size_t unit = sizeof(header);

// base: the head of free space list.
PRIV header base;

// *free_list: 
// the pointer of free space list,
// it always points to a node containing free space.
// it shall initialized as nullptr.
PRIV header *free_list = nullptr;


// make required space aligned with 16*n, namely "unit"*n
// e.g. 3->16, 15->16, 17->32, 33->48

// WARNING: this function only work when "unit" is the power of 2
PRIV size_t align(size_t require)
{
	return (require + (unit - 1)) & ~(unit - 1);
}

// debug, print the header info
PRIV void printinfo(header *h)
{
	perr("! position begin: %p\n", h);
	perr("! end: %p\n", h + SIZE(h) / unit);
	perr("! this size: %ld\n", SIZE(h));
	perr("! -> %p\n", NEXT(h));
}

// debug, print the whole free space list
PRIV void printFree_list()
{
	perr("====free_list====\n");
	perr("! free_list points to %p\n", free_list);
	header *p = free_list;
	do
	{
		printinfo(p);
		p = NEXT(p);
	}while(p && p != free_list);
	perr("=================\n");
}

// debug, print the environment variables
void MMenv()
{
	perr("! unit: %ld\n", unit);
	perr("! base: %p\n", &base);
	perr("! sizeof(size_t): %ld\n", sizeof(size_t));
}

// mymalloc(size):
// return a void pointer, mem,
// points to a free space containing "size" bytes.

// the backward access of mem is illegal,
// may cause segment fault.
MEM_t mymalloc(size_t raw_size)
{
	if (raw_size <= 0) return nullptr;
	
	perr("! call mymalloc(%ld)\n\n", raw_size);
	
	// iterators for free space list
	header *pre, *mem;

	// size: includes the header size and user required size
	size_t size = align(unit + raw_size);

	perr("! size really need: %ld\n", size);
	
	// first use of mymalloc
	if (free_list == nullptr)
	{
		perr("!! first call of myalloc(%ld).\n\n", raw_size);
		// initialize
		free_list = &base;
		base.head.nex = &base;
		base.head.size = 0;
	}
	
	printFree_list();
	
	perr("\n! for loop to find the fit node.\n\n");

	// iterate the whole free space list,
	// to find a proper space for allocate.
	for (pre = free_list, mem = NEXT(free_list); ; pre = mem, mem = NEXT(mem))
	{
		perr("!! for loop:\n");
		perr("!! pre:\n");
		printinfo(pre);
		perr("!! mem:\n");
		printinfo(mem);

		// found a proper space,
		// need to determine that whether this space can be split or not.
		// if after split, the size of free space remains >= 2*unit, split it.
		// if not, just allocate the whole free space.
		if (SIZE(mem) >= size)
		{
			perr("\n!!! inside block: if (SIZE(mem) >= size)\n");
			
			// space remains < 2*unit
			if (SIZE(mem) - size < (unit << 1))
			{
				perr("\n!!!! size of mem(%ld) - requested mem(%ld) < 2*unit(%ld)\n", SIZE(mem), size, unit << 1);

				// remove mem from free space list.
				NEXT(pre) = NEXT(mem);
			}
			// space remains >= 2*unit
			else
			{
				perr("\n!!!! size of mem(%ld) - requested mem(%ld) >= 2*unit(%ld)\n", SIZE(mem), size, unit << 1);
				
				// the beginning of right side of mem
				header *memoffset = mem + size / unit;

				// remove the left side of mem from free space list.
				NEXT(pre) = memoffset;
				SIZE(memoffset) = SIZE(mem) - size;
				NEXT(memoffset) = NEXT(mem);
				SIZE(mem) = size;
			}

			// points to a free node
			free_list = pre;
			
			perr("!!! pre:\n");
			printinfo(pre);
			perr("!!! mem:\n");
			printinfo(mem);
			perr("!!! return %p\n", mem + 1);
			perr("\n\n");
			
			return (MEM_t)(mem + 1);
		}

		// this means the free space list contains no proper space,
		// require more space from OS.
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

	// this section will never be executed,
	// except an error occurs.
	perr("! error occurs in myalloc(%ld)\n", raw_size);
	perr("\n\n");

	return nullptr;
}


// myfree(blocks):
// will release the space of "blocks".
// make this space back to the free space list,
// then can be available for later allocation.

// also, the access of freed pointer is illegal,
// may cause segment fault.
void myfree(void *blocks)
{
	if (blocks == nullptr) return;

	perr("! call myfree(%p)\n\n", blocks);

	header *blkhead = (header *)blocks - 1;
	header *pos;

	perr("! the block to release: %p\n", blkhead);

	// find the location of this space.
	// it may between two nodes in free space list
	// or after the end node of free space list
	for (pos = &base; ; pos = NEXT(pos))
	{
		// between two nodes ?
		if ((MEM_t)pos < (MEM_t)blkhead && (MEM_t)blkhead < (MEM_t)NEXT(pos))
			break;
		// at right end ?
		if ((MEM_t)pos >= (MEM_t)NEXT(pos) && ((MEM_t)blkhead > (MEM_t)pos || (MEM_t)blkhead < (MEM_t)NEXT(pos)))
			break;
	}
	
	perr("! blkhead:\n");
	printinfo(blkhead);

	perr("! pos:\n");
	printinfo(pos);
	
	perr("! pos.next:\n");
	printinfo(NEXT(pos));

	// the right of blkhead is adjacent to a free node,
	// therefore, it can be merged.
	perr("\n");
	if (blkhead + SIZE(blkhead) / unit == NEXT(pos))
	{
		perr("!! right of block can be merged.\n");
		SIZE(blkhead) += SIZE(NEXT(pos));
		NEXT(blkhead) = NEXT(NEXT(pos));
		perr("!! after merged:\n");
	}
	// otherwise, cannot be merged
	else
	{
		perr("!! right of block can't be merged.\n");
		NEXT(blkhead) = NEXT(pos);
	}
	perr("\n");

	perr("! blkhead\n");
	printinfo(blkhead);
	
	perr("\n");
	// the left of blkhead is adjacent to a free node,
	// therefore, it can be merged.
	if (pos + SIZE(pos) / unit == blkhead)
	{
		perr("!! left of block can be merged.\n");
		SIZE(pos) += SIZE(blkhead);
		NEXT(pos) = NEXT(blkhead);
		perr("!! after merged:\n");
	}
	// otherwise, cannot be merged
	else 
	{
		perr("!! left of block can't be merged.\n");
		NEXT(pos) = blkhead;
	}
	perr("\n");

	perr("! pos:\n");
	printinfo(pos);

	perr("\n\n");
	
	free_list = pos;
	printFree_list();
	perr("\n");
}

// myrealloc(address, size):
// make the space contained by address adjust to size.
// the data will remain, but may be truncated if the adjusted size is smaller.
MEM_t myrealloc(void *ptr, size_t raw_size)
{
	perr("! call myrealloc(%p, %ld)\n", ptr, raw_size);
	if (ptr == nullptr) return mymalloc(raw_size);

	size_t size = SIZE(((header *)ptr - 1));

	// the space is adequate
	if (size >= raw_size)
	{
		perr("!! the space have been adequate.\n\n");
		return ptr;
	}

	myfree(ptr);
	void *res = mymalloc(raw_size);
	memcpy(res, ptr, min(raw_size, size));
	
	perr("! myrealloc is done\n\n");
	return res;
}

// mycalloc(n, size):
// is like mymalloc(n*size), then initialize the space as zero.
void *mycalloc(size_t nmemb, size_t size)
{
	void *res = mymalloc(nmemb * size);
	memset(res, 0, nmemb * size);
	return res;
}


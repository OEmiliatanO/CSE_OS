#ifndef __MY_MM_H_INCLUDED__
#define __MY_MM_H_INCLUDED__

#include <stddef.h>

void *mymalloc(size_t raw_size);
void myfree(void *ptr);
void *myrealloc(void *ptr, size_t raw_size);
void *mycalloc(size_t nmemb, size_t raw_size);

#endif

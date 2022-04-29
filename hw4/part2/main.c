#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "mm.h"
#define perr(...) fprintf(stderr, __VA_ARGS__)
#define bool char

extern void init();

int main()
{
	init();
	const size_t clen = 15, ilen = 20, flen = 15, dlen = 30, blen = 100;

	perr("allocate char * %ld\n", clen);
	char *carr = (char *)mymalloc(sizeof(char) * clen);
	perr("allocate int * %ld\n", ilen);
	int *iarr = (int *)mymalloc(sizeof(int) * ilen);
	perr("allocate float * %ld\n", flen);
	float *farr = (float *)mymalloc(sizeof(float) * flen);

	memset(carr, 0, sizeof(char) * clen);
	memcpy(carr, "hello world", sizeof(char) * 11);
	printf("%s\n", carr);

	for (int i = 0; i < ilen; ++i)
		iarr[i] = i;

	for (int i = 0; i < flen; ++i)
		farr[i] = (float)i;

	perr("free char * %ld\n", clen);
	myfree(carr);
	perr("free int * %ld\n", ilen);
	myfree(iarr);
	perr("free float * %ld\n", flen);
	myfree(farr);
	
	perr("heap top = %p\n\n", sbrk(0));

	perr("allocate double * %ld\n", dlen);
	double *darr = (double *)mymalloc(sizeof(double) * dlen);
	perr("allocate bool * %ld\n", blen);
	bool *barr = (bool *)mymalloc(sizeof(bool) * blen);

	perr("free double * %ld\n", dlen);
	myfree(darr);
	perr("free bool * %ld\n", blen);
	myfree(barr);

	return 0;
}

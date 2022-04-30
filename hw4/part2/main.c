#include <stdio.h>
#include <string.h>
#include "mm.h"

#define bool char
#define perr(...) fprintf(stderr, __VA_ARGS__);

int main()
{
	const size_t clen = 15, ilen = 20, flen = 15, dlen = 25, blen = 100;

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

	perr("allocate double * %ld\n", dlen);
	double *darr = (double *)mymalloc(sizeof(double) * dlen);
	for (int i = 0; i < dlen; ++i) darr[i] = (double)i;

	perr("allocate bool * %ld\n", blen);
	bool *barr = (bool *)mymalloc(sizeof(bool) * blen);

	perr("free bool * %ld\n", blen);
	myfree(barr);

	perr("realloc double * %ld\n", dlen + 1);
	darr = (double *)myrealloc(darr, sizeof(double) * (dlen + 1));
	for (int i = 0; i < dlen; ++i) perr("%.02lf ", darr[i]);
	perr("\n");

	perr("realloc double * %ld\n", dlen + 15);
	darr = (double *)myrealloc(darr, sizeof(double) * (dlen + 15));
	for (int i = 0; i < dlen; ++i) perr("%.02lf ", darr[i]);
	perr("\n");

	/*
	perr("realloc double * %ld\n", dlen - 5);
	darr = (double *)myrealloc(darr, sizeof(double) * (dlen - 5));
	for (int i = 0; i < dlen - 5; ++i) perr("%.02lf ", darr[i]);
	perr("\n");
	*/

	perr("free double * %ld\n", dlen + 15);
	myfree(darr);

	return 0;
}

#ifndef __PARSER_H__
#define __PARSER_H__

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cctype>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>

int cntpipe(char *s, int len);

int split(char *src, char sp, char* des);

char** Cmdpar(char *raw_command, int& num);

#endif

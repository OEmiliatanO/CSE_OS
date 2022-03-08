#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cctype>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include "parser.h"

int cntpipe(char *s, int len)
{
	int res = 0;
	for (int i = 0; i < len; ++i)
		res += (s[i] == '|');
	return res;
}

int split(char *src, char sp, char* des)
{
	char format[100] = "%[^ ]s";
	format[3] = sp;
	return sscanf(src, format, des);
}

int strip(char *src, char *des)
{
	while(*src && isspace(*src)) ++src;
	return sscanf(src, "%[^\0]s", des);
}

char** Cmdpar(char *raw_command, int& num)
{
	int lenOfrcmd = strlen(raw_command);
	num = cntpipe(raw_command, lenOfrcmd) + 1;
	char** cmds = (char**)malloc(sizeof(char*) * num);

	char buf[50];
	int idxOfrcmd = 0, idxOfcmds = 0;

	while(~sscanf(raw_command + idxOfrcmd, "%[^|]s", buf))
	{
		int bufl = strlen(buf);
		idxOfrcmd += bufl;
		while(isspace(raw_command[idxOfrcmd]) || raw_command[idxOfrcmd] == '|') ++idxOfrcmd;
		cmds[idxOfcmds] = (char*)calloc(bufl + 1, sizeof(char));
		memcpy(cmds[idxOfcmds], buf, sizeof(char) * bufl);
		cmds[idxOfcmds][bufl] = 0;

		++idxOfcmds;
	}
	// ls -l | cat | sort | grep d
	// after parser:
	// [0]:ls -l
	// [1]:cat
	// [2]:sort
	// [3]:grep d
	// num = 4
	return cmds;
}

/*
int main()
{
	char *s = (char*)malloc(sizeof(char) * 1000);
	while(~scanf("%[^\n]s", s) && getchar())
	{
		int num;
		Cmdpar(s, num);
		printf("%d\n", num);
	}
	free(s);
	return 0;
}
*/

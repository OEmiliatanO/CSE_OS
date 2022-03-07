#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cctype>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>

int cntpipe(char *s, int len)
{
	int res = 0;
	for (int i = 0; i < len; ++i)
		res += (s[i] == '|');
	return res;
}

bool split(char *src, int& i, int n, char sp, char* des)
{
	int p = 0;
	for (; i < strlen(src); des[p++] = src[i], ++i)
		if(src[i] == sp) break;
	++i;
	des[p++] = 0;
	return i < n;
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
	return cmds;
}


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


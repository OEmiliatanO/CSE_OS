#include <iostream>
#include <algorithm>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <pwd.h>
#include <fcntl.h>
#include "parser.h"

char* raw_command;

int cmdcnt;
char** cmds;

int preprocess(char** cmds)
{
	if ( == 0)
	{
	}
}

void execute(char** cmds, int n)
{
	int status;
	char* nowait = strchr(cmds[n - 1], '&');
	pid_t pid;
	if ((pid = fork()))
	{
		if (nowait != nullptr)
		{
			printf("[%d]\n", pid);
			return;
		}
		waitpid(pid, &status, WUNTRACED | WCONTINUED);
		return;
	}

	if(nowait != nullptr) *nowait = ' ';

	int exeidx;
	int fd[2];
	for (exeidx = n - 1; exeidx; --exeidx)
	{
		//printf("exeidx = %d\n", exeidx);
		pipe(fd);
		pid = fork();
		if (pid > 0)
		{
			close(fd[1]);
			dup2(fd[0], STDIN_FILENO);
			close(fd[0]);
			break;
		}
		else if (pid == 0)
		{	
			close(fd[0]);
			dup2(fd[1], STDOUT_FILENO);
			close(fd[1]);
		}
		else
		{
		}
	}

	// take out the redirection argument
	char buf[50]{};
	char *cmd = cmds[exeidx];
	char ***argv = (char***)malloc(sizeof(char**) * 2);
	argv[0] = (char**)malloc(sizeof(char*) * 100);
	argv[1] = (char**)malloc(sizeof(char*) * 100);

	int p = 0, argc = 0, redic = 0;
	//printf("===============\n");
	//printf("to exec: %s\n", cmd);

	while(~split(cmd + p, ' ', buf))
	{
		int bufl = strlen(buf);
		p += bufl;
		while(isspace(cmd[p])) ++p;
		bool flag = (strchr(buf, '>') || strchr(buf, '<'));

		int& idx = (flag ? redic : argc);
		
		argv[flag][idx] = (char*)malloc(sizeof(char) * (bufl + 1));
		memcpy(argv[flag][idx], buf, sizeof(char) * bufl);
		argv[flag][idx][bufl] = 0;
		++idx;
	}
	argv[0][argc] = 0;
	argv[1][redic] = 0;

	// redirection
	// notice, the redirection of my shell must have no space between ">" and "dest", so as "<" and "src".
	char *src = nullptr, *dest = nullptr;
	for (int i = 0; i < redic; ++i)
	{
		char* ch = strchr(argv[1][i], '>');
		int pos;
		if (ch)
		{
			pos = ch - argv[1][i];
			while(isspace(argv[1][i][pos]) || argv[1][i][pos] == '>') ++pos;
			dest = (char*)malloc((strlen(argv[1][i]) - pos + 1) * sizeof(char));
			memcpy(dest, argv[1][i] + pos, strlen(argv[1][i]) - pos);
			dest[strlen(argv[1][i]) - pos] = 0;
		}
		else
		{
			pos = strchr(argv[1][i], '<') - argv[1][i];
			while(isspace(argv[1][i][pos]) || argv[1][i][pos] == '<') ++pos;
			src = (char*)malloc((strlen(argv[1][i]) - pos + 1) * sizeof(char));
			memcpy(src, argv[1][i] + pos, strlen(argv[1][i]) - pos);
			src[strlen(argv[1][i]) - pos] = 0;
		}
	}
	if (src != nullptr)
	{
		//printf("src = %s\n", src);
		FILE *in = (src != nullptr ? fopen(src, "r") : stdin);
		dup2(fileno(in), STDIN_FILENO);
	}
	if (dest != nullptr)
	{
		//printf("dest = %s\n", dest);
		FILE *out = (dest != nullptr ? fopen(dest, "w") : stdout);
		dup2(fileno(out), STDOUT_FILENO);
	}
	/*
	printf("argv section:\n");
	for (int i = 0; i < argc; ++i)
	{
		printf("argv[%d]=%s\n", i, argv[0][i]);
	}
	printf("\nredirection section:\n");
	for (int i = 0; i < redic; ++i)
	{
		printf("redirection[%d]=%s\n", i, argv[1][i]);
	}
	printf("==============\n");
	*/
	
	/*
	for (int i = 0; i < argc; ++i)
	{
		printf("argv[%d]:%s\n", i, argv[0][i]);
	}
	*/

	if (execvp(argv[0][0], argv[0]) < 0)
		puts("command not found");

	// release memory
	for (int i = 0; i < argc; ++i)	free(argv[0][i]);
	for (int i = 0; i < redic; ++i) free(argv[1][i]);
	free(argv[0]); free(argv[1]);
	free(argv);
	free(src); free(dest);
	argv = nullptr;

	exit(0);
}

int main()
{
	/*
	char PATHinterpreter[1000]{};
	getcwd(PATHinterpreter, sizeof(PATHinterpreter));
	strcat(PATHinterpreter, "/interpreter");
	printf("interpreter path = %s\n", PATHinterpreter);
	*/

	raw_command = (char*)malloc(sizeof(char) * 1000);
	while(true)
	{
		fprintf(stdout, "$ ");
		if (scanf("%1000[^\n]s", raw_command) == -1) break;
		if (strcmp(raw_command, "exit") == 0 || strcmp(raw_command, "quit") == 0) break;
		getchar();

		cmds = Cmdpar(raw_command, cmdcnt);
		execute(cmds, cmdcnt);

		// release the space of cmds
		for (int i = 0; i < cmdcnt; ++i)
			free(cmds[i]);
		free(cmds);
	}
	free(raw_command);
	return 0;
}

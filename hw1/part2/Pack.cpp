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

// deal with "cd", "ll", "la",... and the path such that "~/"
void preprocess(char* rcmd){

}

// fork the child based on the pipe order (tail to head),
// this function also deals with the redirection.
// notice, this function will cause the unreversed changing on cmds[i] string.
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
			fprintf(stderr, "pipeline error\n");
			exit(0);
		}
	}

	char buf[50]{};
	char *cmd = cmds[exeidx];
	
	//printf("===============\n");
	//printf("to exec: %s\n", cmd);
	
	// deal with the redirection first
	char *src  = (char*)malloc(sizeof(char) * 100);
	char *dest = (char*)malloc(sizeof(char) * 100);

	// the redirection part of the command will be cover with ' '
	// e.g. "cat filein > fileout" -> "cat filein         "
	if (getRedirFile(cmd, '<', src) <= 0 ) { free(src);  src  = nullptr; }
	if (getRedirFile(cmd, '>', dest) <= 0) { free(dest); dest = nullptr; }

	//printf("after erase the redirection section: %s", cmd);

	// deal with the argument
	char **argv = (char**)calloc(100, sizeof(char*));
	int p = 0, argc = 0;
	while(isspace(cmd[p])) ++p;
	while(cmd[p] && ~split(cmd + p, ' ', buf))
	{
		int bufl = strlen(buf);
		p += bufl;
		while(isspace(cmd[p])) ++p;
		
		argv[argc] = (char*)malloc(sizeof(char) * (bufl + 1));
		memcpy(argv[argc], buf, sizeof(char) * bufl);
		argv[argc][bufl] = 0;
		++argc;
	}
	argv[argc] = 0;

	// redirection
	if (src != nullptr)
	{
		//printf("src = %s\n", src);
		FILE *in = fopen(src, "r");
		dup2(fileno(in), STDIN_FILENO);
	}
	if (dest != nullptr)
	{
		//printf("dest = %s\n", dest);
		FILE *out = fopen(dest, "w");
		dup2(fileno(out), STDOUT_FILENO);
	}
	/*
	printf("argv section:\n");
	for (int i = 0; i < argc; ++i)
	{
		printf("argv[%d]=%s\n", i, argv[0][i]);
	}
	printf("==============\n");
	*/

	if (execvp(argv[0], argv) < 0)
		puts("myshell: Command not found");

	// release memory
	for (int i = 0; i < argc; ++i)	free(argv[i]);
	free(argv);
	free(src); free(dest);
	argv = nullptr;

	exit(0);
}

int main()
{
	raw_command = (char*)malloc(sizeof(char) * 1000);
	while(true)
	{
		fprintf(stdout, "$ ");
		if (scanf("%1000[^\n]s", raw_command) == -1) break;
		if (strcmp(raw_command, "exit") == 0 || strcmp(raw_command, "quit") == 0) break;
		getchar();

		cmds = Cmdpar(raw_command, cmdcnt);
		
		pid_t pid = fork();
		int status;
		if (pid > 0)
			waitpid(pid, &status, 0);
		else
		{
			execute(cmds, cmdcnt);
			exit(0);
		}

		// release the space of cmds
		for (int i = 0; i < cmdcnt; ++i)
			free(cmds[i]);
		free(cmds);
	}
	free(raw_command);
	return 0;
}

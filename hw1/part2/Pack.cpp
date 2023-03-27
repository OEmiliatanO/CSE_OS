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

// GNU readline library
#include <readline/readline.h>
#include <readline/history.h>

int cmdcnt;
char** cmds;

constexpr int MAXN_CMD = 1000;
constexpr size_t MAXN_ARG = 100;

// deal with "cd", "ll", "la".
int preprocess(char* rcmd)
{
	if (rcmd == nullptr) return 1;

	char *p = rcmd;
	char *newcmd = (char*)malloc(sizeof(char) * MAXN_CMD);
	int n = 0;
	while(p && *p && isspace(*p)) ++p;
	
	// simple change work dir
	if (p[0] == 'c' && p[1] == 'd' && p[2] == ' ')
	{
		int i = 2;
		while(isspace(p[i])) ++i;
		if (getcwd(newcmd, sizeof(char) * MAXN_CMD) == nullptr) { fprintf(stderr, "Cannot get current working directory.\n"); }
		int tmp = strlen(newcmd);
		newcmd[tmp] = '/';
		newcmd[tmp + 1] = 0;
		strcat(newcmd, p + i);
		//printf("to dir : %s\n", newcmd);
		if (chdir(newcmd) == -1) { fprintf(stderr, "Cannot get change directory.\n"); }
		return 1;
	}
	
	// ll, la, lA
	p = rcmd;
	while(*p && isspace(*p)) ++p;

	for (size_t i = 0; i < strlen(p); ++i)
		if (p[i] == 'l' && (p[i + 1] == 'l' || p[i + 1] == 'a' || p[i + 1] == 'A') && (p[i + 2] == ' ' || p[i + 2] == 0))
		{
			char s[] = {'l', 's', ' ', '-', p[i + 1]};
			for (int j = 0; j < 5; ++j)
				newcmd[n++] = s[j];
			++i;
		}
		else newcmd[n++] = rcmd[i];

	//printf("newcmd : %s\n", newcmd);
	newcmd[n] = 0;
	memcpy(rcmd, newcmd, sizeof(char) * (n + 1));

	return 0;
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
		if (pipe(fd) < 0) { fprintf(stderr, "pipeline error\n"); }
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
    constexpr size_t MAXN_FNAME = 100;
	char *src  = (char*)malloc(sizeof(char) * MAXN_FNAME);
	char *dest = (char*)malloc(sizeof(char) * MAXN_FNAME);

	// the redirection part of the command will be cover with ' '
	// e.g. "cat filein > fileout" -> "cat filein         "
	int isappend = 0;
	//fprintf(stderr, "erase...\n");
	if (getRedirFile(cmd, '<', src, &isappend) <= 0 ) { free(src);  src  = nullptr; }
	//fprintf(stderr, "after erase the redirection section: %s\n", cmd);
	if (getRedirFile(cmd, '>', dest, &isappend) <= 0) { free(dest); dest = nullptr; }

	//fprintf(stderr, "after erase the redirection section: %s\n", cmd);

	// deal with the argument
	char **argv = (char**)calloc(MAXN_ARG, sizeof(char*));
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
		FILE *out = NULL;
		if (isappend)
			out = fopen(dest, "a");
		else
			out = fopen(dest, "w");
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
		puts("Pack: Command not found");

	// release memory
	for (int i = 0; i < argc; ++i)	free(argv[i]);
	free(argv);
	free(src); free(dest);
	argv = nullptr;

	exit(0);
}

char* prompt(char * const prompt_s, char * const cwd, char const * const prompt_sign)
{
    sprintf(prompt_s, "%s %s ", cwd, prompt_sign);
    char * inp = readline(prompt_s);
    if (strcmp(inp, "") == 0) return inp;
    add_history(inp);
    return inp;
}

int main()
{
    char * const prompt_s = (char *)malloc(sizeof(char) * MAXN_CMD);
    char * const cwd = (char*)malloc(sizeof(char) * MAXN_CMD);
    char prompt_word[] = "$>";
	while(true)
	{
        if (getcwd(cwd, sizeof(char) * MAXN_CMD) == nullptr) { fprintf(stderr, "Cannot get current working directory.\n"); continue; }
        char * raw_command = prompt(prompt_s, cwd, prompt_word);
		if (raw_command == nullptr || strcmp(raw_command, "exit") == 0 || strcmp(raw_command, "quit") == 0) break;
        if (strcmp(raw_command, "") == 0) continue;

		if (preprocess(raw_command)) continue;
		//printf("after proess : %s\n", raw_command);
		cmds = cmdpar(raw_command, cmdcnt);
		
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
        free(raw_command);
	}
	putchar('\n');
	return 0;
}

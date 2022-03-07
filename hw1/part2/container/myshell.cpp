#include <iostream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <pwd.h>
#include <fcntl.h>

char** raw_command;

int predecide(char* rcommand)
{
	int i = 0;
	while(isspace(rcommand[i])) ++i;
	char* command = rcommand + i;
	if (command[0] == 'c' && command[1] == 'd')
	{
		char path[1000]{};
		command += 2;
		while(isspace(*command)) ++command;

		if (*command != '~' && *command != '/')
		{
			getcwd(path, sizeof(path));
			path[strlen(path)] = '/';
			path[strlen(path) + 1] = 0;
			printf("now path = %s\n", path);
		}
		else if (*command == '~')
		{
			struct passwd *pw = getpwuid(getuid());

			strcpy(path, pw->pw_dir);
			int l = strlen(path);
			path[l] = '/';
			path[l + 1] = 0;
			++command;
		}
		
		strcat(path, command);

		printf("going to %s\n", path);

		printf("path = %s\n", path);
		chdir(path);
		
		getcwd(path, sizeof(path));
		printf("now path = %s\n", path);
		return -1;
	}
	return 0;
}

int main()
{
	char PATHinterpreter[1000]{};
	getcwd(PATHinterpreter, sizeof(PATHinterpreter));
	strcat(PATHinterpreter, "/interpreter");
	printf("interpreter path = %s\n", PATHinterpreter);

	int fd = open("error_msg", O_WRONLY);
	dup2(fd, STDERR_FILENO);
	
	raw_command = (char**)malloc(sizeof(char*));
	*raw_command = (char*)malloc(sizeof(char) * 1000);

	while(true)
	{
		fprintf(stdout, "$ ");
		if (scanf("%1000[^\n]s", raw_command[0]) == -1) break;
		if (strcmp(raw_command[0], "exit") == 0) break;
		getchar();
		if (predecide(raw_command[0]) == -1) continue;
	
		//fprintf(stderr, "what i get: %s\n", raw_command[0]);
		if (fork() == 0)
		{
			execve(PATHinterpreter, raw_command, nullptr);
		}
		else
		{
			int status;
			if (waitpid(-1, &status, 0) == -1)
			{
				fprintf(stderr, "error occurs when execute %s\n", raw_command[0]);
				fprintf(stdout, "error occurs when execute %s\n", raw_command[0]);
			}
		}
	}

	free(*raw_command);
	free(raw_command);
	return 0;
}

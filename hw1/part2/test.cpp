#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/wait.h>

char raw_command[1000];

char* cat(char *s1, char *s2)
{
	int len1 = strlen(s1), len2 = strlen(s2);
	int sumlen = len1 + len2;
	char* res = (char *)malloc(sizeof(char) * (sumlen + 1));
	memcpy(res, s1, sizeof(char) * len1);
	memcpy(res + len1, s2, sizeof(char) * len2);
	res[sumlen] = 0;
	return res;
}

int main()
{
	while(true)
	{
		char* argv[1000]{};
		int argc = 0;
		char s[1000]{};
		int p = 0;

		if(scanf("%1000[^\n]s", raw_command) == -1) break;
		getchar();
		
		while(~sscanf(raw_command + p, "%s", s))
		{
			int slen = strlen(s);
			p += slen;
			while(raw_command[p] == ' ') ++p;
			//fprintf(stderr, "token: %s\n", s);
			argv[argc] = (char*)malloc(sizeof(char) * (slen + 1));
			//fprintf(stderr, "address contained by argv[argc] = %p\n", argv[argc]);
			memcpy(argv[argc], s, sizeof(char) * slen);
			argv[argc][slen] = 0;
			++argc;
		}
		
		/*
		printf("\nstore in argv[]:\n");
		for (int i = 0; i < argc; ++i)
		{
			printf("%s\n", argv[i]);
		}
		*/		

		if (fork() == 0)
		{
			char PATH[] = "/bin/";
			char* path = cat(PATH, argv[0]);
			fprintf(stderr, "path = %s\n", path);
			fprintf(stderr, "\nstore in argv[]:\n");
			for (int i = 0; i < argc; ++i)
			{
				fprintf(stderr, "%s\n", argv[i]);
			}
			
			execve(path, argv, nullptr);
		}
		else
		{
			int status;
			if (waitpid(-1, &status, 0) == -1)
			{
				fprintf(stderr, "error\n");
			}
		}

		for (int i = 0; i < argc; ++i)
		{
			free(argv[i]);
			argv[i] = nullptr;
		}
	}
	return 0;
}

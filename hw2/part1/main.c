#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <fcntl.h>

int value = 1;

static void *runner(void *param);

int main(int argc, char **argv)
{
	//int fd = open("testfile", O_WRONLY, 0666);
	//close(STDOUT_FILENO);
	//dup(fd);

	pid_t pid = fork();

	if (pid > 0)
	{
		printf("A = %d\n", value);
	}
	else if (pid == 0)
	{
		pid_t pid = fork();
		if (pid > 0)
		{
			printf("B = %d\n", value);
		}
		else if (pid == 0)
		{
			pid_t pid = fork();

			/*
			pthread_t tid;
			pthread_attr_t attr;
			pthread_attr_init(&attr);
			pthread_create(&tid, &attr, runner, NULL);
			pthread_join(tid, NULL);
			*/
			if (pid > 0)
			{	
				pthread_t tid;
				pthread_attr_t attr;
				pthread_attr_init(&attr);
				pthread_create(&tid, &attr, runner, NULL);
				pthread_join(tid, NULL);

				printf("C = %d\n", value);
			}
			else
			{
				/*
				pthread_t tid;
				pthread_attr_t attr;
				pthread_attr_init(&attr);
				pthread_create(&tid, &attr, runner, NULL);
				pthread_join(tid, NULL);
				*/

				printf("D = %d\n", value);
			}
		}
		else
		{
			exit(1);
		}
	}
	else
	{
		exit(1);
	}

	//close(fd);

	return 0;
}

static void *runner(void *param)
{
	value += 1;
	pthread_exit(0);
}

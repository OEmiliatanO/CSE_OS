#include <iostream>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>

#define perr(...) fprintf(stderr, __VA_ARGS__)

using namespace std;

void sig_handler_CONT(int SIG);
void sig_handler_QUIT(int SIG);

constexpr size_t maxlen = 4096 << 2;

char content[maxlen];
int fd;
void *p;

int main()
{
	if (signal(SIGCONT, sig_handler_CONT) == SIG_ERR || signal(SIGQUIT, sig_handler_QUIT) == SIG_ERR)
	{
		perr("signal error.\n");
		return 1;
	}

	int pid = fork();
	if(pid > 0)
	{
		fd = open("test", O_RDWR | O_CREAT, 0777);
		if (fd < 0)
		{
			perr("can't open this file.\n");
			return 1;
		}

		p = mmap(NULL, getpagesize(), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
		if (p == (void *)-1)
		{
			perr("can't map this file.\n");
			return 1;
		}
		
		char *ptr = (char *)p;
		size_t cap = 0;
		
		printf("writer : write something:\n");
		while (~scanf("%[^\n]s", content))
		{
			getchar();
			size_t len = strlen(content);
			content[len++] = '\n';

			cap += len;
			if (cap > maxlen)
			{
				perr("this file is full.\n");
				break;
			}

			while(ftruncate(fd, cap) == -1)
			{
				perr("can't truncate this file.\n");
			}
			memcpy(ptr, content, len);
			ptr += len;

			kill(pid, SIGCONT);
		}

		printf("writer : finish writing.\n");
		
		munmap(p, getpagesize());
		close(fd);

		kill(pid, SIGQUIT);

		int status;
		waitpid(pid, &status, 0);
		return 0;
	}
	else if(pid == 0)
	{
		fd = open("test", O_RDONLY);
		if (fd < 0)
		{
			perr("can't open this file.\n");
			return 1;
		}

		p = mmap(NULL, getpagesize(), PROT_READ, MAP_SHARED, fd, 0);
		if (p == (void *)-1)
		{
			perr("can't map this file.\n");
			return 1;
		}

		close(fd);
		while(true) pause();
	}
	else
	{
		perr("fork fail.\n");
		return 1;
	}
}

void sig_handler_CONT(int SIG)
{
	printf("\nreader : the content has been change.\n");
	//printf("page size: %ld\n", getpagesize());
	memcpy(content, p, getpagesize());
	printf("reader : recive content:\n");
	printf("%s\n", content);
}
void sig_handler_QUIT(int SIG)
{
	printf("reader : unmap.\n");
	munmap(p, getpagesize());
	exit(0);
}

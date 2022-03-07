#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>


int main()
{
	char ls[] = "ls";
	char* arg[] = {ls, 0};
	execvp(arg[0], arg);
	return 0;
}

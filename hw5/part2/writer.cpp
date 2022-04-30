#include <iostream>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>

using namespace std;

int main()
{
	constexpr size_t maxlen = sizeof(char) * 1000;
	char content[maxlen] = "";

	int fd = open("test", O_RDWR | O_CREAT, 0777);
	if (fd < 0) return 1;

	void *p = mmap(NULL, getpagesize(), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (p == (void *)-1) return 1;
	
	char *ptr = (char *)p;
	size_t pos = 0;
	
	while (~scanf("%[^\n]s", content))
	{
		getchar();
		size_t len = strlen(content);
		content[len++] = '\n';

		pos += len;
		if (pos > maxlen)
		{
			printf("the file is full.\n");
			break;
		}
		ftruncate(fd, pos);
		memcpy(ptr, content, len);
		ptr += len;
	}
	
	munmap(p, getpagesize());
	close(fd);

	return 0;
}

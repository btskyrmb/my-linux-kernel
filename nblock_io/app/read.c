#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>

#define LEN 512

//./a.out hi0
int main(int argc, char **argv)
{
	int fd, ret;
	char buf[LEN];

	if (argc != 2) {
		fprintf(stderr, "Usage: %s devfile\n", argv[0]);
		exit(1);
	}	

	fd = open(argv[1], O_RDONLY | O_NDELAY);
	assert(fd > 0);

	ret = read(fd, buf, LEN);
	assert(ret > 0);
	ret = write(1, buf, ret);
	assert(ret > 0);

	putchar('\n');

	return 0;
}

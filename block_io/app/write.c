#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>

#define LEN 512

//./a.out hi0 file
int main(int argc, char **argv)
{
	int fddev, fd, ret;
	char buf[LEN];

	if (argc != 3) {
		fprintf(stderr, "Usage: %s devfile file\n", argv[0]);
		exit(1);
	}	

	fddev = open(argv[1], O_WRONLY);
	assert(fddev > 0);

	fd = open(argv[2], O_RDONLY | O_NDELAY);
	assert(fd > 0);

	ret = read(fd, buf, LEN);
	assert(ret > 0);

	ret = write(fddev, buf, ret);
	assert(ret > 0);

	putchar('\n');

	return 0;
}

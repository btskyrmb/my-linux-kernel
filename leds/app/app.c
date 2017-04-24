#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <assert.h>

#include "ioctl.h"

void led_usage(const char *str)
{
	fprintf(stderr, "Usage:\n");
	fprintf(stderr, "      %s on/off 1~4\n",str);
	fprintf(stderr, "      %s state/onall/offall\n", str);
	exit(1);
}

int main(int argc, char **argv)
{
	int fd;

	if (argc != 3 && argc != 2) {
		led_usage(argv[0]);	
	}	

	fd = open ("leds", O_RDWR | O_NDELAY);
	assert(fd > 0);

	/*you finish below after ...*/

	return 0;	
}

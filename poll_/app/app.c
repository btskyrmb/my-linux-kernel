#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>

#define CNT 8
#define SZ  8
#define LEN 512

void epoll_read(int);

int main(int argc, char **argv)
{
	
	int fd;
	int epfd, ret;
	int i;
	char buf[SZ];
	struct epoll_event event = {
		.events = EPOLLIN,
	};
	struct epoll_event events[CNT];

	epfd = epoll_create(CNT);
	assert(epfd > 0);

	for (i = 0; i < CNT; i++) {
		snprintf(buf, SZ, "kbuf%d", i);
		fd = open(buf, O_RDONLY);
		assert(fd > 0);

		event.data.fd = fd;
		epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &event);	
	}

	for (;;) {
		ret = epoll_wait(epfd, events, CNT, 5000);
		if (ret == -1) {
			perror("epoll_wait");
			exit(1);
		} else if (ret == 0) {
			printf("wait for read here...\n");
			continue;
		} 

		for (i = 0; i < ret; i++) {
			if (events[i].events&EPOLLIN) {
				epoll_read(events[i].data.fd);
			}
		}
	}

	return 0;
}

void epoll_read(int fd)
{
	int ret;
	char buf[LEN];

	if((ret = read(fd, buf, LEN)) > 0) {
		ret = write(1, buf, ret);
		assert(ret > 0);	
	}
}

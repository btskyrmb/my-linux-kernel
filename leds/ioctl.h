#ifndef MILLET_LED_IOCTL_H_
#define MILLET_LED_IOCTL_H_

#include <linux/ioctl.h>

#define LEDTYPE 'l'

#define LEDON		_IOW(LEDTYPE, 0, int)		
#define LEDOFF		_IOW(LEDTYPE, 1, int)
#define LEDSTAT		_IOWR(LEDTYPE, 2, int)

#define LEDONALL	_IO(LEDTYPE, 3)
#define LEDOFFALL	_IO(LEDTYPE, 4)

#endif


obj-m	:= demo.o getkerneltime.o gettimeofday.o timer.o

KERNEL	:= /lib/modules/`uname -r`/build/

all:
	make -C $(KERNEL) M=$(shell pwd) modules;
clean:
	make -C $(KERNEL) M=$(shell pwd) clean 
	


obj-m	:= demo2.o

KERNEL	:= /linux-3.5/

all:
	make -C $(KERNEL) M=$(shell pwd) modules;
clean:
	make -C $(KERNEL) M=$(shell pwd) clean 
	

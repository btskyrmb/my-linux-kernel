
obj-m	:= demo.o ddemo.o dddemo.o dmaalloc.o

KERNEL	:= /linux-3.5/

all:
	make -C $(KERNEL) M=$(shell pwd) modules;
clean:
	make -C $(KERNEL) M=$(shell pwd) clean 
	

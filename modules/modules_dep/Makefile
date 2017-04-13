
obj-m	:= newmod.o
obj-m	+= mode.o
newmod-objs := demo.o fool.o

KERNEL	:= /lib/modules/`uname -r`/build/

all:
	make -C $(KERNEL) M=$(shell pwd) modules;
install:
	make -C $(KERNEL) M=$(shell pwd) modules_install
clean:
	make -C $(KERNEL) M=$(shell pwd) clean 
	

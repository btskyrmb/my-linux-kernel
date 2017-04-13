
obj-m	:= newmod.o
obj-m	+= mode.o
newmod-objs := demo.o fool.o

KERNEL	:= /linux-3.5/

all:
	make -C $(KERNEL) M=$(shell pwd) modules;
install:
	make -C $(KERNEL) M=$(shell pwd) INSTALL_MOD_PATH=/embfs modules_install
clean:
	make -C $(KERNEL) M=$(shell pwd) clean 
	

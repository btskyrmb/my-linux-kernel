
obj-m	:= demoarray.o

#KERNEL	:= /linux-3.5/
KERNEL	:= /lib/modules/`uname -r`/build/

all:
	make -C $(KERNEL) M=$(shell pwd) modules;
#	mv *.ko /embfs/
clean:
	make -C $(KERNEL) M=$(shell pwd) clean 
	

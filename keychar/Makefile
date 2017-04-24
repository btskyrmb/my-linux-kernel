
obj-m	:= demo.o demo2.o

KERNEL	:= /linux-3.5/

all:
	make -C $(KERNEL) M=$(shell pwd) modules;
	mv *.ko /embfs/;make clean
clean:
	make -C $(KERNEL) M=$(shell pwd) clean 
	

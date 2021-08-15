obj-m:= module_ioctl2.o

KDIR=/lib/modules/$(shell uname -r)/build #for native compilation
PWD:=$(shell pwd)

default:
		make -C $(KDIR) M=$(PWD) modules
		gcc UserApp.c -o UserApp  #Also compile User Space application
clean:
		make -C $(KDIR) M=$(PWD) clean
		rm UserApp


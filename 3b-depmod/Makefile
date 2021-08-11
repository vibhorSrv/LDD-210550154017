obj-m := average.o add.o 

KDIR=/lib/modules/$(shell uname -r)/build #for native
# KDIR= /home/vibhor/rpi/linux #for cross compile


PWD=$(shell pwd)

default:
			$(MAKE) -C $(KDIR) M=$(PWD) modules
install:
			$(MAKE) -C $(KDIR) M=$(PWD) modules_install 
			depmod -a
clean:
			$(MAKE) -C $(KDIR) M=$(PWD) clean
			rm -rf /lib/modules/$(shell uname -r)/build/extra/*

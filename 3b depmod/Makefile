obj-m := average.o add.o 

KDIR=/lib/modules/$(shell uname -r)/build
# KDIR=/home/vibhor/rpi/linux
PWD=$(shell pwd)
default:
	make -C $(KDIR) M=$(PWD) modules
clean:
	make -C $(KDIR) M=$(PWD) clean
install:
	make -C $(KDIR) M=$(PWD) modules_install 
	depmod -a
obj-m := add.o average.o

KDIR=/lib/modules/$(shell uname -r)/build #for native
# KDIR= /home/vibhor/rpi/linux #for cross compile

default:
	$(MAKE) -C $(KDIR) M=$(PWD) modules
clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
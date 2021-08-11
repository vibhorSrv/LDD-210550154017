obj-m := module_param_add_sub.o

KDIR=/lib/modules/$(shell uname -r)/build #for native
# KDIR= /home/vibhor/rpi/linux #for cross compile

PWD=$(shell pwd)

default:
	$(MAKE) -C $(KDIR) M=$(PWD) modules
clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
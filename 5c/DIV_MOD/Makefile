obj-m:= div_module.o

KDIR=/lib/modules/$(shell uname -r)/build #for native compilation
PWD:=$(shell pwd)

default:
		make -C $(KDIR) M=$(PWD) modules
clean:
		make -C $(KDIR) M=$(PWD) clean
install:
		make -C $(KDIR) M=$(PWD) modules_install


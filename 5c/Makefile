obj-y:= ADD_MOD/ DIV_MOD/ MUL_MOD/ SUB_MOD/

KDIR=/lib/modules/$(shell uname -r)/build #for native compilation
PWD:=$(shell pwd)

default:
		make -C $(KDIR) M=$(PWD) modules
		gcc UserApp.c -o UserApp  #Also compile User Space application
clean:
		make -C $(KDIR) M=$(PWD) clean
		rm UserApp
install:
		make -C $(KDIR) M=$(PWD) modules_install
		depmod -a
obj-m := module_param.o

KDIR= /home/vibhor/rpi/linux
PWD=$(shell pwd)

all:
	make -C $(KDIR) M=$(PWD) modules
clean:
	make -C $(KDIR) M=$(PWD) clean

obj-m := hello.o

KDIR=/home/vibhor/rpi/linux
PWD=$(shell pwd)

all:
	make -C $(KDIR) M=$(PWD) modules
clean:
	make -C $(KDIR) M=$(PWD) clean



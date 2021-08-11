obj-m := add.o average.o

KDIR=/home/vibhor/rpi/linux/
all:
	make -C $(KDIR) M=$(PWD) modules
clean:
	make -C $(KDIR) M=$(PWD) clean
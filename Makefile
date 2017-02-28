obj-m += hello-rc.o
# obj-m += hello-packet.o
obj-m += hello-rot13.o

all:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) clean

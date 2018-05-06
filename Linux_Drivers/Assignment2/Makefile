obj-m := kernel_ioport.o

all:
	make -C /lib/modules/4.13.0-16-generic/build M=$(PWD) clean
	clear
	make -C /lib/modules/4.13.0-16-generic/build M=$(PWD) modules
clean:
	make -C /lib/modules/4.13.0-16-generic/build M=$(PWD) clean
	rm -f ioport_exe a.out
	clear
compile:
	gcc user_ioport.c -o ioport_exe
run:
	sudo ./ioport_exe

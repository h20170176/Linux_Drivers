obj-m := kernel_adc.o

all:
	make -C /lib/modules/4.13.0-32-generic/build M=$(PWD) clean
	clear
	make -C /lib/modules/4.13.0-32-generic/build M=$(PWD) modules
clean:
	make -C /lib/modules/4.13.0-32-generic/build M=$(PWD) clean
	rm -f char_exe a.out
	clear
compile:
	gcc user_adc.c -o char_exe
run:
	sudo ./char_exe

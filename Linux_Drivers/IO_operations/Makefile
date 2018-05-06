obj-m := pit_timer.o

all:
	make -C /lib/modules/4.13.0-16-generic/build M=$(PWD) clean
	clear
	make -C /lib/modules/4.13.0-16-generic/build M=$(PWD) modules
clean:
	make -C /lib/modules/4.13.0-16-generic/build M=$(PWD) clean
	rm -f a.out
	clear

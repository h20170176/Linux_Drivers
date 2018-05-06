obj-m := exp_adv.o

all:
	make -C /lib/modules/4.13.0-39-generic/build M=$(PWD) modules
clean:
	make -C /lib/modules/4.13.0-39-generic/build M=$(PWD) clean

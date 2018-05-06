obj-m := module_param.o

all:
	make -C /lib/modules/4.13.0-31-generic/build M=$(PWD) modules
clean:
	make -C /lib/modules/4.13.0-31-generic/build M=$(PWD) clean


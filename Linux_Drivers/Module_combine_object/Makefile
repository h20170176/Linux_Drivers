obj-m := module1_export.o
obj-m += module2_export.o

obj-m :=module12_export.o

module12_export-objs := module1_export.o module2_export.o

all:
	make -C /lib/modules/4.8.0-36-generic/build M=$(PWD) modules
clean:
	make -C /lib/modules/4.8.0-36-generic/build M=$(PWD) clean

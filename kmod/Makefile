MODULENAME=numdevs

obj-m += $(MODULENAME).o

module:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD)

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

CURRENTPATH:=$(shell pwd)
KERNELPATH:=/home/ww/souce/linux-orangepi

obj-m = gpio_tree.o

bulid:modules

modules:
	$(MAKE) -C $(KERNELPATH) M=$(CURRENTPATH) modules
clean:
	$(MAKE) -C $(KERNELPATH) M=$(CURRENTPATH) clean
	
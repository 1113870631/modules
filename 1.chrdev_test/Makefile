KERNELPATH := /home/ww/souce/linux-orangepi
CURRENTPATH := $(shell pwd)

obj-m = chrdevbase.o

build: modules

modules:
	$(MAKE) -C  $(KERNELPATH)  M=$(CURRENTPATH) modules
clean:
	$(MAKE) -C  $(KERNELPATH)  M=$(CURRENTPATH) clean
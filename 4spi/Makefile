CURRENT := $(shell pwd)
KERNEL :=/home/ww/souce/linux-orangepi

obj-m = spi.o

build:modules

modules:
	$(MAKE) -C $(KERNEL) M=$(CURRENT) modules
clean:
	$(MAKE) -C $(KERNEL) M=$(CURRENT) clean

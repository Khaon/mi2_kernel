KERNEL_DIR=/home/khaon/kernels/kernel_samsung_manta/frandom

obj-m := frandom.o
PWD := $(shell pwd)
default:
	$(MAKE) ARCH=arm CROSS_COMPILE=$(CROSS_COMPILE) -C $(PWD) SUBDIRS=$(PWD) modules
clean:
	$(MAKE) -C $(PWD) SUBDIRS=$(PWD) clean

# cross-compile module makefile

DRIVERNAME=udelay

ifneq ($(KERNELRELEASE),)
    obj-m := udelay.o
else
    PWD := $(shell pwd)

default:
ifeq ($(strip $(KERNELDIR)),)
	$(error "KERNELDIR is undefined!")
else
	$(MAKE) -C $(KERNELDIR) scripts
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules 
endif

clean:
	rm -rf *~ *.ko *.o *.mod.c modules.order Module.symvers .hrt* .tmp_versions

endif


# combo driver: MT6620
# If KERNELRELEASE is defined, we've been invoked from the
# kernel build system and can use its language.
ifneq ($(KERNELRELEASE),)
	
	subdir-ccflags-y += -D MTK_WCN_SOC_CHIP_SUPPORT
	ccflags-y += -I$(src)/../conn_soc/common/linux/include
	
	
ifeq ($(CONFIG_MTK_COMBO),y)
	ccflags-y += -I$(src)/drv_init/inc 	
	obj-y += mtk_wcn_stub_alps.o
	obj-y += wmt_stp_exp.o
	
	obj-y += wmt_detect.o
	obj-y += sdio_detect.o
	obj-y += wmt_detect_pwr.o
	
	obj-y += drv_init/
	
endif
	
ifeq ($(CONFIG_MTK_COMBO),m)
	obj-y += mtk_wcn_stub_alps.o
	obj-y += wmt_stp_exp.o
	
	obj-$(CONFIG_MTK_COMBO) += mtk_wmt_detect.o
	mtk_wmt_detect-objs := wmt_detect.o
	mtk_wmt_detect-objs += sdio_detect.o
	mtk_wmt_detect-objs += wmt_detect_pwr.o

endif
  
# Otherwise we were called directly from the command
# line; invoke the kernel build system.
else
    KERNELDIR ?= /lib/modules/$(shell uname -r)/build
    PWD  := $(shell pwd)
default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
endif

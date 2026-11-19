ifneq ($(CONFIG_MTK_CCCI_EXT), y)
include $(srctree)/drivers/misc/mediatek/helpers/Makefile
#CCCI_PLATFORM_DRIVER_DIR :=  $(wildcard $(obj)/$(call to-root,$(obj))mediatek/platform/$(call lc,$(MTK_PLATFORM))/kernel/drivers/ccci)
CCCI_PLATFORM_DRIVER_DIR :=  $(MTK_CUSTOM_PATH)/ccci/mt6582
ifeq ($(CCCI_PLATFORM_DRIVER_DIR),)
$(error Invalid ccci platform path, please check)
endif
ifeq (y,$(CONFIG_MTK_NET_CCMNI))
obj-m	+= ccmni.o
ccmni-$(CONFIG_MTK_NET_CCMNI) := ccmni_pfp.o ccmni_net.o
ccflags-y += -I$(MTK_CUSTOM_PATH)/ccci/include -I$(CCCI_PLATFORM_DRIVER_DIR)\
		 -I$(CCCI_PLATFORM_DRIVER_DIR)/include
else
obj-n += dummy.o
endif

else 
obj- :=dummy.o
endif


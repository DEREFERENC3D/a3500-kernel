include $(srctree)/drivers/misc/mediatek/helpers/Makefile
AUXADC_PLATFORM_DRIVER_DIR :=  $(MTK_CUSTOM_PATH)/auxadc/mt6582
obj-y += mt_auxadc.o
ccflags-y  += -I$(AUXADC_PLATFORM_DRIVER_DIR)
# MT6582 chip-specific implementation
obj-y += mt6582/

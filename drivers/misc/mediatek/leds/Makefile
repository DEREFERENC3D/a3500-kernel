include $(srctree)/drivers/misc/mediatek/helpers/Makefile
LEDS_PLATFORM_DRIVER_DIR :=  $(MTK_CUSTOM_PATH)/leds/mt6582
obj-$(CONFIG_MTK_LEDS) += leds_drv.o
ccflags-y  += -I$(LEDS_PLATFORM_DRIVER_DIR)


# MT6582 chip-specific implementation
obj-y += mt6582/

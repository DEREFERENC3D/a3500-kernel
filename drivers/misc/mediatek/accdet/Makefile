include $(srctree)/drivers/misc/mediatek/helpers/Makefile
LED_PLATFORM_DRIVER_DIR :=  $(MTK_CUSTOM_PATH)/accdet/mt6582
obj-$(CONFIG_MTK_ACCDET) := accdet_drv.o

ccflags-y += -I$(MTK_CUSTOM_PATH)/power\
             -I$(MTK_CUSTOM_PATH)/power
             
ccflags-y  += -I$(LED_PLATFORM_DRIVER_DIR)/

# MT6582 chip-specific implementation
obj-y += mt6582/

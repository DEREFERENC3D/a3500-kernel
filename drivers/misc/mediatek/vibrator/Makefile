include $(srctree)/drivers/misc/mediatek/helpers/Makefile
LED_PLATFORM_DRIVER_DIR :=  $(MTK_CUSTOM_PATH)/vibrator/mt6582
obj-$(CONFIG_MTK_VIBRATOR) := vibrator_drv.o
#ccflags-y := -Idrivers/staging/android/
ccflags-y  += -I$(LED_PLATFORM_DRIVER_DIR)/

# MT6582 chip-specific implementation
obj-y += mt6582/

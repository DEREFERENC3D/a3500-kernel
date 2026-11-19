vcodec_kernel_driver-objs :=        \
    videocodec_kernel_driver.o      \
    common/val/src/val.o			\
    common/drv/src/drv_base.o


obj-$(CONFIG_MTK_VIDEOCODEC_DRIVER) += vcodec_kernel_driver.o


ccflags-y += \
    -I$(MTK_CUSTOM_PATH)/videocodec/mt6582/common/val/inc		\
    -I$(MTK_CUSTOM_PATH)/videocodec/mt6582/common/hal/inc		\
    -I$(MTK_CUSTOM_PATH)/videocodec/mt6582/common/drv/inc		\
    -I$(MTK_PLATFORM_PATH_ROOT)/include/linux/vcodec			\
    -I$(MTK_PLATFORM_PATH_ROOT)/include/linux/vcodec			\
    -I$(MTK_CUSTOM_PATH)/smi		\

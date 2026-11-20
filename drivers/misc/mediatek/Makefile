# Makefile for MediaTek drivers (merged in-tree layout)
#
# This combines the former mediatek/kernel/drivers and
# mediatek/platform/mt6582/kernel/drivers build lists.  Chip-specific
# implementations live in <driver>/mt6582/ subdirectories.

include $(srctree)/drivers/misc/mediatek/Makefile.custom

obj-y	+= kernel/

obj-y	+= keypad/
obj-y	+= mtprof/
obj-y	+= clock/
obj-y	+= sched/
obj-y	+= mmp/
obj-y	+= xlog/
obj-y	+= uart/
obj-y	+= eint/
obj-y	+= boot/
obj-$(CONFIG_ZRAM) += mlog/
obj-y	+= gpu/
obj-y	+= gpio/
obj-y	+= aee/
obj-y	+= wdk/
obj-y	+= wdt/
obj-y	+= ion_drv/
obj-y	+= sync/
obj-y	+= pmic_wrap/
ifneq ($(MTK_BATTERY_NO_HAL),yes)
obj-y	+= power/
endif
obj-y	+= power_gs/
obj-y	+= usb20/
obj-$(CONFIG_MTK_BTIF) += btif/
obj-y	+= videocodec/
obj-y	+= dum-char/

ifneq ($(CONFIG_MTK_FPGA),y)
ifneq ($(MTK_RTC_NO_HAL),yes)
obj-y	+= rtc/
endif
ifeq ($(CONFIG_MTK_EEMCS_DEVICES),y)
endif
obj-y	+= fmradio/
obj-y	+= gps/
obj-y	+= hwmon/
obj-y	+= mhl/
obj-y	+= almk/
obj-y	+= btcvsd/
obj-y	+= bthid/
ifeq ($(MTK_WFD_SUPPORT),yes)
obj-y	+= uibc/
endif
obj-$(CONFIG_GATOR_DRIVER) += $(call all-subdirs-with-makefile,gator-driver)
obj-y	+= leds/
obj-y	+= auxadc/
obj-y	+= ram_console/
obj-y	+= thermal/
obj-y	+= evdo_support/
obj-y	+= wmt_ccci/
endif

# MediaTek display driver (common core + MT6582 panel HAL)
obj-y	+= video/

obj-y	+= dispsys/
obj-y	+= hdmitx/
obj-y	+= m4u/
obj-y	+= smi/
obj-y	+= jpeg/
obj-y	+= i2c/
obj-y	+= spi/
obj-y	+= mmc-host/
obj-y	+= sound/
obj-y	+= devinfo/
obj-y	+= devapc/
obj-y	+= ldvt/
obj-y	+= masp/
obj-y	+= vibrator/
obj-y	+= wmt/
obj-y	+= pmt/
obj-y	+= accdet/
obj-y	+= vibrator/
obj-y	+= pwm/
obj-y	+= freqhopping/
obj-y	+= dbg_dump/

ifeq ($(TRUSTONIC_TEE_SUPPORT),yes)
endif

ifeq ($(MTK_NFC_SUPPORT),yes)
endif

obj-y	+= gpu/

ifneq ($(CONFIG_MTK_COMBO),)
subdir-ccflags-y += -DMTK_WCN_WMT_STP_EXP_SYMBOL_ABSTRACT
subdir-ccflags-y += -DMTK_WCN_REMOVE_KERNEL_MODULE
ifneq ($(filter CONSYS_6582,$(MTK_COMBO_CHIP)),)
obj-y	+= conn_soc/
endif
obj-y	+= common_detect/
endif

obj-y	+= conn_md/
obj-y	+= selinux_warning/
obj-y	+= debug_util/

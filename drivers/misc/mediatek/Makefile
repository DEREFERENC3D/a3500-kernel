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
obj-y	+= ion_drv/
obj-y	+= sync/
obj-y	+= pmic_wrap/
obj-$(CONFIG_MTK_FLIPER) += fliper/
ifneq ($(MTK_BATTERY_NO_HAL),yes)
obj-y	+= power/
endif
obj-y	+= power_gs/
obj-y	+= usb20/
obj-y	+= usb11/
obj-y	+= mu3d/
obj-$(CONFIG_MTK_SYSTRACKER) += systracker/
obj-$(CONFIG_MTK_XHCI) += xhci/
obj-$(CONFIG_MTK_BTIF) += btif/
obj-$(CONFIG_MD32_SUPPORT) += md32_wdt/
obj-y	+= videocodec/
obj-y	+= dum-char/

ifneq ($(CONFIG_MTK_FPGA),y)
ifneq ($(MTK_RTC_NO_HAL),yes)
obj-y	+= rtc/
endif
obj-y	+= ccci/
obj-y	+= dual_ccci/
ifeq ($(CONFIG_MTK_EEMCS_DEVICES),y)
obj-y	+= eemcs/
endif
obj-y	+= fmradio/
obj-y	+= gps/
obj-y	+= hwmon/
obj-y	+= mhl/
obj-y	+= almk/
obj-y	+= bthid/
ifeq ($(MTK_WFD_SUPPORT),yes)
obj-y	+= uibc/
endif
obj-y	+= ccmni/
obj-y	+= emd_ctl/
obj-$(CONFIG_GATOR_DRIVER) += $(call all-subdirs-with-makefile,gator-driver)
obj-y	+= leds/
obj-y	+= auxadc/
obj-y	+= ram_console/
obj-y	+= thermal/
obj-y	+= evdo_support/
obj-$(CONFIG_MTK_IN_HOUSE_TEE_SUPPORT) += trustzone/
obj-y	+= wmt_ccci/
endif

# Common (chip-agnostic) MediaTek display driver
obj-y	+= video/common/
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
obj-y	+= mrdump/
obj-y	+= ldvt/
obj-y	+= masp/
obj-y	+= vibrator/
obj-y	+= wmt/
obj-y	+= pmt/
obj-y	+= accdet/
obj-y	+= vibrator/
obj-y	+= pwm/
obj-y	+= freqhopping/
obj-$(CONFIG_MTK_ETM) += etm/
obj-y	+= dbg_dump/
obj-y	+= misc_via/

ifeq ($(TRUSTONIC_TEE_SUPPORT),yes)
obj-$(CONFIG_TRUSTONIC_TEE_SUPPORT) += gud/
endif

ifeq ($(MTK_NFC_SUPPORT),yes)
obj-y	+= nfc/
endif

obj-y	+= gpu/

ifneq ($(CONFIG_MTK_COMBO),)
subdir-ccflags-y += -DMTK_WCN_WMT_STP_EXP_SYMBOL_ABSTRACT
subdir-ccflags-y += -DMTK_WCN_REMOVE_KERNEL_MODULE
ifneq ($(filter MT6620E3 MT6628 MT6630,$(MTK_COMBO_CHIP)),)
obj-y	+= combo/
subdir-ccflags-y += -DMTK_WCN_COMBO_CHIP_SUPPORT
endif
ifneq ($(filter CONSYS_6572 CONSYS_6582 CONSYS_6592,$(MTK_COMBO_CHIP)),)
obj-y	+= conn_soc/
endif
obj-y	+= common_detect/
endif

obj-y	+= conn_md/
obj-$(CONFIG_MTK_EXTMEM) += exm/
obj-$(CONFIG_MTK_SCHED_CMP_POWER_AWARE_CONTROLLER) += pa/
obj-y	+= selinux_warning/
obj-$(CONFIG_MTKPASR) += mtkpasr/
obj-y	+= debug_util/

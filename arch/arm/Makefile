#
# arch/arm/Makefile
#
# This file is included by the global makefile so that you can add your own
# architecture-specific flags and dependencies.
#
# This file is subject to the terms and conditions of the GNU General Public
# License.  See the file "COPYING" in the main directory of this archive
# for more details.
#
# Copyright (C) 1995-2001 by Russell King

LDFLAGS_vmlinux	:=-p --no-undefined -X
ifeq ($(CONFIG_CPU_ENDIAN_BE8),y)
LDFLAGS_vmlinux	+= --be8
endif

OBJCOPYFLAGS	:=-O binary -R .comment -S
GZFLAGS		:=-9
#KBUILD_CFLAGS	+=-pipe
KBUILD_CFLAGS   += -fno-pic
# Explicitly specifiy 32-bit ARM ISA since toolchain default can be -mthumb:
KBUILD_CFLAGS	+=$(call cc-option,-marm,)

# Never generate .eh_frame
KBUILD_CFLAGS	+= $(call cc-option,-fno-dwarf2-cfi-asm)

# Do not use arch/arm/defconfig - it's always outdated.
# Select a platform tht is kept up-to-date
KBUILD_DEFCONFIG := versatile_defconfig

# defines filename extension depending memory management type.
ifeq ($(CONFIG_MMU),)
MMUEXT		:= -nommu
endif

ifeq ($(CONFIG_FRAME_POINTER),y)
KBUILD_CFLAGS	+=-fno-omit-frame-pointer -mapcs -mno-sched-prolog
endif

ifeq ($(CONFIG_CC_STACKPROTECTOR),y)
KBUILD_CFLAGS	+=-fstack-protector
endif

ifeq ($(CONFIG_CPU_BIG_ENDIAN),y)
KBUILD_CPPFLAGS	+= -mbig-endian
AS		+= -EB
LD		+= -EB
else
KBUILD_CPPFLAGS	+= -mlittle-endian
AS		+= -EL
LD		+= -EL
endif

comma = ,

# This selects which instruction set is used.
# Note that GCC does not numerically define an architecture version
# macro, but instead defines a whole series of macros which makes
# testing for a specific architecture or later rather impossible.
arch-$(CONFIG_CPU_32v7)		:=-D__LINUX_ARM_ARCH__=7 $(call cc-option,-march=armv7-a,-march=armv5t -Wa$(comma)-march=armv7-a)
arch-$(CONFIG_CPU_32v6)		:=-D__LINUX_ARM_ARCH__=6 $(call cc-option,-march=armv6,-march=armv5t -Wa$(comma)-march=armv6)
# Only override the compiler option if ARMv6. The ARMv6K extensions are
# always available in ARMv7
ifeq ($(CONFIG_CPU_32v6),y)
arch-$(CONFIG_CPU_32v6K)	:=-D__LINUX_ARM_ARCH__=6 $(call cc-option,-march=armv6k,-march=armv5t -Wa$(comma)-march=armv6k)
endif
arch-$(CONFIG_CPU_32v5)		:=-D__LINUX_ARM_ARCH__=5 $(call cc-option,-march=armv5te,-march=armv4t)
arch-$(CONFIG_CPU_32v4T)	:=-D__LINUX_ARM_ARCH__=4 -march=armv4t
arch-$(CONFIG_CPU_32v4)		:=-D__LINUX_ARM_ARCH__=4 -march=armv4
arch-$(CONFIG_CPU_32v3)		:=-D__LINUX_ARM_ARCH__=3 -march=armv3

# This selects how we optimise for the processor.
tune-$(CONFIG_CPU_ARM610)	:=-mtune=arm610
tune-$(CONFIG_CPU_ARM710)	:=-mtune=arm710
tune-$(CONFIG_CPU_ARM7TDMI)	:=-mtune=arm7tdmi
tune-$(CONFIG_CPU_ARM720T)	:=-mtune=arm7tdmi
tune-$(CONFIG_CPU_ARM740T)	:=-mtune=arm7tdmi
tune-$(CONFIG_CPU_ARM9TDMI)	:=-mtune=arm9tdmi
tune-$(CONFIG_CPU_ARM940T)	:=-mtune=arm9tdmi
tune-$(CONFIG_CPU_ARM946E)	:=$(call cc-option,-mtune=arm9e,-mtune=arm9tdmi)
tune-$(CONFIG_CPU_ARM920T)	:=-mtune=arm9tdmi
tune-$(CONFIG_CPU_ARM922T)	:=-mtune=arm9tdmi
tune-$(CONFIG_CPU_ARM925T)	:=-mtune=arm9tdmi
tune-$(CONFIG_CPU_ARM926T)	:=-mtune=arm9tdmi
tune-$(CONFIG_CPU_FA526)	:=-mtune=arm9tdmi
tune-$(CONFIG_CPU_SA110)	:=-mtune=strongarm110
tune-$(CONFIG_CPU_SA1100)	:=-mtune=strongarm1100
tune-$(CONFIG_CPU_XSCALE)	:=$(call cc-option,-mtune=xscale,-mtune=strongarm110) -Wa,-mcpu=xscale
tune-$(CONFIG_CPU_XSC3)		:=$(call cc-option,-mtune=xscale,-mtune=strongarm110) -Wa,-mcpu=xscale
tune-$(CONFIG_CPU_FEROCEON)	:=$(call cc-option,-mtune=marvell-f,-mtune=xscale)
tune-$(CONFIG_CPU_V6)		:=$(call cc-option,-mtune=arm1136j-s,-mtune=strongarm)
tune-$(CONFIG_CPU_V6K)		:=$(call cc-option,-mtune=arm1136j-s,-mtune=strongarm)

ifeq ($(CONFIG_AEABI),y)
CFLAGS_ABI	:=-mabi=aapcs-linux -mno-thumb-interwork
else
CFLAGS_ABI	:=$(call cc-option,-mapcs-32,-mabi=apcs-gnu) $(call cc-option,-mno-thumb-interwork,)
endif

ifeq ($(CONFIG_ARM_UNWIND),y)
CFLAGS_ABI	+=-funwind-tables
endif

ifeq ($(CONFIG_THUMB2_KERNEL),y)
AFLAGS_AUTOIT	:=$(call as-option,-Wa$(comma)-mimplicit-it=always,-Wa$(comma)-mauto-it)
AFLAGS_NOWARN	:=$(call as-option,-Wa$(comma)-mno-warn-deprecated,-Wa$(comma)-W)
CFLAGS_THUMB2	:=-mthumb $(AFLAGS_AUTOIT) $(AFLAGS_NOWARN)
AFLAGS_THUMB2	:=$(CFLAGS_THUMB2) -Wa$(comma)-mthumb
# Work around buggy relocation from gas if requested:
ifeq ($(CONFIG_THUMB2_AVOID_R_ARM_THM_JUMP11),y)
CFLAGS_MODULE	+=-fno-optimize-sibling-calls
endif
endif

# Need -Uarm for gcc < 3.x
KBUILD_CFLAGS	+=$(CFLAGS_ABI) $(CFLAGS_THUMB2) $(arch-y) $(tune-y) $(call cc-option,-mshort-load-bytes,$(call cc-option,-malignment-traps,)) -msoft-float -Uarm
KBUILD_AFLAGS	+=$(CFLAGS_ABI) $(AFLAGS_THUMB2) $(arch-y) $(tune-y) -include asm/unified.h -msoft-float

CHECKFLAGS	+= -D__arm__

#Default value
head-y		:= arch/arm/kernel/head$(MMUEXT).o arch/arm/kernel/init_task.o
textofs-y	:= 0x00008000
textofs-$(CONFIG_ARCH_MT6582) := 0x00008000
textofs-$(CONFIG_ARCH_CLPS711X) := 0x00028000
# We don't want the htc bootloader to corrupt kernel during resume
textofs-$(CONFIG_PM_H1940)      := 0x00108000
# SA1111 DMA bug: we don't want the kernel to live in precious DMA-able memory
ifeq ($(CONFIG_ARCH_SA1100),y)
textofs-$(CONFIG_SA1111) := 0x00208000
endif
textofs-$(CONFIG_ARCH_MSM7X30) := 0x00208000
textofs-$(CONFIG_ARCH_MSM8X60) := 0x00208000
textofs-$(CONFIG_ARCH_MSM8960) := 0x00208000

# Machine directory name.  This list is sorted alphanumerically
# by CONFIG_* macro name.
machine-$(CONFIG_ARCH_AT91)		:= at91
machine-$(CONFIG_ARCH_BCMRING)		:= bcmring
machine-$(CONFIG_ARCH_CLPS711X)		:= clps711x
machine-$(CONFIG_ARCH_CNS3XXX)		:= cns3xxx
machine-$(CONFIG_ARCH_DAVINCI)		:= davinci
machine-$(CONFIG_ARCH_DOVE)		:= dove
machine-$(CONFIG_ARCH_EBSA110)		:= ebsa110
machine-$(CONFIG_ARCH_EP93XX)		:= ep93xx
machine-$(CONFIG_ARCH_GEMINI)		:= gemini
machine-$(CONFIG_ARCH_H720X)		:= h720x
machine-$(CONFIG_ARCH_HIGHBANK)		:= highbank
machine-$(CONFIG_ARCH_INTEGRATOR)	:= integrator
machine-$(CONFIG_ARCH_IOP13XX)		:= iop13xx
machine-$(CONFIG_ARCH_IOP32X)		:= iop32x
machine-$(CONFIG_ARCH_IOP33X)		:= iop33x
machine-$(CONFIG_ARCH_IXP2000)		:= ixp2000
machine-$(CONFIG_ARCH_IXP23XX)		:= ixp23xx
machine-$(CONFIG_ARCH_IXP4XX)		:= ixp4xx
machine-$(CONFIG_ARCH_KIRKWOOD)		:= kirkwood
machine-$(CONFIG_ARCH_KS8695)		:= ks8695
machine-$(CONFIG_ARCH_LPC32XX)		:= lpc32xx
machine-$(CONFIG_ARCH_MMP)		:= mmp
machine-$(CONFIG_ARCH_MSM)		:= msm
machine-$(CONFIG_ARCH_MT6582)		:= mt6582
machine-$(CONFIG_ARCH_MV78XX0)		:= mv78xx0
machine-$(CONFIG_ARCH_IMX_V4_V5)	:= imx
machine-$(CONFIG_ARCH_IMX_V6_V7)	:= imx
machine-$(CONFIG_ARCH_MXS)		:= mxs
machine-$(CONFIG_ARCH_NETX)		:= netx
machine-$(CONFIG_ARCH_NOMADIK)		:= nomadik
machine-$(CONFIG_ARCH_OMAP1)		:= omap1
machine-$(CONFIG_ARCH_OMAP2)		:= omap2
machine-$(CONFIG_ARCH_OMAP3)		:= omap2
machine-$(CONFIG_ARCH_OMAP4)		:= omap2
machine-$(CONFIG_ARCH_ORION5X)		:= orion5x
machine-$(CONFIG_ARCH_PICOXCELL)	:= picoxcell
machine-$(CONFIG_ARCH_PNX4008)		:= pnx4008
machine-$(CONFIG_ARCH_PRIMA2)		:= prima2
machine-$(CONFIG_ARCH_PXA)		:= pxa
machine-$(CONFIG_ARCH_REALVIEW)		:= realview
machine-$(CONFIG_ARCH_RPC)		:= rpc
machine-$(CONFIG_ARCH_S3C24XX)		:= s3c24xx s3c2412 s3c2440
machine-$(CONFIG_ARCH_S3C64XX)		:= s3c64xx
machine-$(CONFIG_ARCH_S5P64X0)		:= s5p64x0
machine-$(CONFIG_ARCH_S5PC100)		:= s5pc100
machine-$(CONFIG_ARCH_S5PV210)		:= s5pv210
machine-$(CONFIG_ARCH_EXYNOS4)		:= exynos
machine-$(CONFIG_ARCH_EXYNOS5)		:= exynos
machine-$(CONFIG_ARCH_SA1100)		:= sa1100
machine-$(CONFIG_ARCH_SHARK)		:= shark
machine-$(CONFIG_ARCH_SHMOBILE) 	:= shmobile
machine-$(CONFIG_ARCH_TEGRA)		:= tegra
machine-$(CONFIG_ARCH_U300)		:= u300
machine-$(CONFIG_ARCH_U8500)		:= ux500
machine-$(CONFIG_ARCH_VERSATILE)	:= versatile
machine-$(CONFIG_ARCH_VEXPRESS)		:= vexpress
machine-$(CONFIG_ARCH_VT8500)		:= vt8500
machine-$(CONFIG_ARCH_W90X900)		:= w90x900
machine-$(CONFIG_FOOTBRIDGE)		:= footbridge
machine-$(CONFIG_MACH_SPEAR300)		:= spear3xx
machine-$(CONFIG_MACH_SPEAR310)		:= spear3xx
machine-$(CONFIG_MACH_SPEAR320)		:= spear3xx
machine-$(CONFIG_MACH_SPEAR600)		:= spear6xx
machine-$(CONFIG_ARCH_ZYNQ)		:= zynq

# Platform directory name.  This list is sorted alphanumerically
# by CONFIG_* macro name.
plat-$(CONFIG_ARCH_MXC)		:= mxc
plat-$(CONFIG_ARCH_OMAP)	:= omap
plat-$(CONFIG_ARCH_S3C64XX)	:= samsung
plat-$(CONFIG_ARCH_ZYNQ)	:= versatile
plat-$(CONFIG_PLAT_IOP)		:= iop
plat-$(CONFIG_PLAT_NOMADIK)	:= nomadik
plat-$(CONFIG_PLAT_ORION)	:= orion
plat-$(CONFIG_PLAT_PXA)		:= pxa
plat-$(CONFIG_PLAT_S3C24XX)	:= s3c24xx samsung
plat-$(CONFIG_PLAT_S5P)		:= s5p samsung
plat-$(CONFIG_PLAT_SPEAR)	:= spear
plat-$(CONFIG_PLAT_VERSATILE)	:= versatile

ifeq ($(CONFIG_ARCH_EBSA110),y)
# This is what happens if you forget the IOCS16 line.
# PCMCIA cards stop working.
CFLAGS_3c589_cs.o :=-DISA_SIXTEEN_BIT_PERIPHERAL
export CFLAGS_3c589_cs.o
endif

# The byte offset of the kernel image in RAM from the start of RAM.
TEXT_OFFSET := $(textofs-y)

# The first directory contains additional information for the boot setup code
ifneq ($(machine-y),)
MACHINE  := arch/arm/mach-$(word 1,$(machine-y))/
else
MACHINE  :=
endif

machdirs := $(patsubst %,arch/arm/mach-%/,$(machine-y))

# Generated MediaTek custom headers (previously supplied by the MTK build).
LINUXINCLUDE += $(addprefix -I,$(sort $(dir $(shell find $(srctree)/arch/arm/mach-mt6582/lcsh82_tb_3gfull_kk -type f -name '*.h'))))
LINUXINCLUDE += -I$(srctree)/../out/target/product/lcsh82_tb_3gfull_kk/obj/PTGEN/common \
                -I$(srctree)/../out/target/product/lcsh82_tb_3gfull_kk/obj/PTGEN/kernel \
                -I$(srctree)/../out/target/product/lcsh82_tb_3gfull_kk/obj/NANDGEN/common
platdirs := $(patsubst %,arch/arm/plat-%/,$(plat-y))


ifeq ($(KBUILD_SRC),)
KBUILD_CPPFLAGS += $(patsubst %,-I%include,$(machdirs) $(platdirs))
else
KBUILD_CPPFLAGS += $(patsubst %,-I$(srctree)/%include,$(machdirs) $(platdirs))
endif

export	TEXT_OFFSET GZFLAGS MMUEXT

# Do we have FASTFPE?
FASTFPE		:=arch/arm/fastfpe
ifeq ($(FASTFPE),$(wildcard $(FASTFPE)))
FASTFPE_OBJ	:=$(FASTFPE)/
endif

core-$(CONFIG_FPE_NWFPE)	+= arch/arm/nwfpe/
core-$(CONFIG_FPE_FASTFPE)	+= $(FASTFPE_OBJ)
core-$(CONFIG_VFP)		+= arch/arm/vfp/

# If we have a machine-specific directory, then include it in the build.
core-y				+= arch/arm/kernel/ arch/arm/mm/ arch/arm/common/
core-y				+= arch/arm/net/
core-y				+= $(machdirs) $(platdirs)

drivers-$(CONFIG_OPROFILE)      += arch/arm/oprofile/

libs-y				:= arch/arm/lib/ $(libs-y)

# Default target when executing plain make
ifeq ($(CONFIG_XIP_KERNEL),y)
KBUILD_IMAGE := xipImage
else ifeq ($(CONFIG_BUILD_ARM_APPENDED_DTB_IMAGE),y)
KBUILD_IMAGE := zImage-dtb.$(CONFIG_BUILD_ARM_APPENDED_DTB_IMAGE_NAMES)
else
KBUILD_IMAGE := zImage
endif

all:	$(KBUILD_IMAGE)

boot := arch/arm/boot

archprepare:
	$(Q)$(MAKE) $(build)=arch/arm/tools include/generated/mach-types.h

# Convert bzImage to zImage
bzImage: zImage

zImage Image xipImage bootpImage uImage: vmlinux
	$(Q)$(MAKE) $(build)=$(boot) MACHINE=$(MACHINE) $(boot)/$@

zinstall uinstall install: vmlinux
	$(Q)$(MAKE) $(build)=$(boot) MACHINE=$(MACHINE) $@

%.dtb: scripts
	$(Q)$(MAKE) $(build)=$(boot) MACHINE=$(MACHINE) $(boot)/$@

dtbs: scripts
	$(Q)$(MAKE) $(build)=$(boot) MACHINE=$(MACHINE) $(boot)/$@

zImage-dtb: vmlinux scripts
	$(Q)$(MAKE) $(build)=$(boot) MACHINE=$(MACHINE) $(boot)/$@

# We use MRPROPER_FILES and CLEAN_FILES now
archclean:
	$(Q)$(MAKE) $(clean)=$(boot)

# My testing targets (bypasses dependencies)
bp:;	$(Q)$(MAKE) $(build)=$(boot) MACHINE=$(MACHINE) $(boot)/bootpImage
i zi:;	$(Q)$(MAKE) $(build)=$(boot) MACHINE=$(MACHINE) $@


define archhelp
  echo  '* zImage        - Compressed kernel image (arch/$(ARCH)/boot/zImage)'
  echo  '  Image         - Uncompressed kernel image (arch/$(ARCH)/boot/Image)'
  echo  '* xipImage      - XIP kernel image, if configured (arch/$(ARCH)/boot/xipImage)'
  echo  '  uImage        - U-Boot wrapped zImage'
  echo  '  bootpImage    - Combined zImage and initial RAM disk' 
  echo  '                  (supply initrd image via make variable INITRD=<path>)'
  echo  '  dtbs          - Build device tree blobs for enabled boards'
  echo  '  install       - Install uncompressed kernel'
  echo  '  zinstall      - Install compressed kernel'
  echo  '  uinstall      - Install U-Boot wrapped compressed kernel'
  echo  '                  Install using (your) ~/bin/$(INSTALLKERNEL) or'
  echo  '                  (distribution) /sbin/$(INSTALLKERNEL) or'
  echo  '                  install to $$(INSTALL_PATH) and run lilo'
endef

# Build flags previously injected by MediaTek ProjectConfig.mk / makeMtk
KBUILD_CPPFLAGS += -DMTK_BT_PROFILE_AVRCP \
    -DMTK_GPS_SUPPORT \
    -DMTK_WFD_SUPPORT \
    -DMTK_FM_SUPPORT \
    -DMTK_USES_HD_VIDEO \
    -DMTK_DRM_APP \
    -DMTK_VIDEO_VP9_SUPPORT \
    -DMTK_AUDIO_RAW_SUPPORT \
    -DMTK_GEMINI_SMART_3G_SWITCH \
    -DMTK_AUDIO_APE_SUPPORT \
    -DMTK_BT_PROFILE_MANAGER \
    -DMTK_FM_RECORDING_SUPPORT \
    -DMTK_BT_PROFILE_HIDH \
    -DMTK_IPV6_SUPPORT \
    -DMTK_BT_PROFILE_PBAP \
    -DMTK_OGM_PLAYBACK_SUPPORT \
    -DMTK_BT_PROFILE_PAN \
    -DMTK_BT_PROFILE_A2DP \
    -DMTK_BT_21_SUPPORT \
    -DMTK_BICR_SUPPORT \
    -DMTK_MASS_STORAGE \
    -DMTK_BT_PROFILE_BPP \
    -DMTK_HDR_SUPPORT \
    -DMTK_TABLET_DRAM \
    -DHAVE_AACENCODE_FEATURE \
    -DMTK_AUDIO \
    -DMTK_WIFI_HOTSPOT_SUPPORT \
    -DMTK_COMBO_SUPPORT \
    -DMTK_FLIGHT_MODE_POWER_OFF_MD \
    -DMTK_SHARED_SDCARD \
    -DMTK_MULTI_STORAGE_SUPPORT \
    -DMTK_ENABLE_VIDEO_EDITOR \
    -DMTK_LIVE_PHOTO_SUPPORT \
    -DMTK_AUDIO_ADPCM_SUPPORT \
    -DHAVE_ADPCMENCODE_FEATURE \
    -DHAVE_AWBENCODE_FEATURE \
    -DMTK_FACEBEAUTY_SUPPORT \
    -DMTK_HANDSFREE_DMNR_SUPPORT \
    -DMTK_AUDIO_HD_REC_SUPPORT \
    -DMTK_YAML_SCATTER_FILE_SUPPORT \
    -DMTK_PASSPOINT_R1_SUPPORT \
    -DMTK_BT_40_SUPPORT \
    -DMTK_AVI_PLAYBACK_SUPPORT \
    -DMTK_BT_PROFILE_FTP \
    -DMTK_USES_VR_DYNAMIC_QUALITY_MECHANISM \
    -DMTK_HIGH_QUALITY_THUMBNAIL \
    -DMTK_FLV_PLAYBACK_SUPPORT \
    -DMTK_SUBTITLE_SUPPORT \
    -DMTK_MD_SHUT_DOWN_NT \
    -DMTK_BT_SUPPORT \
    -DMTK_VOICE_UNLOCK_SUPPORT \
    -DMTK_MATV_ANALOG_SUPPORT \
    -DMTK_AUTORAMA_SUPPORT \
    -DMTK_MOBILE_MANAGEMENT \
    -DMTK_MOTION_TRACK_SUPPORT \
    -DCUSTOM_KERNEL_ACCELEROMETER \
    -DMTK_BT_PROFILE_OPP \
    -DMTK_CAMERA_BSP_SUPPORT \
    -DMTK_PRODUCT_INFO_SUPPORT \
    -DMTK_ION_SUPPORT \
    -DMTK_CAMERA_APP_3DHW_SUPPORT \
    -DMTK_WLAN_SUPPORT \
    -DMTK_PQ_SUPPORT \
    -DMTK_TETHERINGIPV6_SUPPORT \
    -DMTK_UART_USB_SWITCH \
    -DMTK_TB_APP_CALL_FORCE_SPEAKER_ON \
    -DMTK_PLATFORM_OPTIMIZE \
    -DMTK_FM_RX_SUPPORT \
    -DMTK_SW_BTCVSD \
    -DMTK_KERNEL_POWER_OFF_CHARGING \
    -DMTK_BT_PROFILE_SPP \
    -DMTK_BT_30_SUPPORT \
    -DMTK_HW_ENHANCE \
    -DUSE_FRAUNHOFER_AAC \
    -DMTK_LCEEFT_SUPPORT \
    -DMTK_AUDIO_CHANGE_SUPPORT \
    -DMTK_DHCPV6C_WIFI \
    -DMTK_BT_PROFILE_PRXM \
    -DMTK_MTKPS_PLAYBACK_SUPPORT \
    -DMTK_BATLOWV_NO_PANEL_ON_EARLY \
    -DMTK_BQ24196_SUPPORT \
    -DMTK_WEB_NOTIFICATION_SUPPORT \
    -DMTK_SWIP_VORBIS \
    -DMTK_SPH_EHN_CTRL_SUPPORT \
    -DMTK_WB_SPEECH_SUPPORT \
    -DCUSTOM_KERNEL_ALSPS \
    -DMTK_BT_PROFILE_PRXR \
    -DMTK_SENSOR_SUPPORT \
    -DMTK_M4U_SUPPORT \
    -DMTK_EMMC_SUPPORT \
    -DMTK_VOIP_ENHANCEMENT_SUPPORT \
    -DHAVE_XLOG_FEATURE \
    -DMTK_IPO_POWERPATH_SUPPORT \
    -DALLOW_LOCAL_PROP_OVERRIDE \
    -DSLT_DEVINFO_KERNEL_SUPPORT \
    -DLCT_APP_2SDCARD \
    -DMT6582 \
    -DDUMMY_LENS \
    -DDUMMY_LENS \
    -DWXGA \
    -DCPT_CLAA070WP0B_HIMAX \
    -DAUO_B070EAN014_HIMAX \
    -DA5142_MIPI_RAW \
    -DMTK_CONSYS_MT6582 \
    -DGC2235_RAW \
    -DDUMMY_FLASHLIGHT \
    -DHI257_YUV \
    -DDUMMY_LENS \
    -DMTK_AUDIO_BLOUD_CUSTOMPARAMETER_V4 \
    -DA5142_MIPI_RAW \
    -DHI257_YUV \
    -DGC2235_RAW \
    -DFM_DIGITAL_INPUT \
    -DCONSYS_6582 \
    -DMTK_GPS_MT6582 \
    -DDUMMY_LENS \
    -DFM_DIGITAL_OUTPUT \
    -DMT6627_FM \
    -DSLT_DRV_P3550_CONFIG \
    -DSLT_DRV_P3550_WIFI_ONLY_CONFIG \
    -DSLT_DEVINFO_LCM \
    -DSLT_DEVINFO_CMM \
    -DSLT_DEVINFO_CTP \
    -DSLT_DEVINFO_EMCP \
    -DMTK_TELEPHONY_BOOTUP_MODE_SLOT2=\"1\" \
    -DMTK_TELEPHONY_BOOTUP_MODE_SLOT1=\"0\" \
    -DLCM_WIDTH=\"800\" \
    -DMTK_FW_UPGRADE=\"yes\" \
    -DMTK_NEON_SUPPORT=\"yes\" \
    -DMTK_COMBO_PSM_TX_TH=\"400\" \
    -DMTK_NFC_SE_NUM=\"1\" \
    -DMTK_COMBO_PSM_RX_TH=\"2000\" \
    -DMTK_TOUCH_PHYSICAL_ROTATION_RELATIVE_TO_LCM=\"0\" \
    -DMTK_SHARE_MODEM_CURRENT=\"1\" \
    -DMTK_SIM2_SOCKET_TYPE=\"1\" \
    -DMTK_SIM1_SOCKET_TYPE=\"1\" \
    -DMTK_LCM_PHYSICAL_ROTATION=\"0\" \
    -DLCM_HEIGHT=\"1280\" \
    -DMTK_SHARE_MODEM_SUPPORT=\"2\" \
    -DCUSTOM_KERNEL_SSW=\"ssw_single\" \
    -DLCT_RUNINTEST_APP=\"yes\" \
    -DUSER_BUILD_KERNEL \
    -DCC_HAVE_ASM_GOTO \
    -DMTK_TB_WIFI_3G_MODE_WIFI_ONLY \
    -D__USING_DUMMY_CCCI_API__ \
    -DMTK_TABLET_PLATFORM=\"MT8382\"

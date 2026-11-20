#ifndef __ARCH_ARM_MACH_MT6575_CUSTOM_BOARD_H
#define __ARCH_ARM_MACH_MT6575_CUSTOM_BOARD_H

#include <generated/autoconf.h>

/*=======================================================================*/
/* MT6575 SD                                                             */
/*=======================================================================*/
#define CFG_DEV_MSDC0
#define CFG_DEV_MSDC1
//#define CFG_DEV_MSDC2
//#define CFG_DEV_MSDC3
//#define CFG_DEV_MSDC4
#if defined(CONFIG_MTK_COMBO) || defined(CONFIG_MTK_COMBO_MODULE)
/*
SDIO slot index number used by connectivity combo chip:
0: invalid (used by memory card)
1: MSDC1
2: MSDC2
*/
#define CONFIG_MTK_WCN_CMB_SDIO_SLOT  (3) /* MSDC3 */
#else
#undef CONFIG_MTK_WCN_CMB_SDIO_SLOT
#endif
/*For MT6582 platform we disable COMBO SDIO SLOT defination*/
#undef CONFIG_MTK_WCN_CMB_SDIO_SLOT


/*=======================================================================*/
/* MT6575 NAND FLASH                                                     */
/*=======================================================================*/
	
	
#define NFI_DEFAULT_ACCESS_TIMING        (0x44333)

//uboot only support 1 cs
#define NFI_CS_NUM                  (2)
#define NFI_DEFAULT_CS				(0)

#define USE_AHB_MODE                	(1)

#define PLATFORM_EVB                (1)

#endif /* __ARCH_ARM_MACH_MT6575_CUSTOM_BOARD_H */


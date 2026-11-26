/*
 * Copyright (C) 2007 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
/*******************************************************************************
 *
 * Filename:
 * ---------
 *   AudDrv_Ana.h
 *
 * Project:
 * --------
 *   MT6583  Audio Driver Ana
 *
 * Description:
 * ------------
 *   Audio register
 *
 * Author:
 * -------
 *   Chipeng Chang (mtk02308)
 *
 *------------------------------------------------------------------------------
 * $Revision: #1 $
 * $Modtime:$
 * $Log:$
 *
 *
 *******************************************************************************/

#ifndef _AUDDRV_ANA_H_
#define _AUDDRV_ANA_H_

/*****************************************************************************
 *                     C O M P I L E R   F L A G S
 *****************************************************************************/


/*****************************************************************************
 *                E X T E R N A L   R E F E R E N C E S
 *****************************************************************************/

#include "AudDrv_Common.h"
#include "AudDrv_Def.h"


/*****************************************************************************
 *                         D A T A   T Y P E S
 *****************************************************************************/


/*****************************************************************************
 *                         M A C R O
 *****************************************************************************/

/*****************************************************************************
 *                  R E G I S T E R       D E F I N I T I O N
 *****************************************************************************/

#define PMIC_ABB_AFE_REG_BASE (0x4000)
#define ABB_AFE_CON0    (PMIC_ABB_AFE_REG_BASE+0x0000)
#define ABB_AFE_CON1    (PMIC_ABB_AFE_REG_BASE+0x0002)
#define ABB_AFE_CON2    (PMIC_ABB_AFE_REG_BASE+0x0004)
#define ABB_AFE_CON3    (PMIC_ABB_AFE_REG_BASE+0x0006)
#define ABB_AFE_CON4    (PMIC_ABB_AFE_REG_BASE+0x0008)
#define ABB_AFE_CON5    (PMIC_ABB_AFE_REG_BASE+0x000A)
#define ABB_AFE_CON6    (PMIC_ABB_AFE_REG_BASE+0x000C)
#define ABB_AFE_CON7    (PMIC_ABB_AFE_REG_BASE+0x000E)
#define ABB_AFE_CON8    (PMIC_ABB_AFE_REG_BASE+0x0010)
#define ABB_AFE_CON9    (PMIC_ABB_AFE_REG_BASE+0x0012)
#define ABB_AFE_CON10   (PMIC_ABB_AFE_REG_BASE+0x0014)
#define ABB_AFE_CON11   (PMIC_ABB_AFE_REG_BASE+0x0016)
#define ABB_AFE_STA0    (PMIC_ABB_AFE_REG_BASE+0x0018)
#define ABB_AFE_STA1    (PMIC_ABB_AFE_REG_BASE+0x001A)
#define ABB_AFE_STA2    (PMIC_ABB_AFE_REG_BASE+0x001C)
#define ABB_AFE_UP8X_FIFO_CFG0  (PMIC_ABB_AFE_REG_BASE+0x001E)
#define ABB_AFE_UP8X_FIFO_LOG_MON0  (PMIC_ABB_AFE_REG_BASE+0x0020)
#define ABB_AFE_UP8X_FIFO_LOG_MON1  (PMIC_ABB_AFE_REG_BASE+0x0022)
#define ABB_AFE_PMIC_NEWIF_CFG0 (PMIC_ABB_AFE_REG_BASE+0x0024)
#define ABB_AFE_PMIC_NEWIF_CFG1 (PMIC_ABB_AFE_REG_BASE+0x0026)
#define ABB_AFE_PMIC_NEWIF_CFG2 (PMIC_ABB_AFE_REG_BASE+0x0028)
#define ABB_AFE_PMIC_NEWIF_CFG3 (PMIC_ABB_AFE_REG_BASE+0x002A)
#define ABB_AFE_TOP_CON0    (PMIC_ABB_AFE_REG_BASE+0x002C)
#define ABB_AFE_MON_DEBUG0  (PMIC_ABB_AFE_REG_BASE+0x002E)
//---------------digital pmic  register define -------------------------------------------
//---------------digital pmic  register define end ---------------------------------------

//---------------analog pmic  register define start --------------------------------------
#include <mach/upmu_hw.h>
void Ana_Set_Reg(uint32 offset, uint32 value, uint32 mask);
uint32  Ana_Get_Reg(uint32 offset);

// for debug usage
void Ana_Log_Print(void);

#endif


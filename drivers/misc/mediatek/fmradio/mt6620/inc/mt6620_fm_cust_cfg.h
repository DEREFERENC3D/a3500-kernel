/* 
 *
 * (C) Copyright 2009
 * MediaTek <www.MediaTek.com>
 * William Chung <William.Chung@MediaTek.com>
 *
 * MT6516 AR10x0 FM Radio Driver
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef __FM_CUST_CFG_H__
#define __FM_CUST_CFG_H__

//scan sort algorithm
enum {
    FM_SCAN_SORT_NON = 0,
    FM_SCAN_SORT_UP,
    FM_SCAN_SORT_DOWN,
    FM_SCAN_SORT_MAX
};
//***********************************FM config for customer ***********************************
//RX
#define FMR_RSSI_TH_LONG_MT6620    0x0301      //FM radio long antenna RSSI threshold(11.375dBuV)
#define FMR_RSSI_TH_SHORT_MT6620   0x02E0      //FM radio short antenna RSSI threshold(-1dBuV)
#define FMR_CQI_TH_MT6620          0x00E9      //FM radio Channel quality indicator threshold(0x0000~0x00FF)
//***********************************FM config for engineer ***********************************
//RX
#define FMR_MR_TH_MT6620           0x01BD      //FM radio MR threshold
//RX
#define FM_RX_RSSI_TH_LONG_MT6620    -296      //FM radio long antenna RSSI threshold(-4dBuV)
#define FM_RX_RSSI_TH_SHORT_MT6620   -296      //FM radio short antenna RSSI threshold(-4dBuV)
#define FM_RX_DESENSE_RSSI_MT6620   -245      
#define FM_RX_PAMD_TH_MT6620          -12     
#define FM_RX_MR_TH_MT6620           -67      
#define FM_RX_ATDC_TH_MT6620           219      
#define FM_RX_PRX_TH_MT6620           64      
#define FM_RX_SMG_TH_MT6620          6      //FM soft-mute gain threshold
#define FM_RX_DEEMPHASIS_MT6620       0           //0-50us, China Mainland; 1-75us China Taiwan
#define FM_RX_OSC_FREQ_MT6620         0           //0-26MHz; 1-19MHz; 2-24MHz; 3-38.4MHz; 4-40MHz; 5-52MHz  
//TX
#define FM_TX_PWR_LEVEL_MAX_MT6620  120  
#define FM_TX_SCAN_HOLE_LOW_MT6620  923         //92.3MHz~95.4MHz should not show to user
#define FM_TX_SCAN_HOLE_HIGH_MT6620 954         //92.3MHz~95.4MHz should not show to user

#endif // __FM_CUST_CFG_H__

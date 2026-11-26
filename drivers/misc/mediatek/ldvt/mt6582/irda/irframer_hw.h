#ifndef _IRFRAMER_HW_H
#define _IRFRAMER_HW_H

#define TAG "Irda_Debug--->"
#ifdef DEBUG
#define dbg(format,arg...) printk(KERN_WARNING "%s" format,TAG,##arg)
#else 
#define dbg(format,arg...)
#endif










#include <net/irda/irda.h>
#include <net/irda/qos.h>
#include <net/irda/irlap.h>
#include <net/irda/irda_device.h>
#define  SIOCGERRSTAT	(SIOCGQOS +1)
#define IOCTL_SIR_DIV_16 0x01
#define IOCTL_SIR_1_61   0x02
#define IOCTL_BOF_C0H	0x04
#define IOCTL_BOF_FFH	0x08
#define IOCTL_HAVEHARDWARE 0x10
#define IOCTL_NOHARDWARE 0x20
struct option_array
{
	const char *option;
	void (*parse)(const char *value);	
};
struct mt_irda_framer
{
		struct net_device *netdev;
		struct irlap_cb *irlap;
		struct qos_info qos;
		int direction;
		void *buff;
		dma_addr_t buff_dma;
		spinlock_t lock;
		__u32 speed;
		__u32 new_speed;
		int rx_size;
		int tx_size;
		
		int sir_mode;
#define	IR_DIV_16 0x00
#define	IR_1_61_us	0x02
		
		int rx_invert;
#define IR_RX_EN_INVERT 0x0002   /*invert rx signal*/
		int tx_invert;
#define IR_TX_EN_INVERT	0x04
		int bof_type;
#define BOF_TYPE_C0H	(1<<7)
#define BOF_TYPE_FFH	(0<<7)
		
		int mir_576_tune;
#define IR_TX_MIR576_NONE       	0x00	
#define IR_TX_MIR576_RX_HALF      (1<<3)		
#define IR_TX_MIR576_RX_QUTER     (1<<4)
		
		int sir_framing_set;
#define SIR_FRAMING_IGN_STOP_BIT	(0<<1)
#define SIR_FRAMING_CHK_STOP_BIT	(1<<1)
		int err_stat;
	};
//#define IRDA_SIR_3_DIV_16
//#define IRDA_SIR_BOF_0XC0
//#define IRDA_MIR_CHANGE_STA

/* Transmit */

#define IR_BUF 	            (IRDA_base+0x0)
#define IR_BUF_CLEAR          (IRDA_base+0x04)
/* Control */
#define IR_MAX_T 	            (IRDA_base+0x08)
#define IR_MIN_T 	            (IRDA_base+0x0c)
#define IR_BOFS 	            (IRDA_base+0x10)
#define IR_DIV     	         (IRDA_base+0x14)
#define IR_TX_FRAME_SIZE      (IRDA_base+0x18)
#define IR_RX_FRAME1_SIZE      (IRDA_base+0x1c) /*read only*/
#define IR_ABORT	            (IRDA_base+0x20)
#define IR_TX_EN	            (IRDA_base+0x24)
#define IR_RX_EN	            (IRDA_base+0x28)
/*Interrupt*/
#define IR_INTTRIGGER         (IRDA_base+0x2c)
#define IR_IRQ_ENABLE         (IRDA_base+0x30)
#define IR_IRQ_STA            (IRDA_base+0x34)
#define IR_LSR                (IRDA_base+0x38)
#define IR_TRANSCEIVER_PDN    (IRDA_base+0x3c)

#define IR_RX_FRAME_MAX       (IRDA_base+0x40)
#define IR_DISCONNECT_TIME    (IRDA_base+0x44)
#define IR_COUNT_ENABLE       (IRDA_base+0x48)
#define IR_RATE               (IRDA_base+0x4c)
#define IR_RATE_FIX           (IRDA_base+0x50)
#define IR_FRAME1_STA         (IRDA_base+0x54)
#define IR_FRAME2_STA         (IRDA_base+0x58)
#define IR_RX_FRAME2_SIZE     (IRDA_base+0x5c)

#define IR_MODE               (IRDA_base+0x60)
#define IR_FIFO_STA           (IRDA_base+0x64)
#define IR_FIR_DIV						(IRDA_base+0x68)

/*=================Bit Definition=============================*/
#define IR_DATA_MASK          0x00ff
#define IR_MAX_T_MASK         0x3fff
#define IR_MIN_T_MASK         0xffff

#define IR_DIV_MASK       0xffff
#define IR_TXFRAME_SIZE_MASK  0x0fff
#define IR_RXFRAME_SIZE_MASK  0x0fff
#define IR_INTTRIGGER_TXMASK  0x0003
#define IR_INTTRIGGER_RXMASK  0x000c

/*IR_INTTRIGGER*/
//#define IR_INTTRIGGER_NORMAL  0x0000   /*TX_TRIG = 2, RX_TRIG=2*/
#define IR_INTTRIGGER_NORMAL  0x0003   /*TX_TRIG = 2, RX_TRIG=2*/

/*FIFO clear*/
#define IR_BUF_CLEAR_BIT      0x0001

/*IR_BOFS*/
#define IR_BOFS_MASK          0x007f
/*IR_ABORT*/
#define IR_ABORT_BIT          0x0001   /*Trx enable*/

/*IR_TX_EN*/
#define IR_TX_EN_BIT           0x0001   /*Trx enable*/
#define IR_TX_EN_MODE          0x0002   /*0:3/16 , 1: 1.61us*/

#define IR_TX_EN_ONESHIT       0x0008   /*HW auto clear tx_en, when frame is sent*/
#define IR_TX_EN_DISABLE       0x0000
/*HW 6228/6229 ECO to cover Vishay transicver*/
#define IR_TX_MIR576_ENABLE       0x8


/*IR_RX_EN*/
#define IR_RX_EN_BIT           0x0001

#define IR_RX_EN_RXONE         0x0004   /*disable when get one frame*/
#define IR_RX_EN_NORMAL        0x0001
#define IR_RX_EN_DISABLE       0x0000

/*IR_IRQ_ENABLE*/
#define IR_IRQ_ENABLE_TXTRIG        0x0001   /*x*/
#define IR_IRQ_ENABLE_RXTRIG        0x0002   /*x*/
#define IR_IRQ_ENABLE_ERROR         0x0004
#define IR_IRQ_ENABLE_TXCOMPLETE    0x0008
#define IR_IRQ_ENABLE_RXCOMPLETE    0x0010
#define IR_IRQ_ENABLE_MINTOUT       0x0020         /*x*/
#define IR_IRQ_ENABLE_MAXTOUT       0x0040         /*x*/
#define IR_IRQ_ENABLE_RXABORT       0x0080
#define IR_IRQ_ENABLE_TXABORT       0x0100
#define IR_IRQ_ENABLE_FIFOTOUT      0x0200   /*x*/
#define IR_IRQ_ENABLE_THRESHTOUT    0x0400         /*x*/

#define IR_IRQ_ENABLE_NORMAL        0x019e   /*Disable all timer*/
#define IR_IRQ_ENABLE_DMANORMAL     0x01dc
#define IR_IRQ_RTX		0x0dc
/*IR_STA*/
#define IR_STA_TXTRIG        0x0001
#define IR_STA_RXTRIG        0x0002
#define IR_STA_LSR           0x0004
#define IR_STA_TXCOMPLETE    0x0008
#define IR_STA_RXCOMPLETE    0x0010
#define IR_STA_2ND_RXCOMPLETE 0x1000  
#define IR_STA_MINTOUT       0x0020
#define IR_STA_MAXTOUT       0x0040
#define IR_STA_RXABORT       0x0080
#define IR_STA_TXABORT       0x0100
#define IR_STA_FIFOTOUT      0x0200
#define IR_STA_THRESHTOUT    0x0400

/*IR_ERROR*/
#define IR_LSR_RXSIZE         0x0001
#define IR_LSR_OVERRUN        0x0002
#define IR_FRAME1_ERROR       0x0010
#define IR_FRAME2_ERROR       0x0020
#define IR_TX_UNDERRUN        0x0040

#define IR_FRAME_FRAMING      0x0001
#define IR_FRAME_CRCFAIL      0x0002
#define IR_FRAME_PF			      0x0004
#define IR_FRAME_UNKNOW	      0x0008
#define IR_FRAME_MIR      		0x0010
#define IR_FRAME_FIR_4PPM  		0x0020
#define IR_FRAME_FIR_STO      0x0040
/*Power on*/
#define IR_POWER_ON           0x0000
#define IR_POWER_OFF          0x0001

#define IR_CRC_REPORT 	      0x0004

extern struct mt_irda_framer *framer_dev;
extern void irda_dma_callback(void *dev);
extern void irda_dma_rx_config(struct mt_irda_framer *framer);
extern void irda_dma_tx_config(struct mt_irda_framer *framer);
//void irda_dma_callback(void *dev);
extern void mt_irda_dev_close(void);
extern void mt_irda_dev_open(void);
extern void irda_irq_init(void);
#endif   /*_IRFRAMER_HW_H*/


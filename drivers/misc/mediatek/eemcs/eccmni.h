#ifndef __EEMCS_NET_H__
#define __EEMCS_NET_H__
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include "eemcs_ccci.h"
#include "eemcs_kal.h"

// #define _ECCMNI_SKB_DBG_

#define ECCMNI_MAX_DEV	(END_OF_CCMNI-START_OF_CCMNI)

#define ECCMNI_LB       0xff

#define  IPV4_VERSION 0x40
#define  IPV6_VERSION 0x60

typedef struct 
{
    struct net_device *dev;
    KAL_UINT8         eemcs_port_id;
    ccci_ch_set       ccci_ch;
#if (defined(_ECCMNI_SEQ_SUPPORT_))
    struct{
        KAL_UINT32 UL;
        KAL_UINT32 DL;
    }                 seqno; 
#endif
}eccmni_inst_t;

#if defined(_ECCMNI_LB_UT_)
KAL_UINT32 eccmniut_register_callback(CCCI_CHANNEL_T chn, EEMCS_CCCI_CALLBACK func_ptr , KAL_UINT32 private_data);
KAL_UINT32 eccmniut_unregister_callback(CCCI_CHANNEL_T chn);
inline KAL_UINT32 eccmniut_UL_write_room_alloc(CCCI_CHANNEL_T chn);
inline KAL_INT32 eccmniut_UL_write_skb_to_swq(CCCI_CHANNEL_T chn, struct sk_buff *skb);
inline struct sk_buff* eccmniut_DL_read_skb_from_swq(CCCI_CHANNEL_T chn);

#define ccci_register(ch_num,cb,para)           eccmniut_register_callback(ch_num,cb,para) 
#define ccci_unregister(ch_num)			        eccmniut_unregister_callback(ch_num) 
#define ccci_write_desc_to_q(ch_num,desc_p)     eccmniut_UL_write_skb_to_swq(ch_num, desc_p)
#define ccci_write_space_alloc(ch_num)          eccmniut_UL_write_room_alloc(ch_num)
//#define ccci_read_desc_from_q(ch_num)		    eccmniut_DL_read_skb_from_swq(ch_num)
#else
#define ccci_register(ch_num,cb,para)           eemcs_ccci_register_callback(ch_num,cb,para) 
#define ccci_unregister(ch_num)			        eemcs_ccci_unregister_callback(ch_num) 
#define ccci_write_desc_to_q(ch_num,desc_p)     eemcs_ccci_UL_write_skb_to_swq(ch_num, desc_p)
#define ccci_write_space_alloc(ch_num)          eemcs_ccci_UL_write_room_alloc(ch_num)
#endif
/*******************************************************************************
*                            A P I s
********************************************************************************/
KAL_INT32 eccmni_mod_init(void);
void  eccmni_deinit_mod_exit(void);

#if defined(_ECCMNI_LB_UT_) || defined(_EEMCS_CCCI_LB_UT)
KAL_INT32 eccmni_swap(struct sk_buff *skb);
#endif
#endif //__EEMCS_NET_H__


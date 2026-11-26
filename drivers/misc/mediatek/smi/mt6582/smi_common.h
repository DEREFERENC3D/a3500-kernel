#ifndef __SMI_COMMON_H__
#define __SMI_COMMON_H__

#include <linux/xlog.h>


#define SMIMSG(string, args...)	xlog_printk(ANDROID_LOG_INFO, SMI_LOG_TAG, "[pid=%d]"string,current->tgid,##args)
#define SMITMP(string, args...)  xlog_printk(ANDROID_LOG_INFO, SMI_LOG_TAG, "[pid=%d]"string,current->tgid,##args)
#define SMIERR(string, args...) do{\
	xlog_printk(ANDROID_LOG_ERROR,  SMI_LOG_TAG, "error: "string, ##args); \
}while(0)


#define MAU_ENTRY_NR    3
#define SMI_LARB_NR     3


extern unsigned int gLarbBaseAddr[SMI_LARB_NR];
extern char *smi_port_name[][17];


int larb_clock_on(int larb_id);
int larb_clock_off(int larb_id);

void smi_dumpDebugMsg(void);

int mau_init(void);
void SMI_DBG_Init(void);


#endif


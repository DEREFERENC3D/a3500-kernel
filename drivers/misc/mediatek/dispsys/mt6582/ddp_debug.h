#ifndef __DDP_DEBUG_H__
#define __DDP_DEBUG_H__

#include <linux/kernel.h>
#include "ddp_drv.h"

//#define DISP_MSG(...)   xlog_printk(ANDROID_LOG_DEBUG, "xlog/disp", __VA_ARGS__)
//#define DISP_DBG(...)   xlog_printk(ANDROID_LOG_WARN,  "xlog/disp", __VA_ARGS__)
//#define DISP_ERR(...)   xlog_printk(ANDROID_LOG_ERROR, "xlog/disp", __VA_ARGS__)

extern unsigned int dbg_log;
extern unsigned int irq_log;
extern unsigned int irq_err_log;

#define DISP_IRQ(string, args...) if(irq_log) printk("[DDP]"string,##args)  // default off
#define DISP_DBG(string, args...) if(dbg_log) printk("[DDP]"string,##args)  // default off, use "adb shell "echo dbg_log:1 > sys/kernel/debug/dispsys" to enable
#define DISP_MSG(string, args...) printk("[DDP]"string,##args)  // default on, important msg, not err
#define DISP_ERR(string, args...) printk("[DDP]error:"string,##args)  //default on, err msg
#define DDP_IRQ_ERR(string) if(irq_err_log) DISP_ERR(string)  // default off

void ddp_debug_init(void);
void ddp_debug_exit(void);
int ddp_mem_test(void);
int ddp_mem_test2(void);
void ddp_enable_bls(int BLS_switch);
int ddp_dump_info(DISP_MODULE_ENUM module);

#endif /* __DDP_DEBUG_H__ */

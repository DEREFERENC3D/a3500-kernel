#ifndef __MTK_RAM_CONSOLE_H__
#define __MTK_RAM_CONSOLE_H__

#include <linux/console.h>

#ifdef CONFIG_MTK_RAM_CONSOLE


extern void ram_console_write(struct console *console, const char *s, unsigned int count);

#ifdef CONFIG_MTK_EMMC_SUPPORT
extern void last_kmsg_store_to_emmc(void);
#endif

#else


static inline void ram_console_write(struct console *console, const char *s, unsigned int count){}

#ifdef CONFIG_MTK_EMMC_SUPPORT
static inline void last_kmsg_store_to_emmc(void){}
#endif

#endif


#endif

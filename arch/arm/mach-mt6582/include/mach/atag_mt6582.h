#ifndef __MACH_ATAG_MT6582_H
#define __MACH_ATAG_MT6582_H

#include <mach/dfo_boot.h>
#include <mach/mt_devinfo.h>

/* boot information */
#define ATAG_BOOT	0x41000802

struct tag_boot {
	u32 bootmode;
};

/* META com port information */
#define ATAG_META_COM	0x41000803

struct tag_meta_com {
	u32 meta_com_type;	/* identify meta via uart or usb */
	u32 meta_com_id;	/* multiple meta need to know com port id */
};

#endif /* __MACH_ATAG_MT6582_H */

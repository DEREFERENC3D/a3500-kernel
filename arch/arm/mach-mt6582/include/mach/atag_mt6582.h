#ifndef __MACH_ATAG_MT6582_H
#define __MACH_ATAG_MT6582_H

#include <asm/setup.h>
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

/* Battery voltage supplied by the bootloader; consumed by the fuel gauge. */
struct tag_revision_vbat {
	struct tag_header hdr;
	u32 rev;
	u32 vbat;
};

/* MT6582-only ATAG payloads. Kept out of the generic struct tag union. */
struct mtk_tag {
	struct tag_header hdr;
	union {
		struct tag_boot boot;
		struct tag_meta_com meta_com;
		struct tag_devinfo_data devinfo_data;
		tag_dfo_boot dfo_data;
		struct tag_revision_vbat revision;
	} u;
};

#endif /* __MACH_ATAG_MT6582_H */

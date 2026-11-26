#ifndef     _HDMI_TYPES_H_
#define     _HDMI_TYPES_H_

#include <linux/ion_drv.h>
#include <linux/list.h>
#include <asm/atomic.h>

#include "mtk_sync.h"
#include "hdmi_drv.h"
#include "hdmitx.h"


//~~~~~~~~~~~~~~~~~~~~~~~~typedef definition declare~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
typedef struct
{
    bool is_reconfig_needed; // whether need to reset HDMI memory
    bool is_enabled;         // whether HDMI is enabled or disabled by user
    bool is_force_disable;   //used for camera scenario.
    bool is_clock_on;        // DPI is running or not
    atomic_t state;          // HDMI_POWER_STATE state
    int     lcm_width;       // LCD write buffer width
    int     lcm_height;      // LCD write buffer height
    bool    lcm_is_video_mode;
    int     hdmi_width;      // DPI read buffer width
    int     hdmi_height;     // DPI read buffer height
    int     bg_width;        // DPI read buffer width
    int     bg_height;       // DPI read buffer height
    HDMI_VIDEO_RESOLUTION output_video_resolution;
    HDMI_AUDIO_FORMAT     output_audio_format;
    int     orientation;     // MDP's orientation, 0 means 0 degree, 1 means 90 degree, 2 means 180 degree, 3 means 270 degree
    HDMI_OUTPUT_MODE    output_mode;
    int     scaling_factor;
} _t_hdmi_context;

typedef enum
{
    create_new,
    insert_new,
    reg_configed,
    reg_updated,
    buf_read_done
} BUFFER_STATE;

typedef struct _hdmi_buffer_list
{
    hdmi_video_buffer_info buffer_info;
    BUFFER_STATE buf_state;

    unsigned int idx;   ///fence count
    int fence;          ///fence fd
    struct ion_handle *hnd;
    unsigned int mva;
    unsigned int va;
    struct list_head list;
} hdmi_video_buffer_list;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//~~~~~~~~~~~~~~~~~~~~~~~the definition~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define HDMI_DEVNAME "hdmitx"

#undef OUTREG32
#define OUTREG32(x, y) {/*printk("[hdmi]write 0x%08x to 0x%08x\n", (y), (x)); */__OUTREG32((x),(y))}
#define __OUTREG32(x,y) {*(unsigned int*)(x)=(y);}

#define RETIF(cond, rslt)       if ((cond)){HDMI_LOG("return in %d\n",__LINE__);return (rslt);}
#define RET_VOID_IF(cond)       if ((cond)){HDMI_LOG("return in %d\n",__LINE__);return;}
#define RETIF_NOLOG(cond, rslt) if ((cond)){return (rslt);}
#define RET_VOID_IF_NOLOG(cond) if ((cond)){return;}
#define RETIFNOT(cond, rslt)    if (!(cond)){HDMI_LOG("return in %d\n",__LINE__);return (rslt);}

#define ALIGN_TO(x, n) (((x) + ((n) - 1)) & ~((n) - 1))
#define hdmi_abs(a) (((a) < 0) ? -(a) : (a))
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#endif

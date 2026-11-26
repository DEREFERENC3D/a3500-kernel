#ifdef BUILD_UBOOT
#define ENABLE_DPI_INTERRUPT        0
#define ENABLE_DPI_REFRESH_RATE_LOG 0

#include <asm/arch/disp_drv_platform.h>
#else

#define ENABLE_DPI_INTERRUPT        1
#define ENABLE_DPI_REFRESH_RATE_LOG 0

#if ENABLE_DPI_REFRESH_RATE_LOG && !ENABLE_DPI_INTERRUPT
#error "ENABLE_DPI_REFRESH_RATE_LOG should be also ENABLE_DPI_INTERRUPT"
#endif

#if defined(MTK_HDMI_SUPPORT) && !ENABLE_DPI_INTERRUPT
//#error "enable MTK_HDMI_SUPPORT should be also ENABLE_DPI_INTERRUPT"
#endif

#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/hrtimer.h>
#include <asm/io.h>
#include <disp_drv_log.h>
#include <linux/wait.h>
#include <linux/interrupt.h>
#include <linux/sched.h>
#include <linux/delay.h>
#include "disp_drv_platform.h"

#include "dpi_reg.h"
#include "dsi_reg.h"
#include "dpi_drv.h"
#include "lcd_drv.h"
#include <mach/mt_clkmgr.h>
#include "debug.h"

#if ENABLE_DPI_INTERRUPT
//#include <linux/interrupt.h>
//#include <linux/wait.h>

#include <mach/irqs.h>
#include "mtkfb.h"
#endif
static wait_queue_head_t _vsync_wait_queue_dpi;
static bool dpi_vsync = false;
static bool wait_dpi_vsync = false;
static struct hrtimer hrtimer_vsync_dpi;
#include <linux/module.h>
#endif

#include <mach/sync_write.h>
#ifdef OUTREG32
  #undef OUTREG32
  #define OUTREG32(x, y) mt65xx_reg_sync_writel(y, x)
#endif

#ifndef OUTREGBIT
#define OUTREGBIT(TYPE,REG,bit,value)  \
                    do {    \
                        TYPE r = *((TYPE*)&INREG32(&REG));    \
                        r.bit = value;    \
                        OUTREG32(&REG, AS_UINT32(&r));    \
                    } while (0)
#endif

static PDPI_REGS const DPI_REG = (PDPI_REGS)(DPI_BASE);
static PDSI_PHY_REGS const DSI_PHY_REG_DPI = (PDSI_PHY_REGS)(MIPI_CONFIG_BASE);
//static UINT32 const PLL_SOURCE = APMIXEDSYS_BASE + 0x44;
static BOOL s_isDpiPowerOn = FALSE;
static DPI_REGS regBackup;
static void (*dpiIntCallback)(DISP_INTERRUPT_EVENTS);

#define DPI_REG_OFFSET(r)       offsetof(DPI_REGS, r)
#define REG_ADDR(base, offset)  (((BYTE *)(base)) + (offset))

extern LCM_PARAMS *lcm_params;

const UINT32 BACKUP_DPI_REG_OFFSETS[] =
{
    DPI_REG_OFFSET(INT_ENABLE),
    DPI_REG_OFFSET(SIZE),
    DPI_REG_OFFSET(CLK_CNTL),

    DPI_REG_OFFSET(TGEN_HWIDTH),
    DPI_REG_OFFSET(TGEN_HPORCH),

	DPI_REG_OFFSET(TGEN_VWIDTH_LODD),
    DPI_REG_OFFSET(TGEN_VPORCH_LODD),

    //DPI_REG_OFFSET(TGEN_VWIDTH_LEVEN),
    //DPI_REG_OFFSET(TGEN_VPORCH_LEVEN),
    //DPI_REG_OFFSET(TGEN_VWIDTH_RODD),

    //DPI_REG_OFFSET(TGEN_VPORCH_RODD),
    //DPI_REG_OFFSET(TGEN_VWIDTH_REVEN),

//	DPI_REG_OFFSET(TGEN_VPORCH_REVEN),
//    DPI_REG_OFFSET(ESAV_VTIML),
//    DPI_REG_OFFSET(ESAV_VTIMR),
//	DPI_REG_OFFSET(ESAV_FTIM),
	DPI_REG_OFFSET(BG_HCNTL),
  
  	DPI_REG_OFFSET(BG_VCNTL),
    DPI_REG_OFFSET(BG_COLOR),
//	DPI_REG_OFFSET(TGEN_POL),
//	DPI_REG_OFFSET(EMBSYNC_SETTING),

    DPI_REG_OFFSET(CNTL),
};

static void _BackupDPIRegisters(void)
{
    DPI_REGS *reg = &regBackup;
    UINT32 i;

    for (i = 0; i < ARY_SIZE(BACKUP_DPI_REG_OFFSETS); ++ i)
    {
        OUTREG32(REG_ADDR(reg, BACKUP_DPI_REG_OFFSETS[i]),
                 AS_UINT32(REG_ADDR(DPI_REG, BACKUP_DPI_REG_OFFSETS[i])));
    }
}

static void _RestoreDPIRegisters(void)
{
    DPI_REGS *reg = &regBackup;
    UINT32 i;

    for (i = 0; i < ARY_SIZE(BACKUP_DPI_REG_OFFSETS); ++ i)
    {
        OUTREG32(REG_ADDR(DPI_REG, BACKUP_DPI_REG_OFFSETS[i]),
                 AS_UINT32(REG_ADDR(reg, BACKUP_DPI_REG_OFFSETS[i])));
    }
}

static void _ResetBackupedDPIRegisterValues(void)
{
    DPI_REGS *regs = &regBackup;
    memset((void*)regs, 0, sizeof(DPI_REGS));

//    OUTREG32(&regs->CLK_CNTL, 0x00000101);
}


#if ENABLE_DPI_REFRESH_RATE_LOG
static void _DPI_LogRefreshRate(DPI_REG_INTERRUPT status)
{
    static unsigned long prevUs = 0xFFFFFFFF;

    if (status.VSYNC)
    {
        struct timeval curr;
        do_gettimeofday(&curr);

        if (prevUs < curr.tv_usec)
        {
            DISP_LOG_PRINT(ANDROID_LOG_INFO, "DPI", "Receive 1 vsync in %lu us\n",
                   curr.tv_usec - prevUs);
        }
        prevUs = curr.tv_usec;
    }
}
#else
#define _DPI_LogRefreshRate(x)  do {} while(0)
#endif

extern void dsi_handle_esd_recovery(void);

void DPI_DisableIrq(void)
{
#if ENABLE_DPI_INTERRUPT
		DPI_REG_INTERRUPT enInt = DPI_REG->INT_ENABLE;
		//enInt.FIFO_EMPTY = 0;
		//enInt.FIFO_FULL = 0;
		//enInt.OUT_EMPTY = 0;
		//enInt.CNT_OVERFLOW = 0;
		//enInt.LINE_ERR = 0;
		enInt.VSYNC = 0;
		OUTREG32(&DPI_REG->INT_ENABLE, AS_UINT32(&enInt));
#endif
}
void DPI_EnableIrq(void)
{
#if ENABLE_DPI_INTERRUPT
		DPI_REG_INTERRUPT enInt = DPI_REG->INT_ENABLE;
		//enInt.FIFO_EMPTY = 1;
		//enInt.FIFO_FULL = 0;
		//enInt.OUT_EMPTY = 0;
		//enInt.CNT_OVERFLOW = 0;
		//enInt.LINE_ERR = 0;
		enInt.VSYNC = 1;
		OUTREG32(&DPI_REG->INT_ENABLE, AS_UINT32(&enInt));
#endif
}

static int dpi_vsync_irq_count = 0;
#if ENABLE_DPI_INTERRUPT
static irqreturn_t _DPI_InterruptHandler(int irq, void *dev_id)
{
    static int counter = 0;
    DPI_REG_INTERRUPT status = DPI_REG->INT_STATUS;
//    if (status.FIFO_EMPTY) ++ counter;

    if(status.VSYNC)
    {
	dpi_vsync_irq_count++;
	if(dpi_vsync_irq_count > 120)
	{
		printk("dpi vsync\n");
		dpi_vsync_irq_count = 0;
	}
        if(dpiIntCallback)
           dpiIntCallback(DISP_DPI_VSYNC_INT);
#ifndef BUILD_UBOOT
		if(wait_dpi_vsync){
			if(-1 != hrtimer_try_to_cancel(&hrtimer_vsync_dpi)){
				dpi_vsync = true;
//			hrtimer_try_to_cancel(&hrtimer_vsync_dpi);
				wake_up_interruptible(&_vsync_wait_queue_dpi);
			}
		}
#endif
    }

    if (status.VSYNC && counter) {
        DISP_LOG_PRINT(ANDROID_LOG_ERROR, "DPI", "[Error] DPI FIFO is empty, "
               "received %d times interrupt !!!\n", counter);
        counter = 0;
    }

    _DPI_LogRefreshRate(status);
	OUTREG32(&DPI_REG->INT_STATUS, 0);
    return IRQ_HANDLED;
}
#endif

#define VSYNC_US_TO_NS(x) (x * 1000)
unsigned int vsync_timer_dpi = 0;
void DPI_WaitVSYNC(void)
{
#ifndef BUILD_UBOOT
	wait_dpi_vsync = true;
	hrtimer_start(&hrtimer_vsync_dpi, ktime_set(0, VSYNC_US_TO_NS(vsync_timer_dpi)), HRTIMER_MODE_REL);
	wait_event_interruptible(_vsync_wait_queue_dpi, dpi_vsync);
	dpi_vsync = false;
	wait_dpi_vsync = false;
#endif
}

void DPI_PauseVSYNC(bool enable)
{
}

#ifndef BUILD_UBOOT
enum hrtimer_restart dpi_vsync_hrtimer_func(struct hrtimer *timer)
{
//	long long ret;
	if(wait_dpi_vsync)
	{
		dpi_vsync = true;
		wake_up_interruptible(&_vsync_wait_queue_dpi);
//		printk("hrtimer Vsync, and wake up\n");
	}
//	ret = hrtimer_forward_now(timer, ktime_set(0, VSYNC_US_TO_NS(vsync_timer_dpi)));
//	printk("hrtimer callback\n");
    return HRTIMER_NORESTART;
}
#endif
void DPI_InitVSYNC(unsigned int vsync_interval)
{
#ifndef BUILD_UBOOT
    ktime_t ktime;
	vsync_timer_dpi = vsync_interval;
	ktime = ktime_set(0, VSYNC_US_TO_NS(vsync_timer_dpi));
	hrtimer_init(&hrtimer_vsync_dpi, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
	hrtimer_vsync_dpi.function = dpi_vsync_hrtimer_func;
//	hrtimer_start(&hrtimer_vsync_dpi, ktime, HRTIMER_MODE_REL);
#endif
}

/*
void MIPI_clk_setting(LCM_PARAMS *lcm_params)
{//need re-write
	unsigned int pll_index = 0;
	DSI_PHY_REG_DPI->MIPITX_DSI_TOP_CON.RG_DSI_LNT_IMP_CAL_CODE = 8;
	DSI_PHY_REG_DPI->MIPITX_DSI_TOP_CON.RG_DSI_LNT_HS_BIAS_EN = 1;

	DSI_PHY_REG_DPI->MIPITX_DSI_BG_CON.RG_DSI_V032_SEL = 4;
	DSI_PHY_REG_DPI->MIPITX_DSI_BG_CON.RG_DSI_V04_SEL = 4;
	DSI_PHY_REG_DPI->MIPITX_DSI_BG_CON.RG_DSI_V072_SEL = 4;
	DSI_PHY_REG_DPI->MIPITX_DSI_BG_CON.RG_DSI_V10_SEL = 4;
	DSI_PHY_REG_DPI->MIPITX_DSI_BG_CON.RG_DSI_V12_SEL = 4;
	DSI_PHY_REG_DPI->MIPITX_DSI_BG_CON.RG_DSI_BG_CKEN = 1;
	DSI_PHY_REG_DPI->MIPITX_DSI_BG_CON.RG_DSI_BG_CORE_EN = 1;
	mdelay(10);

	DSI_PHY_REG_DPI->MIPITX_DSI0_CON.RG_DSI0_CKG_LDOOUT_EN = 1;
	DSI_PHY_REG_DPI->MIPITX_DSI0_CON.RG_DSI0_LDOCORE_EN = 1;

	DSI_PHY_REG_DPI->MIPITX_DSI_PLL_PWR.DA_DSI0_MPPLL_SDM_PWR_ON = 1;
	DSI_PHY_REG_DPI->MIPITX_DSI_PLL_PWR.DA_DSI0_MPPLL_SDM_ISO_EN = 1;
	mdelay(10);

	DSI_PHY_REG_DPI->MIPITX_DSI_PLL_PWR.DA_DSI0_MPPLL_SDM_ISO_EN = 0;

	pll_index = custom_pll_clock_remap(lcm_params->dsi.PLL_CLOCK);

	DSI_PHY_REG_DPI->MIPITX_DSI_PLL_CON0.RG_DSI0_MPPLL_PREDIV = 0;  // preiv
	DSI_PHY_REG_DPI->MIPITX_DSI_PLL_CON0.RG_DSI0_MPPLL_POSDIV = 0;  // posdiv

	if(0 != pll_index){
		DSI_PHY_REG_DPI->MIPITX_DSI_PLL_CON0.RG_DSI0_MPPLL_TXDIV1 = pll_config[pll_index].TXDIV1;  // div1
		DSI_PHY_REG_DPI->MIPITX_DSI_PLL_CON0.RG_DSI0_MPPLL_TXDIV0 = pll_config[pll_index].TXDIV0;  // div0
		DSI_PHY_REG_DPI->MIPITX_DSI_PLL_CON2.RG_DSI0_MPPLL_SDM_PCW_H = (pll_config[pll_index].SDM_PCW>>24);
		DSI_PHY_REG_DPI->MIPITX_DSI_PLL_CON2.RG_DSI0_MPPLL_SDM_PCW_L = (pll_config[pll_index].SDM_PCW & 0xFFFFFF);
		
		DSI_PHY_REG_DPI->MIPITX_DSI_PLL_CON1.RG_DSI0_MPPLL_SDM_SSC_PH_INIT = pll_config[pll_index].SSC_PH_INIT;
		DSI_PHY_REG_DPI->MIPITX_DSI_PLL_CON1.RG_DSI0_MPPLL_SDM_SSC_PRD= pll_config[pll_index].SSC_PRD;
		DSI_PHY_REG_DPI->MIPITX_DSI_PLL_CON3.RG_DSI0_MPPLL_SDM_SSC_DELTA= pll_config[pll_index].SSC_DELTA;
		DSI_PHY_REG_DPI->MIPITX_DSI_PLL_CON3.RG_DSI0_MPPLL_SDM_SSC_DELTA1= pll_config[pll_index].SSC_DELTA1;
		DSI_PHY_REG_DPI->MIPITX_DSI_PLL_CON1.RG_DSI0_MPPLL_SDM_FRA_EN = 1;
	}
	else{
		DSI_PHY_REG_DPI->MIPITX_DSI_PLL_CON0.RG_DSI0_MPPLL_TXDIV1 = lcm_params->dsi.pll_div2;  // div1
		DSI_PHY_REG_DPI->MIPITX_DSI_PLL_CON0.RG_DSI0_MPPLL_TXDIV0 = lcm_params->dsi.pll_div1;  // div0
		DSI_PHY_REG_DPI->MIPITX_DSI_PLL_CON2.RG_DSI0_MPPLL_SDM_PCW_H = ((lcm_params->dsi.fbk_div)<< 2);
		DSI_PHY_REG_DPI->MIPITX_DSI_PLL_CON2.RG_DSI0_MPPLL_SDM_PCW_L = 0;
		DSI_PHY_REG_DPI->MIPITX_DSI_PLL_CON1.RG_DSI0_MPPLL_SDM_FRA_EN = 0;
	}

	DSI_PHY_REG_DPI->MIPITX_DSI0_CLOCK_LANE.RG_DSI0_LNTC_RT_CODE = 0x8;
	DSI_PHY_REG_DPI->MIPITX_DSI0_CLOCK_LANE.RG_DSI0_LNTC_PHI_SEL = 0x1;
	DSI_PHY_REG_DPI->MIPITX_DSI0_CLOCK_LANE.RG_DSI0_LNTC_LDOOUT_EN = 1;
	
	if(lcm_params->dsi.LANE_NUM > 0)
	{
		DSI_PHY_REG_DPI->MIPITX_DSI0_DATA_LANE0.RG_DSI0_LNT0_RT_CODE = 0x8;
		DSI_PHY_REG_DPI->MIPITX_DSI0_DATA_LANE0.RG_DSI0_LNT0_LDOOUT_EN = 1;
	}
	
	if(lcm_params->dsi.LANE_NUM > 1)
	{
		DSI_PHY_REG_DPI->MIPITX_DSI0_DATA_LANE1.RG_DSI0_LNT1_RT_CODE = 0x8;
		DSI_PHY_REG_DPI->MIPITX_DSI0_DATA_LANE1.RG_DSI0_LNT1_LDOOUT_EN = 1;
	}
	
	if(lcm_params->dsi.LANE_NUM > 2)
	{
		DSI_PHY_REG_DPI->MIPITX_DSI0_DATA_LANE2.RG_DSI0_LNT2_RT_CODE = 0x8;
		DSI_PHY_REG_DPI->MIPITX_DSI0_DATA_LANE2.RG_DSI0_LNT2_LDOOUT_EN = 1;
	}

	if(lcm_params->dsi.LANE_NUM > 3)
	{
		DSI_PHY_REG_DPI->MIPITX_DSI0_DATA_LANE3.RG_DSI0_LNT3_RT_CODE = 0x8;
		DSI_PHY_REG_DPI->MIPITX_DSI0_DATA_LANE3.RG_DSI0_LNT3_LDOOUT_EN = 1;
	}
	
	DSI_PHY_REG_DPI->MIPITX_DSI_PLL_CON0.RG_DSI0_MPPLL_PLL_EN = 1;
	mdelay(1);
	
	if(0 != pll_index)
		DSI_PHY_REG_DPI->MIPITX_DSI_PLL_CON1.RG_DSI0_MPPLL_SDM_SSC_EN = 1;
	else
		DSI_PHY_REG_DPI->MIPITX_DSI_PLL_CON1.RG_DSI0_MPPLL_SDM_SSC_EN = 0;

	// default POSDIV by 4
	DSI_PHY_REG_DPI->MIPITX_DSI_PLL_TOP.RG_MPPLL_PRESERVE_L = 3;


}
*/
//#define DPI_HAL_CODE_SUPPORT_1024x600
//PANDA, For 6582 bring up DPI
void  DPI_MIPI_clk_setting(unsigned int mipi_pll_clk_ref,unsigned int mipi_pll_clk_div1,unsigned int mipi_pll_clk_div2){

	UINT32 i, j;

	UINT32 txdiv0  = 0;  
	UINT32 txdiv1  = 0;
	UINT32 posdiv  = 0;
	UINT32 prediv  = 0;
        UINT32 txmul = 0;

	UINT32 sdm_ssc_en     = 0;
	UINT32 sdm_ssc_prd    = 0;  // 0~65535
	UINT32 sdm_ssc_delta1 = 0;  // 0~65535
	UINT32 sdm_ssc_delta  = 0;  // 0~65535  
	
       UINT32   loopback_en = 0;
       UINT32   lane0_en = 1;
       UINT32   lane1_en = 1;
       UINT32   lane2_en = 1;
       UINT32   lane3_en = 1; 

       txmul = mipi_pll_clk_ref ;
       posdiv = mipi_pll_clk_div1;
	prediv   = mipi_pll_clk_div2;

	   
	//initial MIPI PLL
	OUTREG32((MIPI_CONFIG_BASE+0x050), 0x0);
	OUTREG32((MIPI_CONFIG_BASE+0x068), 0x2);
	OUTREG32((MIPI_CONFIG_BASE+0x044), 0x88492480);
	OUTREG32((MIPI_CONFIG_BASE+0x000), 0x400);
	OUTREG32((MIPI_CONFIG_BASE+0x054), 0x2);
	OUTREG32((MIPI_CONFIG_BASE+0x058), 0x0);
	OUTREG32((MIPI_CONFIG_BASE+0x05C), 0x0);

	OUTREG32((MIPI_CONFIG_BASE+0x004), 0x820);
	OUTREG32((MIPI_CONFIG_BASE+0x008), 0x400);
	OUTREG32((MIPI_CONFIG_BASE+0x00C), 0x100);
	OUTREG32((MIPI_CONFIG_BASE+0x010), 0x100);
	OUTREG32((MIPI_CONFIG_BASE+0x014), 0x100);
	
	OUTREG32((MIPI_CONFIG_BASE+0x040), 0x80);
	OUTREG32((MIPI_CONFIG_BASE+0x064), 0x0);
	OUTREG32((MIPI_CONFIG_BASE+0x074), 0x0);
	OUTREG32((MIPI_CONFIG_BASE+0x080), 0x0);
	OUTREG32((MIPI_CONFIG_BASE+0x084), 0x0);
	OUTREG32((MIPI_CONFIG_BASE+0x088), 0x0);
	OUTREG32((MIPI_CONFIG_BASE+0x090), 0x0);

	OUTREG32((MIPI_CONFIG_BASE+0x064), 0x300);

	printk("[DPI] MIPIPLL Initialed 222\n");

	//Setting MIPI PLL
	OUTREG32((MIPI_CONFIG_BASE+0x068), 0x3);
	OUTREG32((MIPI_CONFIG_BASE+0x068), 0x1);
	OUTREG32((MIPI_CONFIG_BASE+0x044), INREG32((MIPI_CONFIG_BASE+0x044)) | 0x00000013);
	OUTREG32((MIPI_CONFIG_BASE+0x040), (INREG32((MIPI_CONFIG_BASE+0x040)) | 0x00000002));
	OUTREG32((MIPI_CONFIG_BASE+0x000), ((INREG32((MIPI_CONFIG_BASE+0x000)) & 0xfffffbff ) | 0x00000003));
	OUTREG32((MIPI_CONFIG_BASE+0x050), ((prediv << 1) |(txdiv0 << 3) |(txdiv1 << 5) |(posdiv << 7)) );
	OUTREG32((MIPI_CONFIG_BASE+0x054), (0x3 | (sdm_ssc_en<<2) | (sdm_ssc_prd<<16)) );
	OUTREG32((MIPI_CONFIG_BASE+0x058), txmul);
	OUTREG32((MIPI_CONFIG_BASE+0x05C), ((sdm_ssc_delta<<16) | sdm_ssc_delta1));

	OUTREG32((MIPI_CONFIG_BASE+0x004), INREG32(MIPI_CONFIG_BASE+0x004) |0x00000001 |(loopback_en<<1));
	if(lane0_en)
		OUTREG32((MIPI_CONFIG_BASE+0x008), INREG32(MIPI_CONFIG_BASE+0x008) |0x00000001 |(loopback_en<<1));
	if(lane1_en)
		OUTREG32((MIPI_CONFIG_BASE+0x00C), INREG32(MIPI_CONFIG_BASE+0x00C) |0x00000001 |(loopback_en<<1));
	if(lane2_en)
		OUTREG32((MIPI_CONFIG_BASE+0x010), INREG32(MIPI_CONFIG_BASE+0x010) |0x00000001 |(loopback_en<<1));
	if(lane3_en)
		OUTREG32((MIPI_CONFIG_BASE+0x014), INREG32(MIPI_CONFIG_BASE+0x014) |0x00000001 |(loopback_en<<1));
	
	OUTREG32((MIPI_CONFIG_BASE+0x050), (INREG32((MIPI_CONFIG_BASE+0x050)) | 0x1));
	OUTREG32((MIPI_CONFIG_BASE+0x060), 0);
	OUTREG32((MIPI_CONFIG_BASE+0x060), 1);

	for(i=0; i<100; i++)   // wait for PLL stable
	{
		j = INREG32((MIPI_CONFIG_BASE+0x050));
	}
	printk("[DPI] MIPIPLL Exit\n");
}


DPI_STATUS DPI_Init(BOOL isDpiPoweredOn)
{
    //DPI_REG_CNTL cntl;
    //DPI_REG_EMBSYNC_SETTING embsync;

/*    if (isDpiPoweredOn) {
        _BackupDPIRegisters();
    } else {
        _ResetBackupedDPIRegisterValues();
    }*/
//mipi_tx_phy_config(0, 1, 1 ,1, 1);
//    DPI_PowerOn();
//if(lcm_params->type   == LCM_TYPE_DPI)
//	DSI_PowerOff();

    _BackupDPIRegisters();
    DPI_PowerOn();
        

// gpio setting
    OUTREG32(0xf0005000 + 0xCC4, 0x0000000f);
    OUTREG32(0xf0005000 + 0xB18, 0x00000ff0);
    OUTREG32(0xf0005000 + 0xB14, 0x00000660);
    OUTREG32(0xf0005000 + 0xB28, 0x000000ff);
    OUTREG32(0xf0005000 + 0xB24, 0x00000066);


/*
	OUTREG32(DPI_BASE+ 0x64, 0x400);//
	OUTREG32(DPI_BASE+ 0x6C, 0x400);//
	OUTREG32(DPI_BASE+ 0x74, 0x400);//
	OUTREG32(DPI_BASE+ 0x8C, 0x0FFF0000);//
	OUTREG32(DPI_BASE+ 0x90, 0x0FFF0000);//
	MASKREG32(DISPSYS_BASE + 0x60, 0x1, 0x1);
	*/
#if ENABLE_DPI_INTERRUPT
    if (request_irq(MT6582_DISP_DPI0_IRQ_ID,
        _DPI_InterruptHandler, IRQF_TRIGGER_LOW, "mtkdpi", NULL) < 0)
    {
        DISP_LOG_PRINT(ANDROID_LOG_INFO, "DPI", "[ERROR] fail to request DPI irq\n");
        return DPI_STATUS_ERROR;
    }

    {
        DPI_REG_INTERRUPT enInt = DPI_REG->INT_ENABLE;
        enInt.VSYNC = 1;
        OUTREG32(&DPI_REG->INT_ENABLE, AS_UINT32(&enInt));
    }
#endif
	LCD_W2M_NeedLimiteSpeed(TRUE);
    return DPI_STATUS_OK;
}
EXPORT_SYMBOL(DPI_Init);

DPI_STATUS DPI_FreeIRQ(void)
{
#if ENABLE_DPI_INTERRUPT
    free_irq(MT6582_DISP_DPI0_IRQ_ID, NULL);
#endif
    return DPI_STATUS_OK;
}
EXPORT_SYMBOL(DPI_FreeIRQ);

DPI_STATUS DPI_Deinit(void)
{
    DPI_DisableClk();
    DPI_PowerOff();

    return DPI_STATUS_OK;
}
EXPORT_SYMBOL(DPI_Deinit);

void DPI_mipi_switch(bool on)
{
	if(on)
	{
	// may call enable_mipi(), but do this in DPI_Init_PLL
	}
	else
	{
#ifdef DPI_MIPI_API
		disable_mipi(MT65XX_MIPI_TX, "DPI");
#endif
	}
}

#ifndef BULID_UBOOT
extern UINT32 FB_Addr;
#endif
DPI_STATUS DPI_Init_PLL(unsigned int mipi_pll_clk_ref,unsigned int mipi_pll_clk_div1,unsigned int mipi_pll_clk_div2)
{
    unsigned int reg_value = 0;

    DPI_MIPI_clk_setting( mipi_pll_clk_ref,  mipi_pll_clk_div1, mipi_pll_clk_div2);

	return DPI_STATUS_OK;
}

EXPORT_SYMBOL(DPI_Init_PLL);

DPI_STATUS DPI_Set_DrivingCurrent(LCM_PARAMS *lcm_params)
{
	DISP_LOG_PRINT(ANDROID_LOG_WARN, "DPI", "DPI_Set_DrivingCurrent not implement for 6575");
	return DPI_STATUS_OK;
}

#ifdef BUILD_UBOOT
DPI_STATUS DPI_PowerOn()
{
    if (!s_isDpiPowerOn)
    {
#if 1   // FIXME
        MASKREG32(0x14000110, 0x40, 0x0);//dpi0 clock gate clear
#endif
        _RestoreDPIRegisters();
        s_isDpiPowerOn = TRUE;
    }

    return DPI_STATUS_OK;
}


DPI_STATUS DPI_PowerOff()
{
    if (s_isDpiPowerOn)
    {
        BOOL ret = TRUE;
        _BackupDPIRegisters();
#if 1   // FIXME
        MASKREG32(0x14000110, 0x40, 0x40);//dpi0 clock gate setting
#endif
        ASSERT(ret);
        s_isDpiPowerOn = FALSE;
    }

    return DPI_STATUS_OK;
}

#else

DPI_STATUS DPI_PowerOn()
{
    int ret = 0;
    if (!s_isDpiPowerOn)
    {
#if 1
             ret += enable_clock(MT_CG_DISP1_DPI_DIGITAL_LANE, "DPI");
             ret += enable_clock(MT_CG_DISP1_DPI_ENGINE, "DPI");
		if(ret > 0)
		{
			DISP_LOG_PRINT(ANDROID_LOG_ERROR, "DPI", "power manager API return FALSE\n");
		}      
        //enable_pll(LVDSPLL, "dpi0");
#endif
        _RestoreDPIRegisters();
        s_isDpiPowerOn = TRUE;
    }
    return DPI_STATUS_OK;
}

DPI_STATUS DPI_PowerOff()
{
    int ret = 0;
	
    if (s_isDpiPowerOn)
    {
       
#if 1
        _BackupDPIRegisters();
		//disable_pll(LVDSPLL, "dpi0");
        ret += disable_clock(MT_CG_DISP1_DPI_DIGITAL_LANE, "DPI");
	 ret += disable_clock(MT_CG_DISP1_DPI_ENGINE, "DPI");
		if(ret >0)
		{
			DISP_LOG_PRINT(ANDROID_LOG_ERROR, "DPI", "power manager API return FALSE\n");
		}       
#endif
        s_isDpiPowerOn = FALSE;
    }
    return DPI_STATUS_OK;
}
#endif
EXPORT_SYMBOL(DPI_PowerOn);

EXPORT_SYMBOL(DPI_PowerOff);

DPI_STATUS DPI_EnableClk()
{
	DPI_REG_EN en = DPI_REG->DPI_EN;
    en.EN = 1;
    OUTREG32(&DPI_REG->DPI_EN, AS_UINT32(&en));
   //release mutex0
//#ifndef BUILD_UBOOT
    return DPI_STATUS_OK;
}
EXPORT_SYMBOL(DPI_EnableClk);

DPI_STATUS DPI_DisableClk()
{
    DPI_REG_EN en = DPI_REG->DPI_EN;
    en.EN = 0;
    OUTREG32(&DPI_REG->DPI_EN, AS_UINT32(&en));

    return DPI_STATUS_OK;
}
EXPORT_SYMBOL(DPI_DisableClk);

DPI_STATUS DPI_DisableMipiClk()
{
    // disable mipi clock
    OUTREGBIT(MIPITX_DSI_PLL_CON0_REG,DSI_PHY_REG_DPI->MIPITX_DSI_PLL_CON0,RG_DSI0_MPPLL_PLL_EN,0);
    mdelay(1);
    OUTREGBIT(MIPITX_DSI_PLL_TOP_REG, DSI_PHY_REG_DPI->MIPITX_DSI_PLL_TOP, RG_MPPLL_PRESERVE_L, 0);
    mdelay(1);

    OUTREGBIT(MIPITX_DSI_PLL_PWR_REG, DSI_PHY_REG_DPI->MIPITX_DSI_PLL_PWR, DA_DSI0_MPPLL_SDM_ISO_EN, 1);
    OUTREGBIT(MIPITX_DSI_PLL_PWR_REG, DSI_PHY_REG_DPI->MIPITX_DSI_PLL_PWR, DA_DSI0_MPPLL_SDM_PWR_ON, 0);

    OUTREG32(&DSI_PHY_REG_DPI->MIPITX_DSI_PLL_CON1, 0x00000000);
    OUTREG32(&DSI_PHY_REG_DPI->MIPITX_DSI_PLL_CON2, 0x50000000);
    mdelay(1);

    return DPI_STATUS_OK;
}
EXPORT_SYMBOL(DPI_DisableMipiClk);

DPI_STATUS DPI_EnableSeqOutput(BOOL enable)
{
    return DPI_STATUS_OK;
}
EXPORT_SYMBOL(DPI_EnableSeqOutput);

DPI_STATUS DPI_SetRGBOrder(DPI_RGB_ORDER input, DPI_RGB_ORDER output)
{
    return DPI_STATUS_OK;
}
EXPORT_SYMBOL(DPI_SetRGBOrder);

DPI_STATUS DPI_ConfigPixelClk(DPI_POLARITY polarity, UINT32 divisor, UINT32 duty)
{
    DPI_REG_CLKCNTL ctrl;
/*
    ASSERT(divisor >= 2);
    ASSERT(duty > 0 && duty < divisor);

    ctrl.POLARITY = (DPI_POLARITY_FALLING == polarity) ? 1 : 0;
    ctrl.DIVISOR = divisor - 1;
    ctrl.DUTY = duty;
*/
    ctrl.CLK_POL = (DPI_POLARITY_FALLING == polarity) ? 1 : 0;

    OUTREG32(&DPI_REG->CLK_CNTL, AS_UINT32(&ctrl));
    return DPI_STATUS_OK;
}
EXPORT_SYMBOL(DPI_ConfigPixelClk);

DPI_STATUS DPI_ConfigLVDS(LCM_PARAMS *lcm_params)
{
	//DPI_REG_CLKCNTL ctrl = DPI_REG->CLK_CNTL;
//	DPI_REG_EMBSYNC_SETTING embsync;
//	ctrl.EMBSYNC_EN = lcm_params->dpi.embsync;
    //ctrl.DUAL_EDGE_SEL = (lcm_params->dpi.i2x_en>0)?1:0;
	//MASKREG32(DISPSYS_BASE + 0x60, 0x7, (lcm_params->dpi.i2x_en << 1)|(lcm_params->dpi.i2x_edge << 2) | 1);
    //OUTREG32(&DPI_REG->CLK_CNTL, AS_UINT32(&ctrl));

    return DPI_STATUS_OK;
}
EXPORT_SYMBOL(DPI_ConfigLVDS);

DPI_STATUS DPI_ConfigDualEdge(bool enable)
{
    DPI_REG_CLKCNTL ctrl = DPI_REG->CLK_CNTL;
    ctrl.DUAL_EDGE_SEL = enable;
    OUTREG32(&DPI_REG->CLK_CNTL, AS_UINT32(&ctrl));

    return DPI_STATUS_OK;
}
EXPORT_SYMBOL(DPI_ConfigDualEdge);

DPI_STATUS DPI_ConfigHDMI()
{

    return DPI_STATUS_OK;
}
EXPORT_SYMBOL(DPI_ConfigHDMI);

DPI_STATUS DPI_ConfigBG(BOOL enable, UINT32 BG_W, UINT32 GB_H)
{
    if(enable = false)
    {
        DPI_REG_CNTL pol = DPI_REG->CNTL;
        pol.DPI_BG_EN = 0;
        OUTREG32(&DPI_REG->CNTL, AS_UINT32(&pol));
        return DPI_STATUS_OK;
    }
    
    if((BG_W == 0) || (GB_H == 0))
    {
        return DPI_STATUS_OK;
    }
        
    DPI_REG_CNTL pol = DPI_REG->CNTL;
    pol.DPI_BG_EN = 1;
    OUTREG32(&DPI_REG->CNTL, AS_UINT32(&pol));

    DPI_REG_BG_HCNTL pol2 = DPI_REG->BG_HCNTL;
    pol2.BG_RIGHT = BG_W/2;
    pol2.BG_LEFT = BG_W/2;
    OUTREG32(&DPI_REG->BG_HCNTL, AS_UINT32(&pol2));

    DPI_REG_BG_VCNTL pol3 = DPI_REG->BG_VCNTL;
    pol3.BG_BOT = GB_H/2;
    pol3.BG_TOP = GB_H/2;
    OUTREG32(&DPI_REG->BG_VCNTL, AS_UINT32(&pol3));

    DPI_REG_BG_COLOR pol4 = DPI_REG->BG_COLOR;
    pol4.BG_B = 0;
    pol4.BG_G = 0;
    pol4.BG_R = 0;
    OUTREG32(&DPI_REG->BG_COLOR, AS_UINT32(&pol4));
    
    return DPI_STATUS_OK;
}
EXPORT_SYMBOL(DPI_ConfigBG);

DPI_STATUS DPI_ConfigInRBSwap(BOOL enable)
{
    DPI_REG_CNTL pol = DPI_REG->CNTL;
	 
    pol.IN_RB_SWAP = enable ? 1 : 0;
    OUTREG32(&DPI_REG->CNTL, AS_UINT32(&pol));
    
    return DPI_STATUS_OK;
}
EXPORT_SYMBOL(DPI_ConfigInRBSwap);

DPI_STATUS DPI_ConfigDataEnable(DPI_POLARITY polarity)
{

    DPI_REG_CLKCNTL pol = DPI_REG->CLK_CNTL;
    pol.DE_POL = (DPI_POLARITY_FALLING == polarity) ? 1 : 0;
    OUTREG32(&DPI_REG->CLK_CNTL, AS_UINT32(&pol));
    return DPI_STATUS_OK;
}
EXPORT_SYMBOL(DPI_ConfigDataEnable);

DPI_STATUS DPI_ConfigVsync(DPI_POLARITY polarity, UINT32 pulseWidth, UINT32 backPorch,
                           UINT32 frontPorch)
{
    DPI_REG_TGEN_VWIDTH_LODD vwidth_lodd  = DPI_REG->TGEN_VWIDTH_LODD;
	DPI_REG_TGEN_VPORCH_LODD vporch_lodd  = DPI_REG->TGEN_VPORCH_LODD;
    DPI_REG_CLKCNTL pol = DPI_REG->CLK_CNTL;
    
	pol.VSYNC_POL = (DPI_POLARITY_FALLING == polarity) ? 1 : 0;
    vwidth_lodd.VPW_LODD = pulseWidth;
    vporch_lodd.VBP_LODD= backPorch;
    vporch_lodd.VFP_LODD= frontPorch;

    OUTREG32(&DPI_REG->CLK_CNTL, AS_UINT32(&pol));
    OUTREG32(&DPI_REG->TGEN_VWIDTH_LODD, AS_UINT32(&vwidth_lodd));
	OUTREG32(&DPI_REG->TGEN_VPORCH_LODD, AS_UINT32(&vporch_lodd));

    return DPI_STATUS_OK;
}
EXPORT_SYMBOL(DPI_ConfigVsync);


DPI_STATUS DPI_ConfigHsync(DPI_POLARITY polarity, UINT32 pulseWidth, UINT32 backPorch,
                           UINT32 frontPorch)
{
    DPI_REG_TGEN_HPORCH hporch = DPI_REG->TGEN_HPORCH;
    DPI_REG_CLKCNTL pol = DPI_REG->CLK_CNTL;
    
	pol.HSYNC_POL = (DPI_POLARITY_FALLING == polarity) ? 1 : 0;
    DPI_REG->TGEN_HWIDTH = pulseWidth;
    hporch.HBP = backPorch;
    hporch.HFP = frontPorch;

	pol.HSYNC_POL = (DPI_POLARITY_FALLING == polarity) ? 1 : 0;
    //DPI_REG->TGEN_HWIDTH = pulseWidth;
    OUTREG32(&DPI_REG->TGEN_HWIDTH,pulseWidth);
    hporch.HBP = backPorch;
    hporch.HFP = frontPorch;
    OUTREG32(&DPI_REG->CLK_CNTL, AS_UINT32(&pol));
    OUTREG32(&DPI_REG->TGEN_HPORCH, AS_UINT32(&hporch));

    return DPI_STATUS_OK;
}
EXPORT_SYMBOL(DPI_ConfigHsync);

DPI_STATUS DPI_EnableColorBar(BOOL enable)
{
	DPI_REG_PATTERN pattern = DPI_REG->PATTERN;

	if(enable)
	{
    		pattern.PAT_EN = 1;
		pattern.PAT_SEL = 4; // Color Bar
	}
	else
	{
		pattern.PAT_EN = 0;
		pattern.PAT_SEL = 0; // Color Bar
	}

    OUTREG32(&DPI_REG->PATTERN, AS_UINT32(&pattern));

    return DPI_STATUS_OK;
}
EXPORT_SYMBOL(DPI_EnableColorBar);


DPI_STATUS DPI_FBEnable(DPI_FB_ID id, BOOL enable)
{
    return DPI_STATUS_OK;
}
EXPORT_SYMBOL(DPI_FBEnable);

DPI_STATUS DPI_FBSyncFlipWithLCD(BOOL enable)
{
    return DPI_STATUS_OK;
}
EXPORT_SYMBOL(DPI_FBSyncFlipWithLCD);

DPI_STATUS DPI_SetDSIMode(BOOL enable)
{
    return DPI_STATUS_OK;
}


BOOL DPI_IsDSIMode(void)
{
//	return DPI_REG->CNTL.DSI_MODE ? TRUE : FALSE;
	return FALSE;
}


DPI_STATUS DPI_FBSetFormat(DPI_FB_FORMAT format)
{
    return DPI_STATUS_OK;
}
EXPORT_SYMBOL(DPI_FBSetFormat);

DPI_FB_FORMAT DPI_FBGetFormat(void)
{
    return 0;
}
EXPORT_SYMBOL(DPI_FBGetFormat);


DPI_STATUS DPI_FBSetSize(UINT32 width, UINT32 height)
{
    DPI_REG_SIZE size;
    size.WIDTH = width;
    size.HEIGHT = height;

    OUTREG32(&DPI_REG->SIZE, AS_UINT32(&size));

    return DPI_STATUS_OK;
}
EXPORT_SYMBOL(DPI_FBSetSize);

DPI_STATUS DPI_FBSetAddress(DPI_FB_ID id, UINT32 address)
{
    return DPI_STATUS_OK;
}
EXPORT_SYMBOL(DPI_FBSetAddress);

DPI_STATUS DPI_FBSetPitch(DPI_FB_ID id, UINT32 pitchInByte)
{
    return DPI_STATUS_OK;
}
EXPORT_SYMBOL(DPI_FBSetPitch);

DPI_STATUS DPI_SetFifoThreshold(UINT32 low, UINT32 high)
{
    return DPI_STATUS_OK;
}
EXPORT_SYMBOL(DPI_SetFifoThreshold);


DPI_STATUS DPI_DumpRegisters(void)
{
    UINT32 i;

    DISP_LOG_PRINT(ANDROID_LOG_WARN, "DPI", "---------- Start dump DPI registers ----------\n");

    for (i = 0; i < sizeof(DPI_REGS); i += 4)
    {
        DISP_LOG_PRINT(ANDROID_LOG_WARN, "DPI", "DPI+%04x : 0x%08x\n", i, INREG32(DPI_BASE + i));
    }

    return DPI_STATUS_OK;
}

EXPORT_SYMBOL(DPI_DumpRegisters);

UINT32 DPI_GetCurrentFB(void)
{
	return 0;
}
EXPORT_SYMBOL(DPI_GetCurrentFB);

DPI_STATUS DPI_Capture_Framebuffer(unsigned int pvbuf, unsigned int bpp)
{
	unsigned int mva;
    unsigned int ret = 0;
    M4U_PORT_STRUCT portStruct;

    struct disp_path_config_mem_out_struct mem_out = {0};
    printk("enter DPI_Capture_FB!\n");

    if(bpp == 32)
        mem_out.outFormat = eARGB8888;
    else if(bpp == 16)
        mem_out.outFormat = eRGB565;
    else if(bpp == 24)
        mem_out.outFormat = eRGB888;
    else
        printk("DPI_Capture_FB, fb color format not support\n");

    printk("before alloc MVA: va = 0x%x, size = %d\n", pvbuf, lcm_params->height*lcm_params->width*bpp/8);
    ret = m4u_alloc_mva(DISP_WDMA,
                        pvbuf,
                        lcm_params->height*lcm_params->width*bpp/8,
                        0,
                        0,
                        &mva);
    if(ret!=0)
    {
        printk("m4u_alloc_mva() fail! \n");
        return DPI_STATUS_OK;
    }
    printk("addr=0x%x, format=%d \n", mva, mem_out.outFormat);

    m4u_dma_cache_maint(DISP_WDMA,
                        (void *)pvbuf,
                        lcm_params->height*lcm_params->width*bpp/8,
                        DMA_BIDIRECTIONAL);

    portStruct.ePortID = DISP_WDMA;           //hardware port ID, defined in M4U_PORT_ID_ENUM
    portStruct.Virtuality = 1;
    portStruct.Security = 0;
    portStruct.domain = 0;            //domain : 0 1 2 3
    portStruct.Distance = 1;
    portStruct.Direction = 0;
    m4u_config_port(&portStruct);

    mem_out.enable = 1;
    mem_out.dstAddr = mva;
    mem_out.srcROI.x = 0;
    mem_out.srcROI.y = 0;
    mem_out.srcROI.height= lcm_params->height;
    mem_out.srcROI.width= lcm_params->width;

    disp_path_get_mutex();
    disp_path_config_mem_out(&mem_out);
    printk("Wait DPI idle \n");

    disp_path_release_mutex();

    msleep(20);

    disp_path_get_mutex();
    mem_out.enable = 0;
    disp_path_config_mem_out(&mem_out);

    disp_path_release_mutex();

    portStruct.ePortID = DISP_WDMA;           //hardware port ID, defined in M4U_PORT_ID_ENUM
    portStruct.Virtuality = 0;
    portStruct.Security = 0;
    portStruct.domain = 0;            //domain : 0 1 2 3
    portStruct.Distance = 1;
    portStruct.Direction = 0;
    m4u_config_port(&portStruct);

    m4u_dealloc_mva(DISP_WDMA,
                    pvbuf,
                        lcm_params->height*lcm_params->width*bpp/8,
                        mva);

    return DPI_STATUS_OK;
}

static void _DPI_RDMA0_IRQ_Handler(unsigned int param)
{
    if (param & 4)
    {
        dpiIntCallback(DISP_DPI_SCREEN_UPDATE_END_INT);
    }
    if (param & 8)
    {
    }
    if (param & 2)
    {
        dpiIntCallback(DISP_DPI_SCREEN_UPDATE_START_INT);
#if (ENABLE_DPI_INTERRUPT == 0)
        if(dpiIntCallback)
            dpiIntCallback(DISP_DPI_VSYNC_INT);
#endif
    }
    if (param & 0x20)
    {
        dpiIntCallback(DISP_DPI_TARGET_LINE_INT);
    }
}

static void _DPI_MUTEX_IRQ_Handler(unsigned int param)
{
    if(dpiIntCallback)
    {
        if (param & 1)
        {
            dpiIntCallback(DISP_DPI_REG_UPDATE_INT);
        }
    }
}

DPI_STATUS DPI_EnableInterrupt(DISP_INTERRUPT_EVENTS eventID)
{
#if ENABLE_DPI_INTERRUPT
    switch(eventID)
    {
        case DISP_DPI_VSYNC_INT:
            //DPI_REG->INT_ENABLE.VSYNC = 1;
            OUTREGBIT(DPI_REG_INTERRUPT,DPI_REG->INT_ENABLE,VSYNC,1);
            break;
        case DISP_DPI_TARGET_LINE_INT:
            disp_register_irq(DISP_MODULE_RDMA, _DPI_RDMA0_IRQ_Handler);
            break;
        case DISP_DPI_SCREEN_UPDATE_START_INT:
        	disp_register_irq(DISP_MODULE_RDMA, _DPI_RDMA0_IRQ_Handler);
        	break;
        case DISP_DPI_SCREEN_UPDATE_END_INT:
        	disp_register_irq(DISP_MODULE_RDMA, _DPI_RDMA0_IRQ_Handler);
        	break;
        case DISP_DPI_REG_UPDATE_INT:
            disp_register_irq(DISP_MODULE_MUTEX, _DPI_MUTEX_IRQ_Handler);
            break;
        default:
            return DPI_STATUS_ERROR;
    }

    return DPI_STATUS_OK;
#else
    switch(eventID)
    {
        case DISP_DPI_VSYNC_INT:
            OUTREGBIT(DPI_REG_INTERRUPT,DPI_REG->INT_ENABLE,VSYNC,1);
            disp_register_irq(DISP_MODULE_RDMA, _DPI_RDMA0_IRQ_Handler);
            break;
        case DISP_DPI_TARGET_LINE_INT:
            disp_register_irq(DISP_MODULE_RDMA, _DPI_RDMA0_IRQ_Handler);
            break;
        case DISP_DPI_REG_UPDATE_INT:
            disp_register_irq(DISP_MODULE_MUTEX, _DPI_MUTEX_IRQ_Handler);
            break;
        default:
            return DPI_STATUS_ERROR;
    }

    return DPI_STATUS_OK;
    ///TODO: warning log here
    //return DPI_STATUS_ERROR;
#endif
}


DPI_STATUS DPI_SetInterruptCallback(void (*pCB)(DISP_INTERRUPT_EVENTS))
{
    dpiIntCallback = pCB;

    return DPI_STATUS_OK;
}


DPI_STATUS DPI_FMDesense_Query(void)
{
    return DPI_STATUS_ERROR;
}

DPI_STATUS DPI_FM_Desense(unsigned long freq)
{
    return DPI_STATUS_OK;
}

DPI_STATUS DPI_Reset_CLK(void)
{
	return DPI_STATUS_OK;
}

DPI_STATUS DPI_Get_Default_CLK(unsigned int *clk)
{
    return DPI_STATUS_OK;
}

DPI_STATUS DPI_Get_Current_CLK(unsigned int *clk)
{
    return DPI_STATUS_OK;
}

DPI_STATUS DPI_Change_CLK(unsigned int clk)
{
    return DPI_STATUS_OK;
}

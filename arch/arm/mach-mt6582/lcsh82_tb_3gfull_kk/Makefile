# Project-specific sources: the custgen-selected set actually built by the stock kernel.
include $(srctree)/drivers/misc/mediatek/helpers/common.mk

# Selection defines for the customizable drivers
subdir-ccflags-y += -DCPT_CLAA070WP0B_HIMAX -DAUO_B070EAN014_HIMAX \
                    -DA5142_MIPI_RAW -DHI257_YUV -DGC2235_RAW \
                    -DDUMMY_FLASHLIGHT -DDUMMY_LENS

# Board definition
obj-y += core/board.o

obj-y += accelerometer/accel.o \
    accelerometer/bma222E.o \
    accelerometer/cust_acc.o \
    alsps/cust_alsps.o \
    alsps/sn5001.o \
    cam_cal/dummy_cam_cal.o \
    camera/dummyds.o \
    camera/kd_camera_hw.o \
    core/board.o \
    dct/pmic_drv.o \
    eeprom/dummy_eeprom.o \
    flashlight/dummy_flashlight.o \
    flashlight/kd_flashlightlist.o \
    flashlight/main_strobe_2ndPart_2.o \
    flashlight/strobe_part_id.o \
    flashlight/sub_strobe.o \
    flashlight/sub_strobe_2ndPart_2.o \
    headset/accdet_custom.o \
    imgsensor/a5142_otp.o \
    imgsensor/a5142mipi_Sensor.o \
    imgsensor/gc2235_Sensor.o \
    imgsensor/hi257yuv_Sensor.o \
    imgsensor/kd_sensorlist.o \
    kpd/mtk_kpd_bkl.o \
    lcm/AUO_B070EAN014_HIMAX.o \
    lcm/CPT_CLAA070WP0B_HIMAX.o \
    lcm/mt65xx_lcm_list.o \
    leds/cust_leds.o \
    lens/dummy_lens.o \
    sound/yusu_android_speaker.o \
    ssw/sim_switch.o \
    touchpanel/goodix_tool.o \
    touchpanel/gt9xx_driver.o \
    touchpanel/gt9xx_openshort.o \
    touchpanel/gt9xx_update.o \
    touchpanel/mtk_tpd.o \
    touchpanel/tpd_button.o \
    touchpanel/tpd_calibrate.o \
    touchpanel/tpd_debug.o \
    touchpanel/tpd_default.o \
    touchpanel/tpd_init.o \
    touchpanel/tpd_misc.o \
    touchpanel/tpd_setting.o \
    vibrator/cust_vibrator.o

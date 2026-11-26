obj-$(CONFIG_THERMAL) += mtk_thermal_platform.o

ifneq ($(CONFIG_MTK_COMBO_WIFI),)
    obj-$(CONFIG_THERMAL) += mtk_ts_wmt.o
endif

# Only included in non-EVB projects
ifeq ($(CONFIG_MTK_SMART_BATTERY), y)
    obj-$(CONFIG_THERMAL) += mtk_cooler_bcct.o
endif

obj-y += mtk_ts_cpu.o

ifeq ($(CONFIG_THERMAL),y)
    obj-y += mtk_ts_pmic.o
    obj-y += mtk_ts_abb.o
    obj-y += mtk_ts_xtal.o

    ifeq ($(CONFIG_MTK_SMART_BATTERY), y)
        obj-y += mtk_ts_battery.o
    endif
endif

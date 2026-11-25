#include <linux/leds.h>
//#include <leds_sw.h>
#include <cust_leds.h>
#include <cust_leds_def.h>

/* MTK high-resolution brightness level adjustment */
#define LED_INCREASE_LED_LEVEL_MTKPATCH
#define LED_RESERVEBIT_SHIFT 16
#define LED_RESERVEBIT_PATTERN 1

/****************************************************************************
 * LED DRV functions
 ***************************************************************************/
extern int mt65xx_leds_brightness_set(enum mt65xx_led_type type, enum led_brightness level);


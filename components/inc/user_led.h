#ifndef __USER_LED_H__
#define __USER_LED_H__
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

extern void led_init(void);
extern void led_frequently(void);
extern void led_breathe(void);
extern void led_flash(void);




#endif
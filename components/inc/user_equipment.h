#ifndef __USER_EQUIPMENT_H__
#define __USER_EQUIPMENT_H__
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "user_led.h"
#include "usart1.h"


extern void equipment_init(void);//输入传感器初始化
extern void equipment_control_task(void);//传感器输入信号控制



#endif
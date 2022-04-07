#ifndef __USART1_H__
#define __USART1_H__
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"
#include "driver/uart.h"
#include "string.h"
#include "driver/gpio.h"


extern void usart1_init(void);
extern int sendData(const char* logName, const char* data);
extern void tx_task(void *arg);
extern void rx_task(void *arg);




#endif
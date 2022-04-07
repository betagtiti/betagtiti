#ifndef __USER_MQTT_H__
#define __USER_MQTT_H__


// #include <stdio.h>
// #include <stdint.h>
// #include <stddef.h>
// #include <string.h>
// #include "esp_wifi.h"
// #include "esp_system.h"
// #include "nvs_flash.h"
// #include "esp_event.h"
// #include "esp_netif.h"

// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
// #include "freertos/semphr.h"
// #include "freertos/queue.h"

// #include "lwip/sockets.h"
// #include "lwip/dns.h"
// #include "lwip/netdb.h"

// #include "esp_log.h"
// #include "mqtt_client.h"
// #include "os.h"


void user_mqtt_app_start(void);



void mqtt_test_task(void *pvParameters);

#endif
// #pragma once

// #ifdef __cplusplus
// extern "C" {
// // #endif

// void user_mqtt_app_start(void);


// #ifdef __cplusplus
// }
// #endif


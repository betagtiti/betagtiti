// #include "user_wifi.h"

// #define EXAMPLE_ESP_WIFI_SSID      "DerkIOT"
// #define EXAMPLE_ESP_WIFI_PASS      "88888888dk."
// #define EXAMPLE_ESP_MAXIMUM_RETRY  1 

// // char * esp_wifi_name; //WI-FI名称
// // strcpy(esp_wifi_name, "LSWL");
// // char * esp_wifi_passwoord;//WI-FI密码
// // strcpy(esp_wifi_passwoord, "66666666");


// // //wifi连上事件
// // static EventGroupHandle_t wifi_event_group;

// /* FreeRTOS event group to signal when we are connected*/
// static EventGroupHandle_t s_wifi_event_group;

// /* The event group allows multiple bits for each event, but we only care about two events:
//  * - we are connected to the AP with an IP
//  * - we failed to connect after the maximum amount of retries */
// #define WIFI_CONNECTED_BIT BIT0
// #define WIFI_FAIL_BIT      BIT1


// // const static int CONNECTED_BIT = BIT0;

// static const char *TAG = "wifi station";

// static int s_retry_num = 0;


// // char esp32_wifi_ssid[16] = "DerkIOT";//初始WIFI名称
// // char esp32_wifi_password[16] = "88888888dk.";//初始WIFI密码

// // strcpy(esp32_wifi_ssid, "LSWL")
// // strcpy(esp32_wifi_password, "66666666");


// static void event_handler(void* arg, esp_event_base_t event_base,
//                                 int32_t event_id, void* event_data)
// {
//     if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
//         esp_wifi_connect();
//     } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
//         if (s_retry_num < EXAMPLE_ESP_MAXIMUM_RETRY) {
//             esp_wifi_connect();
//             s_retry_num++;
//             ESP_LOGI(TAG, "retry to connect to the AP");
//         } else {
//             xEventGroupSetBits(s_wifi_event_group, WIFI_FAIL_BIT);
//         }
//         ESP_LOGI(TAG,"connect to the AP fail");
//     } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
//         ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
//         ESP_LOGI(TAG, "got ip:" IPSTR, IP2STR(&event->ip_info.ip));
//         s_retry_num = 0;
//         xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
//     }
// }

// // static esp_err_t wifi_event_handler(void *ctx, system_event_t *event)
// // {
// //     switch (event->event_id) {
// //         case SYSTEM_EVENT_STA_START:
// //             esp_wifi_connect();
// //             break;
// //         case SYSTEM_EVENT_STA_GOT_IP:
// //             xEventGroupSetBits(wifi_event_group, CONNECTED_BIT);

// //             break;
// //         case SYSTEM_EVENT_STA_DISCONNECTED:
// //             esp_wifi_connect();
// //             xEventGroupClearBits(wifi_event_group, CONNECTED_BIT);
// //             break;
// //         default:
// //             break;
// //     }
// //     return ESP_OK;
// // }

// //wifi初始化
// // static void wifi_init(void)
// // {
// //     tcpip_adapter_init();
// //     wifi_event_group = xEventGroupCreate();
// //     mqtt_event_group = xEventGroupCreate();
// //     ESP_ERROR_CHECK(esp_event_loop_init(wifi_event_handler, NULL));
// //     wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
// //     ESP_ERROR_CHECK(esp_wifi_init(&cfg));
// //     ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
// //     wifi_config_t wifi_config = {
// //         .sta = {
// //             .ssid = "LSWL",
// //             .password = "66666666",
// //         },
// //     };
// //     ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
// //     ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config));
// //     ESP_LOGI(TAG, "start the WIFI SSID:[%s]", wifi_config.sta.ssid);
// //     ESP_ERROR_CHECK(esp_wifi_start());
// //     ESP_LOGI(TAG, "Waiting for wifi");
// //     //等待wifi连上
// //     xEventGroupWaitBits(wifi_event_group, CONNECTED_BIT, false, true, portMAX_DELAY);
// // }

// void wifi_init_sta1(void)
// {
//     s_wifi_event_group = xEventGroupCreate();

//     /*Initialize the underlying TCP/IP stack. 
//     NOTE:This function should be called exactly once from application code, when the application starts up. */
//     ESP_ERROR_CHECK(esp_netif_init());

//     /*Create default event loop. */
//     ESP_ERROR_CHECK(esp_event_loop_create_default());

// 	/*Creates default WIFI STA. In case of any init error this API aborts. */
//     esp_netif_create_default_wifi_sta();
 
//     /*Init WiFi Alloc resource for WiFi driver, such as WiFi control structure, RX/TX buffer, WiFi NVS structure etc, 
//     this WiFi also start WiFi task. */
//     wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
//     ESP_ERROR_CHECK(esp_wifi_init(&cfg));

//     esp_event_handler_instance_t instance_any_id;
//     esp_event_handler_instance_t instance_got_ip;
//     ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
//                                                         ESP_EVENT_ANY_ID,
//                                                         &event_handler,
//                                                         NULL,
//                                                         &instance_any_id));
//     ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,
//                                                         IP_EVENT_STA_GOT_IP,
//                                                         &event_handler,
//                                                         NULL,
//                                                         &instance_got_ip));
//     //配置wifi连接密码
//     wifi_config_t wifi_config = { 
//         .sta = {
//                 // strcpy(wifi_config.sta.ssid, (uint8_t *)esp32_wifi_ssid),
//                 // strcpy(wifi_config.sta.password, (uint8_t *)esp32_wifi_password),
//             .ssid = EXAMPLE_ESP_WIFI_SSID,
//             .password =EXAMPLE_ESP_WIFI_PASS,
//             /* Setting a password implies station will connect to all security modes including WEP/WPA.
//              * However these modes are deprecated and not advisable to be used. Incase your Access point
//              * doesn't support WPA2, these mode can be enabled by commenting below line */
// 	     .threshold.authmode = WIFI_AUTH_WPA2_PSK,
//             .pmf_cfg = {
//                 .capable = true,
//                 .required = false
//             },
//         },
      
//     };
  

//     // //配置wifi连接密码
//     // wifi_config_t wifi_config ;
//     // strcpy((char *)wifi_config.sta.ssid, esp32_wifi_ssid);
//     // strcpy((char *)wifi_config.sta.password, esp32_wifi_password);
//     // wifi_config.sta.threshold.authmode = WIFI_AUTH_WPA2_PSK;
//     // wifi_config.sta.pmf_cfg.capable = true;
//     // wifi_config.sta.pmf_cfg.required = false;   
    




//     ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA) );
//     ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config) );
//     ESP_ERROR_CHECK(esp_wifi_start() );

//     ESP_LOGI(TAG, "wifi_init_sta finished.");

//     /* Waiting until either the connection is established (WIFI_CONNECTED_BIT) or connection failed for the maximum
//      * number of re-tries (WIFI_FAIL_BIT). The bits are set by event_handler() (see above) */
//     EventBits_t bits = xEventGroupWaitBits(s_wifi_event_group,
//             WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,
//             pdFALSE,
//             pdFALSE,
//             portMAX_DELAY);

//     /* xEventGroupWaitBits() returns the bits before the call returned, hence we can test which event actually
//      * happened. */
//     if (bits & WIFI_CONNECTED_BIT) {
//         ESP_LOGI(TAG, "connected to ap SSID:%s password:%s",
//                  EXAMPLE_ESP_WIFI_SSID, EXAMPLE_ESP_WIFI_PASS);
//     } else if (bits & WIFI_FAIL_BIT) {
//         ESP_LOGI(TAG, "Failed to connect to SSID:%s, password:%s",
//                  EXAMPLE_ESP_WIFI_SSID, EXAMPLE_ESP_WIFI_PASS);
//     } else {
//         ESP_LOGI(TAG, "UNEXPECTED EVENT");
//     }

//     /* The event will not be processed after unregister */
//     ESP_ERROR_CHECK(esp_event_handler_instance_unregister(IP_EVENT, IP_EVENT_STA_GOT_IP, instance_got_ip));
//     ESP_ERROR_CHECK(esp_event_handler_instance_unregister(WIFI_EVENT, ESP_EVENT_ANY_ID, instance_any_id));
//     vEventGroupDelete(s_wifi_event_group);
// }


#include "user_wifi.h"


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "esp_wifi.h"
#include "driver/gpio.h"
#include "nvs_flash.h"
#include "esp_smartconfig.h"

#define GPIO_LED_NUM 2

/* 宏定义WiFi更新标识码、WiFi名称和密码 */
#define MY_WIFI_UPDATE  1002               /* 对数值进行修改表示更新NVS的WiFi名称和密码*/
#define MY_WIFI_SSID    "DerkIOT"
#define MY_WIFI_PASSWD  "88888888dk."

static uint32_t change_nvs_flag = 2048;              /* 对数值进行修改表示更新NVS的WiFi名称和密码*/
char esp32_wifi_ssid[16] = "DerkIOT";//初始WIFI名称
char esp32_wifi_password[16] = "88888888dk.";//初始WIFI密码
extern int  change_wifi_flag;//WIFI更改信号标志

/* 宏定义WiFi连接事件标志位、连接失败标志位及智能配网标志位 */
#define WIFI_CONNECTED_BIT  BIT0
#define WIFI_FAIL_BIT       BIT1
#define SMART_CONFIG_BIT    BIT2

/* 定义一个WiFi连接事件标志组句柄 */
static EventGroupHandle_t wifi_event_group_handler;


void wifi_init_start(void)
{
   
        /* 打印Hello world! */
        printf("Hello world!\n");

        /* 初始化非易失性存储库 (NVS) */
        esp_err_t err = nvs_flash_init();
        if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
            /* NVS分区被截断，需要删除,然后重新初始化NVS */
            ESP_ERROR_CHECK(nvs_flash_erase());
            err = nvs_flash_init();
        }
        ESP_ERROR_CHECK( err );

        /* 定义一个NVS操作句柄 */
        nvs_handle wificfg_nvs_handler;
        /* 打开一个NVS命名空间 */
        ESP_ERROR_CHECK( nvs_open("WiFi_cfg", NVS_READWRITE, &wificfg_nvs_handler) );

        uint32_t wifi_update = 0;
        err = nvs_get_u32(wificfg_nvs_handler,"wifi_update",&wifi_update);
        if(change_nvs_flag == wifi_update )
            printf("wifi_cfg needn't to update. \n");
        else
        {
            printf("wifi_cfg update now... \n");
            ESP_ERROR_CHECK( nvs_set_str(wificfg_nvs_handler,"wifi_ssid",esp32_wifi_ssid) );
            ESP_ERROR_CHECK( nvs_set_str(wificfg_nvs_handler,"wifi_passwd",esp32_wifi_password) ); 
            ESP_ERROR_CHECK( nvs_set_u32(wificfg_nvs_handler,"wifi_update",change_nvs_flag) );
            printf("wifi_cfg update ok. \n");
        }
        ESP_ERROR_CHECK( nvs_commit(wificfg_nvs_handler) ); /* 提交 */
        nvs_close(wificfg_nvs_handler);                     /* 关闭 */

        printf("ESP_WIFI_MODE_STA \n");
        wifi_init_sta();
    
    

}

/* 系统事件循环处理函数 */
static void event_handler(void* arg, esp_event_base_t event_base,int32_t event_id, void* event_data)
{
    static int retry_num = 0;           /* 记录wifi重连次数 */
    /* 系统事件为WiFi事件 */
    if (event_base == WIFI_EVENT) 
    {
        if(event_id == WIFI_EVENT_STA_START)    /* 事件id为STA开始 */
            esp_wifi_connect();
        else if (event_id == WIFI_EVENT_STA_DISCONNECTED) /* 事件id为失去STA连接 */ 
        {
            esp_wifi_connect();
            retry_num++;
            printf("retry to connect to the AP %d times. \n",retry_num);
            if (retry_num > 10)  /* WiFi重连次数大于10 */
            {
                /* 将WiFi连接事件标志组的WiFi连接失败事件位置1 */
                xEventGroupSetBits(wifi_event_group_handler, WIFI_FAIL_BIT);
            }
            /* 清除WiFi连接成功标志位 */
            xEventGroupClearBits(wifi_event_group_handler, WIFI_CONNECTED_BIT);
        }
    } 
    /* 系统事件为ip地址事件，且事件id为成功获取ip地址 */
    else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) 
    {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data; /* 获取IP地址信息*/
        printf("got ip:%d.%d.%d.%d \n" , IP2STR(&event->ip_info.ip));  /* 打印ip地址*/
        retry_num = 0;                                              /* WiFi重连次数清零 */
        /* 将WiFi连接事件标志组的WiFi连接成功事件位置1 */
        xEventGroupSetBits(wifi_event_group_handler, WIFI_CONNECTED_BIT);
    }
    /* 系统事件为智能配网事件 */
    else if (event_base == SC_EVENT)
    {
        if(event_id == SC_EVENT_SCAN_DONE )             /* 开始扫描智能配网设备端 */
            printf("Scan done\n");
        else if(event_id == SC_EVENT_FOUND_CHANNEL)     /* 得到了智能配网通道 */
            printf("Found channel \n");
        else if(event_id == SC_EVENT_GOT_SSID_PSWD)     /* 得到了智能配网设备提供的ssid和password */
        {
            printf("smartconfig got SSID and password\n");
            /* 获取智能配网设备端提供的数据信息 */
            smartconfig_event_got_ssid_pswd_t *evt = (smartconfig_event_got_ssid_pswd_t *)event_data;
            /* 定义WiFi配置结构体和用了结收ssid和password的数组 */
            wifi_config_t wifi_config;
            char ssid[32] = { 0 };
            char password[64] = { 0 };

            bzero(&wifi_config, sizeof(wifi_config_t)); /* 将结构体数据清零 */
            /* 将智能配网设备发送来的WiFi的ssid、password及MAC地址复制到wifi_config */
            memcpy(wifi_config.sta.ssid, evt->ssid, sizeof(wifi_config.sta.ssid));
            memcpy(wifi_config.sta.password, evt->password, sizeof(wifi_config.sta.password));
            wifi_config.sta.bssid_set = evt->bssid_set;
            if (wifi_config.sta.bssid_set == true) {
                memcpy(wifi_config.sta.bssid, evt->bssid, sizeof(wifi_config.sta.bssid));
            }
            
            /* 打印WiFi名称和密码 */
            memcpy(ssid, evt->ssid, sizeof(evt->ssid));
            memcpy(password, evt->password, sizeof(evt->password));
            printf("SSID:%s \n", ssid);
            printf("PASSWORD:%s \n", password);

            /* 将得到的WiFi名称和密码存入NVS*/
            nvs_handle wificfg_nvs_handler;
            ESP_ERROR_CHECK( nvs_open("WiFi_cfg", NVS_READWRITE, &wificfg_nvs_handler) );
            ESP_ERROR_CHECK( nvs_set_str(wificfg_nvs_handler,"wifi_ssid",ssid) );
            ESP_ERROR_CHECK( nvs_set_str(wificfg_nvs_handler,"wifi_passwd",password) );
            ESP_ERROR_CHECK( nvs_commit(wificfg_nvs_handler) ); /* 提交 */
            nvs_close(wificfg_nvs_handler);                     /* 关闭 */ 
            printf("smartconfig save wifi_cfg to NVS .\n");

            /* 根据得到的WiFi名称和密码连接WiFi*/
            ESP_ERROR_CHECK( esp_wifi_disconnect() );
            ESP_ERROR_CHECK( esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config) );
            ESP_ERROR_CHECK( esp_wifi_connect() );
        }
        else if (event_id == SC_EVENT_SEND_ACK_DONE)    /* 智能配网成功，已经给智能配网设备发送应答*/
        {
            xEventGroupSetBits(wifi_event_group_handler, SMART_CONFIG_BIT);
        }
    }
}

static void smartconfig_init_start(void)
{
    /* 设置智能配网类型为 AirKiss */
    ESP_ERROR_CHECK( esp_smartconfig_set_type(SC_TYPE_AIRKISS) );
    /* 通过SMARTCONFIG_START_CONFIG_DEFAULT宏 来获取一个默认的smartconfig配置参数结构体变量*/
    smartconfig_start_config_t cfg = SMARTCONFIG_START_CONFIG_DEFAULT();
    /* 开始智能配网 */
    ESP_ERROR_CHECK( esp_smartconfig_start(&cfg) );
    printf("smartconfig start ....... \n");

    /* 使用事件标志组等待连接建立(WIFI_CONNECTED_BIT)或连接失败(WIFI_FAIL_BIT)事件 */
    EventBits_t uxBits;  /* 定义一个事件位变量来接收事件标志组等待函数的返回值 */
    /* 等待事件标志组，退出前清除设置的事件标志，任意置1就会返回*/
    uxBits = xEventGroupWaitBits(wifi_event_group_handler, WIFI_CONNECTED_BIT | SMART_CONFIG_BIT, 
                                    true, false, portMAX_DELAY); 
    if(uxBits & WIFI_CONNECTED_BIT) 
    {
        printf("WiFi Connected to ap ok. \n");
        esp_smartconfig_stop(); /* 关闭智能配网 */
    }
    if(uxBits & SMART_CONFIG_BIT) 
    {
        printf("smartconfig over \n");
        esp_smartconfig_stop(); /* 关闭智能配网 */
    }
}


void wifi_init_sta(void)
{
    /* 创建一个事件标志组 */
    wifi_event_group_handler = xEventGroupCreate();

    /* 初始化底层TCP/IP堆栈。在应用程序启动时，应该调用此函数一次。*/
    ESP_ERROR_CHECK(esp_netif_init());

    /* 创建默认事件循环,*/
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    /* 创建一个默认的WIFI-STA网络接口，如果初始化错误，此API将中止。*/
    esp_netif_create_default_wifi_sta();				

    /* 使用WIFI_INIT_CONFIG_DEFAULT() 来获取一个默认的wifi配置参数结构体变量*/
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    /* 根据cfg参数初始化wifi连接所需要的资源 */
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

	/* 将事件处理程序注册到系统默认事件循环，分别是WiFi事件、IP地址事件及smartconfig事件 */
    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(SC_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL));

    nvs_handle wificfg_nvs_handler; /* 定义一个NVS操作句柄 */
    char wifi_ssid[32] = { 0 };     /* 定义一个数组用来存储ssid*/
    char wifi_passwd[64] = { 0 };   /* 定义一个数组用来存储passwd */
    size_t len;
    /* 打开一个NVS命名空间 */
    ESP_ERROR_CHECK( nvs_open("WiFi_cfg", NVS_READWRITE, &wificfg_nvs_handler) );
    len = sizeof(wifi_ssid);    /* 从NVS中获取ssid */
    ESP_ERROR_CHECK( nvs_get_str(wificfg_nvs_handler,"wifi_ssid",wifi_ssid,&len) );
    len = sizeof(wifi_passwd);      /* 从NVS中获取ssid */
    ESP_ERROR_CHECK( nvs_get_str(wificfg_nvs_handler,"wifi_passwd",wifi_passwd,&len) );
    ESP_ERROR_CHECK( nvs_commit(wificfg_nvs_handler) ); /* 提交 */
    nvs_close(wificfg_nvs_handler);                     /* 关闭 */
    /* 设置WiFi连接的ssid和password参数 */
    wifi_config_t wifi_config;
    bzero(&wifi_config, sizeof(wifi_config_t)); /* 将结构体数据清零 */
    memcpy(wifi_config.sta.ssid, wifi_ssid, sizeof(wifi_config.sta.ssid));
    memcpy(wifi_config.sta.password, wifi_passwd, sizeof(wifi_config.sta.password));

    /* 设置WiFi的工作模式为 STA */
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA) );
    /* 设置WiFi连接的参数，主要是ssid和password */
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config));
    /* 启动WiFi连接 */
    ESP_ERROR_CHECK(esp_wifi_start());

    printf("wifi_init_sta finished. \n");

    /* 使用事件标志组等待连接建立(WIFI_CONNECTED_BIT)或连接失败(WIFI_FAIL_BIT)事件 */
    EventBits_t bits;  /* 定义一个事件位变量来接收事件标志组等待函数的返回值 */ 
    bits = xEventGroupWaitBits( wifi_event_group_handler,	        /* 需要等待的事件标志组的句柄 */
                                WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,	/* 需要等待的事件位 */
                                pdFALSE,    /* 为pdFALSE时，在退出此函数之前所设置的这些事件位不变，为pdFALSE则清零*/ 
                                pdFALSE,    /* 为pdFALSE时，设置的这些事件位任意一个置1就会返回，为pdFALSE则需全为1才返回 */
                                portMAX_DELAY);	                    /* 设置为最长阻塞等待时间，单位为时钟节拍 */

    /* 根据事件标志组等待函数的返回值获取WiFi连接状态 */
    if (bits & WIFI_CONNECTED_BIT)  /* WiFi连接成功事件 */
	{
        printf("connected to ap %s OK \n",wifi_ssid);
        vEventGroupDelete(wifi_event_group_handler);    /* 删除WiFi连接事件标志组，WiFi连接成功后不再需要 */
    } 
	else if (bits & WIFI_FAIL_BIT) /* WiFi连接失败事件 */
	{
        printf("Failed to connect to ap %s. \n",wifi_ssid);
        smartconfig_init_start();   /* 开启智能配网 */
    } 
	else 
    {
        printf("UNEXPECTED EVENT \n");  /* 没有等待到事件 */
        smartconfig_init_start();   /* 开启智能配网 */
    }
}











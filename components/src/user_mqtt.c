/* MQTT (over TCP) Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include "user_mqtt.h"

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "esp_wifi.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_log.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"
#include "freertos/event_groups.h"

#include "lwip/sockets.h"
#include "lwip/dns.h"
#include "lwip/netdb.h"

#include "esp_log.h"
#include "mqtt_client.h"
#include "usart1.h"

#include "esp_event.h"
#include "cJSON.h"


static const char *TAG = "MQTT_EXAMPLE";

static const char *TX_TASK_TAG = "MQTT_TX_TASK";

char * mqtt_data_topic;
char * mqtt_data_msg;

extern int mqtt_task_flag;


/*Broker Address：${YourProductKey}.iot-as-mqtt.${YourRegionId}.aliyuncs.com*/
#define   Aliyun_host       "gup19ypSI99.iot-as-mqtt.cn-shanghai.aliyuncs.com"
#define   Aliyun_port       1883
/*Client ID：     ${ClientID}|securemode=${Mode},signmethod=${SignMethod}|*/
#define   Aliyun_client_id  "112233|securemode=3,signmethod=hmacsha1|"
/*User Name：     ${DeviceName}&${ProductKey}*/
#define   Aliyun_username   "mqtt_temp&gup19ypSI99"
/*使用官网 MQTT_Password 工具生成*/
#define   Aliyun_password   "D8EFA4B9AA6D728349A9392B4642A5F5214A2CAB"

#define   AliyunSubscribeTopic_user_get     "/a1tUbQR2faQ/dev-esp32/user/get"
#define   AliyunPublishTopic_user_update    "/a1tUbQR2faQ/dev-esp32/user/update"

char mqtt_publish_data1[] = "mqtt connect ok ";
char mqtt_publish_data2[] = "mqtt subscribe successful";
char mqtt_publish_data3[] = "mqtt i am esp32";

esp_mqtt_client_handle_t client;

char local_data_buffer[1024] = {0}; 
char topicbuf[4096];
char databuf[1024];


static esp_err_t mqtt_event_handler_cb(esp_mqtt_event_handle_t event)
{
    
    //获取MQTT客户端结构体指针
    ////////esp_mqtt_client_handle_t data = event_data;
    esp_mqtt_client_handle_t client = event->client;
    int  msg_id;
	
    // your_context_t *context = event->context;
    switch (event->event_id) {
        //建立连接成功
        case MQTT_EVENT_CONNECTED:
            ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
            msg_id = esp_mqtt_client_publish(client, AliyunPublishTopic_user_update, mqtt_publish_data1, strlen(mqtt_publish_data1), 1, 0);
            ESP_LOGI(TAG, "sent publish successful, msg_id=%d", msg_id);
            //订阅主题
            msg_id = esp_mqtt_client_subscribe(client, AliyunSubscribeTopic_user_get, 0);
            ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);

            break;
        //客户端断开连接
        case MQTT_EVENT_DISCONNECTED:
            ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
            break;
        //主题订阅成功
        case MQTT_EVENT_SUBSCRIBED:
            ESP_LOGI(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
            msg_id = esp_mqtt_client_publish(client, AliyunPublishTopic_user_update, mqtt_publish_data2, strlen(mqtt_publish_data2), 0, 0);
            ESP_LOGI(TAG, "sent publish successful, msg_id=%d", msg_id);
           
            break;
        //取消订阅
        case MQTT_EVENT_UNSUBSCRIBED:
            ESP_LOGI(TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
            break;
        //主题发布成功
        case MQTT_EVENT_PUBLISHED:
            ESP_LOGI(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
            break;
        //已收到订阅的主题消息
        case MQTT_EVENT_DATA:
            ESP_LOGI(TAG, "MQTT_EVENT_DATA");
            printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
            printf("DATA=%.*s\r\n", event->data_len, event->data);
            // vTaskDelay(50);
           

            // char *str = strstr(event->data, "LEDSwitchC");
            // printf("%s\r\n", str);


            memcpy(topicbuf, event->topic, event->topic_len);
            memcpy(databuf, event->data, event->data_len);
            printf("TOPIC=%.*s\r\n", event->topic_len, topicbuf);
            printf("DATA=%.*s\r\n", event->data_len, databuf);

            cJSON *pJsonRoot = cJSON_Parse(databuf);//判断数据是否是jSON格式
            if(pJsonRoot != NULL)
            {   
                // int swtich1,swtich2,swtich3,swtich4,swtich5,swtich6,swtich7,swtich8,swtich9,
                //     swtich10,swtich11,swtich12,scene_swtich1,scene_swtich2,scene_swtich3,scene_swtich4,scene_swtich5,
                //     scene_swtich6,scene_swtich7,scene_swtich8,scene_swtich9,scene_swtich10;
                cJSON *pValue = cJSON_GetObjectItem(pJsonRoot, "params");       // 解析params字段内容
                if (!pValue) return;                                            // 判断params字段是否json格式
                else
                {
                    if(strstr(databuf, "LEDSwitch1") != NULL)
                    {
                        printf("data run LEDSwitch1\r\n");
                        cJSON *pLEDSwitch1 = cJSON_GetObjectItem(pValue, "LEDSwitch1");         // 解析子节点pValue的day字段字符串内容
                        if (!pLEDSwitch1) break;                                                // 判断day字段是否json格式
                        else
                        {
                            printf("data run here LEDSwitch1 yyy\r\n");
                            cJSON *pLEDSwitch1 = cJSON_GetObjectItem(pValue, "LEDSwitch1");         // 解析子节点pValue的day字段字符串内容
                            if(cJSON_IsNumber(pLEDSwitch1))                                      // 判断day字段是否为Number类型
                            {
                                char ptr_1[36]; 
                                sprintf(ptr_1, "LEDSwitch1=%d", pLEDSwitch1->valueint);
                                printf("%s\r\n", ptr_1); 
                                sendData(TAG, ptr_1);
                                printf("LEDSwitch1 ==========================%d\r\n", pLEDSwitch1->valueint);
                                memset(ptr_1,0,sizeof(ptr_1));                                   //清空数组数据 ，该函数在string.h中，其中a要为指针变量 
        
                            }
                            else
                            {
                                printf("string fail!!!!\r\n");
                                break;
                            }
                        }
                    }
                
                    if(strstr(databuf, "LEDSwitch2") != NULL)
                    {
                        //////////////////////////////////////////////////////////////////////////////////////
                        printf("data run LEDSwitch2\r\n");
                        cJSON *pLEDSwitch2 = cJSON_GetObjectItem(pValue, "LEDSwitch2");         // 解析子节点pValue的day字段字符串内容
                        if (!pLEDSwitch2) break;                                                // 判断day字段是否json格式
                        else
                        {
                            printf("data run here LEDSwitch2 yyy\r\n");
                            if(cJSON_IsNumber(pLEDSwitch2))                                      // 判断day字段是否为Number类型
                            {
                                char ptr_2[36]; 
                                sprintf(ptr_2, "LEDSwitch2=%d", pLEDSwitch2->valueint);
                                printf("%s\r\n", ptr_2); 
                                sendData(TAG, ptr_2);
                                printf("LEDSwitch2 ==========================%d\r\n", pLEDSwitch2->valueint);
        
                            }
                            else
                            {
                                printf("string fail!!!!\r\n");
                            }
                        }
                    }
                    if(strstr(databuf, "LEDSwitch3") != NULL)
                    {
                        ////////////////////////////////////////////////////////////////////////////////////////
                        printf("data run LEDSwitch3\r\n");
                        cJSON *pLEDSwitch3 = cJSON_GetObjectItem(pValue, "LEDSwitch3");         // 解析子节点pValue的day字段字符串内容
                        if (!pLEDSwitch3) break;                                                // 判断day字段是否json格式
                        else
                        {
                            printf("data run here LEDSwitch3 yyy\r\n");
                            if(cJSON_IsNumber(pLEDSwitch3))                                      // 判断day字段是否为Number类型
                            {
                                char ptr_3[36]; 
                                sprintf(ptr_3, "LEDSwitch3=%d", pLEDSwitch3->valueint);
                                printf("%s\r\n", ptr_3); 
                                sendData(TAG, ptr_3);
                                printf("LEDSwitch3 ==========================%d\r\n", pLEDSwitch3->valueint);
        
                            }
                            else
                            {
                                printf("string fail!!!!\r\n");
                            }
                        }
                    }
                    ////////////////////////////////////////////////////////////////////////////////////////
                    if(strstr(databuf, "LEDSwitch4") != NULL)
                    {
                        
                        printf("data run LEDSwitch4\r\n");
                        cJSON *pLEDSwitch4 = cJSON_GetObjectItem(pValue, "LEDSwitch4");         // 解析子节点pValue的day字段字符串内容
                        if (!pLEDSwitch4) break;                                                // 判断day字段是否json格式
                        else
                        {
                            printf("data run here LEDSwitch4 yyy\r\n");
                            if(cJSON_IsNumber(pLEDSwitch4))                                      // 判断day字段是否为Number类型
                            {
                                char ptr_4[36]; 
                                sprintf(ptr_4, "LEDSwitch4=%d", pLEDSwitch4->valueint);
                                printf("%s\r\n", ptr_4); 
                                sendData(TAG, ptr_4);
                                printf("LEDSwitch4 ==========================%d\r\n", pLEDSwitch4->valueint);
        
                            }
                            else
                            {
                                printf("string fail!!!!\r\n");
                            }
                        }
                    }
                    
                    ////////////////////////////////////////////////////////////////////////////////////////
                    if(strstr(databuf, "LEDSwitch5") != NULL)
                    {
                        printf("data run LEDSwitch5\r\n");
                        cJSON *pLEDSwitch5 = cJSON_GetObjectItem(pValue, "LEDSwitch5");         // 解析子节点pValue的day字段字符串内容
                        if (!pLEDSwitch5) break;                                                // 判断day字段是否json格式
                        else
                        {
                            printf("data run here LEDSwitch5 yyy\r\n");
                            if(cJSON_IsNumber(pLEDSwitch5))                                      // 判断day字段是否为Number类型
                            {
                                char ptr_5[36]; 
                                sprintf(ptr_5, "LEDSwitch5=%d", pLEDSwitch5->valueint);
                                printf("%s\r\n", ptr_5); 
                                sendData(TAG, ptr_5);
                                printf("LEDSwitch5 ==========================%d\r\n", pLEDSwitch5->valueint);
        
                            }
                            else
                            {
                                printf("string fail!!!!\r\n");
                            }
                        }
                    }
                    
                    ////////////////////////////////////////////////////////////////////////////////////////
                    if(strstr(databuf, "LEDSwitch6") != NULL)
                    {
                        printf("data run LEDSwitch6\r\n");
                        cJSON *pLEDSwitch6 = cJSON_GetObjectItem(pValue, "LEDSwitch6");         // 解析子节点pValue的day字段字符串内容
                        if (!pLEDSwitch6) break;                                                // 判断day字段是否json格式
                        else
                        {
                            printf("data run here LEDSwitch6 yyy\r\n");
                            if(cJSON_IsNumber(pLEDSwitch6))                                      // 判断day字段是否为Number类型
                            {
                                char ptr_6[36]; 
                                sprintf(ptr_6, "LEDSwitch6=%d", pLEDSwitch6->valueint);
                                printf("%s\r\n", ptr_6); 
                                sendData(TAG, ptr_6);
                                printf("LEDSwitch6 ==========================%d\r\n", pLEDSwitch6->valueint);
        
                            }
                            else
                            {
                                printf("string fail!!!!\r\n");
                            }
                        }
                    }
                    
                    ///////////////////////////////////////////////////////////////////////////////////////
                    if(strstr(databuf, "LEDSwitch7") != NULL)
                    {
                          printf("data run LEDSwitch7\r\n");
                        cJSON *pLEDSwitch7 = cJSON_GetObjectItem(pValue, "LEDSwitch7");         // 解析子节点pValue的day字段字符串内容
                        if (!pLEDSwitch7) break;                                                // 判断day字段是否json格式
                        else
                        {
                            printf("data run here LEDSwitch7 LEDSwitch7\r\n");
                            if(cJSON_IsNumber(pLEDSwitch7))                                      // 判断day字段是否为Number类型
                            {
                                char ptr_7[36]; 
                                sprintf(ptr_7, "LEDSwitch7=%d", pLEDSwitch7->valueint);
                                printf("%s\r\n", ptr_7); 
                                sendData(TAG, ptr_7);
                                printf("LEDSwitch7 ==========================%d\r\n", pLEDSwitch7->valueint);
        
                            }
                            else
                            {
                                printf("string fail!!!!\r\n");
                            }
                        }
                    }
                  
                    //////////////////////////////////////////////////////////////////////////////////////
                    if(strstr(databuf, "LEDSwitch8") != NULL)
                    {
                        printf("data run LEDSwitch8\r\n");
                        cJSON *pLEDSwitch8 = cJSON_GetObjectItem(pValue, "LEDSwitch8");         // 解析子节点pValue的day字段字符串内容
                        if (!pLEDSwitch8) break;                                                // 判断day字段是否json格式
                        else
                        {
                            printf("data run here LEDSwitch8 yyy\r\n");
                            if(cJSON_IsNumber(pLEDSwitch8))                                      // 判断day字段是否为Number类型
                            {
                                char ptr_8[36]; 
                                sprintf(ptr_8, "LEDSwitch8=%d", pLEDSwitch8->valueint);
                                printf("%s\r\n", ptr_8); 
                                sendData(TX_TASK_TAG, ptr_8);
                                printf("LEDSwitch8 ==========================%d\r\n", pLEDSwitch8->valueint);
        
                            }
                            else
                            {
                                printf("string fail!!!!\r\n");
                            }
                        }
                    }
                  
                    //////////////////////////////////////////////////////////////////////////////////////
                    if(strstr(databuf, "LEDSwitch9") != NULL)
                    {
                        printf("data run LEDSwitch9\r\n");
                        cJSON *pLEDSwitch9 = cJSON_GetObjectItem(pValue, "LEDSwitch9");         // 解析子节点pValue的day字段字符串内容
                        if (!pLEDSwitch9) break;                                                // 判断day字段是否json格式
                        else
                        {
                            printf("data run here LEDSwitch9 yyy\r\n");
                            if(cJSON_IsNumber(pLEDSwitch9))                                      // 判断day字段是否为Number类型
                            {
                                char ptr_9[36]; 
                                sprintf(ptr_9, "LEDSwitch9=%d", pLEDSwitch9->valueint);
                                printf("%s\r\n", ptr_9); 
                                sendData(TX_TASK_TAG, ptr_9);
                                printf("LEDSwitch9 ==========================%d\r\n", pLEDSwitch9->valueint);
        
                            }
                            else
                            {
                                printf("string fail!!!!\r\n");
                            }
                        }
                    }
                   
                    //////////////////////////////////////////////////////////////////////////////////////
                    if(strstr(databuf, "LEDSwitchA") != NULL)
                    {
                        printf("data run LEDSwitchA\r\n");
                        cJSON *pLEDSwitchA = cJSON_GetObjectItem(pValue, "LEDSwitchA");         // 解析子节点pValue的day字段字符串内容
                        if (!pLEDSwitchA) break;                                                // 判断day字段是否json格式
                        else
                        {
                            printf("data run here LEDSwitchA yyy\r\n");
                            if(cJSON_IsNumber(pLEDSwitchA))                                      // 判断day字段是否为Number类型
                            {
                                char ptr_10[36]; 
                                sprintf(ptr_10, "LEDSwitchA=%d", pLEDSwitchA->valueint);
                                printf("%s\r\n", ptr_10); 
                                sendData(TX_TASK_TAG, ptr_10);
                                printf("LEDSwitchA ==========================%d\r\n", pLEDSwitchA->valueint);
        
                            }
                            else
                            {
                                printf("string fail!!!!\r\n");
                            }
                        }
                    }
                   
                    //////////////////////////////////////////////////////////////////////////////////////
                    if(strstr(databuf, "LEDSwitchB") != NULL)
                    {
                        printf("data run LEDSwitchB\r\n");
                        cJSON *pLEDswtichB = cJSON_GetObjectItem(pValue, "LEDSwitchB");         // 解析子节点pValue的day字段字符串内容
                        if (!pLEDswtichB) break;                                                // 判断day字段是否json格式
                        else
                        {
                            printf("data run here LEDSwitchB yyy\r\n");
                            if(cJSON_IsNumber(pLEDswtichB))                                      // 判断day字段是否为Number类型
                            {
                                char ptr_11[36]; 
                                sprintf(ptr_11, "LEDSwitchB=%d", pLEDswtichB->valueint);
                                printf("%s\r\n", ptr_11); 
                                sendData(TX_TASK_TAG, ptr_11);
                                printf("LEDSwitchB ==========================%d\r\n", pLEDswtichB->valueint);
                            }
                            else
                            {
                                printf("string fail!!!!\r\n");
                            }
                        }
                    }
                   
                    ////////////////////////////////////////////////////////////////////////////////////////
                    if(strstr(databuf, "LEDSwitchC") != NULL)
                    {
                        printf("data run LEDSwitchC\r\n");
                        cJSON *pLEDswtichC = cJSON_GetObjectItem(pValue, "LEDSwitchC");         // 解析子节点pValue的day字段字符串内容
                        if (!pLEDswtichC) break;                                                // 判断day字段是否json格式
                        else
                        {
                            printf("data run here LEDSwitchCyyy\r\n");
                            if(cJSON_IsNumber(pLEDswtichC))                                      // 判断day字段是否为Number类型
                            {
                                char ptr_12[36]; 
                                sprintf(ptr_12, "LEDSwitchC=%d", pLEDswtichC->valueint);
                                printf("%s\r\n", ptr_12); 
                                sendData(TX_TASK_TAG, ptr_12);
                                printf("LEDSwitchC ==========================%d\r\n", pLEDswtichC->valueint);
                            }
                            else
                            {
                                printf("string fail!!!!\r\n");
                            }
                        }   
                    }
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    if(strstr(databuf, "LEDsceneS1") != NULL)
                    {
                        printf("data run LEDsceneS1\r\n");
                        cJSON *pLEDsceneS1 = cJSON_GetObjectItem(pValue, "LEDsceneS1");         // 解析子节点pValue的day字段字符串内容
                        if (!pLEDsceneS1) break;                                                // 判断day字段是否json格式
                        else
                        {
                            printf("data run here LEDsceneS1\r\n");
                            if(cJSON_IsNumber(pLEDsceneS1))                                      // 判断day字段是否为Number类型
                            {
                                char ptr_13[36]; 
                                sprintf(ptr_13, "LEDsceneS1=%d", pLEDsceneS1->valueint);
                                printf("%s\r\n", ptr_13); 
                                sendData(TX_TASK_TAG, ptr_13);
                                printf("LEDsceneS1 ==========================%d\r\n", pLEDsceneS1->valueint);
                            }
                            else
                            {
                                printf("string fail!!!!\r\n");
                            }
                        }   
                    }
                    ////////////////////////////////////////////////////////////////////////////////////////
                    if(strstr(databuf, "LEDsceneS2") != NULL)
                    {
                        printf("data run LEDsceneS2\r\n");
                        cJSON *pLEDsceneS2 = cJSON_GetObjectItem(pValue, "LEDsceneS2");         // 解析子节点pValue的day字段字符串内容
                        if (!pLEDsceneS2) break;                                                // 判断day字段是否json格式
                        else
                        {
                            printf("data run here LEDsceneS2yyy\r\n");
                            if(cJSON_IsNumber(pLEDsceneS2))                                      // 判断day字段是否为Number类型
                            {
                                char ptr_14[36]; 
                                sprintf(ptr_14, "LEDsceneS2=%d", pLEDsceneS2->valueint);
                                printf("%s\r\n", ptr_14); 
                                sendData(TX_TASK_TAG, ptr_14);
                                printf("LEDsceneS2 ==========================%d\r\n", pLEDsceneS2->valueint);
                            }
                            else
                            {
                                printf("string fail!!!!\r\n");
                            }
                        }   
                    }
                   ////////////////////////////////////////////////////////////////////////////////////////
                    if(strstr(databuf, "LEDsceneS3") != NULL)
                    {
                        printf("data run LEDsceneS3\r\n");
                        cJSON *pLEDsceneS3 = cJSON_GetObjectItem(pValue, "LEDsceneS3");         // 解析子节点pValue的day字段字符串内容
                        if (!pLEDsceneS3) break;                                                // 判断day字段是否json格式
                        else
                        {
                            printf("data run here LEDsceneS3yyy\r\n");
                            if(cJSON_IsNumber(pLEDsceneS3))                                      // 判断day字段是否为Number类型
                            {
                                char ptr_15[36]; 
                                sprintf(ptr_15, "LEDsceneS3=%d", pLEDsceneS3->valueint);
                                printf("%s\r\n", ptr_15); 
                                sendData(TX_TASK_TAG, ptr_15);
                                printf("LEDsceneS3 ==========================%d\r\n", pLEDsceneS3->valueint);
                            }
                            else
                            {
                                printf("string fail!!!!\r\n");
                            }
                        }   
                    }
                    ////////////////////////////////////////////////////////////////////////////////////////
                    if(strstr(databuf, "LEDsceneS4") != NULL)
                    {
                        printf("data run LEDsceneS4\r\n");
                        cJSON *pLEDsceneS4 = cJSON_GetObjectItem(pValue, "LEDsceneS4");         // 解析子节点pValue的day字段字符串内容
                        if (!pLEDsceneS4) break;                                                // 判断day字段是否json格式
                        else
                        {
                            printf("data run here LEDsceneS4yyy\r\n");
                            if(cJSON_IsNumber(pLEDsceneS4))                                      // 判断day字段是否为Number类型
                            {
                                char ptr_16[36]; 
                                sprintf(ptr_16, "LEDsceneS4=%d", pLEDsceneS4->valueint);
                                printf("%s\r\n", ptr_16); 
                                sendData(TX_TASK_TAG, ptr_16);
                                printf("LEDsceneS4 ==========================%d\r\n", pLEDsceneS4->valueint);
                            }
                            else
                            {
                                printf("string fail!!!!\r\n");
                            }
                        }   
                    }
                    ////////////////////////////////////////////////////////////////////////////////////////
                    if(strstr(databuf, "LEDsceneS5") != NULL)
                    {
                        printf("data run LEDsceneS5\r\n");
                        cJSON *pLEDsceneS5 = cJSON_GetObjectItem(pValue, "LEDsceneS5");         // 解析子节点pValue的day字段字符串内容
                        if (!pLEDsceneS5) break;                                                // 判断day字段是否json格式
                        else
                        {
                            printf("data run here LEDsceneS5yyy\r\n");
                            if(cJSON_IsNumber(pLEDsceneS5))                                      // 判断day字段是否为Number类型
                            {
                                char ptr_17[36]; 
                                sprintf(ptr_17, "LEDsceneS5=%d", pLEDsceneS5->valueint);
                                printf("%s\r\n", ptr_17); 
                                sendData(TX_TASK_TAG, ptr_17);
                                printf("LEDsceneS5 ==========================%d\r\n", pLEDsceneS5->valueint);
                            }
                            else
                            {
                                printf("string fail!!!!\r\n");
                            }
                        }   
                    }
                    ////////////////////////////////////////////////////////////////////////////////////////
                    if(strstr(databuf, "LEDsceneS6") != NULL)
                    {
                        printf("data run LEDsceneS6\r\n");
                        cJSON *pLEDsceneS6 = cJSON_GetObjectItem(pValue, "LEDsceneS6");         // 解析子节点pValue的day字段字符串内容
                        if (!pLEDsceneS6) break;                                                // 判断day字段是否json格式
                        else
                        {
                            printf("data run here LEDsceneS6yyy\r\n");
                            if(cJSON_IsNumber(pLEDsceneS6))                                      // 判断day字段是否为Number类型
                            {
                                char ptr_18[36]; 
                                sprintf(ptr_18, "LEDsceneS6=%d", pLEDsceneS6->valueint);
                                printf("%s\r\n", ptr_18); 
                                sendData(TX_TASK_TAG, ptr_18);
                                printf("LEDsceneS6 ==========================%d\r\n", pLEDsceneS6->valueint);
                            }
                            else
                            {
                                printf("string fail!!!!\r\n");
                            }
                        }   
                    }
                     ////////////////////////////////////////////////////////////////////////////////////////
                    if(strstr(databuf, "LEDsceneS7") != NULL)
                    {
                        printf("data run LEDsceneS7\r\n");
                        cJSON *pLEDsceneS7 = cJSON_GetObjectItem(pValue, "LEDsceneS7");         // 解析子节点pValue的day字段字符串内容
                        if (!pLEDsceneS7) break;                                                // 判断day字段是否json格式
                        else
                        {
                            printf("data run here LEDsceneS7yyy\r\n");
                            if(cJSON_IsNumber(pLEDsceneS7))                                      // 判断day字段是否为Number类型
                            {
                                char ptr_19[36]; 
                                sprintf(ptr_19, "LEDsceneS7=%d", pLEDsceneS7->valueint);
                                printf("%s\r\n", ptr_19); 
                                sendData(TX_TASK_TAG, ptr_19);
                                printf("LEDsceneS7 ==========================%d\r\n", pLEDsceneS7->valueint);
                            }
                            else
                            {
                                printf("string fail!!!!\r\n");
                            }
                        }   
                    }
                    ////////////////////////////////////////////////////////////////////////////////////////
                    if(strstr(databuf, "LEDsceneS8") != NULL)
                    {
                        printf("data run LEDsceneS8\r\n");
                        cJSON *pLEDsceneS8 = cJSON_GetObjectItem(pValue, "LEDsceneS8");         // 解析子节点pValue的day字段字符串内容
                        if (!pLEDsceneS8) break;                                                // 判断day字段是否json格式
                        else
                        {
                            printf("data run here LEDsceneS8yyy\r\n");
                            if(cJSON_IsNumber(pLEDsceneS8))                                      // 判断day字段是否为Number类型
                            {
                                char ptr_20[36]; 
                                sprintf(ptr_20, "LEDsceneS8=%d", pLEDsceneS8->valueint);
                                printf("%s\r\n", ptr_20); 
                                sendData(TX_TASK_TAG, ptr_20);
                                printf("LEDsceneS8 ==========================%d\r\n", pLEDsceneS8->valueint);
                            }
                            else
                            {
                                printf("string fail!!!!\r\n");
                            }
                        }   
                    }
                     ////////////////////////////////////////////////////////////////////////////////////////
                    if(strstr(databuf, "LEDsceneS9") != NULL)
                    {
                        printf("data run LEDsceneS9\r\n");
                        cJSON *pLEDsceneS9 = cJSON_GetObjectItem(pValue, "LEDsceneS9");         // 解析子节点pValue的day字段字符串内容
                        if (!pLEDsceneS9) break;                                                // 判断day字段是否json格式
                        else
                        {
                            printf("data run here LEDsceneS9yyy\r\n");
                            if(cJSON_IsNumber(pLEDsceneS9))                                      // 判断day字段是否为Number类型
                            {
                                char ptr_21[36]; 
                                sprintf(ptr_21, "LEDsceneS9=%d", pLEDsceneS9->valueint);
                                printf("%s\r\n", ptr_21); 
                                sendData(TX_TASK_TAG, ptr_21);
                                printf("LEDsceneS9 ==========================%d\r\n", pLEDsceneS9->valueint);
                            }
                            else
                            {
                                printf("string fail!!!!\r\n");
                            }
                        }   
                    }
                     ////////////////////////////////////////////////////////////////////////////////////////
                    if(strstr(databuf, "LEDsceneS10") != NULL)
                    {
                        printf("data run LEDsceneS10\r\n");
                        cJSON *pLEDsceneS10 = cJSON_GetObjectItem(pValue, "LEDsceneS10");         // 解析子节点pValue的day字段字符串内容
                        if (!pLEDsceneS10) break;                                                // 判断day字段是否json格式
                        else
                        {
                            printf("data run here LEDsceneS10yyy\r\n");
                            if(cJSON_IsNumber(pLEDsceneS10))                                      // 判断day字段是否为Number类型
                            {
                                char ptr_22[36]; 
                                sprintf(ptr_22, "LEDsceneS10=%d", pLEDsceneS10->valueint);
                                printf("%s\r\n", ptr_22); 
                                sendData(TX_TASK_TAG, ptr_22);
                                printf("LEDsceneS10 ==========================%d\r\n", pLEDsceneS10->valueint);
                            }
                            else
                            {
                                printf("string fail!!!!\r\n");
                            }
                        }   
                    }
                   
                    
                }
            }
            else
            {
                printf("data is not Json format\r\n");
            }



            // temp = strcmp(topicBuf,AliyunSubscribeTopic_ota_upgrade);

            
  



            break;
        //客户端遇到错误
        case MQTT_EVENT_ERROR:
            ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
            break;
        default:
            ESP_LOGI(TAG, "Other event id:%d", event->event_id);
            break;
    }
    return ESP_OK;
}

void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data) {
    ESP_LOGD(TAG, "Event dispatched from event loop base=%s, event_id=%d", base, event_id);
    mqtt_event_handler_cb(event_data);
}

//esp32发送消息到阿里云平台，间隔两秒一共发送6次
void mqtt_test_task(void *pvParameters)
{
    uint8_t num = 0;

    while(1)
    {
       esp_mqtt_client_publish(client, AliyunPublishTopic_user_update, mqtt_publish_data3, strlen(mqtt_publish_data3), 1, 0);
       vTaskDelay(2000 / portTICK_PERIOD_MS);
	   if(num++ > 5) break;
	}
    vTaskDelete(NULL);
}



void user_mqtt_app_start(void)
{
    //1,定义一个MQTT客户端结构体，输入MQTT的url
    esp_mqtt_client_config_t mqtt_cfg = {
    // .uri = "mqtt://120.78.240.8",
    // .username = "kuki",
    // .password = "88888888"
    
		.host = Aliyun_host,
		.port = Aliyun_port,
		.client_id = Aliyun_client_id,
		.username = Aliyun_username,
		.password = Aliyun_password,
    };

    //2,通过esp_mqtt_client_init获取一个MQTT客户端结构体指针，参数是MQTT客户端配置结构体
    client = esp_mqtt_client_init(&mqtt_cfg);
    //3,注册MQTT事件
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, client);
    //开启MQTT功能
    esp_mqtt_client_start(client);

    mqtt_task_flag = 1;
	
	// xTaskCreate(&mqtt_test_task, "mqtt_test_task", 4096, NULL, 5, NULL);
    // xTaskCreate(&mqtt_deal_task, "mqtt_deal_task", 4096, (void *)event, 6, NULL);
}




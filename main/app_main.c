#include "user_blufi.h"
#include "user_mqtt.h"
#include "user_wifi.h"
#include "usart1.h"
#include "user_equipment.h"


int mqtt_task_flag = 0;

void app_main(void)
{

  
    usart1_init();

    equipment_init(); 
    
    blufi_init_start();
    user_mqtt_app_start();
    
   
    
    xTaskCreate(equipment_control_task, "equipment_control_task", 4096, NULL, 5, NULL);


  
}
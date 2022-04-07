#include "user_equipment.h"

#define sensor_state_0     0      //传感器初始状态
#define sensor_state_1     1      //传感器触发状态
#define sensor_state_2     2      //传感器恢复初始状态

static const char *EQU_TAG = "EQUIPMENT_EXAMPLE";
extern int mqtt_task_flag;


//传感器抬起时返回传感器状态 
int read_sensor_state(void)
{
    static uint8_t sensor_state = sensor_state_0;   //状态          
	uint8_t sensor_state1;   	//传感器1电平
	uint8_t sensor_state2;   	//传感器2电平
	uint8_t sensor_state3;   	//传感器3电平              
	uint8_t sensor_state4;  	//传感器4电平
	uint8_t sensor_state5;   	//传感器5电平
	uint8_t sensor_state6;   	//传感器6电平                     
	
    static uint8_t sensor_state_return = 0;  //返回传感器状态 
	//读取电平值
   
	sensor_state1 = gpio_get_level(34);			//读传感器1电平
	sensor_state2 = gpio_get_level(35);			//读传感器2电平
	sensor_state3 = gpio_get_level(36);			//读传感器3电平
	sensor_state4 = gpio_get_level(37);			//读传感器4电平
	sensor_state5 = gpio_get_level(38);			//读传感器5电平
	sensor_state6 = gpio_get_level(39);			//读传感器6电平
	
	
	//判断
    switch (sensor_state)
    {
        case sensor_state_0:                 // 传感器初始态
			if (sensor_state1 || sensor_state2 || sensor_state3 || sensor_state4 || sensor_state5 || sensor_state6  )        //如果检测到任意一个传感器触发，转为传感器确认状态       
            {
                sensor_state = sensor_state_1;
            }	
            break;
                                
        case sensor_state_1:                   // 传感器触发状态
            if (sensor_state1)   //如果是传感器1触发
            {
                sensor_state_return = 11;         // 更新传感器状态 
            }
            else if(sensor_state2)//如果是传感器2触发
            {
				sensor_state_return = 21;         // 更新传感器状态 
             }
			else if(sensor_state3)//如果是传感器3触发
			{
				sensor_state_return = 31;         // 更新传感器状态 
			}
			else if(sensor_state4)//如果是传感器4触发
			{
				sensor_state_return = 41;         // 更新传感器状态 
			}
			else if(sensor_state5)//如果是传感器5触发
			{
				sensor_state_return = 51;         // 更新传感器状态 
			}
			else if(sensor_state6)//如果是传感器6触发
			{
				sensor_state_return = 61;         // 更新传感器状态 
			}
			
			sensor_state = sensor_state_2;// 状态转换到键释放状态
            break;
                                
        case sensor_state_2:                   // 传感器恢复初始状态
            if (!sensor_state1)   //如果是传感器1恢复
            {
                sensor_state_return = 10;         // 更新传感器状态  
            }
            else if(!sensor_state2)//如果是传感器2恢复
            {
				sensor_state_return = 20;         // 更新传感器状态 
             }
			else if(!sensor_state3)//如果是传感器3恢复
			{
				sensor_state_return = 30;         // 更新传感器状态 
			}
			else if(!sensor_state4)//如果是传感器4恢复
			{
				sensor_state_return = 40;         // 更新传感器状态 
			}
			else if(!sensor_state5)//如果是传感器5恢复
			{
				sensor_state_return = 50;         // 更新传感器状态 
			}
			else if(!sensor_state6)//如果是传感器6恢复
			{
				sensor_state_return = 60;         // 更新传感器状态 
			}
			
			sensor_state = sensor_state_0;// 传感器恢复未触发状态
            break;
   }
	return 0; 
}

void equipment_init(void)
{
    gpio_pad_select_gpio(GPIO_NUM_34);//人感2
    gpio_pad_select_gpio(GPIO_NUM_35);//SOS
    gpio_pad_select_gpio(GPIO_NUM_36);//门磁1
    gpio_pad_select_gpio(GPIO_NUM_37);//门磁2
    gpio_pad_select_gpio(GPIO_NUM_38);//取电
    gpio_pad_select_gpio(GPIO_NUM_39);//人感1
  
    gpio_set_direction(GPIO_NUM_34, GPIO_MODE_INPUT);
    gpio_set_direction(GPIO_NUM_35, GPIO_MODE_INPUT);
    gpio_set_direction(GPIO_NUM_36, GPIO_MODE_INPUT);
    gpio_set_direction(GPIO_NUM_37, GPIO_MODE_INPUT);
    gpio_set_direction(GPIO_NUM_38, GPIO_MODE_INPUT);
    gpio_set_direction(GPIO_NUM_39, GPIO_MODE_INPUT);

}

void signal_processing(int num)
{
    uint8_t pin_num = num;
    char htr[36];
    if(gpio_get_level(pin_num) == 1)
    {
        
        sprintf(htr, "jdq_%d=1", num);
        uart_write_bytes(UART_NUM_1, htr, strlen(htr)); 
        ESP_LOGI(EQU_TAG, "%d Wrote HIGH  %d bytes ", num, strlen(htr));
        
       
    } 
    if(gpio_get_level(pin_num) == 0)
    {
        sprintf(htr, "jdq_%d=0", num);
        uart_write_bytes(UART_NUM_1, htr, strlen(htr)); 
        ESP_LOGI(EQU_TAG, "LOW Wrote %d bytes", strlen(htr));
        
    }
}


void equipment_control_task(void)
{
    char htr[36];
    int sensor_state_flag = 0;
    ESP_LOGI(EQU_TAG, "------------------------------------------------------------------------");
    // char htr[1024];
    while(1)
    {
        sensor_state_flag = read_sensor_state();
        switch(sensor_state_flag)
        {
            case 11: 
                sprintf(htr, "jdq_%d=1", 34);
                uart_write_bytes(UART_NUM_1, htr, strlen(htr)); 
                ESP_LOGI(EQU_TAG, "Wrote HIGH  %d bytes ", strlen(htr));
                break;
            case 10: 
                sprintf(htr, "jdq_%d=0", 34);
                uart_write_bytes(UART_NUM_1, htr, strlen(htr)); 
                ESP_LOGI(EQU_TAG, "LOW Wrote %d bytes", strlen(htr));
                break;
/////////////////////////////////////////////////////////////////////////////////////////
            case 21: 
                sprintf(htr, "jdq_%d=1", 35);
                uart_write_bytes(UART_NUM_1, htr, strlen(htr)); 
                ESP_LOGI(EQU_TAG, "Wrote HIGH  %d bytes ", strlen(htr));
                break;
            case 20: 
                sprintf(htr, "jdq_%d=0", 35);
                uart_write_bytes(UART_NUM_1, htr, strlen(htr)); 
                ESP_LOGI(EQU_TAG, "LOW Wrote %d bytes", strlen(htr));
                break;
/////////////////////////////////////////////////////////////////////////////////////////
            case 31: 
                sprintf(htr, "jdq_%d=1", 36);
                uart_write_bytes(UART_NUM_1, htr, strlen(htr)); 
                ESP_LOGI(EQU_TAG, "Wrote HIGH  %d bytes ", strlen(htr));
                break;
            case 30: 
                sprintf(htr, "jdq_%d=0", 36);
                uart_write_bytes(UART_NUM_1, htr, strlen(htr)); 
                ESP_LOGI(EQU_TAG, "LOW Wrote %d bytes", strlen(htr));
                break;
/////////////////////////////////////////////////////////////////////////////////////////
            case 41: 
                sprintf(htr, "jdq_%d=1", 37);
                uart_write_bytes(UART_NUM_1, htr, strlen(htr)); 
                ESP_LOGI(EQU_TAG, "Wrote HIGH  %d bytes ", strlen(htr));
                break;
            case 40: 
                sprintf(htr, "jdq_%d=0", 37);
                uart_write_bytes(UART_NUM_1, htr, strlen(htr)); 
                ESP_LOGI(EQU_TAG, "LOW Wrote %d bytes", strlen(htr));
                break;
/////////////////////////////////////////////////////////////////////////////////////////
            case 51: 
                sprintf(htr, "jdq_%d=1", 38);
                uart_write_bytes(UART_NUM_1, htr, strlen(htr)); 
                ESP_LOGI(EQU_TAG, " Wrote HIGH  %d bytes ", strlen(htr));
                break;
            case 50: 
                sprintf(htr, "jdq_%d=0", 38);
                uart_write_bytes(UART_NUM_1, htr, strlen(htr)); 
                ESP_LOGI(EQU_TAG, "LOW Wrote %d bytes", strlen(htr));
                break;
/////////////////////////////////////////////////////////////////////////////////////////
            case 61: 
                sprintf(htr, "jdq_%d=1", 39);
                uart_write_bytes(UART_NUM_1, htr, strlen(htr)); 
                ESP_LOGI(EQU_TAG, " Wrote HIGH  %d bytes ", strlen(htr));
                break;
            case 60: 
                sprintf(htr, "jdq_%d=0", 39);
                uart_write_bytes(UART_NUM_1, htr, strlen(htr)); 
                ESP_LOGI(EQU_TAG, "LOW Wrote %d bytes", strlen(htr));
                break;
/////////////////////////////////////////////////////////////////////////////////////////
            default:
                break;

        }       

    } 
    
}
#include "user_led.h"


void led_init(void)
{
    gpio_pad_select_gpio(GPIO_NUM_20);
    gpio_set_direction(GPIO_NUM_20, GPIO_MODE_OUTPUT);
    gpio_set_level(GPIO_NUM_20, 1);
}


void led_frequently(void)
{
    gpio_set_level(GPIO_NUM_20,0);
}

void led_breathe(void)
{
    gpio_set_level(GPIO_NUM_20, 0);
    vTaskDelay(500);
    gpio_set_level(GPIO_NUM_20, 1);
    vTaskDelay(500);
}

void led_flash(void)
{
    gpio_set_level(GPIO_NUM_20, 0);
    vTaskDelay(200);
    gpio_set_level(GPIO_NUM_20, 1);
    vTaskDelay(200);
}
#include "bsp_led.h"
#include "main.h"

/*RESET_����*/
void led_green_off(void)
{
	  HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_SET);
}
void led_green_on(void)
{
	  HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_RESET);
}
void led_green_toggle(void)
{
		HAL_GPIO_TogglePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin);
}
void led_red_off(void)
{
	  HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_SET);
}
void led_red_on(void)
{
	  HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_RESET);
}
void led_red_toggle(void)
{
		HAL_GPIO_TogglePin(LED_RED_GPIO_Port, LED_RED_Pin);
}

void flow_led_off(uint16_t num)
{
		HAL_GPIO_WritePin(GPIOG, GPIO_PIN_8>>num, GPIO_PIN_RESET);
}
void flow_led_on(uint16_t num)
{
		HAL_GPIO_WritePin(GPIOG, GPIO_PIN_8>>num, GPIO_PIN_SET);
}
void flow_led_toggle(uint16_t num)
{
		HAL_GPIO_TogglePin(GPIOG, GPIO_PIN_8>>num);
}












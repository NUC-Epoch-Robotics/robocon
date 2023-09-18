  /**
  ****************************(C) COPYRIGHT 2022 NUCLEAR****************************
  * @file       bsp_rc.c/h
  * @brief      本文件为遥控器和接收器的驱动，包括：串口DMA接收、遥控器协议解码、遥控器控制结构体的定义
  * @note       
  * @history
  *  Version    Date            Author          Modification
  *  V1.0.0     Jan-23-2022     Junshuo         1. 完成对DJI例程的移植，添加了一些中文注释
	*  V1.1.0			Jan-26-2022     Junshuo         1. 增加了对遥控器左上角拨轮的解析
  *
  ****************************(C) COPYRIGHT 2022 NUCLEAR****************************
  */

#ifndef __BSP_RC_H__
#define __BSP_RC_H__

#include "usart.h"

#define UART_RX_DMA_SIZE (1024)
#define DBUS_MAX_LEN     (50)
#define DBUS_BUFLEN      (18)
#define DBUS_HUART       huart1 /* for dji remote controler reciever */
/** 
  * @brief  remote control information
  */
typedef __packed struct
{
  /* rocker channel information */
	int16_t ch[5];//ch[0]:拨轮	
	
/* left and right lever information */
  char sw[2];

} rc_info_t;
/* ----------------------- RC Switch Definition----------------------------- */
#define RC_SW_UP ((uint16_t)1)
#define RC_SW_MID ((uint16_t)3)
#define RC_SW_DOWN ((uint16_t)2)
#define switch_is_down(s) (s == RC_SW_DOWN)
#define switch_is_mid(s) (s == RC_SW_MID)
#define switch_is_up(s) (s == RC_SW_UP)

#define RW_SW_S1_CHANNEL 0
#define RW_SW_S2_CHANNEL 1



void uart_receive_handler(UART_HandleTypeDef *huart);
void dbus_uart_init(void);
void USART_SendData(USART_TypeDef* USARTx, uint16_t Data);
const rc_info_t *get_remote_control_point(void);

#endif


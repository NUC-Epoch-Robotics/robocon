/**
  ******************************************************************************
  * @file     .h
  * @author   
  * @version  V1.0
  * @date     
  * @brief    This file contains the headers of 
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __UART_GYRO_H
#define __UART_GYRO_H

/* Includes ------------------------------------------------------------------*/
#include "struct_typedef.h"
#include "stdio.h"
/* Exported types ------------------------------------------------------------*/
#if defined(__CC_ARM)
#pragma anon_unions
#elif defined(__ICCARM__)
#endif
typedef union gyro_rx_data
{
	struct
	{
		float a;
		float b;
		float c;
	};
	uint8_t buff[12];
}gyro_rx_data;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void get_gyro_data(void);





#endif

/****************** (C) COPYRIGHT 2023 EPOCH *****END OF FILE*************/


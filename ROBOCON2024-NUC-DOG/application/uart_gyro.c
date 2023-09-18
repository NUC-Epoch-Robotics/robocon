/**
  ******************************************************************************
  * @file     
  * @author   
  * @version  V1.0
  * @date     
  * @brief    
  ******************************************************************************
  * @attention
  * 注意事项
  *
  *
  * 
  ******************************************************************************
  */ 
/* Includes -------------------------------------------------------------------*/
#include "uart_gyro.h"
#include "string.h"
#include "usart.h"

/* Private  typedef -----------------------------------------------------------*/
/* Private  define ------------------------------------------------------------*/
/* Private  macro -------------------------------------------------------------*/
/* Private  variables ---------------------------------------------------------*/
gyro_rx_data gyro_rx;
uint8_t rx_judge_data[24];
/* Extern   variables ---------------------------------------------------------*/

/* Extern   function prototypes -----------------------------------------------*/
/* Private  function prototypes -----------------------------------------------*/
/* Private  functions ---------------------------------------------------------*/

/**
  * @brief  获取陀螺仪数据
  * @param  None
  * @retval None
  */
void get_gyro_data(void)
{
 HAL_UART_Receive_DMA(&huart2,(uint8_t *)rx_judge_data,24);
  for (int i = 0; i < 28; i++)
  {
    if (rx_judge_data[i] == 0xd8)
    {
      gyro_rx.buff[0] = rx_judge_data[i+1];
      gyro_rx.buff[1] = rx_judge_data[i+2];
      gyro_rx.buff[2] = rx_judge_data[i+3];
      gyro_rx.buff[3] = rx_judge_data[i+4];
      gyro_rx.buff[4] = rx_judge_data[i+5];
      gyro_rx.buff[5] = rx_judge_data[i+6];
      gyro_rx.buff[6] = rx_judge_data[i+7];
      gyro_rx.buff[7] = rx_judge_data[i+8];
      gyro_rx.buff[8] = rx_judge_data[i+9];
      gyro_rx.buff[9] = rx_judge_data[i+10];
      gyro_rx.buff[10] = rx_judge_data[i+11];
      gyro_rx.buff[11] = rx_judge_data[i+12];
      if (rx_judge_data[i + 13] == 0xee)
      {
        memset(&rx_judge_data, '0', sizeof(rx_judge_data));
        break;
      }
    }
  }

}

/************************ (C) COPYRIGHT 2023 EPOCH *****END OF FILE****/


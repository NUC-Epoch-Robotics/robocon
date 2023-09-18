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
                                                                                                              
#include "string.h"
#include "stdlib.h"
#include "bsp_rc.h"
#include "usart.h"
#include "main.h"
uint8_t   dbus_buf[DBUS_BUFLEN];
rc_info_t rc;



/**
  * @brief      enable global uart it and do not use DMA transfer done it
  * @param[in]  huart: uart IRQHandler id
  * @param[in]  pData: receive buff 
  * @param[in]  Size:  buff size
  * @retval     set success or fail
  */
static int uart_receive_dma_no_it(UART_HandleTypeDef* huart, uint8_t* pData, uint32_t Size)
{
  uint32_t tmp1 = 0;

  tmp1 = huart->RxState;
	
	if (tmp1 == HAL_UART_STATE_READY)
	{
		if ((pData == NULL) || (Size == 0))
		{
			return HAL_ERROR;
		}

		huart->pRxBuffPtr = pData;
		huart->RxXferSize = Size;
		huart->ErrorCode  = HAL_UART_ERROR_NONE;

		/* Enable the DMA Stream */
		HAL_DMA_Start(huart->hdmarx, (uint32_t)&huart->Instance->DR, (uint32_t)pData, Size);

		/* 
		 * Enable the DMA transfer for the receiver request by setting the DMAR bit
		 * in the UART CR3 register 
		 */
		SET_BIT(huart->Instance->CR3, USART_CR3_DMAR);

		return HAL_OK;
	}
	else
	{
		return HAL_BUSY;
	}
}

/**
  * @brief      returns the number of remaining data units in the current DMAy Streamx transfer.
  * @param[in]  dma_stream: where y can be 1 or 2 to select the DMA and x can be 0
  *             to 7 to select the DMA Stream.
  * @retval     The number of remaining data units in the current DMAy Streamx transfer.
  */
uint16_t dma_current_data_counter(DMA_Stream_TypeDef *dma_stream)
{
  /* Return the number of remaining data units for DMAy Streamx */
  return ((uint16_t)(dma_stream->NDTR));
}



/**
  * @brief       handle received rc data
  * @param[out]  rc:   structure to save handled rc data
  * @param[in]   buff: the buff which saved raw rc data
  * @retval 
  */
void rc_callback_handler(rc_info_t *rc, uint8_t *buff)
{
	rc->ch[0] = (buff[16] | (buff[17] << 8)) & 0x07FF;
  rc->ch[0] -= 1024;
  rc->ch[1] = (buff[0] | buff[1] << 8) & 0x07FF;
  rc->ch[1] -= 1024;
  rc->ch[2] = (buff[1] >> 3 | buff[2] << 5) & 0x07FF;
  rc->ch[2] -= 1024;
  rc->ch[3] = (buff[2] >> 6 | buff[3] << 2 | buff[4] << 10) & 0x07FF;
  rc->ch[3] -= 1024;
  rc->ch[4] = (buff[4] >> 1 | buff[5] << 7) & 0x07FF;
  rc->ch[4] -= 1024;

  rc->sw[0] = ((buff[5] >> 4) & 0x000C) >> 2;
  rc->sw[1] = (buff[5] >> 4) & 0x0003;
  
  if ((abs(rc->ch[1]) > 660) || \
      (abs(rc->ch[2]) > 660) || \
      (abs(rc->ch[3]) > 660) || \
      (abs(rc->ch[4]) > 660))
  {
    memset(rc, 0, sizeof(rc_info_t));
  }		
}

/**
  * @brief      clear idle it flag after uart receive a frame data
  * @param[in]  huart: uart IRQHandler id
  * @retval  
  */
static void uart_rx_idle_callback(UART_HandleTypeDef* huart)
{
	/* clear idle it flag avoid idle interrupt all the time */
	__HAL_UART_CLEAR_IDLEFLAG(huart);

	/* handle received data in idle interrupt */
	if (huart == &DBUS_HUART)
	{
		/* clear DMA transfer complete flag */
		__HAL_DMA_DISABLE(huart->hdmarx);

		/* handle dbus data dbus_buf from DMA */
		if ((DBUS_MAX_LEN - dma_current_data_counter(huart->hdmarx->Instance)) == DBUS_BUFLEN)
		{
			rc_callback_handler(&rc, dbus_buf);	
		}
		
		/* restart dma transmission */
		__HAL_DMA_SET_COUNTER(huart->hdmarx, DBUS_MAX_LEN);
		__HAL_DMA_ENABLE(huart->hdmarx);
	}
}

/**
  * @brief      callback this function when uart interrupt
	* @note				挂到stm32f4xx_it.h的串口1中断函数中
  * @param[in]  huart: uart IRQHandler id
  * @retval  
  */
void uart_receive_handler(UART_HandleTypeDef *huart)
{  
	if (__HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE) && 
			__HAL_UART_GET_IT_SOURCE(huart, UART_IT_IDLE))
	{
		uart_rx_idle_callback(huart);
//		detect_hook(DBUS_TOE);
	}
}

/**
  * @brief   初始化dbus uart 
	* @note		 在主函数中调用
  * @param   
  * @retval  
  */
void dbus_uart_init(void)
{
	/* open uart idle it */
	__HAL_UART_CLEAR_IDLEFLAG(&DBUS_HUART);
	__HAL_UART_ENABLE_IT(&DBUS_HUART, UART_IT_IDLE);

	uart_receive_dma_no_it(&DBUS_HUART, dbus_buf, DBUS_MAX_LEN);
}

/**
  * @brief          获取遥控器数据指针
  * @param[in]      none
  * @retval         遥控器数据指针
  */
const rc_info_t *get_remote_control_point(void)
{
    return &rc;
}


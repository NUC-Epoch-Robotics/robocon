/**
****************************(C) COPYRIGHT 2023 EPOCH****************************
* @file       bsp_can.c/h
* @brief      1. CAN初始化函数,使能CAN1和CAN2的中断，使能CAN1和CAN2的滤波器
*             2. CAN发送函数
*             3. CAN接收函数
* @note
* @history
*  Version    Date            Author          Modification
*  V1.0.0     Mar-28-2023     Junshuo         1. 完成
****************************(C) COPYRIGHT 2023 EPOCH****************************
*/

#ifndef BSP_CAN_H
#define BSP_CAN_H
#include "struct_typedef.h"
#include "CAN_defines.h"


typedef struct CAN_RX_Typedef
{
	int message_timestamp;
	int data_length;
	int data[8];
	int filter_index;
	int frame_type;
	int id_type;
	int32_t ID;
}CAN_RX_Typedef;


typedef struct CAN_TX_Typedef
{
	int id_type;
	int frame_type;
	int send_timestamp;
	int32_t ID;
	int data_length;
	uint8_t data[8];
}CAN_TX_Typedef;


void CAN_Filter_Init(void);

void CAN_Send_Packet(CAN_HandleTypeDef *hcan, CAN_TX_Typedef *tx);

void CAN_Get_Packet(CAN_HandleTypeDef *hcan, CAN_RX_Typedef *rx);


#endif


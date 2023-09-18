/**
****************************(C) COPYRIGHT 2023 EPOCH****************************
* @file       can_receive.c/h
* @brief      fifo0中断接收函数，接收dji和odrive的电机数据；
* @note
* @history
*  Version    Date            Author          Modification
*  V1.0.0     Mar-28-2023     Junshuo         1. 完成
*
****************************(C) COPYRIGHT 2023 EPOCH****************************
*/

#include "can_receive.h"
#include "detect_task.h"

#include "can_dji.h"
#include "can_odrive.h"

extern CAN_HandleTypeDef hcan1;
extern CAN_HandleTypeDef hcan2;

//odrive电机数据
extern axis_t odrive_axis[4]; 
extern CAN_RX_Typedef RX;

//大疆电机数据
extern motor_measure_t motor_chassis_dir[4];


/**
  * @brief          hal库CAN回调函数,接收dji和odrive电机数据
  * @param[in]      hcan:CAN句柄指针
  * @retval         none
  */
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	
	if(hcan->Instance == CAN1)
	{

		CAN_RxHeaderTypeDef rx_header;
		uint8_t rx_data[8];

		HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &rx_header, rx_data);

		switch(rx_header.StdId)
		{
		case CAN_2006_M1_ID:
		case CAN_2006_M2_ID:
		case CAN_2006_M3_ID:
		case CAN_2006_M4_ID:
		{
			static uint8_t i = 0;
			//get motor id
			i = rx_header.StdId - CAN_2006_M1_ID;
			get_motor_measure(&motor_chassis_dir[i], rx_data);
			get_total_angle(&motor_chassis_dir[i]);

			detect_hook( motor_dir1_TOE + i);
			break;
		}

		default:
		{
			break;
		}
		}
	
	}
	else if(hcan->Instance == CAN2)
	{
		int32_t ID = 0;
    	CAN_Get_Packet(hcan, &RX);
    	ID = RX.ID;
    	int32_t NODE_ID = (ID >> 5);
    	int32_t CMD_ID = (ID & 0x01F);

		static uint8_t i = 0;
		i = NODE_ID - CAN_ODRIVE_M1_ID;		//get motor id
	
		odrv_get_axis_status(&odrive_axis[i], CMD_ID);
	}
	else
	{
		//do nothing
	}
}



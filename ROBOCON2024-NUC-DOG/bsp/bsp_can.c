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
#include "bsp_can.h"
#include "main.h"

extern CAN_HandleTypeDef hcan1;
extern CAN_HandleTypeDef hcan2;

/**
  * @brief          CAN初始化，在main函数中调用
  */
void CAN_Filter_Init(void)
{

	CAN_FilterTypeDef can_filter_st;
	can_filter_st.FilterActivation = ENABLE;
	can_filter_st.FilterMode = CAN_FILTERMODE_IDMASK;
	can_filter_st.FilterScale = CAN_FILTERSCALE_32BIT;
	can_filter_st.FilterIdHigh = 0x0000;
	can_filter_st.FilterIdLow = 0x0000;
	can_filter_st.FilterMaskIdHigh = 0x0000;
	can_filter_st.FilterMaskIdLow = 0x0000;
	can_filter_st.FilterBank = 0;
	can_filter_st.FilterFIFOAssignment = CAN_RX_FIFO0;
	can_filter_st.SlaveStartFilterBank = 14;

	HAL_CAN_ConfigFilter(&hcan1, &can_filter_st);
	HAL_CAN_Start(&hcan1);
	HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);

	can_filter_st.FilterBank = 14;
	HAL_CAN_ConfigFilter(&hcan2, &can_filter_st);
	HAL_CAN_Start(&hcan2);
	HAL_CAN_ActivateNotification(&hcan2, CAN_IT_RX_FIFO0_MSG_PENDING);

}

/**
  * @brief          发送CAN数据包
  * @param[in]      hcan:CAN句柄
  * @param[in]     	tx:发送数据
  */
void CAN_Send_Packet(CAN_HandleTypeDef *hcan, CAN_TX_Typedef *tx)
{
	while ( !(HAL_CAN_GetTxMailboxesFreeLevel(hcan)) ){} //等待空邮箱

	uint32_t send_mail_box;
	static CAN_TxHeaderTypeDef  chassis_tx_message;

	chassis_tx_message.StdId = tx->ID;
	chassis_tx_message.IDE = tx->id_type;
	chassis_tx_message.RTR = tx->frame_type;
	chassis_tx_message.DLC = tx->data_length;

	HAL_CAN_AddTxMessage(hcan, &chassis_tx_message, tx->data, &send_mail_box);
	
}


/**
  * @brief          获取CAN数据包，在can中断中调用
  * @param[in]      hcan:CAN句柄
  * @param[out]      rx:接收数据包
  */
void CAN_Get_Packet(CAN_HandleTypeDef *hcan, CAN_RX_Typedef *rx)
{

	int frame_type = 0;
	int id_type = 0;


	id_type =  (CAN1 -> sFIFOMailBox[0].RIR & CAN_RI0R_IDE_Msk) >> CAN_RI0R_IDE_Pos ;
	frame_type = (CAN1 -> sFIFOMailBox[0].RIR & CAN_RI0R_RTR_Msk) >> CAN_RI0R_RTR_Pos ;

	if(id_type)
	{
		//Extended ID
		rx->id_type = CAN_ID_Extended;
		rx->ID = (hcan -> Instance -> sFIFOMailBox[0].RIR & CAN_RI0R_EXID_Msk) >> CAN_RI0R_EXID_Pos;
	}
	else
	{
		//Standard ID
		rx->id_type = CAN_ID_Standard;
		rx->ID = (hcan -> Instance -> sFIFOMailBox[0].RIR & CAN_RI0R_STID_Msk) >> CAN_RI0R_STID_Pos;
	}

	if(frame_type)
	{
		//RTR Frame
		rx->frame_type = CAN_Frame_Remote;
		rx->data_length = (hcan -> Instance -> sFIFOMailBox[0].RDTR & CAN_RDT0R_DLC_Msk) >> CAN_RDT0R_DLC_Pos;
		hcan -> Instance -> RF0R |= CAN_RF0R_RFOM0;
		while((hcan -> Instance -> RF0R & CAN_RF0R_RFOM0)){}
	}
	else
	{
		//Data Frame
		rx->frame_type = CAN_Frame_Data;
		rx->data_length = (hcan -> Instance -> sFIFOMailBox[0].RDTR & CAN_RDT0R_DLC_Msk) >> CAN_RDT0R_DLC_Pos;
		for(int i = 0; i < rx->data_length; i++)
		{
			if(i < 4)
			{
				rx->data[i] =  (hcan -> Instance -> sFIFOMailBox[0].RDLR & ( 0xFF << (8*i))) >> (8*i);
			}
			else
			{
				rx->data[i] =  (hcan -> Instance -> sFIFOMailBox[0].RDHR & ( 0xFF << (8*(i-4)))) >> (8*(i-4));
			}
		}

		hcan -> Instance -> RF0R |= CAN_RF0R_RFOM0;	}


}


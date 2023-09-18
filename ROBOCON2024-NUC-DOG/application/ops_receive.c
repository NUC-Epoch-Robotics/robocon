///**
//****************************(C) COPYRIGHT 2023 EPOCH****************************
//* @file       ops_receive.c/h
//* @brief      定位系统数据解析，发送更新位置信息指令.
//* @note
//* @history
//*  Version    Date            Author          Modification
//*  V1.0.0     Apr-06-2023     Junshuo         1. 移植。
//*
//****************************(C) COPYRIGHT 2023 EPOCH****************************
//*/

//#include "ops_receive.h"
//#include "usart.h"
//#include "stdio.h"

//extern chassis_move_t chassis_move;

//static void Stract (char strDestination[], char strSource[], int num);


///**
// * @brief  数据解析函数
// * @param  rec 串口接收到的字节数据
// * @param  chassis_ops 底盘结构体
// */
//void ops_data_analyse(uint8_t rec, chassis_move_t *chassis_ops)
//{
//	static uint8_t ch;
//	static union
//	{
//		uint8_t date[24];
//		float ActVal[6];
//	}posture;
//	static uint8_t count=0;
//	static uint8_t i=0;

//	ch=rec;
//	switch(count)
//	{
//		case 0:
//			if(ch==0x0d)
//				count++;
//			else
//				count=0;
//			break;
//		case 1:
//			if(ch==0x0a)
//			{
//				i=0;
//				count++;
//			}
//			else if(ch==0x0d);
//			else
//				count=0;
//			break;
//		case 2:
//			posture.date[i]=ch;
//			i++;
//			if(i>=24)
//			{
//				i=0;
//				count++;
//			}
//			break;
//		case 3:
//			if(ch==0x0a)
//				count++;
//			else
//				count=0;
//			break;
//		case 4:
//			if(ch==0x0d)
//			{
//				chassis_ops->car_yaw=posture.ActVal[0];
//				chassis_ops->angle_x=posture.ActVal[1];//没有返回值
//				chassis_ops->angle_y=posture.ActVal[2];//没有返回值
//				chassis_ops->pos_x=posture.ActVal[3];
//				chassis_ops->pos_y=posture.ActVal[4];
//				chassis_ops->w_z=posture.ActVal[5];
//			}
//			count=0;
//			break;
//		default:
//			count=0;
//		break;
//	}
//}


// /**
//   * @brief This function handles USART3 global interrupt.
//   */
//// void USART3_IRQHandler(void)
//// {
////   uint8_t r;

////   if (__HAL_UART_GET_FLAG(&huart3, UART_FLAG_RXNE) != RESET)
////   {
////     r = (uint8_t)(huart3.Instance->DR & 0xFF); // 从数据寄存器中读取接收到的数据
////     ops_data_analyse(r, &chassis_move);  // 处理接收到的数据
////   }
////   __HAL_UART_CLEAR_FLAG(&huart3, UART_FLAG_RXNE); // 清除接收中断标志位

//// }

///**
// * @brief 重置定位系统
// */
//void ops_reset()
//{
//	static char reset[4]="ACT0";
//	HAL_UART_Transmit(&huart6, (uint8_t *)reset, 4, HAL_MAX_DELAY); 

//}


///**
// * @brief 更新X坐标
// * @param  New_X 新坐标值
// */
//void Update_X(float New_X)
//{
//	char Update_x[8]="ACTX";
//	static union
//	{
//		float X;
//		char data[4];
//	}New_set;//联合体，用于将float转换为char

//	New_set.X = New_X;
//	Stract (Update_x,New_set.data,4);
//	
//  for (int i = 0; i < 8; i++) 
//	{
//    while (HAL_UART_GetState(&huart6) != HAL_UART_STATE_READY); 
//    HAL_UART_Transmit(&huart6, (uint8_t *)&Update_x[i], 1, HAL_MAX_DELAY); 
//  }

//}


///**
// * @brief 更新Y坐标
// * @param  New_Y 新坐标值
// */
//void Update_Y(float New_Y)
//{
//	char Update_y[8]="ACTY";
//	static union
//	{
//		float Y;
//		char data[4];
//	}New_set;

//	New_set.Y = New_Y;
//	Stract (Update_y,New_set.data,4);
//	
//  for (int i = 0; i < 8; i++) 
//	{
//    while (HAL_UART_GetState(&huart6) != HAL_UART_STATE_READY); 
//    HAL_UART_Transmit(&huart6, (uint8_t *)&Update_y[i], 1, HAL_MAX_DELAY); 
//  }

//}

///**
// * @brief 更新航向角
// * @param  New_A 新坐标值
// */
//void Update_A(float New_A)
//{
//	char update_a[8]="ACTJ";
//	static union
//	{
//		float A;
//		char data[4];
//	}New_set;

//	New_set.A = New_A;
//	Stract (update_a,New_set.data,4);
//	
//	for (int i = 0; i < 8; i++) 
//	{
//		while (HAL_UART_GetState(&huart6) != HAL_UART_STATE_READY); 
//		HAL_UART_Transmit(&huart6, (uint8_t *)&update_a[i], 1, HAL_MAX_DELAY); 
//	}

//}

///**
// * @brief 字符串拼接
// * @param  strDestination 目标字符串
// * @param  strSource 源字符串
// * @param  num 拼接长度
//*/
//void Stract (char strDestination[], char strSource[], int num)
//{
//	int i = 0;
//	int j = 0;

//	while (strDestination[i] != '\0')
//	{
//		i++;
//	}

//	for ( j=0; j < num; j++)
//	{
//		strDestination[i++]=strSource[j];
//	}

//}


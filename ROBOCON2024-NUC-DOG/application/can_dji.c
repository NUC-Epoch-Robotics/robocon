/**
****************************(C) COPYRIGHT 2023 EPOCH****************************
* @file       can_dji.c/h
* @brief      1. CAN发送函数，发送电机电流控制dji电机
*             2. m2006位置闭环控制
* @note
* @history
*  Version    Date            Author          Modification
*  V1.0.0     Jan-24-2022     Junshuo         1. 在DJI C板例程的的基础上，去掉了和云台、发射相关的代码
*  V1.5.0     Mar-25-2023     Junshuo         1. 修改了文件名（原来叫CAN_receive，在C板例程里）
*                                             2. 为适配舵轮底盘，专门用于控制大疆电机
*  V1.6.0     Mar-28-2023     Junshuo         1. 删除了接收部分，接收部分专门放在can_receive.c中。
*                                             2. 位置闭环控制
*
****************************(C) COPYRIGHT 2023 EPOCH****************************
*/

#include "can_dji.h"
#include "cmsis_os.h"
#include "main.h"


extern CAN_HandleTypeDef hcan1;


/*电机数据:  0:底盘转向电机1 2006电机,  	1:底盘转向电机1 2006电机,
				    2:底盘转向电机1 2006电机,		 3:底盘转向电机1 2006电机;*/
motor_measure_t motor_chassis_dir[4]; //原来是static，为了在can_receive.c中使用，改为了extern


static CAN_TxHeaderTypeDef  chassis_tx_message;
static uint8_t              chassis_can_send_data[8];



/**
  * @brief          发送ID为0x700的CAN包,它会设置3508电机进入快速设置ID
  * @param[in]      none
  * @retval         none
  */
void CAN_cmd_chassis_reset_ID(void)
{
	uint32_t send_mail_box;
	chassis_tx_message.StdId = 0x700;
	chassis_tx_message.IDE = CAN_ID_STD;
	chassis_tx_message.RTR = CAN_RTR_DATA;
	chassis_tx_message.DLC = 0x08;
	chassis_can_send_data[0] = 0;
	chassis_can_send_data[1] = 0;
	chassis_can_send_data[2] = 0;
	chassis_can_send_data[3] = 0;
	chassis_can_send_data[4] = 0;
	chassis_can_send_data[5] = 0;
	chassis_can_send_data[6] = 0;
  chassis_can_send_data[7] = 0;

	HAL_CAN_AddTxMessage(&CHASSIS_DIR_CAN, &chassis_tx_message, chassis_can_send_data, &send_mail_box);
}



/**
  * @brief          发送电机控制电流(0x201,0x202,0x203,0x204)
  * @param[in]      motor1: (0x201) 2006电机控制电流, 范围 [-10000,10000]
  * @param[in]      motor2: (0x202) 2006电机控制电流, 范围 [-10000,10000]
  * @param[in]      motor3: (0x203) 2006电机控制电流, 范围 [-10000,10000]
  * @param[in]      motor4: (0x204) 2006电机控制电流, 范围 [-10000,10000]
  * @retval         none
  */
void set_motor_current(int16_t motor1, int16_t motor2, int16_t motor3, int16_t motor4)
{
	uint32_t send_mail_box;
	chassis_tx_message.StdId = CAN_CHASSIS_ALL_ID;
	chassis_tx_message.IDE = CAN_ID_STD;
	chassis_tx_message.RTR = CAN_RTR_DATA;
	chassis_tx_message.DLC = 0x08;
	chassis_can_send_data[0] = motor1 >> 8;
	chassis_can_send_data[1] = motor1;
	chassis_can_send_data[2] = motor2 >> 8;
	chassis_can_send_data[3] = motor2;
	chassis_can_send_data[4] = motor3 >> 8;
	chassis_can_send_data[5] = motor3;
	chassis_can_send_data[6] = motor4 >> 8;
	chassis_can_send_data[7] = motor4;

	HAL_CAN_AddTxMessage(&CHASSIS_DIR_CAN, &chassis_tx_message, chassis_can_send_data, &send_mail_box);

}




/**
  * @brief          返回底盘电机 2006电机数据指针
  * @param[in]      i: 电机编号,范围[0,3]
  * @retval         电机数据指针
  */
const motor_measure_t *get_chassis_dir_motor_measure_point(uint8_t i)
{
	return &motor_chassis_dir[(i & 0x03)];
}


/**
	* @brief    接收2006电机通过CAN发过来的信息，在can_receive.c中调用
	* @param
	* @retval		None
	*/
void get_motor_measure(motor_measure_t *ptr, uint8_t *Data)
{
	static int cnt = 0;
	cnt++;
	ptr->last_angle = ptr->angle;
	ptr->angle = (uint16_t)(Data[0] << 8 | Data[1]) ;
	ptr->speed_rpm  = (int16_t)(Data[2] << 8 | Data[3]);
	ptr->real_current = (Data[4] << 8 | Data[5]) * 5.f / 16384.f;
	ptr->hall = Data[6];

	//	if(ptr->angle - ptr->last_angle > 4096)
	//		ptr->round_cnt --;
	//	else if (ptr->angle - ptr->last_angle < -4096)
	//		ptr->round_cnt ++;
	if(cnt <= 5) //初始化零角度，可以实现每次上电自动将初始位置归为0角
	{
		ptr->offset_angle = ptr->angle;
		ptr->total_angle = ptr->round_cnt * 8192 + ptr->angle - ptr->offset_angle;
	}

}



#define ABS(x)	( (x>0) ? (x) : (-x) )
/**
	* 	@brief          电机上电角度=0， 之后用这个函数更新3510电机的相对开机后（为0）的相对角度。
	* 	@param[in]      p: 电机数据指针
	*/
void get_total_angle(motor_measure_t *p)
{

	int res1, res2, delta;
	if(p->angle < p->last_angle) {			//可能的情况
		res1 = p->angle + 8192 - p->last_angle;	//正转，delta=+
		res2 = p->angle - p->last_angle;				//反转	delta=-
	} else {	//angle > last
		res1 = p->angle - 8192 - p->last_angle ;//反转	delta -
		res2 = p->angle - p->last_angle;				//正转	delta +
	}
	//不管正反转，肯定是转的角度小的那个是真的
	if(ABS(res1) < ABS(res2))
		delta = res1;
	else
		delta = res2;

	p->total_angle += delta;
	p->last_angle = p->angle;

}



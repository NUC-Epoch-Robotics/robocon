  /**
  ****************************(C) COPYRIGHT 2023 EPOCH****************************
  * @file       can_dji.c/h
  * @brief      fifo0中断接收函数，接收电机数据；CAN发送函数，发送电机电流控制dji电机
  * @note       
  * @history
  *  Version    Date            Author          Modification
  *  V1.0.0     Jan-24-2022     Junshuo         1. 在DJI C板例程的的基础上，去掉了和云台、发射相关的代码
  *  V1.5.0     Mar-25-2023     Junshuo         1. 修改了文件名（原来叫CAN_receive）
  *                                             2. 为适配舵轮底盘，专门用于控制大疆电机
  *
  ****************************(C) COPYRIGHT 2023 EPOCH****************************
  */

#ifndef CAN_DJI_H
#define CAN_DJI_H

#include "struct_typedef.h"
#include "pid.h"


#define CHASSIS_DIR_CAN hcan1

//底盘2006最大can发送电流值
#define MAX_MOTOR_CAN_CURRENT 9000.0f
//底盘电机位置环PID
#define M2006_MOTOR_POS_PID_KP 0.25f
#define M2006_MOTOR_POS_PID_KI 0.0f
#define M2006_MOTOR_POS_PID_KD 0.1f
#define M2006_MOTOR_POS_PID_MAX_OUT MAX_MOTOR_CAN_CURRENT
#define M2006_MOTOR_POS_PID_MAX_IOUT 0.0f
#define M2006_MOTOR_POS_PID_DEADBAND 5.0f
//底盘电机速度环PID
#define M2006_MOTOR_SPEED_PID_KP 2.0f
#define M2006_MOTOR_SPEED_PID_KI 0.1f
#define M2006_MOTOR_SPEED_PID_KD 0.0f
#define M2006_MOTOR_SPEED_PID_MAX_OUT MAX_MOTOR_CAN_CURRENT
#define M2006_MOTOR_SPEED_PID_MAX_IOUT 5000.0f
#define M2006_MOTOR_SPEED_PID_DEADBAND 100.0f


/* CAN send and receive ID */
typedef enum
{
    CAN_CHASSIS_ALL_ID = 0x200,
    CAN_2006_M1_ID = 0x201,
    CAN_2006_M2_ID = 0x202,
    CAN_2006_M3_ID = 0x203,
    CAN_2006_M4_ID = 0x204,
} can_dji_id_e;


#define FILTER_BUF_LEN		5
typedef struct
{
    int16_t speed_rpm;
    float  	real_current;
    int16_t given_current;
  
    uint8_t  	hall;
    uint16_t 	angle;				//abs angle range:[0,8191]
    uint16_t 	last_angle;	//abs angle range:[0,8191]
    uint16_t	offset_angle;
    int32_t		round_cnt;
    int32_t		total_angle;
  
    uint8_t		buf_idx;
    uint16_t	angle_buf[FILTER_BUF_LEN];
    uint16_t	fited_angle;
    uint32_t	msg_cnt;
    
} motor_measure_t;


typedef struct
{
  const motor_measure_t *chassis_motor_measure;
  fp32 angle_set;
  int16_t give_current;

} Motor_Dji;

/**
  * @brief          发送ID为0x700的CAN包,它会设置3508电机进入快速设置ID
  * @param[in]      none
  * @retval         none
  */
extern void CAN_cmd_chassis_reset_ID(void);

extern void set_motor_current(int16_t motor1, int16_t motor2, int16_t motor3, int16_t motor4);

extern const motor_measure_t *get_chassis_dir_motor_measure_point(uint8_t i);

void get_motor_measure(motor_measure_t *ptr, uint8_t *Data);

void get_total_angle(motor_measure_t *p);

void m2006_position_closed_control(Motor_Dji *motor_dir);



#endif

  /**
  ****************************(C) COPYRIGHT 2023 EPOCH****************************
  * @file       chassis_task.c/h
  * @brief      舵轮底盘控制任务
  * @note       
  *
  ****************************(C) COPYRIGHT 2023 EPOCH****************************
  */
	
#ifndef CHASSIS_TASK_H
#define CHASSIS_TASK_H
#include "struct_typedef.h"
#include "pid.h"
#include "bsp_rc.h"
#include "user_lib.h"

//任务开始空闲一段时间
#define CHASSIS_TASK_INIT_TIME 357


//前后的遥控器通道号码
#define CHASSIS_X_CHANNEL 1
//左右的遥控器通道号码
#define CHASSIS_Y_CHANNEL 4
//旋转的遥控器通道号码
#define CHASSIS_WZ_CHANNEL 0

//TODO: 换算成现实单位
//遥控器前进摇杆（max 660）转化成车体前进速度（m/s）的比例660*0.005=3.30
#define CHASSIS_VX_RC_SEN 0.005f
//遥控器左右摇杆（max 660）转化成车体左右速度（m/s）的比例
#define CHASSIS_VY_RC_SEN 0.005f
//遥控器旋转摇杆/拨轮（max 660）转化成车体速度（m/s）的比例
#define CHASSIS_WZ_RC_SEN 0.005f 

//影响一阶低通滤波的系数，这个系数越大，滤波效果越明显（越平滑），加速越慢
#define CHASSIS_ACCEL_VEL 0.01f
#define CHASSIS_ACCEL_YAW 0.0f //未使用


//摇杆死区
#define CHASSIS_RC_DEADLINE 10


//底盘任务控制间隔 2ms
#define CHASSIS_CONTROL_TIME_MS 2
//底盘任务控制间隔 0.002s
#define CHASSIS_CONTROL_TIME 0.002f
//底盘任务控制频率
#define CHASSIS_CONTROL_FREQUENCE 500.0f


//底盘运动过程最大前进速度
#define NORMAL_MAX_CHASSIS_SPEED_X 3.0f
//底盘运动过程最大平移速度
#define NORMAL_MAX_CHASSIS_SPEED_Y 3.0f

#include "robocon.h"

#define IsReady  1
#define NotReady 0

#define Lm298n_break 0
#define Lm298n_forward 1
#define Lm298n_backward 2

void motor_run(void);
static void chassis_init(void);
void coordinate_trans(float X,float Y);

typedef struct
{
    float ref_agle[8];
    float out[8];
} temp_data;

extern u16 pid_spd_out_limit;
extern bool IsMotoReadyOrNot;
extern float ref_agle[8];
extern temp_data temp_pid;
extern float test_speed;

void send_chassis_cur1_4(int16_t motor1, int16_t motor2, int16_t motor3, int16_t motor4);
void send_chassis_cur5_8(int16_t motor5, int16_t motor6, int16_t motor7, int16_t motor8);

void Lm298n_pin_init(void);
void Lm298n_ctrl( int  mode );



/**
  * @brief          底盘任务
  * @param[in]      pvParameters: 空
  * @retval         none
  */
extern void chassis_task(void const *pvParameters);

//TODO: 把串口接收单独写到一个文件里，不要放在底盘任务里
//接收ops数据
extern uint8_t rec;


#endif

/******************************************************************************
/// @brief
/// @copyright Copyright (c) 2017 <dji-innovations, Corp. RM Dept.>
/// @license MIT License
/// Permission is hereby granted, free of charge, to any person obtaining a copy
/// of this software and associated documentation files (the "Software"), to deal
/// in the Software without restriction,including without limitation the rights
/// to use, copy, modify, merge, publish, distribute, sublicense,and/or sell
/// copies of the Software, and to permit persons to whom the Software is furnished
/// to do so,subject to the following conditions:
///
/// The above copyright notice and this permission notice shall be included in
/// all copies or substantial portions of the Software.
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
/// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
/// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
/// THE SOFTWARE.
*******************************************************************************/
#ifndef __pid_H
#define __pid_H
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"



enum {
    LLAST	= 0,
    LAST 	= 1,
    NOW 	= 2,

    POSITION_PID,
    DELTA_PID,
};
typedef struct __pid_t
{
    float p;
    float i;
    float d;

    float set[3];				//Ä¿±êÖµ,°üº¬NOW£¬ LAST£¬ LLASTÉÏÉÏ´Î
    float get[3];				//²âÁ¿Öµ
    float err[3];				//Îó²î


    float pout;							//pÊä³ö
    float iout;							//iÊä³ö
    float dout;							//dÊä³ö

    float pos_out;						//±¾´ÎÎ»ÖÃÊ½Êä³ö
    float last_pos_out;				//ÉÏ´ÎÊä³ö
    float delta_u;						//±¾´ÎÔöÁ¿Öµ
    float delta_out;					//±¾´ÎÔöÁ¿Ê½Êä³ö = last_delta_out + delta_u
    float last_delta_out;

    float max_err;
    float deadband;				//err < deadband return
    uint32_t pid_mode;
    uint32_t MaxOutput;				//Êä³öÏÞ·ù
    uint32_t IntegralLimit;		//»ý·ÖÏÞ·ù

    void (*f_param_init)(struct __pid_t *pid,  //PID²ÎÊý³õÊ¼»¯
                         uint32_t pid_mode,
                         uint32_t maxOutput,
                         uint32_t integralLimit,
                         float p,
                         float i,
                         float d);
    void (*f_pid_reset)(struct __pid_t *pid, float p, float i, float d);		//pidÈý¸ö²ÎÊýÐÞ¸Ä

} pid_t;

void PID_struct_init(
    pid_t* pid,
    uint32_t mode,
    uint32_t maxout,
    uint32_t intergral_limit,

    float 	kp,
    float 	ki,
    float 	kd);

float pid_calc(pid_t* pid, float fdb, float ref);
void pid_reset(pid_t	*pid, float kp, float ki, float kd);
void pid_reset_kpkd(pid_t	*pid, float kp, float kd);

extern pid_t pid_pos[8];
extern pid_t pid_spd[8];
extern pid_t pid_imu[3];
extern pid_t pid_track;
extern pid_t pid_climbing;
extern pid_t pid_test1;
extern pid_t pid_openmv_dev;
extern pid_t pid_rotate[2];

//extern pid_t pid_rol;
//extern pid_t pid_pit;
//extern pid_t pid_yaw;
//extern pid_t pid_pit_omg;
//extern pid_t pid_yaw_omg;

//extern pid_t pid_yaw_alfa;
//extern pid_t pid_chassis_angle;
//extern pid_t pid_poke;
//extern pid_t pid_poke_omg;
//extern pid_t pid_imu_tmp;		//imu_temperature
//extern pid_t pid_cali_bby;	//big buff yaw
//extern pid_t pid_cali_bbp;
//extern pid_t pid_omg;
//extern pid_t pid_pos;
//extern pid_t pid_spd[2];
//extern pid_t pid_pos[2];
#endif


  /**
  ****************************(C) COPYRIGHT 2023 EPOCH****************************
  * @file       chassis_task.c/h
  * @brief      舵轮底盘控制任务
  * @note       
  *
  ****************************(C) COPYRIGHT 2023 EPOCH****************************
  */


#include "robocon.h"
#include "motor_ctrl.h"



static void chassis_init(void);
void motor_run(void);
void send_chassis_cur1_4(int16_t motor1, int16_t motor2, int16_t motor3, int16_t motor4);
void send_chassis_cur5_8(int16_t motor5, int16_t motor6, int16_t motor7, int16_t motor8);

bool IsMotoReadyOrNot= NotReady;
float ref_agle[8]= {0};
float temp_angle;
temp_data temp_pid= {0};     //pidÖÐ¼äÊý¾Ý

float test_speed=0;
u16 pid_spd_out_limit = 6720;

#if INCLUDE_uxTaskGetStackHighWaterMark
uint32_t chassis_high_water;
#endif


void chassis_task(void const *pvParameters)
{
  //空闲一段时间
  vTaskDelay(CHASSIS_TASK_INIT_TIME);
//	chassis_init(&chassis_move);
//  UART_Start_Receive_IT(&huart6, &rec, 1);
	
  while(1)
  {
		motor_run();
		//系统延时
		vTaskDelay(CHASSIS_CONTROL_TIME_MS);
#if INCLUDE_uxTaskGetStackHighWaterMark
        chassis_high_water = uxTaskGetStackHighWaterMark(NULL);
#endif

  }

}



/**
  * @brief          初始化"chassis_move"变量，包括pid初始化， 遥控器指针初始化，3508底盘电机指针初始化，云台电机初始化，陀螺仪角度指针初始化
  * @param[out]     chassis_move_init:"chassis_move"变量指针.
  * @retval         none
  */
static void chassis_init(void)
{
    for (int i = 0; i < 8; i++)//  						20,0.01,0  37,0.008   8.0f, 0.000f   16
        PID_struct_init(&pid_pos[i], POSITION_PID, 100000.0f, 2000.0f, 8.0f, 0.0008f, 0.0f);  //Î»ÖÃ»·PID²ÎÊýÉèÖÃ£¨pid½á¹¹Ìå£¬PIDÀàÐÍ£¬×î´óÊä³ö£¬±ÈÀýÏÞÖÆ£¬P , I , D £©

    for (int i = 0; i < 8; i++)  //									16384.0f¶ÔÓ¦20A					15.5f,0,0 // 16.0f, 0.001f   20.0f, 0.004297f   22.0f, 0.01399f
        PID_struct_init(&pid_spd[i], POSITION_PID, 16000.0f, 2000.0f, 15.5f, 0.000100, 0.0f);		//ËÙ¶È»·PID£¨pid½á¹¹Ìå£¬PIDÀàÐÍ£¬×î´óÊä³ö£¬±ÈÀýÏÞÖÆ£¬P , I , D £©
    //20.0f, 0.004297f, 0.0f     22.0f, 0.001

    PID_struct_init(&pid_imu[0], POSITION_PID, 10000, 0, 1.5f, 0.01, 0);		//roll

    PID_struct_init(&pid_imu[1], POSITION_PID, 10000, 0, 1.5f, 0.01, 0);		//pitch

    PID_struct_init(&pid_imu[2], POSITION_PID, 10000, 0, 1.30f, 0, 0);		//yaw

    PID_struct_init(&pid_climbing, POSITION_PID, 10000, 10000, 3.5f, 0, 0);		//yaw 1.8

    PID_struct_init(&pid_test1, POSITION_PID, 10000, 0, 1.5f, 0, 0);		//yaw

    PID_struct_init(&pid_openmv_dev, POSITION_PID, 10000, 0, 1.5f, 0, 0);		//yaw
	}



void motor_run(void) 
{ 
     if(IsMotoReadyOrNot== IsReady) {
        for(int i=0; i<8; i++)
         ref_agle[i]=temp_pid.ref_agle[i];
        IsMotoReadyOrNot= NotReady;
    }

    for(int i=0; i<8; i++) {

        //pid_calc(&pid_pos[i],moto_chassis[i].total_angle/100,ref_agle[i]/100);  //Î»ÖÃ»· ½Ç¶È¿ØÖÆ
        pid_calc(&pid_pos[i],moto_chassis[i].total_angle*360/8191,ref_agle[i]*360/8191);  //Î»ÖÃ»· ½Ç¶È¿ØÖÆ

        if(pid_pos[i].pos_out>pid_spd_out_limit)pid_pos[i].pos_out=pid_spd_out_limit;	//×î¸ßËÙ¶ÈÏÞÖÆ
        else if(pid_pos[i].pos_out<-pid_spd_out_limit)pid_pos[i].pos_out=-pid_spd_out_limit; //×î¸ßËÙ¶ÈÏÞÖÆ

        //temp_pid.out[i] = pid_calc(&pid_spd[i],moto_chassis[i].speed_rpm,test_speed);  //ËÙ¶È»· ²âÊÔ
        //temp_pid.out[i] = pid_calc(&pid_spd[i],moto_chassis[i].speed_rpm,pid_pos[i].pos_out);  //ËÙ¶È»· ËÙ¶È¿ØÖÆ
        moto_chassis[i].given_current = pid_calc(&pid_spd[i],moto_chassis[i].speed_rpm,pid_pos[i].pos_out);  //ËÙ¶È»· ËÙ¶È¿ØÖÆ
    }

    send_chassis_cur5_8(moto_chassis[4].given_current,moto_chassis[5].given_current,moto_chassis[6].given_current,moto_chassis[7].given_current);		//´«µÝ5-8Êý¾Ý¸øcanÊÕ·¢Æ÷
    send_chassis_cur1_4(moto_chassis[0].given_current,moto_chassis[1].given_current,moto_chassis[2].given_current,moto_chassis[3].given_current);		//´«µÝ1-4Êý¾Ý¸øcanÊÕ·¢Æ÷

    osDelay(3);		//¿ØÖÆÆµÂÊ  ¸ø	1	7ºÅºÍ8ºÅµç»ú»áÊ§¿Ø
}

void send_chassis_cur1_4(int16_t motor1, int16_t motor2, int16_t motor3, int16_t motor4)
{
    hcan1.pTxMsg->StdId   = 0x200;
    hcan1.pTxMsg->IDE     = CAN_ID_STD;
    hcan1.pTxMsg->RTR     = CAN_RTR_DATA;
    hcan1.pTxMsg->DLC     = 0x08;
    hcan1.pTxMsg->Data[0] = motor1 >> 8;
    hcan1.pTxMsg->Data[1] = motor1;
    hcan1.pTxMsg->Data[2] = motor2 >> 8;
    hcan1.pTxMsg->Data[3] = motor2;
    hcan1.pTxMsg->Data[4] = motor3 >> 8;
    hcan1.pTxMsg->Data[5] = motor3;
    hcan1.pTxMsg->Data[6] = motor4 >> 8;
    hcan1.pTxMsg->Data[7] = motor4;
    HAL_CAN_Transmit(&hcan1, 10);
}

//·¢ËÍµ×ÅÌµç»ú¿ØÖÆÃüÁî
void send_chassis_cur5_8(int16_t motor5, int16_t motor6, int16_t motor7, int16_t motor8)
{
    hcan1.pTxMsg->StdId   = 0x1ff;
    hcan1.pTxMsg->IDE     = CAN_ID_STD;
    hcan1.pTxMsg->RTR     = CAN_RTR_DATA;
    hcan1.pTxMsg->DLC     = 0x08;
    hcan1.pTxMsg->Data[0] = motor5 >> 8;
    hcan1.pTxMsg->Data[1] = motor5;
    hcan1.pTxMsg->Data[2] = motor6 >> 8;
    hcan1.pTxMsg->Data[3] = motor6;
    hcan1.pTxMsg->Data[4] = motor7 >> 8;
    hcan1.pTxMsg->Data[5] = motor7;
    hcan1.pTxMsg->Data[6] = motor8 >> 8;
    hcan1.pTxMsg->Data[7] = motor8;
    HAL_CAN_Transmit(&hcan1, 10);
}

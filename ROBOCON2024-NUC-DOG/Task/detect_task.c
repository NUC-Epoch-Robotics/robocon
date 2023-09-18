#include "detect_task.h"
#include "cmsis_os.h"
#include "bsp_led.h"

#include "can_dji.h"


#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"


//红灯闪，灭函数，切换闪灭
#define DETECT_LED_R_TOGGLE() led_red_toggle()
#define DETECT_LED_R_ON() led_red_on()
#define DETECT_LED_R_OFF() led_red_off()
//流水灯闪灭函数
#define DETECT_FLOW_LED_ON(i) flow_led_on(i)
#define DETECT_FLOW_LED_OFF(i) flow_led_off(i)


//初始化错误列表
static void detect_init(uint32_t time);
//显示优先级最高的错误，传入的参数为 显示的错误的错误码
static void DetectDisplay(uint8_t num);

static error_t error_list[ERROR_LIST_LENGHT + 1];


#if INCLUDE_uxTaskGetStackHighWaterMark
uint32_t detect_task_stack;
#endif


void detect_task(void const *pvParameters)
{
    static uint32_t system_time;
    system_time = xTaskGetTickCount();
    //init,初始化
    detect_init(system_time);
//    error_list[15].solve_lost_fun = solve;
    // error_list[1].solve_lost_fun = solve;
    // error_list[2].solve_lost_fun = solve;
    // error_list[3].solve_lost_fun = solve;
    // error_list[4].solve_lost_fun = solve;
    // error_list[5].solve_lost_fun = solve;
    // error_list[6].solve_lost_fun = solve;
    // error_list[7].solve_lost_fun = solve;
    // error_list[8].solve_lost_fun = solve;
    // error_list[9].solve_lost_fun = solve;

    vTaskDelay(DETECT_TASK_INIT_TIME);    //wait a time.空闲一段时间

    while (1)
    {
        static uint8_t error_num_display = 0;
        system_time = xTaskGetTickCount();

        error_num_display = ERROR_LIST_LENGHT;
        error_list[ERROR_LIST_LENGHT].is_lost = 0;
        error_list[ERROR_LIST_LENGHT].error_exist = 0;

        for (int i = 0; i < ERROR_LIST_LENGHT; i++)
        {
            //disable, continue
            //未使能，跳过


            if (error_list[i].enable == 0)
            {
                continue;
            }

            //judge offline.判断掉线
            if (system_time - error_list[i].new_time > error_list[i].set_offline_time)
            {
                if (error_list[i].error_exist == 0)
                {
                    //record error and time
                    //记录错误以及掉线时间
                    error_list[i].is_lost = 1;
                    error_list[i].error_exist = 1;
                    error_list[i].lost_time = system_time;
                }
                //judge the priority,save the highest priority ,
                //判断错误优先级， 保存优先级最高的错误码
                if (error_list[i].priority > error_list[error_num_display].priority)
                {
                    error_num_display = i;
                }
                

                error_list[ERROR_LIST_LENGHT].is_lost = 1;
                error_list[ERROR_LIST_LENGHT].error_exist = 1;
                //if solve_lost_fun != NULL, run it
                //如果提供解决函数，运行解决函数
                if (error_list[i].solve_lost_fun != NULL)
                {
                    error_list[i].solve_lost_fun();
                }
            }
            else if (system_time - error_list[i].work_time < error_list[i].set_online_time)
            {
                //just online, maybe unstable, only record
                //刚刚上线，可能存在数据不稳定，只记录不丢失，
                error_list[i].is_lost = 0;
                error_list[i].error_exist = 1;
            }
            else
            {
                error_list[i].is_lost = 0;
                //判断是否存在数据错误
                //judge if exist data error
                if (error_list[i].data_is_error != NULL)
                {
                    error_list[i].error_exist = 1;
                }
                else
                {
                    error_list[i].error_exist = 0;
                }
                //calc frequency
                //计算频率
                if (error_list[i].new_time > error_list[i].last_time)
                {
                    error_list[i].frequency = configTICK_RATE_HZ / (fp32)(error_list[i].new_time - error_list[i].last_time);
                }
            }
        }
//标准库有一个显示函数
				DetectDisplay(error_num_display + 1);
        vTaskDelay(DETECT_CONTROL_TIME);
#if INCLUDE_uxTaskGetStackHighWaterMark
        detect_task_stack = uxTaskGetStackHighWaterMark(NULL);
#endif
    }
}
/**
  * @brief          获取设备对应的错误状态
  * @param[in]      toe:设备目录
  * @retval         true(错误) 或者false(没错误)
  */
bool_t toe_is_error(uint8_t toe)
{
    return (error_list[toe].error_exist == 1);
}
/**
  * @brief          记录时间
  * @param[in]      toe:设备目录
  * @retval         none
  */
void detect_hook(uint8_t toe)
{
    error_list[toe].last_time = error_list[toe].new_time;
    error_list[toe].new_time = xTaskGetTickCount();
    
    if (error_list[toe].is_lost)
    {
        error_list[toe].is_lost = 0;
        error_list[toe].work_time = error_list[toe].new_time;
    }
    
    if (error_list[toe].data_is_error_fun != NULL)
    {
        if (error_list[toe].data_is_error_fun())
        {
            error_list[toe].error_exist = 1;
            error_list[toe].data_is_error = 1;

            if (error_list[toe].solve_data_error_fun != NULL)
            {
                error_list[toe].solve_data_error_fun();
            }
        }
        else
        {
            error_list[toe].data_is_error = 0;
        }
    }
    else
    {
        error_list[toe].data_is_error = 0;
    }
}
/**
  * @brief          得到错误列表
  * @param[in]      none
  * @retval         error_list的指针
  */
const error_t *get_error_list_point(void)
{
    return error_list;
}
/*
*		显示错误码，移植自标准库
*/
static void DetectDisplay(uint8_t num)
{
	static uint8_t last_num = ERROR_LIST_LENGHT + 1;
	uint8_t i=0;
	//8个流水显示列表前八的错误码的情况
	    for (i = 1; i <= motor_vel4_TOE; i++)
    {
//        if (error_list[i].error_exist)
//        {
//            DETECT_FLOW_LED_OFF(i);
//        }
//        else
//        {
//            DETECT_FLOW_LED_ON(i);
//        }
    }
	//错误码 通过红灯闪烁次数来判断
			if(num==ERROR_LIST_LENGHT+1)
			{
				DETECT_LED_R_OFF();
				last_num=ERROR_LIST_LENGHT+1;
			}
			else 
			{
				static uint8_t i = 0, led_flag = 0, cnt_num = 0, time = 0;
        //记录最新的最高优先级的错误码，等下一轮闪烁
        if (last_num != num)
        {
            last_num = num;
        }

        if (cnt_num == 0)
        {
            //cnt_num 记录还有几次闪烁，到0后，灭一段时间才开始下一轮
            time++;
            if (time > 50)
            {
                time = 0;
                cnt_num = last_num;
            }
            return;
				}	
        if (i == 0)
        {

            DETECT_LED_R_TOGGLE();
            if (led_flag)
            {
                //红灯闪灭各一次，将要剩余次数减一
                led_flag = 0;
                cnt_num--;
            }
            else
            {
                led_flag = 1;
            }
				}
        //i为计时次数，20为半个周期，切换一次红灯闪灭
        i++;

        if (i > 20)
        {
            i = 0;
        }						
      }			
}	


static void detect_init(uint32_t time)
{
    //设置离线时间，上线稳定工作时间，优先级 offlineTime onlinetime priority
    uint16_t set_item[ERROR_LIST_LENGHT][3] =
        {
            {30, 40, 15}, //SBUS
            {10, 10, 14}, //drv1
            {10, 10, 13}, //drv2
            {10, 10, 12}, //drv3
            {10, 10, 11}, //drv4
            {10, 10, 10}, //vel1
            {10, 10, 9},  //vel2
            {10, 10, 8},  //vel3
            {10, 10, 7},  //vel4
            {10, 10, 6},  //get_drv
            {10, 10, 5},  //get_lift
            {10, 10, 4},  //shoot1
            {10, 10, 4},  //shoot2
            {10, 10, 4},  //shoot3
            {10, 10, 4},  //shoot4
            {10, 10, 3},  //ops
            {30, 20, 1},  //oled   
			
		};
    for (uint8_t i = 0; i < ERROR_LIST_LENGHT; i++)
    {
        error_list[i].set_offline_time = set_item[i][0];
        error_list[i].set_online_time = set_item[i][1];
        error_list[i].priority = set_item[i][2];
        error_list[i].data_is_error_fun = NULL;
        error_list[i].solve_lost_fun = NULL;
        error_list[i].solve_data_error_fun = NULL;

        error_list[i].enable = 1;
        error_list[i].error_exist = 1;
        error_list[i].is_lost = 1;
        error_list[i].data_is_error = 1;
        error_list[i].frequency = 0.0f;
        error_list[i].new_time = time;
        error_list[i].last_time = time;
        error_list[i].lost_time = time;
        error_list[i].work_time = time;
    }

}









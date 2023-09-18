#ifndef DETECT_TASK_H
#define DETECT_TASK_H
#include "struct_typedef.h"


#define DETECT_TASK_INIT_TIME 57
#define DETECT_CONTROL_TIME 10


//错误码以及对应设备顺序
enum errorList
{
    DBUS_TOE =0, 
    motor_dir1_TOE,
    motor_dir2_TOE,
    motor_dir3_TOE,
    motor_dir4_TOE,
    motor_vel1_TOE,
    motor_vel2_TOE,
    motor_vel3_TOE,
    motor_vel4_TOE,
    motor_getdrv__TOE,
    motor_getlift__TOE,
    motor_shoot1_TOE,
    motor_shoot2_TOE,
    motor_shoot3_TOE,
    motor_shoot4_TOE,
	  OPS_TOE,
    OLED_TOE,
    ERROR_LIST_LENGHT,
};
typedef __packed struct
{
    uint32_t new_time;
    uint32_t last_time;
    uint32_t lost_time;
    uint32_t work_time;
    uint16_t set_offline_time : 12;
    uint16_t set_online_time : 12;
    uint8_t enable : 1;
    uint8_t priority : 4;
    uint8_t error_exist : 1;
    uint8_t is_lost : 1;
    uint8_t data_is_error : 1;

    fp32 frequency;
    bool_t (*data_is_error_fun)(void);
    void (*solve_lost_fun)(void);
    void (*solve_data_error_fun)(void);
} error_t;

extern void detect_task(void const *pvParameters);
extern bool_t toe_is_error(uint8_t err);						//获取设备对应的错误状态：true or false
extern void detect_hook(uint8_t toe);								//记录时间：toe设备目录
extern const error_t *get_error_list_point(void);		//得到错误列表



















#endif


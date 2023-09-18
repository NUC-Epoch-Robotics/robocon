#include "path.h"
#define MAX_PLAN_VEL 5.0f
double Auto_x,Auto_y;
Pose_t StartPos;
Pose_t EndPos;
void PathInit(Pose_t *endPos)
{
   
}
void PathFlow( chassis_move_t *Pos)
{
        static int flag=0;
        Auto_x=testPath[flag].point.x-Pos->pos_x;
        Auto_y=testPath[flag].point.y-Pos->pos_y;
        Pos->car_move_angle_set= R2DEG(atan2(Auto_x, Auto_y));
        Pos->motor_vel[flag].speed_rpm_set=testPath[flag].vel;
        if((Auto_x>-10||Auto_x<10)&&(Auto_y>-10||Auto_y<10)) flag++;
}



Pose_t testPath[6]=
{
{	0.f	,	0.f	,	0.0f	,	0.0f	},
{	-500.0f	,	0.f	,	0.0f	,	MAX_PLAN_VEL	},
{	-1000.0f	,	0.f	,	0.0f	,	MAX_PLAN_VEL	},
{	-1500.0f	,	0.f	,	0.0f	,	MAX_PLAN_VEL	},
{	-2000.0f	,	0.f	,	0.0f	,	MAX_PLAN_VEL	},
{	-2500.0f	,	0.f	,	0.0f	,	0.0f	}
};

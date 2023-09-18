#include "chassis_task.h"
typedef struct
{
	float x;
	float y;
}Point_t;
//点斜式结构体 ，斜率用角度制的角度代替
typedef struct
{
	Point_t point;
	//角度制
	float   direction;
	//速度
	float vel;
}Pose_t;

extern void PathInit(Pose_t *endPos);
extern void PathFlow( chassis_move_t *startPos);
extern Pose_t testPath[6];
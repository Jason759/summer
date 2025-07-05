#include "zf_common_headfile.h"
#include "Picture.h"
#include "PID.h"
#include "motor.h"
int16 basespeed=40;            //基准速度
extern uint8 center_line[];
PID_t dir={         //方向PID
	  .kp=3,
	  .ki=0,
	  .kd=0,
	  .maxout=100,
	  .minout=-100,
	 .targ=93
};
void Tracking(){    	//循迹函数
	dir.actual=center_line[90];   //预瞄点，速度越快，前瞻有大
	PID_update(&dir);
	motor_set_target(basespeed-dir.out,basespeed+dir.out);
}
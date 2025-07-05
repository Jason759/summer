#include "zf_common_headfile.h"
#include "Picture.h"
#include "PID.h"
#include "motor.h"
int16 basespeed=40;            //��׼�ٶ�
extern uint8 center_line[];
PID_t dir={         //����PID
	  .kp=3,
	  .ki=0,
	  .kd=0,
	  .maxout=100,
	  .minout=-100,
	 .targ=93
};
void Tracking(){    	//ѭ������
	dir.actual=center_line[90];   //Ԥ��㣬�ٶ�Խ�죬ǰհ�д�
	PID_update(&dir);
	motor_set_target(basespeed-dir.out,basespeed+dir.out);
}
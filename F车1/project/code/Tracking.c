#include "zf_common_headfile.h"
#include "Picture.h"
#include "PID.h"
#include "motor.h"
extern uint8 center_line[image_h];
int speed;            //基准速度
int dev=0;           //偏差
float fac=0.01;         //近大远小导致的偏差系数
PID_t dir={         //方向PID
	  .kp=0.08,
	  .ki=0.09,
	  .kd=0,
	  .maxout=100,
	  .minout=-100,
};
void Dev_calculate(){         //偏差计算
	for(int i=0;0<image_h-i-2;i++){
		dev=dev+(94-center_line[image_h-i-1])*(1+i*fac);
	}
}
void Tracking(){              //循迹函数
	Dev_calculate();
	dir.error0=dev;
	PID_update(&dir);
	motor(speed-dir.out,speed+dir.out);
}
#include "zf_common_headfile.h"
#include "Picture.h"
#include "PID.h"
#include "motor.h"
int speed=0;            //��׼�ٶ�
int dev=0;           //ƫ��
float fac=0.01;         //����ԶС���µ�ƫ��ϵ��
PID_t dir={         //����PID
	  .kp=3,
	  .ki=0,
	  .kd=0,
	  .maxout=100,
	  .minout=-100,
};
void Dev_calculate(){         //ƫ�����
	for(int i=0;0<hightest;i++){
		dev=dev+(94-center_line[image_h-i-1])*(1+i*fac);
	}
}
void Tracking(){              //ѭ������
	Dev_calculate();
	dir.error0=dev;
	PID_update(&dir);
	motor_set_target(speed-dir.out,speed+dir.out);
}
#include "zf_common_headfile.h"
#include "Picture.h"
#include "PID.h"
#include "motor.h"
int16 basespeed=30;            //��׼�ٶ�
extern uint8 center_line[];
PID_t dir={         //����PID
	  .kp=0.95,
	  .ki=0,
	  .kd=0.2,
	  .maxout=100,
	  .minout=-100,
	  .targ=93
};
uint8 check(){
	int i,j,count=0;
	for(i = image_h-1;i>image_h-5;i--)
  {
      for(j = 0;j<image_w;j++)
      {
          if(bin_image[i][j]==0)
         count++;
      }
  }
	if(count>=image_w*4)
	    return 0;
	else return 1;
}
void Tracking(){    	//ѭ������
	if(check()){
		dir.actual=center_line[45];   //Ԥ��㣬�ٶ�Խ�죬ǰհԽԶ
	 PID_update(&dir);
	motor_set_target(basespeed-dir.out,basespeed+dir.out);
	}
	else{
	
		motor_set_target(0,0);}
}
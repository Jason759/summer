#include "zf_common_headfile.h"
#include "Picture.h"
#include "PID.h"
#include "motor.h"
extern uint8 center_line[];
int16 basespeed=80;            //��׼�ٶ�
uint16 per=60; //ǰհ  60
PID_t dir={         //����PID
	  .kp=0.55,     //0.55
	  .ki=0.,
	  .kd=1,         //1
	  .maxout=30,
	  .minout=-30,
	  .targ=93 
};
uint8 check(){
	int i,j,count=0;
	for(i = image_h-1;i>image_h-5;i--)
  {
      {
      for(j = 0;j<image_w;j++)
          if(bin_image[i][j]==0)
         count++;
      }
  }
	if(count>=image_w*2)
	    return 0;
	else return 1;
}
uint8 flag=0;
void Tracking(){ 	//ѭ������
	
	if(black_stop()){
		flag=1;
	}
	if(flag==1){
		motor_set_target(0,0);
	}
	if(check()){
		dir.actual=center_line[per];   //Ԥ��㣬�ٶ�Խ�죬ǰհԽԶ 53
	 PID_update(&dir);
	 motor_set_target(basespeed,basespeed);
	}
	else{
		motor_set_target(0,0);}
	if(flag==1){
		motor_set_target(0,0);
	}
}


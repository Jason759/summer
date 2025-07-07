#include "zf_common_headfile.h"
#include "Picture.h"
#include "PID.h"
#include "motor.h"
int16 basespeed=30;            //基准速度
extern uint8 center_line[];
PID_t dir={         //方向PID
	  .kp=1.5,     //1.3
	  .ki=0,
	  .kd=0.7,    //0.7
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
void Tracking(){    	//循迹函数
	if(check()){
		dir.actual=center_line[60];   //预瞄点，速度越快，前瞻越远 53
	 PID_update(&dir);
	motor_set_target(basespeed-dir.out,basespeed+dir.out);
	}
	else{
	
		motor_set_target(0,0);}
}
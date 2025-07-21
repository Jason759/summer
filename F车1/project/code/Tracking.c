#include "zf_common_headfile.h"
#include "Picture.h"
#include "PID.h"
#include "motor.h"
extern uint8 center_line[];
extern uint8 status;
extern uint8 hightest;
int gyro;
int16 basespeed=120;    //110  //120      //基准速度
uint16 per=53; //前瞻  //53  //50
PID_t dir={         //方向PID
	  .kp=0.86,     //0.7  //0.86
	  .ki=0,
	  .kd=1.2,     	//1  //1.2
   	.kd2=0.002, //0.002  //0,002
	  .maxout=50,
	  .minout=-50,
	  .targ=93 
};
//PID_t Angle={      
//	  .kp=2,     
//	  .ki=0,
//	  .kd=0,         
//	  .maxout=50,
//	  .minout=-50,
//	  .targ=0
//};
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
	if(count>=image_w*3)
	    return 0;
	else return 1;
}
uint8 flag=0;
uint8 count=0;
void Tracking(){       	//循迹函数
	if(black_stop()){
		count++;
		if(count>5){
		  count=0;
			flag=1;
		}
	}
	if(check()){
		mpu6050_get_gyro();
		gyro=mpu6050_gyro_z;
		if(hightest>per){
			per=hightest;
		}
		  dir.actual=center_line[per];		//预瞄点，速度越快，前瞻越远 53
	    PID_gyro_update(&dir,gyro);
	    motor_set_target(basespeed,basespeed);
	}	
	else{
		motor_set_target(0,0);}
//	if(center_line[per]<95&&center_line[per]>90){
//		motor_set_target(110,110);
//	}
	if(status==3||status==4){
	  motor_set_target(80,80);
	}
	if(flag==1){
		motor_set_target(0,0);
	}
}

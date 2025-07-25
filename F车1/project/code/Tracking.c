#include "zf_common_headfile.h"
#include "Picture.h"
#include "PID.h"
#include "motor.h"
extern uint8 center_line[];
extern uint8 status;
extern uint8 hightest;
int gyro_z,gyro_y;
int16 basespeed=120;    //110  //120    //基准速度
uint16 per=58; //前瞻  //58  //58
PID_t dir={         //方向PID
	  .kp=0.82,      //0.69  //0.82
	  .ki=0,
	  .kd=1,     	//1  //1
   	.kd2=0.002, //0.002  //0,002
	  .maxout=50,
	  .minout=-50,
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
	if(count>=image_w*3)
	    return 0;
	else return 1;
}
uint8 flag=0,stop=0;
uint8 count=0;
void Tracking(){      //循迹函数
	if(black_stop()&&flag==0){
	flag=1;
	}
	if(flag==1&&black_stop()==0){
		flag=2;
	}
	if(flag==2&&black_stop()){
	  flag=3;
	}
	if(check()){
		mpu6050_get_gyro();
		gyro_z=mpu6050_gyro_z;
		gyro_y=mpu6050_gyro_y;
		if(hightest>per){
			per=hightest;
		}
		  dir.actual=center_line[per];		//预瞄点，速度越快，前瞻越远 53
	    PID_gyro_update(&dir,gyro_z);
	    motor_set_target(basespeed,basespeed);
	}	
	else{
		stop=1;
		}
//	if(center_line[hightest]<95&&center_line[hightest]>90){
//		motor_set_target(130,130);
//	}
	if(status==1||status==2||status==3||status==4||status==5){   //圆环减速
	  motor_set_target(100,100);
	}  
		if(flag==3||stop==1){
		motor_set_target(0,0);
	}
}

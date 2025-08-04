#include "zf_common_headfile.h"
#include "Picture.h"
#include "PID.h"
#include "motor.h"
#include "math.h"
extern uint8 center_line[];
extern uint8 status;
extern uint8 hightest;
int gyro_z,gyro_y;
int16 basespeed=150;    //110  //120  //140    //145基准速度
uint16 per=57; //前瞻  //58  //57   //57   //57
PID_t dir={         //方向PID
	  .kp=1.5,     //0.69  //0.82  //1.45  //1.2
	  .ki=0,
	  .kd=10,     	//1  //1   //2.5   //8.5
   	.kd2=0.005, //0.002  //0,002   //0.005  //0.004
	  .maxout=40,
	  .minout=-40,
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
void Tracking(){   	//循迹函数
	uint8 i;
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
		if(hightest>per-2){
			per=hightest+2;
		}
		dir.actual=(center_line[per-2]+center_line[per-1]+center_line[per]+center_line[per+1]+center_line[per+2])/5;		//预瞄点，速度越快，前瞻越远 53
	    PID_gyro_update(&dir,gyro_z);
	    motor_set_target(basespeed,basespeed);
	}	
	else{
		stop=1;
		}
	if(center_line[per-30]<97&&center_line[per-30]>89&&(per-30)>hightest&&R_lose(hightest,120)<5&&L_lose(hightest,120)<5){
		motor_set_target(240,240);
	}
	if(status==1||status==2||status==3||status==4||status==5){   //圆环减速
	  motor_set_target(120,120);
	}  
		if(flag==3||stop==1){
		motor_set_target(0,0);
		dir.out=0;
	}
}

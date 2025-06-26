#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Motor.h"
#include "Key.h"
#include "PWM.h"
#include "Encoder.h"
#include "Timer.h"
#include "Serial.h"
#include <stdio.h>
#include <stdlib.h>
#include "PID.h"
#include "GrayscaleSensor.h"
#include "Tracking.h"
uint8_t KeyNum;
int count=0;
float actual=0,targ=0;
void controldata(void);
float getvofadata(void){
	 char data[100];
  for(int i=0;i<tail-head-1;i++)
	   data[i]=Serial_RxPacket[head+i+1];
	return atof(data);
}
PID_t left={
	  .kp=0.08,
	  .ki=0.09,
	  .kd=0,
	  .maxout=100,
	  .minout=-100,
	  .targ=0
};
PID_t right={
	  .kp=0.08,
	  .ki=0.09,
	  .kd=0,
	  .maxout=100,
	  .minout=-100,
    .targ=0
};
int main(void)
{ 
	GrayscaleSensor_Init();
	Serial_Init();
	OLED_Init();
	Motor_Init();
	Key_Init();
	Encoder_Init();
	Timer_Init();
	while(1){
		 if (Serial_GetRxFlag()==1){
			 
		 }
			  if(RxData==1)
					Advance();
				if(RxData==2)
					Recoil();
				if(RxData==3)
					Left_turn();
				if(RxData==4)
					Right_turn();
				if(RxData==5)
					Stop();
		 
		 
		 
		 controldata();
		 //Serial_Printf("%.2f,%.2f,%.2f,%.2f,%.2f\n",right.kp,right.ki,right.kd,right.targ,right.actual);
	}
}
void TIM1_UP_IRQHandler(void)
{ 
	count++;
	if(count>5){
	if (TIM_GetITStatus(TIM1, TIM_IT_Update) == SET)
	{ 
		left.actual=Encoder1_GetSpeed();
		right.actual=Encoder2_GetSpeed();
		PID_update(&left);
		PID_update(&right);
		Motor_SetLeftSpeed(left.out);
    Motor_SetRightSpeed(right.out);
	}
	count=0;
	TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
}
}
void controldata(){
	if(Serial_RxPacket[0]=='P'&&Serial_RxPacket[1]=='1')
			left.kp=getvofadata();	
	if(Serial_RxPacket[0]=='I'&&Serial_RxPacket[1]=='1')
			left.ki=getvofadata();	
	if(Serial_RxPacket[0]=='D'&&Serial_RxPacket[1]=='1')
			left.kd=getvofadata();	
	if(Serial_RxPacket[0]=='T'&&Serial_RxPacket[1]=='1')
			left.targ=getvofadata();	
	if(Serial_RxPacket[0]=='P'&&Serial_RxPacket[1]=='2')
			right.kp=getvofadata();	
	if(Serial_RxPacket[0]=='I'&&Serial_RxPacket[1]=='2')
			right.ki=getvofadata();	
	if(Serial_RxPacket[0]=='D'&&Serial_RxPacket[1]=='2')
			right.kd=getvofadata();	
	if(Serial_RxPacket[0]=='T'&&Serial_RxPacket[1]=='2')
			right.targ=getvofadata();	
}
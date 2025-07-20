#include "PID.h"
void PID_update(PID_t *p){
		p->error1=p->error0;
		p->error0=p->targ-p->actual;
	  p->errorint+=p->error0;
		if(p->errorint>1000){p->errorint=1000;}
    if(p->errorint<-1000){p->errorint=-1000;}
	   p->out=p->kp*p->error0+p->ki*p->errorint+p->kd*(p->error0-p->error1);
		if(p->out>p->maxout)
		{p->out=p->maxout;}
		if(p->out<p->minout)
		{p->out=p->minout;}
}
void increment_pid_update(PID_t *p) {
   p->error2=p->error1;
	 p->error1=p->error0;
	 p->error0=p->targ-p->actual;
	 if(p->errorint>2000){p->errorint=2000;}
   if(p->errorint<-2000){p->errorint=-2000;}
	   p->out+=p->kp*(p->error0-p->error1)+p->ki*p->error0+p->kd*(p->error0-2*p->error1+p->error2);
	 if(p->out>p->maxout)
		{p->out=p->maxout;}
	 if(p->out<p->minout)
		{p->out=p->minout;}
	
}
void PID_calculate(int error,PID_t *p){
	  p->error1=p->error0;
	  p->error0=error;
	  p->errorint+=p->error0;
	  if(p->errorint>1000){p->errorint=1000;}
    if(p->errorint<-1000){p->errorint=-1000;}
		p->out=p->kp*p->error0+p->ki*p->errorint+p->kd*(p->error0-p->error1);
		if(p->out>p->maxout)
		{p->out=p->maxout;}
		if(p->out<p->minout)
		{p->out=p->minout;}
}
void PID_gyro_update(PID_t *p,int gyro){
	p->error1=p->error0;
		p->error0=p->targ-p->actual;
	  p->errorint+=p->error0;
		if(p->errorint>1000){p->errorint=1000;}
    if(p->errorint<-1000){p->errorint=-1000;}
	   p->out=p->kp*p->error0+p->ki*p->errorint+p->kd*(p->error0-p->error1)+p->kd2*gyro;
		if(p->out>p->maxout)
		{p->out=p->maxout;}
		if(p->out<p->minout)
		{p->out=p->minout;}
}
/*void controldata(){
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
float getvofadata(void){
	 char data[100];
  for(int i=0;i<tail-head-1;i++)
	   data[i]=Serial_RxPacket[head+i+1];
	return atof(data);
}*/


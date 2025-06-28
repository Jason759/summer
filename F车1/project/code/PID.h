#ifndef __PID_H
#define __PID_H
typedef struct{
  float out;
  float kp,ki,kd;
	float error0,error1,errorint;
  float actual,targ;
	float maxout,minout;
}PID_t;
void PID_update(PID_t*p);
void PID_calculate(int error,PID_t *p);

#endif
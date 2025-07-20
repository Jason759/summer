#ifndef __PID_H
#define __PID_H
typedef struct{
  float out;
  float kp,ki,kd,kd2;
	float error0,error1,error2,errorint;
  float actual,targ;
	float maxout,minout;
}PID_t;
void PID_update(PID_t*p);
void PID_calculate(int error,PID_t *p);
void increment_pid_update(PID_t *p);
void PID_gyro_update();
#endif
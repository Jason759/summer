#ifndef motor_h
#define motor_h

#define DIR_L               (A0 )
#define PWM_L               (TIM5_PWM_CH2_A1)

#define DIR_R               (A2 )
#define PWM_R               (TIM5_PWM_CH4_A3)
void motor(int left,int right);
void Motor_Init(void);
void motor_set_traget(int l,int r);
#endif
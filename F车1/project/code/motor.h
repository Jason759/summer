#ifndef motor_h
#define motor_h

#define MAX_DUTY            (50 )                                               // 最大 MAX_DUTY% 占空比
#define DIR_L               (A0 )
#define PWM_L               (TIM5_PWM_CH2_A1)

#define DIR_R               (A2 )
#define PWM_R               (TIM5_PWM_CH4_A3)
void motor(int left,int right);
void Motor_Init(void);

#endif
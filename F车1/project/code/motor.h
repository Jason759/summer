#ifndef motor_h
#define motor_h

#define DIR_L               (A2 )
#define PWM_L               (TIM5_PWM_CH4_A3)

#define DIR_R               (A0 )
#define PWM_R               (TIM5_PWM_CH2_A1)
//#define PWM_L_H               (TIM5_PWM_CH3_A2)
//#define PWM_L_L               (TIM5_PWM_CH4_A3)

//#define PWM_R_H               (TIM5_PWM_CH1_A0)
//#define PWM_R_L              (TIM5_PWM_CH2_A1)
void motor(int left,int right);
void Motor_Init(void);
void motor_set_target(int l,int r);
#endif
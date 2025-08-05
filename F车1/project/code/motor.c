#include "zf_common_headfile.h"
#include "PID.h"
#include "motor.h"
int8 duty = 0;
extern PID_t left;
extern PID_t right;
/**
  * ��    ����ֱ�������ʼ��
  * ��    ������
  * �� �� ֵ����
  */
void Motor_Init(void)
{
   
    gpio_init(DIR_L, GPO, GPIO_HIGH, GPO_PUSH_PULL);                            // GPIO ��ʼ��Ϊ��� Ĭ�����������
    pwm_init(PWM_L, 17000, 0);                                                  // PWM ͨ����ʼ��Ƶ�� 17KHz ռ�ձȳ�ʼΪ 0
    
    gpio_init(DIR_R, GPO, GPIO_HIGH, GPO_PUSH_PULL);                            // GPIO ��ʼ��Ϊ��� Ĭ�����������
    pwm_init(PWM_R, 17000, 0);	// PWM ͨ����ʼ��Ƶ�� 17KHz ռ�ձȳ�ʼΪ 0
//	pwm_init(PWM_L_H, 17000, 0);
//	pwm_init(PWM_L_L, 17000, 0);
//	pwm_init(PWM_R_H, 17000, 0);
//	pwm_init(PWM_R_L, 17000, 0);
}

/**
  * ��    ����ֱ����������ٶ�
  * ��    ����duty Ҫ���õ��ٶȣ���Χ��-100~100
  * �� �� ֵ����
  */
void Motor_SetLeftSpeed(int8_t duty)
{		
	if (duty >= 0)							//���������ת���ٶ�ֵ
	{
		gpio_set_level(DIR_L, GPIO_HIGH);
		pwm_set_duty(PWM_L, duty*100 );                  
                              
		
	}
	else									//���򣬼����÷�ת���ٶ�ֵ
	{  
		 gpio_set_level(DIR_L, GPIO_LOW);
     pwm_set_duty(PWM_L, (-duty)*100);
	}
}
void Motor_SetRightSpeed(int8_t duty)
{
	if (duty >= 0)							//���������ת���ٶ�ֵ
	{
		gpio_set_level(DIR_R, GPIO_HIGH);
		pwm_set_duty(PWM_R, duty*100);                  // ����ռ�ձ�           
	}
	else									//���򣬼����÷�ת���ٶ�ֵ
	{ 
		gpio_set_level(DIR_R, GPIO_LOW);
    pwm_set_duty(PWM_R, (-duty)*100);               // ����ռ��
}
	}
//void Motor_SetLeftSpeed(int8_t duty)
//{		
//	if (duty >= 0)							//���������ת���ٶ�ֵ
//	{
//		
//		pwm_set_duty(PWM_L_H, duty*100 );
//    pwm_set_duty(PWM_L_L, 0 );   	
//                              
//	}
//	else									//���򣬼����÷�ת���ٶ�ֵ
//	{  
//		 pwm_set_duty(PWM_L_H, 0 );   
//     pwm_set_duty(PWM_L_L, (-duty)*100);
//	}
//}
//void Motor_SetRightSpeed(int8_t duty)
//{
//	if (duty >= 0)							//���������ת���ٶ�ֵ
//	{
//		pwm_set_duty(PWM_R_L,0);
//		pwm_set_duty(PWM_R_H, duty*100);                  // ����ռ�ձ�           
//	}
//	else									//���򣬼����÷�ת���ٶ�ֵ
//	{ 
//		pwm_set_duty(PWM_R_H,0);
//    pwm_set_duty(PWM_R_L, (-duty)*100);               // ����ռ��
//}
//	}
void motor(int left,int right){
	if ((left*100)>=PWM_DUTY_MAX){left=PWM_DUTY_MAX/100;}
	if (((-left)*100)>=PWM_DUTY_MAX){left=-(PWM_DUTY_MAX/100);}
	if ((right*100)>=PWM_DUTY_MAX){right=PWM_DUTY_MAX/100;}
	if (((-right)*100)>=PWM_DUTY_MAX){right=-(PWM_DUTY_MAX/100);}
	Motor_SetRightSpeed(right);
	Motor_SetLeftSpeed(left);
}
void motor_set_target(int l,int r){
	left.targ=l;
	right.targ=r;
}

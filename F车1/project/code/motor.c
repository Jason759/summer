#include "zf_common_headfile.h"
#include "PID.h"
#include "motor.h"
int8 duty = 0;
extern PID_t left;
extern PID_t right;
/**
  * 函    数：直流电机初始化
  * 参    数：无
  * 返 回 值：无
  */
void Motor_Init(void)
{
   
    gpio_init(DIR_L, GPO, GPIO_HIGH, GPO_PUSH_PULL);                            // GPIO 初始化为输出 默认上拉输出高
    pwm_init(PWM_L, 17000, 0);                                                  // PWM 通道初始化频率 17KHz 占空比初始为 0

    gpio_init(DIR_R, GPO, GPIO_HIGH, GPO_PUSH_PULL);                            // GPIO 初始化为输出 默认上拉输出高
    pwm_init(PWM_R, 17000, 0);                                                  // PWM 通道初始化频率 17KHz 占空比初始为 0
}

/**
  * 函    数：直流电机设置速度
  * 参    数：duty 要设置的速度，范围：-100~100
  * 返 回 值：无
  */
void Motor_SetLeftSpeed(int8_t duty)
{
	if ((duty*100)>PWM_DUTY_MAX){duty=PWM_DUTY_MAX/100;}
	if (((-duty)*100)>PWM_DUTY_MAX){duty=-PWM_DUTY_MAX/100;}
		
	if (duty >= 0)							//如果设置正转的速度值
	{
		gpio_set_level(DIR_L, GPIO_HIGH);
		pwm_set_duty(PWM_L, duty*100 );                  
                              
	}
	else									//否则，即设置反转的速度值
	{  
		 gpio_set_level(DIR_L, GPIO_LOW);
     pwm_set_duty(PWM_L, (-duty)*100);
	}
}
void Motor_SetRightSpeed(int8_t duty)
{
	if (duty >= 0)							//如果设置正转的速度值
	{
		gpio_set_level(DIR_R, GPIO_HIGH);
		pwm_set_duty(PWM_R, duty*100);                  // 计算占空比           
	}
	else									//否则，即设置反转的速度值
	{ 
		gpio_set_level(DIR_R, GPIO_LOW);
    pwm_set_duty(PWM_R, (-duty)*100);               // 计算占空
}
	}
void motor(int left,int right){
	Motor_SetRightSpeed(left);
	Motor_SetLeftSpeed(right);
}
void motor_set_traget(int l,int r){
	left.targ=l;
	right.targ=r;
}

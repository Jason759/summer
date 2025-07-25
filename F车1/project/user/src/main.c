/*********************************************************************************************************************
* MM32F327X-G8P Opensourec Library 即（MM32F327X-G8P 开源库）是一个基于官方 SDK 接口的第三方开源库
* Copyright (c) 2022 SEEKFREE 逐飞科技
* 
* 本文件是 MM32F327X-G8P 开源库的一部分
* 
* MM32F327X-G8P 开源库 是免费软件
* 您可以根据自由软件基金会发布的 GPL（GNU General Public License，即 GNU通用公共许可证）的条款
* 即 GPL 的第3版（即 GPL3.0）或（您选择的）任何后来的版本，重新发布和/或修改它
* 
* 本开源库的发布是希望它能发挥作用，但并未对其作任何的保证
* 甚至没有隐含的适销性或适合特定用途的保证
* 更多细节请参见 GPL
* 
* 您应该在收到本开源库的同时收到一份 GPL 的副本
* 如果没有，请参阅<https://www.gnu.org/licenses/>
* 
* 额外注明：
* 本开源库使用 GPL3.0 开源许可证协议 以上许可申明为译文版本
* 许可申明英文版在 libraries/doc 文件夹下的 GPL3_permission_statement.txt 文件中
* 许可证副本在 libraries 文件夹下 即该文件夹下的 LICENSE 文件
* 欢迎各位使用并传播本程序 但修改内容时必须保留逐飞科技的版权声明（即本声明）
* 
* 文件名称          main
* 公司名称          成都逐飞科技有限公司
* 版本信息          查看 libraries/doc 文件夹内 version 文件 版本说明
* 开发环境          IAR 8.32.4 or MDK 5.37
* 适用平台          MM32F327X_G8P
* 店铺链接          https://seekfree.taobao.com/
* 
* 修改记录
* 日期              作者                备注
* 2022-08-10        Teternal            first version
********************************************************************************************************************/
#include "zf_common_headfile.h"
#include "Picture.h"
#include "motor.h"
#include "Encoder.h"
#include "auto_menu.h"
#include "PID.h"
#include "BEEP.h"
extern uint8 image_thereshold;
uint8 showflag=0;
PID_t left={
	  .kp=0.3,   //0.3
	  .ki=0.012,  //0.02//0.01
	  .kd=0,
	  .maxout=50,
	  .minout=-50,
	  .targ=0,
};

PID_t right={
	  .kp=0.0,   //0.3
	  .ki=0.00,  //0.02//0.01
	  .kd=0,
	  .maxout=50,
	  .minout=-50,
    .targ=0
};
int main (void)
{   
    clock_init(SYSTEM_CLOCK_120M);                                              // 初始化芯片时钟 工作频率为 120MHz
    debug_init();                                                               // 初始化默认 Debug UART
    mt9v03x_init();
	  mpu6050_init();
	  wireless_uart_init();
	  //ImagePerspective_Init();   //逆透视初始化
	  Encoder_Init();
	  pit_ms_init(TIM2_PIT, 1);   
    pit_ms_init(TIM6_PIT, 1);  
	  interrupt_set_priority(TIM2_IRQn, 0);
	  interrupt_set_priority(TIM6_IRQn, 1);
	  menu_init();
	  while(1){
		beep_progress();
		menu_adaptive_display();
    show_process(NULL);
		image_process();
		//picture_process();
		 //image_show();
	switch(showflag){
	  case 1:  image_show(); break;
    case 2:  picture_process(); break;
		default: break;
	}
	//rep_show(); //显示逆透视图像
    }
}
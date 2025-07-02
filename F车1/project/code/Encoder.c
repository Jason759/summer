#include "zf_common_headfile.h"
int32 encoder1;
int32 encoder2;
void Encoder_Init(){
encoder_quad_init(TIM3_ENCODER, TIM3_ENCODER_CH1_B4, TIM3_ENCODER_CH2_B5);
encoder_quad_init(TIM4_ENCODER, TIM4_ENCODER_CH1_B6, TIM4_ENCODER_CH2_B7);
}
int32 Encoder1_get(){
	
	return encoder1/50;
}
int32 Encoder2_get(){
	
	return -encoder2/50;
}
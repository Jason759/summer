#include "zf_common_headfile.h"
#ifndef __PICTURE_H
#define __PICTURE_H
#define image_h	120//ͼ��߶�
#define image_w	188//ͼ����
#define white_pixel	255
#define black_pixel	0
#define uesr_RED     0XF800    //��ɫ
#define uesr_GREEN   0X07E0    //��ɫ
#define uesr_BLUE    0X001F    //��ɫ
#define bin_jump_num	1//�����ĵ���
#define border_max	image_w-2 //�߽����ֵ
#define border_min	1	//�߽���Сֵ	
extern uint8 original_image[image_h][image_w];
extern uint8 bin_image[image_h][image_w];//ͼ������
int adaptiveThreshold(int *image);
uint8 otsuThreshold(uint8 *image);
void picture_process();
extern void image_process(void); //ֱ�����жϻ�ѭ������ô˳���Ϳ���ѭ��ִ����

#endif

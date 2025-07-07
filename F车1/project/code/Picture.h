#include "zf_common_headfile.h"
#ifndef __PICTURE_H
#define __PICTURE_H
#define image_h	120//图像高度
#define image_w	188//图像宽度
#define white_pixel	255
#define black_pixel	0
#define uesr_RED     0XF800    //红色
#define uesr_GREEN   0X07E0    //绿色
#define uesr_BLUE    0X001F    //蓝色
#define bin_jump_num	1//跳过的点数
#define border_max	image_w-3 //边界最大值
#define border_min	2	//边界最小值	
#define RESULT_ROW 100//结果图行列
#define RESULT_COL 114
extern uint8 center_line[image_h];
extern uint8 hightest;
extern uint8 original_image[image_h][image_w];
extern uint8 bin_image[image_h][image_w];//图像数组
int adaptiveThreshold(int *image);
uint8 otsuThreshold(uint8 *image);
void picture_process();
void image_show();
void get_turning_point();
extern void image_process(void); //直接在中断或循环里调用此程序就可以循环执行了
void ImagePerspective_Init(void);
void rep_show();
void ImagePerspective_Init(void);
#endif

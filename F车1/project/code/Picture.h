#include "zf_common_headfile.h"
#ifndef __PICTURE_H
#define __PICTURE_H
#define image_h	120//ͼ��߶�
#define image_w	188//ͼ����

int adaptiveThreshold(int *image);
uint8 otsuThreshold(const uint8 *image);
void picture_process();
#endif

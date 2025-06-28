#include "zf_common_headfile.h"
#ifndef __PICTURE_H
#define __PICTURE_H
#define image_h	120//Í¼Ïñ¸ß¶È
#define image_w	188//Í¼Ïñ¿í¶È

int adaptiveThreshold(int *image);
uint8 otsuThreshold(const uint8 *image);
void picture_process();
#endif

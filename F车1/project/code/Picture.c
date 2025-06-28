#include "zf_common_headfile.h"
#include "Picture.h""
/*变量声明*/
uint8 original_image[image_h][image_w];
uint8 image_thereshold;//图像分割阈值
void Get_image(uint8(*mt9v03x_image)[image_w])
{
#define use_num		1	//1就是不压缩，2就是压缩一倍	
	uint8 i = 0, j = 0, row = 0, line = 0;
    for (i = 0; i < image_h; i += use_num)          //
    {
        for (j = 0; j <image_w; j += use_num)     //
        {
            original_image[row][line] = mt9v03x_image[i][j];//这里的参数填写摄像头采集到的图像
			line++;
        }
        line = 0;
        row++;
    }
}

uint8 otsuThreshold(const uint8 *image) //找阈值——大津法
{
#define GrayScale 256
	  int i;
    const uint8 *data = image;
    int HistGram[GrayScale] = {0};
	  int Amount =128;
    int PixelBack = 0;   //前景像素点数
    int PixelIntegralBack = 0;  
    int PixelIntegral = 0;
    int PixelIntegralFore = 0;
    int PixelFore = 0;
    double OmegaBack=0, OmegaFore=0, MicroBack=0, MicroFore=0, SigmaB=0, Sigma=0; // 类间方差;
    int MinValue=0, MaxValue=0;
    int Threshold = 0;
    for (i = 0; i <MT9V03X_H; i++)
    {
        for(int j=0;j<MT9V03X_W;j++){
        HistGram[data[i*MT9V03X_W+j]]++; //统计每个灰度值的个数信息
        }
    }
    for (MinValue = 0; MinValue < 256 && HistGram[MinValue] == 0; MinValue++) ;        //获取最小灰度的值
    for (MaxValue = 255; MaxValue > MinValue && HistGram[MinValue] == 0; MaxValue--) ; //获取最大灰度的值

    if (MaxValue == MinValue)
    {
        return MaxValue;          // 图像中只有一个颜色
    }
    if (MinValue + 1 == MaxValue)
    {
        return MinValue;      // 图像中只有二个颜色
    }
    PixelIntegral = 0;
    for (i= MinValue; i <= MaxValue; i++)
    {
        PixelIntegral += HistGram[i] * i;//灰度值总数
    }
    SigmaB = -1;
    for (i = MinValue; i < MaxValue; i++)
    {
          PixelFore = PixelFore + HistGram[i];    //前景像素点数
          PixelBack = Amount - PixelFore;         //背景像素点数
          OmegaFore = (double)PixelFore / Amount;//前景像素百分比
          OmegaBack = (double)PixelBack / Amount;//背景像素百分比
          PixelIntegralFore += HistGram[i] * i;  //前景灰度值
          PixelIntegralBack = PixelIntegral - PixelIntegralBack;//背景灰度值
          MicroFore = (double)PixelIntegralFore / PixelFore;//前景灰度百分比
          MicroBack = (double)PixelIntegralBack / PixelBack;//背景灰度百分比
          Sigma = OmegaBack * OmegaFore * (MicroFore - MicroBack) * (MicroFore - MicroBack);//类间方差
          if (Sigma > SigmaB)//遍历最大的类间方差g
          {
              SigmaB = Sigma;
              Threshold = (int)i;
          }
    }
   return Threshold;
}
void picture_process(){
	image_thereshold=otsuThreshold((const uint8*)mt9v03x_image);
	if(mt9v03x_finish_flag)
{
	ips200_show_gray_image(0, 0, mt9v03x_image[0], MT9V03X_H, MT9V03X_W, 240,180 ,image_thereshold);
	mt9v03x_finish_flag=0;
}
}
#include "zf_common_headfile.h"
#include "Picture.h"
int my_abs(int value)
{
if(value>=0) return value;
else return -value;
}

int16 limit_a_b(int16 x, int a, int b)
{
    if(x<a) x = a;
    if(x>b) x = b;
    return x;
}
void image_display();
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

uint8 otsuThreshold(uint8 *image) //找阈值——大津法(已优化-遇到递减直接退出)
{
#define GrayScale 256
	  int i;
    const uint8 *data = image;
    int HistGram[GrayScale] = {0};
	  int Amount =image_w*image_h/4;
    int PixelBack = 0;   //前景像素点数
    int PixelIntegralBack = 0;  
    int PixelIntegral = 0;
    int PixelIntegralFore = 0;
    int PixelFore = 0;
    double OmegaBack=0, OmegaFore=0, MicroBack=0, MicroFore=0, SigmaB=0, Sigma=0; // 类间方差;
    int MinValue=0, MaxValue=0;
    int Threshold = 0;
    for (i = 0; i <MT9V03X_H; i+=2)
    {
        for(int j=0;j<MT9V03X_W;j+=2){
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
    for (i= MinValue; i <= MaxValue; i+=2)
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
					if(Sigma<SigmaB){break;}
						
    }
   return Threshold;
}
uint8 bin_image[image_h][image_w];//图像数组
void turn_to_bin(void)
{
  uint8 i,j;
 image_thereshold =otsuThreshold(original_image[0]);
  for(i = 0;i<image_h;i++)
  {
      for(j = 0;j<image_w;j++)
      {
          if(original_image[i][j]>image_thereshold)bin_image[i][j] = white_pixel;
          else bin_image[i][j] = black_pixel;
      }
  }
}


/*
函数名称：void get_start_point()
功能说明：从最下方开始寻找两个边界的边界点作为八邻域循环的起始点,找不到一直找，直到找到
参数说明：
函数返回：无
备    注：
example：  get_start_point(image_h-2)
 */
uint8 start_point_l[2] = { 0 };//左边起点的x，y值
uint8 start_point_r[2] = { 0 };//右边起点的x，y值
uint8 get_start_point(void)
{ 
	//清零
	start_point_l[0] = 0;//x
	start_point_l[1] = 0;//y

	start_point_r[0] = 0;//x
	start_point_r[1] = 0;//y
	uint8 i = 0,l_found = 0,r_found = 0;
	for(uint8 j=image_h-2;j>image_h-10;j--){

		//从中间往左边，先找起点
	for (i = image_w / 2; i > border_min; i--)
	{
		start_point_l[0] = i-2;//x
		start_point_l[1] = j;//y
		if (bin_image[j][i] == 255 && bin_image[j][i - 1] == 255&&bin_image[j][i-2]==0)
		{
			//printf("找到左边起点image[%d][%d]\n", j,i-2);
			l_found = 1;
			break;
		}
	}

	for (i = image_w / 2; i < border_max; i++)
	{
		start_point_r[0] = i+2;//x
		start_point_r[1] = j;//y
		if (bin_image[j][i] == 255 && bin_image[j][i + 1] == 255&&bin_image[j][i + 2]==0)
		{
			//printf("找到右边起点image[%d][%d]\n",start_row, i);
			r_found = 1;
			break;
		}
	}
if(l_found&&r_found) { //找到退出

   break;}	
	}
   return 1;	
	}

/*
函数名称：void search_l_r(uint16 break_flag, uint8(*image)[image_w],uint16 *l_stastic, uint16 *r_stastic,
							uint8 l_start_x, uint8 l_start_y, uint8 r_start_x, uint8 r_start_y,uint8*hightest)

功能说明：八邻域正式开始找右边点的函数，输入参数有点多，调用的时候不要漏了，这个是左右线一次性找完。
参数说明：
break_flag_r			：最多需要循环的次数
(*image)[image_w]		：需要进行找点的图像数组，必须是二值图,填入数组名称即可
					   特别注意，不要拿宏定义名字作为输入参数，否则数据可能无法传递过来
*l_stastic				：统计左边数据，用来输入初始数组成员的序号和取出循环次数
*r_stastic				：统计右边数据，用来输入初始数组成员的序号和取出循环次数
l_start_x				：左边起点横坐标
l_start_y				：左边起点纵坐标
r_start_x				：右边起点横坐标
r_start_y				：右边起点纵坐标
hightest				：循环结束所得到的最高高度
函数返回：无
备    注：
example：
	search_l_r((uint16)USE_num,image,&data_stastics_l, &data_stastics_r,start_point_l[0],
				start_point_l[1], start_point_r[0], start_point_r[1],&hightest);
 */
#define USE_num	image_h*3	//定义找点的数组成员个数按理说300个点能放下，但是有些特殊情况确实难顶，多定义了一点

 //存放点的x，y坐标
uint16 points_l[(uint16)USE_num][2] = { {  0 } };//左线
uint16 points_r[(uint16)USE_num][2] = { {  0 } };//右线
uint16 dir_r[(uint16)USE_num] = { 0 };//用来存储右边生长方向
uint16 dir_l[(uint16)USE_num] = { 0 };//用来存储左边生长方向
uint16 data_stastics_l = 0;//统计左边找到点的个数
uint16 data_stastics_r = 0;//统计右边找到点的个数
uint8 hightest = 0;//最高点
void search_l_r(uint16 break_flag, uint8(*image)[image_w], uint16 *l_stastic, uint16 *r_stastic, uint8 l_start_x, uint8 l_start_y, uint8 r_start_x, uint8 r_start_y, uint8*hightest)
{

	uint8 i = 0, j = 0;

	//左边变量
	uint8 search_filds_l[8][2] = { {  0 } };    //中心点八领域的点坐标
	uint8 index_l = 0;                  //索引下标
	uint8 temp_l[8][2] = { {  0 } };   //缓存
	uint8 center_point_l[2] = {  0 };  //中心坐标点
	uint16 l_data_statics;//统计左边
	//定义八个邻域
	static int8 seeds_l[8][2] = { {0,  1},{-1,1},{-1,0},{-1,-1},{0,-1},{1,-1},{1,  0},{1, 1}, };
	//{-1,-1},{0,-1},{+1,-1},
	//{-1, 0},	     {+1, 0},
	//{-1,+1},{0,+1},{+1,+1},
	//这个是顺时针

	//右边变量
	uint8 search_filds_r[8][2] = { {  0 } };
	uint8 center_point_r[2] = { 0 };//中心坐标点
	uint8 index_r = 0;//索引下标
	uint8 temp_r[8][2] = { {  0 } };
	uint16 r_data_statics;//统计右边
	//定义八个邻域
	static int8 seeds_r[8][2] = { {0,  1},{1,1},{1,0}, {1,-1},{0,-1},{-1,-1}, {-1,  0},{-1, 1}, };
	//{-1,-1},{0,-1},{+1,-1},
	//{-1, 0},	     {+1, 0},
	//{-1,+1},{0,+1},{+1,+1},
	//这个是逆时针

	l_data_statics = *l_stastic;//统计找到了多少个点，方便后续把点全部画出来
	r_data_statics = *r_stastic;//统计找到了多少个点，方便后续把点全部画出来

	//第一次更新坐标点  将找到的起点值传进来
	center_point_l[0] = l_start_x;//x
	center_point_l[1] = l_start_y;//y
	center_point_r[0] = r_start_x;//x
	center_point_r[1] = r_start_y;//y

		//开启邻域循环
	while (break_flag--)
	{

		//左边
		for (i = 0; i < 8; i++)//传递8F坐标
		{
			search_filds_l[i][0] = center_point_l[0] + seeds_l[i][0];//x
			search_filds_l[i][1] = center_point_l[1] + seeds_l[i][1];//y
		}
		//中心坐标点填充到已经找到的点内
		points_l[l_data_statics][0] = center_point_l[0];//x
		points_l[l_data_statics][1] = center_point_l[1];//y
		l_data_statics++;//索引加一

		//右边
		for (i = 0; i < 8; i++)//传递8F坐标
		{
			search_filds_r[i][0] = center_point_r[0] + seeds_r[i][0];//x
			search_filds_r[i][1] = center_point_r[1] + seeds_r[i][1];//y
		}
		//中心坐标点填充到已经找到的点内
		points_r[r_data_statics][0] = center_point_r[0];//x
		points_r[r_data_statics][1] = center_point_r[1];//y

		index_l = 0;//先清零，后使用
		for (i = 0; i < 8; i++)
		{
			temp_l[i][0] = 0;//先清零，后使用   
			temp_l[i][1] = 0;//先清零，后使用
		}

		//左边判断
		for (i = 0; i < 8; i++)
		{
			if (image[search_filds_l[i][1]][search_filds_l[i][0]] == 0
				&& image[search_filds_l[(i + 1) & 7][1]][search_filds_l[(i + 1) & 7][0]] == 255)
			{
				temp_l[index_l][0] = search_filds_l[(i)][0];
				temp_l[index_l][1] = search_filds_l[(i)][1];
				index_l++;
				dir_l[l_data_statics - 1] = (i);//记录生长方向
			}

			if (index_l)
			{
				//更新坐标点
				center_point_l[0] = temp_l[0][0];//x
				center_point_l[1] = temp_l[0][1];//y
				for (j = 0; j < index_l; j++)
				{
					if (center_point_l[1] > temp_l[j][1])
					{
						center_point_l[0] = temp_l[j][0];//x
						center_point_l[1] = temp_l[j][1];//y
					}
				}
			}

		}
		if ((points_r[r_data_statics][0]== points_r[r_data_statics-1][0]&& points_r[r_data_statics][0] == points_r[r_data_statics - 2][0]
			&& points_r[r_data_statics][1] == points_r[r_data_statics - 1][1] && points_r[r_data_statics][1] == points_r[r_data_statics - 2][1])
			||(points_l[l_data_statics-1][0] == points_l[l_data_statics - 2][0] && points_l[l_data_statics-1][0] == points_l[l_data_statics - 3][0]
				&& points_l[l_data_statics-1][1] == points_l[l_data_statics - 2][1] && points_l[l_data_statics-1][1] == points_l[l_data_statics - 3][1]))
		{
			//printf("三次进入同一个点，退出\n");
			break;
		}
		if (my_abs(points_r[r_data_statics][0] - points_l[l_data_statics - 1][0]) < 2
			&& my_abs(points_r[r_data_statics][1] - points_l[l_data_statics - 1][1] < 2)
			)
		{
			//printf("\n左右相遇退出\n");	
			*hightest = (points_r[r_data_statics][1] + points_l[l_data_statics - 1][1]) >> 1;//取出最高点
			//printf("\n在y=%d处退出\n",*hightest);
			break;
		}
		if ((points_r[r_data_statics][1] < points_l[l_data_statics - 1][1]))
		{
			printf("\n如果左边比右边高了，左边等待右边\n");	
			continue;//如果左边比右边高了，左边等待右边
		}
		if (dir_l[l_data_statics - 1] == 7
			&& (points_r[r_data_statics][1] > points_l[l_data_statics - 1][1]))//左边比右边高且已经向下生长了
		{
			//printf("\n左边开始向下了，等待右边，等待中... \n");
			center_point_l[0] = points_l[l_data_statics - 1][0];//x
			center_point_l[1] = points_l[l_data_statics - 1][1];//y
			l_data_statics--;
		}
		r_data_statics++;//索引加一

		index_r = 0;//先清零，后使用
		for (i = 0; i < 8; i++)
		{
			temp_r[i][0] = 0;//先清零，后使用
			temp_r[i][1] = 0;//先清零，后使用
		}

		//右边判断
		for (i = 0; i < 8; i++)
		{
			if (image[search_filds_r[i][1]][search_filds_r[i][0]] == 0
				&& image[search_filds_r[(i + 1) & 7][1]][search_filds_r[(i + 1) & 7][0]] == 255)
			{
				temp_r[index_r][0] = search_filds_r[(i)][0];
				temp_r[index_r][1] = search_filds_r[(i)][1];
				index_r++;//索引加一
				dir_r[r_data_statics - 1] = (i);//记录生长方向
				//printf("dir[%d]:%d\n", r_data_statics - 1, dir_r[r_data_statics - 1]);
			}
			if (index_r)
			{

				//更新坐标点
				center_point_r[0] = temp_r[0][0];//x
				center_point_r[1] = temp_r[0][1];//y
				for (j = 0; j < index_r; j++)
				{
					if (center_point_r[1] > temp_r[j][1])
					{
						center_point_r[0] = temp_r[j][0];//x
						center_point_r[1] = temp_r[j][1];//y
					}
				}

			}
		}


	}


	//取出循环次数
	*l_stastic = l_data_statics;
	*r_stastic = r_data_statics;

}
/*
函数名称：void get_left(uint16 total_L)
功能说明：从八邻域边界里提取需要的边线，找的是最靠近中间的点
参数说明：
total_L	：找到的点的总数
函数返回：无
备    注：
example： get_left(data_stastics_l );
 */
uint8 l_border[image_h];//左线数组
uint8 r_border[image_h];//右线数组
uint8 center_line[image_h];//中线数组
void get_left(uint16 total_L)
{
	uint8 i = 0;
	uint16 j = 0;
	uint8 h = 0;
	//初始化
	for (i = 0;i<image_h;i++)
	{
		l_border[i] = border_min;
	}
	h = image_h - 2;
	//左边
	for (j = 0; j < total_L; j++)
	{
		//printf("%d\n", j);
		if (points_l[j][1] == h)
		{
			l_border[h] = points_l[j][0]+1;
		}
		else continue; //每行只取一个点，没到下一行就不记录
		h--;
		if (h == 0) 
		{
			break;//到最后一行退出
		}
	}
}
/*
函数名称：void get_right(uint16 total_R)
功能说明：从八邻域边界里提取需要的边线
参数说明：
total_R  ：找到的点的总数
函数返回：无
备    注：
example：get_right(data_stastics_r);
 */
void get_right(uint16 total_R)
{
	uint8 i = 0;
	uint16 j = 0;
	uint8 h = 0;
	for (i = 0; i < image_h; i++)
	{
		r_border[i] = border_max;//右边线初始化放到最右边，左边线放到最左边，这样八邻域闭合区域外的中线就会在中间，不会干扰得到的数据
	}
	h = image_h - 2;
	//右边
	for (j = 0; j < total_R; j++)
	{
		if (points_r[j][1] == h)
		{
			r_border[h] = points_r[j][0] - 1;
		}
		else continue;//每行只取一个点，没到下一行就不记录
		h--;
		if (h == 0)break;//到最后一行退出
	}
}
/*
函数名称：uint8 judge_border(uint16 total_L,uint16 total_R)
功能说明：判断丢线
参数说明：total_L,total_R  ：找到的点的总数
函数返回：1,0,
备    注：
example：get_right(data_stastics_r);
 */
uint8 judge_border(uint16 total_L,uint16 total_R){
	uint8 i=0,outl=0,outr=0;
	for(i=0;i< total_L;i++){
		if(l_border[i]<3)  {outl++;}
	}
	for(i=0;i< total_R;i++){
		if(r_border[i]>154)  {outr++;}
	}
	if(outl>30&&outr>30)
		 return 1;
	else 
		 return 0;
}
//定义膨胀和腐蚀的阈值区间
#define threshold_max	255*5//此参数可根据自己的需求调节
#define threshold_min	255*2//此参数可根据自己的需求调节
//图像滤波
void image_filter(uint8(*bin_image)[image_w])//形态学滤波，简单来说就是膨胀和腐蚀的思想
{
	uint16 i, j;
	uint32 num = 0;


	for (i = 1; i < image_h - 1; i++)
	{
		for (j = 1; j < (image_w - 1); j++)
		{
			//统计八个方向的像素值
			num =
				bin_image[i - 1][j - 1] + bin_image[i - 1][j] + bin_image[i - 1][j + 1]
				+ bin_image[i][j - 1] + bin_image[i][j + 1]
				+ bin_image[i + 1][j - 1] + bin_image[i + 1][j] + bin_image[i + 1][j + 1];


			if (num >= threshold_max && bin_image[i][j] == 0)
			{
				bin_image[i][j] = 255;//白  可以搞成宏定义，方便更改

			}
			if (num <= threshold_min && bin_image[i][j] == 255)
			{
				bin_image[i][j] = 0;//黑
			}
		}
	}

}

/*
函数名称：void image_draw_rectan(uint8(*image)[image_w])
功能说明：给图像画一个黑框，防止找不到边线
参数说明：uint8(*image)[image_w]	图像首地址
函数返回：无
修改时间：2022年9月8日
备    注：
example： image_draw_rectan(bin_image);
 */
void image_draw_rectan(uint8(*image)[image_w])
{

	uint8 i = 0;
	for (i = 0; i < image_h; i++)
	{
		image[i][0] = 0;
		image[i][1] = 0;
		image[i][image_w - 1] = 0;
		image[i][image_w - 2] = 0;

	}
	for (i = 0; i < image_w; i++)
	{
		image[0][i] = 0;
		image[1][i] = 0;
		//image[image_h-1][i] = 0;

	}
}
float Slope_Calculate(uint8 begin, uint8 end, uint8 *border)   //最小二乘法，计算斜率
{
	float xsum = 0, ysum = 0, xysum = 0, x2sum = 0;
	int16 i = 0;
	float result = 0;
	static float resultlast;

	for (i = begin; i < end; i++)
	{
		xsum += i;
		ysum += border[i];
		xysum += i * (border[i]);
		x2sum += i * i;

	}
	if ((end - begin)*x2sum - xsum * xsum) //判断除数是否为零
	{
		result = ((end - begin)*xysum - xsum * ysum) / ((end - begin)*x2sum - xsum * xsum);
		resultlast = result;
	}
	else
	{
		result = resultlast;
	}
	return result;
}

void calculate_s_i(uint8 start, uint8 end, uint8 *border, float *slope_rate, float *intercept)
{
	uint16 i, num = 0;
	uint16 xsum = 0, ysum = 0;
	float y_average, x_average;

	num = 0;
	xsum = 0;
	ysum = 0;
	y_average = 0;
	x_average = 0;
	for (i = start; i < end; i++)
	{
		xsum += i;
		ysum += border[i];
		num++;
	}

	//计算各个平均数
	if (num)
	{
		x_average = (float)(xsum / num);
		y_average = (float)(ysum / num);

	}

	/*计算斜率*/
	*slope_rate = Slope_Calculate(start, end, border);//斜率
	*intercept = y_average - (*slope_rate)*x_average;//截距
}
//* @brief 十字补线函数
//* @param uint8(*image)[image_w]		输入二值图像
//* @param uint8 *l_border			输入左边界首地址
//* @param uint8 *r_border			输入右边界首地址
//* @param uint16 total_num_l			输入左边循环总次数
//* @param uint16 total_num_r			输入右边循环总次数
//* @param uint16 *dir_l				输入左边生长方向首地址
//* @param uint16 *dir_r				输入右边生长方向首地址
//* @param uint16(*points_l)[2]		输入左边轮廓首地址
//* @param uint16(*points_r)[2]		输入右边轮廓首地址
//*  @see CTest		cross_fill(image,l_border, r_border, data_statics_l, data_statics_r, dir_l, dir_r, points_l, points_r);
//* @return 返回说明
//*     -<em>false</em> fail
//*     -<em>true</em> succeed
void cross_fill(uint8(*image)[image_w], uint8 *l_border, uint8 *r_border, uint16 total_num_l, uint16 total_num_r,
							 uint16 *dir_l, uint16 *dir_r, uint16(*points_l)[2], uint16(*points_r)[2])
{
	uint8 i;
	uint8 flaga=0,flagb=0,flagc=0,flagd=0;
	uint8 break_num_a = 0;
	uint8 break_num_b= 0;
	uint8 break_num_c= 0;
	uint8 break_num_d= 0;
	uint8 start, end;
	float slope_l_rate = 0, intercept_l = 0;
	uint8 lnum6=0;
	uint8 rnum6=0;
	for (i = 0; i < total_num_l; i++)
	{
		if (dir_l[i] == 6 )    //生长方向为六的点
		{
		    lnum6++;    
		}
		if(dir_l[i]==6&&dir_l[i+1]==4){ //判断拐点是否存在:
		          flagc++;
		}
	}
	for (i = 0; i < total_num_r; i++)
	{
		if (dir_r[i] == 6 )
		{
		    rnum6++;    
		}
		if(dir_r[i]==6&&dir_r[i+1]==4){
		          flagd++;
		}
	}
	if(lnum6>10&&rnum6>10&&flagc&&flagd){
	for (i = 2; i < total_num_l; i++)
	{
		if (dir_l[i - 2] == 6 && dir_l[i-1] == 6 && dir_l[i] == 6 && dir_l[i+1]!=6)   //找拐点c
		{
			break_num_c = points_l[i][1];//传递y坐标
			break;
		}
	}
	for (i = 2; i < total_num_l; i++)
	{
		if (dir_r[i - 2] == 6 && dir_r[i-1] == 6 && dir_r[i] == 6 && dir_r[i+1]!=6)   //找拐点d
		{
			break_num_d = points_l[i][1];//传递y坐标
			break;
		}
	}
	
	if (break_num_c&&break_num_d&&image[image_h - 1][4] && image[image_h - 1][image_w - 4])//两边生长方向都符合条件
	{
		//计算斜率
		start = break_num_c - 15;
		start = limit_a_b(start, 0, image_h);
		end = break_num_c - 5;
		calculate_s_i(start, end, l_border, &slope_l_rate, &intercept_l);
		//printf("slope_l_rate:%d\nintercept_l:%d\n", slope_l_rate, intercept_l);
		for (i = break_num_c - 5; i < image_h - 1; i++)
		{
			l_border[i] = slope_l_rate * (i)+intercept_l;//y = kx+b
			l_border[i] = limit_a_b(l_border[i], border_min, border_max);//限幅
		}

		//计算斜率
		start = break_num_d - 15;//起点
		start = limit_a_b(start, 0, image_h);//限幅
		end = break_num_d - 5;//终点
		calculate_s_i(start, end, r_border, &slope_l_rate, &intercept_l);
		//printf("slope_l_rate:%d\nintercept_l:%d\n", slope_l_rate, intercept_l);
		for (i = break_num_d - 5; i < image_h - 1; i++)
		{
			r_border[i] = slope_l_rate * (i)+intercept_l;
			r_border[i] = limit_a_b(r_border[i], border_min, border_max);
		}
	}

}
}
/*
函数名称：void image_process(void)
功能说明：最终处理函数
参数说明：无
函数返回：无
修改时间：2022年9月8日
备    注：
example： image_process();
 */
void image_process(void)
{
uint16 i;
uint8 hightest = 0;//定义一个最高行，tip：这里的最高指的是y值的最小
/*这是离线调试用的*/
Get_image(mt9v03x_image);
turn_to_bin();
/*提取赛道边界*/
image_filter(bin_image);//滤波
image_draw_rectan(bin_image);//预处理
//清零
data_stastics_l = 0;
data_stastics_r = 0;
if (get_start_point())//找到起点了，再执行八领域，没找到就一直找
{
	
	search_l_r((uint16)USE_num, bin_image, &data_stastics_l, &data_stastics_r, start_point_l[0], start_point_l[1], start_point_r[0], start_point_r[1], &hightest);
	// 从爬取的边界线内提取边线 ， 这个才是最终有用的边线
	get_left(data_stastics_l);
	get_right(data_stastics_r);
	//处理函数放这里
//	if(judge_border(data_stastics_l,data_stastics_r)){
//  //cross_fill(bin_image,l_border, r_border, data_stastics_l, data_stastics_r, dir_l, dir_r, points_l, points_r);
//}
}
//显示图像   
    if(mt9v03x_finish_flag)
{
	ips200_show_gray_image(0, 0, (const uint8*)bin_image, MT9V03X_W, MT9V03X_H, MT9V03X_W,MT9V03X_H ,image_thereshold);
	 //camera_send_image(DEBUG_UART_INDEX, (const uint8 *)mt9v03x_image, MT9V03X_IMAGE_SIZE);
	mt9v03x_finish_flag=0;
}
	//根据最终循环次数画出边界点
	for (i = 0; i < data_stastics_l; i++)
	{
		ips200_draw_point(points_l[i][0]+2, points_l[i][1], uesr_BLUE);//显示起点
	}
	for (i = 0; i < data_stastics_r; i++)
	{
		ips200_draw_point(points_r[i][0]-2, points_r[i][1], uesr_RED);//显示起点
	}
	for (i = hightest; i < image_h-1; i++)
	{
		center_line[i] = (l_border[i] + r_border[i]) >> 1;//求中线
		//求中线最好最后求，不管是补线还是做状态机，全程最好使用一组边线，中线最后求出，不能干扰最后的输出
		//当然也有多组边线的找法，但是个人感觉很繁琐，不建议
		ips200_draw_point(center_line[i], i, uesr_GREEN);//显示起点 显示中线	
		ips200_draw_point(l_border[i], i, uesr_GREEN);//显示起点 显示左边线
		ips200_draw_point(r_border[i], i, uesr_GREEN);//显示起点 显示右边线
	}
}
void image_display(){
	uint8 i;
	if(mt9v03x_finish_flag)
{
	ips200_show_gray_image(0, 0, (const uint8*)bin_image, MT9V03X_W, MT9V03X_H, MT9V03X_W,MT9V03X_H ,image_thereshold);
	 //camera_send_image(DEBUG_UART_INDEX, (const uint8 *)mt9v03x_image, MT9V03X_IMAGE_SIZE);
	mt9v03x_finish_flag=0;
}
	//根据最终循环次数画出边界点
	for (i = 0; i < data_stastics_l; i++)
	{
		ips200_draw_point(points_l[i][0]+2, points_l[i][1], uesr_BLUE);//显示起点
	}
	for (i = 0; i < data_stastics_r; i++)
	{
		ips200_draw_point(points_r[i][0]-2, points_r[i][1], uesr_RED);//显示起点
	}
	for (i = hightest; i < image_h-1; i++)
	{
		center_line[i] = (l_border[i] + r_border[i]) >> 1;//求中线
		//求中线最好最后求，不管是补线还是做状态机，全程最好使用一组边线，中线最后求出，不能干扰最后的输出
		//当然也有多组边线的找法，但是个人感觉很繁琐，不建议
		ips200_draw_point(center_line[i], i, uesr_GREEN);//显示起点 显示中线	
		ips200_draw_point(l_border[i], i, uesr_GREEN);//显示起点 显示左边线
		ips200_draw_point(r_border[i], i, uesr_GREEN);//显示起点 显示右边线
	}
}
void picture_process(){
		if(mt9v03x_finish_flag)
{ 
//	//camera_send_image(DEBUG_UART_INDEX, (const uint8 *)mt9v03x_image, MT9V03X_IMAGE_SIZE);
	ips200_show_gray_image(0,0, (const uint8 *)mt9v03x_image, MT9V03X_W, MT9V03X_H, MT9V03X_W, MT9V03X_H, 0);
	mt9v03x_finish_flag=0;
}
}
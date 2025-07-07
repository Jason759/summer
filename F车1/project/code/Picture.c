#include "zf_common_headfile.h"
#include "Picture.h"
#include "math.h"
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
#define use_num	 1	//1就是不压缩，2就是压缩一倍	
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

uint8 otsuThreshold(uint8 *image)   //注意计算阈值的一定要是原图像
{
#define GrayScale 256
    int Pixel_Max=0;
    int Pixel_Min=255;
    uint16 width = MT9V03X_W;
    uint16 height = MT9V03X_H;
    int pixelCount[GrayScale];
    float pixelPro[GrayScale];
    int i, j, pixelSum = width * height/4;
    uint8 threshold=0,last_threshold = 0;
    uint8* data = image;  //指向像素数据的指针
    for (i = 0; i < GrayScale; i++)
    {
        pixelCount[i] = 0;
        pixelPro[i] = 0;
    }

    uint32 gray_sum=0;
    //统计灰度级中每个像素在整幅图像中的个数
    for (i = 0; i < height; i+=2)
    {
        for (j = 0; j < width; j+=2)
        {
            pixelCount[(int)data[i * width + j]]++;  //将当前的点的像素值作为计数数组的下标
            gray_sum+=(int)data[i * width + j];       //灰度值总和
            if(data[i * width + j]>Pixel_Max)   Pixel_Max=data[i * width + j];
            if(data[i * width + j]<Pixel_Min)   Pixel_Min=data[i * width + j];
        }
    }

    //计算每个像素值的点在整幅图像中的比例

    for (i = Pixel_Min; i < Pixel_Max; i++)
    {
        pixelPro[i] = (float)pixelCount[i] / pixelSum;

    }

    //遍历灰度级[0,255]
    float w0, w1, u0tmp, u1tmp, u0, u1, u, deltaTmp, deltaMax = 0;

    w0 = w1 = u0tmp = u1tmp = u0 = u1 = u = deltaTmp = 0;
    for (j = Pixel_Min; j < Pixel_Max; j++)
    {

        w0 += pixelPro[j];  //背景部分每个灰度值的像素点所占比例之和   即背景部分的比例
        u0tmp += j * pixelPro[j];  //背景部分 每个灰度值的点的比例 *灰度值

        w1=1-w0;
        u1tmp=gray_sum/pixelSum-u0tmp;

        u0 = u0tmp / w0;              //背景平均灰度
        u1 = u1tmp / w1;              //前景平均灰度
        u = u0tmp + u1tmp;            //全局平均灰度
        deltaTmp = (float)(w0 *w1* (u0 - u1)* (u0 - u1)) ;
        if (deltaTmp > deltaMax)
        {
            deltaMax = deltaTmp;
            threshold = j;
        }
        if (deltaTmp < deltaMax)
        {
            break;
        }

    }

//    if(threshold>90 && threshold<130)
//        last_threshold = threshold;
//    else
//        threshold = last_threshold;

    return threshold;
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
函数名称：Get_angle(float Ax, float Ay, float Bx, float By, float Cx, float Cy)
功能说明：获取逆透视后的角度，即真实角度
参数说明：
函数返回：无
备    注：
example：  get_start_point(image_h-2)
 */
double hd[3][3] ={{0.222140,-0.155125,10.441998},{0.004990,0.011549,7.435414},{0.000135,-0.001700,0.237177}};//{{-0.128245,0.104439,0.877183},{-0.000917,0.002861,-2.085388},{-0.000022,0.001098,-0.068185}};
//x=[18,18,69,69]  % 依次为A、B、D、C在摄像头获取的照片中的的纵坐标
//y=[70,121,70,121] % 依次为A、B、D、C在摄像头获取的照片中的的横坐标
double Get_angle(double Ax, double Ay, double Bx, double By, double Cx, double Cy)
{

    double BA = 0.00;//向量BA的模
    double BC = 0.00;
    double SBA_BC = 0.00;//向量点乘的值
    double angle = 0.00;

    double AX=((hd[0][0] * Ax + hd[0][1] * Ay + hd[0][2])/(hd[2][0] * Ax + hd[2][1] * Ay + hd[2][2]));
    double AY=((hd[1][0] * Ax + hd[1][1] * Ay + hd[1][2])/(hd[2][0] * Ax + hd[2][1] * Ay + hd[2][2]));
    double BX=((hd[0][0] * Bx + hd[0][1] * By + hd[0][2])/(hd[2][0] * Bx + hd[2][1] * By + hd[2][2]));
    double BY=((hd[1][0] * Bx + hd[1][1] * By + hd[1][2])/(hd[2][0] * Bx + hd[2][1] * By + hd[2][2]));
    double CX=((hd[0][0] * Cx + hd[0][1] * Cy + hd[0][2])/(hd[2][0] * Cx + hd[2][1] * Cy + hd[2][2]));
    double CY=((hd[1][0] * Cx + hd[1][1] * Cy + hd[1][2])/(hd[2][0] * Cx + hd[2][1] * Cy + hd[2][2]));

    BA = sqrt((AX-BX)*(AX-BX)+(AY-BY)*(AY-BY));
    BC = sqrt((CX-BX)*(CX-BX)+(CY-BY)*(CY-BY));

    SBA_BC = (AX-BX)*(CX-BX)+(AY-BY)*(CY-BY);

    angle =  acos(SBA_BC*1.00/(BA*BC));
    return angle*57.3;
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
			//sprintf("找到右边起点image[%d][%d]\n",start_row, i);
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
							uint8 l_start_x, uint8 start_point_l[1], uint8 r_start_x, uint8 start_point_r[1],uint8*hightest)

功能说明：八邻域正式开始找右边点的函数，输入参数有点多，调用的时候不要漏了，这个是左右线一次性找完。
参数说明：
break_flag_r			：最多需要循环的次数
(*image)[image_w]		：需要进行找点的图像数组，必须是二值图,填入数组名称即可
					   特别注意，不要拿宏定义名字作为输入参数，否则数据可能无法传递过来
*l_stastic				：统计左边数据，用来输入初始数组成员的序号和取出循环次数
*r_stastic				：统计右边数据，用来输入初始数组成员的序号和取出循环次数
l_start_x				：左边起点横坐标
start_point_l[1]				：左边起点纵坐标
r_start_x				：右边起点横坐标
start_point_r[1]				：右边起点纵坐标
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
void search_l_r(uint16 break_flag, uint8(*image)[image_w], uint16 *l_stastic, uint16 *r_stastic, uint8 l_start_x, uint8 l_start_y, uint8 r_start_x, uint8 r_start_ys, uint8*hightest)
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
	center_point_l[1] = start_point_l[1];//y
	center_point_r[0] = r_start_x;//x
	center_point_r[1] = start_point_r[1];//y

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
			
			break;
		}
		if (my_abs(points_r[r_data_statics][0] - points_l[l_data_statics - 1][0]) < 2
			&& my_abs(points_r[r_data_statics][1] - points_l[l_data_statics - 1][1] < 2)
			)
		{
			*hightest = (points_r[r_data_statics][1] + points_l[l_data_statics - 1][1]) >> 1;//取出最高点
			
			break;
		}
		if ((points_r[r_data_statics][1] < points_l[l_data_statics - 1][1]))
		{
			continue;//如果左边比右边高了，左边等待右边
		}
		if (dir_l[l_data_statics - 1] == 7
			&& (points_r[r_data_statics][1] > points_l[l_data_statics - 1][1]))//左边比右边高且已经向下生长了
		{

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
函数名称：L_lose
功能说明：找丢线的点的个数
参数说明：
函数返回：无
备    注：
example：  
*/
uint16 L_lose(uint8 begin,uint8 end){
	uint8 i;
uint16	l_lose_num=0;
	for(i=begin;i<=end;i++){
	if(points_l[i][0]<4)
		l_lose_num++;
	}
	return l_lose_num;
}
uint16 R_lose(uint8 begin,uint8 end){
	uint8 i;
 uint16 r_lose_num;
	for(i=begin;i<=end;i++){
	if(points_r[i][0]>image_w-4)
		r_lose_num++;
	}
	return r_lose_num;
}
/*
函数名称：get_turning_point(void)；
功能说明：找拐点
参数说明：
函数返回：无
备    注：
example：  
*/
uint16 L_corner_row1 = 0;//左拐点所在行
uint16 L_corner_col1 = 0;//左拐点所在列
uint16 L_corner_row2 = 0;//左拐点所在行
uint16 L_corner_col2 = 0;//左拐点所在列
int L_corner_angle = 0;//左拐点角度
uint16 R_corner_row1 = 0;//右拐点所在行
uint16 R_corner_col1 = 0;//右拐点所在列
uint16 R_corner_row2 = 0;//右拐点所在行
uint16 R_corner_col2 = 0;//右拐点所在列
int R_corner_angle = 0;//右拐点角度

void get_turning_point(void)
{    
	uint8 flag1=0,flag2=0;
	uint16 i;
    L_corner_row1 = 0;
    L_corner_col1 = 0;
	  L_corner_row2 = 0;
    L_corner_col2 = 0;
    L_corner_angle = 0;  
	  R_corner_row1 = 0;
    R_corner_col1 = 0;
		R_corner_row2 = 0;
    R_corner_col2 = 0;
    R_corner_angle = 0;
//            for(int i = 0; i<data_stastics_l-5;i++)
//            {    
	for (i = 1; i < data_stastics_l; i++)
	{
		if (dir_l[i - 1] == 6 && dir_l[i] == 6 && dir_l[i + 1] == 6 && dir_l[i + 3] !=6 && dir_l[i + 5]!=6&&L_lose(0,data_stastics_l-1)>30)
		{
			L_corner_row2 = points_l[i][1];//传递y坐标
			break;
		}
	}
	for (i = 1; i < data_stastics_l; i++)
	{
		if (dir_l[i - 1] == 4 && dir_l[i] == 4 && dir_l[i + 1] == 2 && dir_l[i + 3] == 2 && dir_l[i + 5] == 2&&L_lose(0,data_stastics_l-1)>30)
		{
			L_corner_row1 = points_l[i][1];//传递y坐标
			break;
		}
	}
	for (i = 1; i < data_stastics_r; i++)
	{
		if (dir_r[i - 1] == 6 && dir_r[i] == 6 && dir_r[i + 1] == 6 && dir_r[i + 3]!=6 && dir_r[i + 5]!=6&&R_lose(0,data_stastics_r-1)>30)
		{
			R_corner_row2 = points_r[i][1];//传递y坐标
			break;
		}
	}
	for (i = 1; i < data_stastics_r; i++)
	{
		if (dir_r[i - 1] == 4 && dir_r[i] == 4 && dir_r[i + 1] == 2 && dir_r[i + 3] == 2 && dir_r[i + 5] ==2&&R_lose(0,data_stastics_r-1)>30)
		{
			R_corner_row1 = points_r[i][1];//传递y坐标
			break;
		}
}
	

//							 //int dot_product = (points_l[i][0] - points_l[i+2][0]) * (points_l[i+4][0] - points_l[i+2][0]) +
//                //             (points_l[i][1] - points_l[i+2][1]) * (points_l[i+4][1] - points_l[i+2][1]);
//                if(points_l[i+4][1]>5&&L_lose(0,data_stastics_l-1)>30)//&&dot_product>=0)
//                {
//                        L_corner_angle = Get_angle(points_l[i][0], points_l[i][1], points_l[i + 2][0], points_l[i + 2][1], points_l[i + 4][0], points_l[i + 4][1]); //求角度
//                        if(points_l[i+2][0]>points_l[i+4][0]&&points_l[i][1]<points_l[i+2][1]&&L_corner_angle>=28&&L_corner_angle<=110&&flag1==0)
//                        {
//                            L_corner_row1 = points_l[i+2][1];
//                            L_corner_col1 = points_l[i+2][0];
//													  flag1=1;
//													 
//                        }
//												else if(points_l[i+2][0]>points_l[i][0]&&points_l[i+2][1]<points_l[i+4][1]&&L_corner_angle>=28&&L_corner_angle<=110){
//                            L_corner_row2 = points_l[i+2][1];
//                            L_corner_col2 = points_l[i+2][0];
//													  break;
//												}
//                    
//                }
//            }

   
//       
//            for(int i = 0; i<data_stastics_r-5;i++)
//            {
//                if(points_r[i+4][1]>5&&R_lose(0,data_stastics_r-1)>30)
//                {        //int dot_product = (points_r[i][0] - points_r[i+2][0]) * (points_r[i+4][0] - points_r[i+2][0]) +
//                          //   (points_r[i][1] - points_r[i+2][1]) * (points_r[i+4][1] - points_r[i+2][1]);
//                     R_corner_angle = Get_angle(points_r[i][0], points_r[i][1], points_r[i + 2][0], points_r[i + 2][1], points_r[i + 4][0], points_r[i + 4][1]); //求角度
//                        if(points_r[i+4][0]>points_r[i+2][0]&&points_r[i+2][1]>points_r[i][1]&&R_corner_angle>=28&&R_corner_angle<=110&&flag2==0)
//                        {
//                            R_corner_row1 = points_r[i+2][1];
//                            R_corner_col1 = points_r[i+2][0];
//													  flag2=1;
//                        }
//												else if(points_r[i+2][0]<points_r[i][0]&&points_r[i+4][1]>points_r[i+2][1]&&R_corner_angle>=28&&R_corner_angle<=110){
//                            R_corner_row2 = points_r[i+2][1];
//                            R_corner_col2 = points_r[i+2][0];
//													  
//													  break;
//                    }
//                }
//            }
						ips200_show_int(190,40,L_corner_row1,3);
						ips200_show_int(190,60,L_corner_row2,3);
						ips200_show_int(190,80,R_corner_row1,3);
						ips200_show_int(190,100,R_corner_row2,3);
        }

//void get_turning_point(void)
//{
//    // 左侧拐点初始化
//    L_corner_row1 = L_corner_col1 = 0;
//    L_corner_row2 = L_corner_col2 = 0;
//    L_corner_angle = 0;
//    
//    // 右侧拐点初始化  
//    R_corner_row1 = R_corner_col1 = 0;
//    R_corner_row2 = R_corner_col2 = 0;
//    R_corner_angle = 0;

//    // 左侧处理
////    if(data_stastics_l > 9 && L_lose(0,data_stastics_l) <= image_h/2 && L_lose(0,data_stastics_l) > 20)
////    {
//        for(int i = 0; i < data_stastics_l-10; i++)
//        {
//            // 向量点积判断角度
//            int dot_product = (points_l[i][0] - points_l[i+4][0]) * (points_l[i+8][0] - points_l[i+4][0]) +
//                             (points_l[i][1] - points_l[i+4][1]) * (points_l[i+8][1] - points_l[i+4][1]);
//            
//            if(dot_product >= 0 && points_l[i+8][1] > 5) // 锐角/直角且有效高度
//            {
//                if(points_l[i+4][0] > points_l[i+8][0]) // 左拐点1
//                {
//                    L_corner_row1 = points_l[i+4][1];
//                    L_corner_col1 = points_l[i+4][0];
//                }
//                else if(points_l[i+4][0] < points_l[i][0]) // 左拐点2
//                {
//                    L_corner_row2 = points_l[i+4][1];
//                    L_corner_col2 = points_l[i+4][0];
//                }
//            }
//        }
//    }

    // 右侧处理（修正数组引用错误）
//    if(data_stastics_r > 9 && R_lose(0,data_stastics_r) <= image_h/2 && R_lose(0,data_stastics_r) > 20)
//    {
//        for(int i = 0; i < data_stastics_r-10; i++)
//        {
//            // 向量点积判断角度
//            int dot_product = (points_r[i][0] - points_r[i+4][0]) * (points_r[i+8][0] - points_r[i+4][0]) +
//                             (points_r[i][1] - points_r[i+4][1]) * (points_r[i+8][1] - points_r[i+4][1]);
//            
//            if(dot_product >= 0 && points_r[i+8][1] > 5) // 锐角/直角且有效高度
//            {
//                if(points_r[i+8][0] > points_r[i+4][0]) // 右拐点1
//                {
//                    R_corner_row1 = points_r[i+4][1];
//                    R_corner_col1 = points_r[i+4][0];
//                }
//                else if(points_r[i+4][0] > points_r[i][0]) // 修正这里原代码的错误
//                {
//                    R_corner_row2 = points_r[i+4][1];
//                    R_corner_col2 = points_r[i+4][0];
//                }
//            }
//        }
//    }
//}
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
		//sprintf("%d\n", j);
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
uint16 L_A,L_P,L_V;
uint8 L_duan_A(){
	uint16 i,count=0,flag=0;
	L_A=0;
 for(i=1;i<data_stastics_l-1;i++){
	 if(points_l[i-1][0]>points_l[i][0]&&points_l[i][0]<points_l[i+1][0]){
		 L_A=i;
		 flag=1;
		 break;
	 }
 }
 if(flag==1){
    return 1;
 }
 else return 0;
}
uint8 L_duan_P(){
	uint16 i,count=0,flag=0;
	L_P=0;
 for(i=1;i<data_stastics_l-1;i++){
	 if(l_border[i-1]<l_border[i]&&l_border[i]>l_border[i+1]){
		 L_P=i;
		 flag=1;
	 }
 }
 if(flag==1){
    return 1;
 }
 else return 0;
}
uint8 L_duan_V(){
	uint16 i,count=0,flag=0;
	L_A=0;
 for(i=1;i<data_stastics_l-1;i++){
	 if(points_l[i-1][0]<points_l[i][0]&&points_l[i][0]>points_l[i+1][0]){
		 L_V=i;
		 flag=1;
		 break;
	 }
 }
 if(flag==1){
    return 1;
 }
 else return 0;
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

void cross_fill()
{
	uint16 i;
	uint8 start, end;
	float slope_l_rate = 0, intercept_l = 0;
	get_turning_point();
	if (L_corner_row2&&bin_image[image_h - 2][4])//两边生长方向都符合条件
	{
		//计算斜率		
		start = L_corner_row2 - 15;
		start = limit_a_b(start, 0, image_h);
		end = L_corner_row2- 5;
		calculate_s_i(start, end, l_border, &slope_l_rate, &intercept_l);
		for (i = L_corner_row2 - 5; i < image_h - 1; i++)
		{
			l_border[i] = slope_l_rate * (i)+intercept_l;//y = kx+b
			l_border[i] = limit_a_b(l_border[i], border_min, border_max);//限幅
		}
	}
		if(R_corner_row2&& bin_image[image_h - 2][image_w - 4]){
		//计算斜率
		start = R_corner_row2 - 15;//起点
		start = limit_a_b(start, 0, image_h);//限幅
		end = R_corner_row2 - 5;//终点
		calculate_s_i(start, end, r_border, &slope_l_rate, &intercept_l);
	
		for (i = R_corner_row2 - 5; i < image_h - 1; i++)
		{
			r_border[i] = slope_l_rate * (i)+intercept_l;
			r_border[i] = limit_a_b(r_border[i], border_min, border_max);
		}
	}
	if(L_corner_row1&&L_corner_row2){
		slope_l_rate=(float)(L_corner_col1-L_corner_col2)/(L_corner_row1-L_corner_row2);
	 intercept_l=L_corner_col1-L_corner_row1*slope_l_rate;        ;
		for (i = L_corner_row2; i<L_corner_row1; i++)
		{
			l_border[i] = slope_l_rate * (i)+intercept_l;//y = kx+b
			l_border[i] = limit_a_b(l_border[i], border_min, border_max);//限幅
		}
	}
		if(R_corner_row1&&R_corner_row2){
	 slope_l_rate=(float)(R_corner_col1-R_corner_col2)/(R_corner_row1-R_corner_row2);
	 intercept_l=R_corner_col1-R_corner_row1*slope_l_rate;        ;
		for (i = R_corner_row2; i<R_corner_row1; i++)
		{
			r_border[i] = slope_l_rate * (i)+intercept_l;//y = kx+b
			r_border[i] = limit_a_b(r_border[i], border_min, border_max);//限幅
		}
		
	}
}
uint8 status=0;
void leftcircle()
{
	uint16 start,end,i;
	float slope_l_rate = 0, intercept_l = 0;
	if(status==0&&L_duan_A()&&L_duan_P()&&R_lose(0,data_stastics_r)<10&&L_lose(0,data_stastics_l)>20){
	 slope_l_rate=(points_l[L_A][0]-l_border[L_P])/(points_l[L_A][1]-L_P);
	 intercept_l=points_l[L_A][0]-points_l[L_A][1]*slope_l_rate;        ;
		for (i = points_l[L_P][1]; i < L_P; i++)
		{
			l_border[i] = slope_l_rate * (i)+intercept_l;//y = kx+b
			l_border[i] = limit_a_b(l_border[i], border_min, border_max);//限幅
		}
		status=1;
	}
	if(status==1&&L_duan_P()&&L_duan_A()==0&&R_lose(0,data_stastics_r)<5&&L_lose(0,data_stastics_l)>20){
	
	  slope_l_rate=(-l_border[L_P])/(image_h-1-L_P);
	  intercept_l=L_P-l_border[L_P]*slope_l_rate;  
		for (i =L_P; i < image_h-1; i++)
		{
			l_border[i] = slope_l_rate * (i)+intercept_l;//y = kx+b
			l_border[i] = limit_a_b(l_border[i], border_min, border_max);//限幅
		}
		status=2;
	}
	if(status==2&&L_duan_V()&&L_lose(0,data_stastics_l)<10&&R_lose(0,data_stastics_r)<10){   //拉线入环
	 slope_l_rate=(image_h-1-L_V)/(start_point_r[0]-l_border[L_V]);
	  intercept_l=l_border[L_V]-L_V*slope_l_rate;  
		for (i =L_V; i <image_h-1 ; i++)
		{
			l_border[i] = slope_l_rate * (i)+intercept_l;//y = kx+b
			l_border[i] = limit_a_b(l_border[i], border_min, border_max);//限幅
		}
		status=3;
	}
	if(status==3&&L_lose(0,data_stastics_l)<10&&R_lose(0,data_stastics_r)<10){
		status=4;
	}
	if(status==4&&L_duan_V()&&L_lose(0,data_stastics_l)>30&&R_lose(0,data_stastics_r)<10){   //拉线出环
		slope_l_rate=(-l_border[L_V])/(image_h-1-L_V);
	  intercept_l=l_border[L_V]-L_V*slope_l_rate;  
		for (i = L_V; i < image_h-1; i++)
		{
			l_border[i] = slope_l_rate * (i)+intercept_l;//y = kx+b
			l_border[i] = limit_a_b(l_border[i], border_min, border_max);//限幅
		}
		status=5;
	}
	if(status==5&&L_lose(0,data_stastics_l)<10&&R_lose(0,data_stastics_r)<10){   //回到普通赛道
		status=0;
	}
}
/*
函数名称：void image_process(void)
功能说明：最终处理函数
参数说明：无
函数返回：无
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
	  //leftcircle();
   //cross_fill();
	 //get_turning_point();
}
for (i = hightest; i < image_h-1; i++)
	{
		center_line[i] = (l_border[i] + r_border[i]) >> 1;//求中线
	}
}
//
// Created by RUPC on 2022/9/20.
//
#define RESULT_ROW 100//结果图行列
#define RESULT_COL 114
#define         USED_ROW                120  //用于透视图的行列
#define         USED_COL                188
#define PER_IMG     mt9v03x_image//SimBinImage:用于透视变换的图像
#define ImageUsed   *PerImg_ip//*PerImg_ip定义使用的图像，ImageUsed为用于巡线和识别的图像                                      // 无符号  8 bits
uint8 *PerImg_ip[RESULT_ROW][RESULT_COL];
void ImagePerspective_Init(void) {
 
    static uint8_t BlackColor = 0;
    double change_un_Mat[3][3] =         //114w*100h
            {{0.222140,-0.155125,10.441998},{0.004990,0.011549,7.435414},{0.000135,-0.001700,0.237177}};
    for (int i = 0; i < RESULT_COL ;i++) {
        for (int j = 0; j < RESULT_ROW ;j++) {
            int local_x = (int) ((change_un_Mat[0][0] * i
                    + change_un_Mat[0][1] * j + change_un_Mat[0][2])
                    / (change_un_Mat[2][0] * i + change_un_Mat[2][1] * j
                            + change_un_Mat[2][2]));
            int local_y = (int) ((change_un_Mat[1][0] * i
                    + change_un_Mat[1][1] * j + change_un_Mat[1][2])
                    / (change_un_Mat[2][0] * i + change_un_Mat[2][1] * j
                            + change_un_Mat[2][2]));
            if (local_x
                    >= 0&& local_y >= 0 && local_y < USED_ROW && local_x < USED_COL){
                PerImg_ip[j][i] = &PER_IMG[local_y][local_x];
            }
            else {
                PerImg_ip[j][i] = &BlackColor;          //&PER_IMG[0][0];
            }
 
        }
    }
 
}
void rep_show()        //显示逆透视后的图像
{
        if (mt9v03x_finish_flag == 1) {
            uint8_t show[RESULT_ROW][RESULT_COL];
                for(int i=0;i<RESULT_ROW;i++)
                {
                    for(int j=0;j<RESULT_COL;j++)
                    {
                        show[i][j]=ImageUsed[i][j];
                    }
                }
            ips200_show_gray_image(0,0,show[0],RESULT_COL,RESULT_ROW,RESULT_COL,RESULT_ROW,0);
            mt9v03x_finish_flag= 0;
        }
}
void image_show(){
	uint16 i;
	//显示图像   
    if(mt9v03x_finish_flag)
{
	ips200_show_gray_image(0, 0, (const uint8*)bin_image, MT9V03X_W, MT9V03X_H, MT9V03X_W,MT9V03X_H ,image_thereshold);
	 //camera_send_image(DEBUG_UART_INDEX, (const uint8 *)mt9v03x_image, MT9V03X_IMAGE_SIZE);

	//根据最终循环次数画出边界点
//	for (i = 0; i < data_stastics_l; i++)
//	{
//		ips200_draw_point(points_l[i][0]+2, points_l[i][1], uesr_BLUE);//显示起点
//	}
//	for (i = 0; i < data_stastics_r; i++)
//	{
//		ips200_draw_point(points_r[i][0]-2, points_r[i][1], uesr_RED);//显示起点
//	}
	for (i = hightest; i < image_h-1; i++)
	{
		ips200_draw_point(center_line[i], i, uesr_GREEN);//显示起点 显示中线	
		ips200_draw_point(l_border[i], i, uesr_GREEN);//显示起点 显示左边线
		ips200_draw_point(r_border[i], i, uesr_GREEN);//显示起点 显示右边线
	}
	mt9v03x_finish_flag=0;
}
}
void picture_process(){
		if(mt9v03x_finish_flag)
{ 
	camera_send_image(DEBUG_UART_INDEX, (const uint8 *)mt9v03x_image, MT9V03X_IMAGE_SIZE);
	//ips200_show_gray_image(0,0, (const uint8 *)mt9v03x_image, MT9V03X_W, MT9V03X_H, MT9V03X_W, MT9V03X_H, 0);
	mt9v03x_finish_flag=0;
}
}
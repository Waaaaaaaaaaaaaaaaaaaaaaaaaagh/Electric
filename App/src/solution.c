#include "solution.h"
#include "include.h"
#include "string.h"
#include <math.h>

//单次采样时间1.125us

max_Typedef ADC_Channel_max_time[6];

void for_max_time(__IO uint16_t (*p)[6])
{
	memset(ADC_Channel_max_time,0,sizeof(max_Typedef)*6);
    for(int i=0;i<6;i++)
        for(int j=0;j<1000;j++)
        {
            if(p[i][j]>ADC_Channel_max_time[j].max)
            {
                ADC_Channel_max_time[j].max=p[i][j];
                ADC_Channel_max_time[j].id=i;
            }
            
        }
}

const float rad2degree = 180/3.1415926535;


/**
 @brief             用于将位置信息转换为 Yaw 轴角度信息
 @param     x       坐标的 x 轴值
 @param     y       坐标的 y 轴值
 @return    float 
 */

float location_2_Yaw( float x, float y )/* 计算 Yaw 轴的角度 */
{
    float angle;
    if( x >=0 )
        angle = atanf( x/(y+17.2f) )*rad2degree;
    else
        angle = atanf( x/(y+17.2f) )*rad2degree;
    return angle;
}



/**
 @brief             用于将位置信息转换为 Pitch 轴角度信息
 @param     x       坐标的 x 轴值
 @param     y       坐标的 y 轴值
 @return float 
 */

float location_2_Pitch( float x, float y )/* 计算 Pitch 轴的角度 */
{
    float angle;

    angle = atanf( 152/sqrtf(x*x + y*y) )*rad2degree;

    return angle;
}



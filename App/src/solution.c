#include "solution.h"
#include "include.h"
#include "string.h"
#include <math.h>

//单次采样时间1.125us

max_Typedef ADC_Channel_max_time[NOFCHANEL];
Channel_Info_Typedef Channel_Info[NOFCHANEL]; 

static uint16_t normalization(uint16_t temp,int j)//归一差异化
{
    return (temp-Channel_Info[j].MIN)/(Channel_Info[j].MAX-Channel_Info[j].MIN);
}

void for_max_time(__IO uint16_t (*p)[NOFCHANEL])//数据的处理
{
	memset(ADC_Channel_max_time,0,sizeof(max_Typedef)*NOFCHANEL);
    for(int i=0;i<6;i++)//初始化数据
    {
        Channel_Info[i].number=0;
        Channel_Info[i].id=0;
        Channel_Info[i].last_value=Channel_Info[i].average;
    }
    for(int i=0;i<NOFCHANEL;i++)
        for(int j=0;j<LOFCHANEL;j++)
        {
            if(p[i][j]>ADC_Channel_max_time[j].max)
            {
                ADC_Channel_max_time[j].max=p[i][j];
                ADC_Channel_max_time[j].id=i;
            }//峰值判别
            if(Channel_Info[j].last_value>Channel_Info[j].LOW&&(p[i][j]-Channel_Info[j].average)<Channel_Info[j].LOW\
            ||Channel_Info[j].last_value<Channel_Info[j].HIGH&&(p[i][j]-Channel_Info[j].average)>Channel_Info[j].HIGH)
            {
                Channel_Info[j].number++;
                if(!Channel_Info[j].id&&Channel_Info[j].number>Channel_Info[j].NUMBERMUST)Channel_Info[j].id=i;
            }
            Channel_Info[j].last_value=p[i][j]-Channel_Info[j].average;
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
        angle = atanf( x/(y+19) )*rad2degree;
    else
        angle = atanf( x/(y+19) )*rad2degree;
    return angle;
}



/**
 @brief             用于将位置信息转换为 Pitch 轴角度信息
 @param     x       坐标的 x 轴值
 @param     y       坐标的 y 轴值
 @return    float 
 */


float location_2_Pitch( float x, float y )/* 计算 Pitch 轴的角度 */
{
    float angle;
    float u = x*x + (y+19)*(y+19);
    float trans = sqrtf(u*(u-11));
    float l2;
    l2 = ( (-11 + 2*u)/trans + 2.03333333f )/( 0.03333333f + 1/trans );

    angle = -acosf( (l2 - 61)/60 )*rad2degree;

    return angle;
}



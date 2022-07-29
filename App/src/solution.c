#include "stm32f10x.h"
#include <stdint.h>
#include "solution.h"
#include "include.h"
#include "string.h"
#include <math.h>

//单次采样时间1.125us

//max_Typedef ADC_Channel_max_time[NOFCHANEL];
//Channel_Info_Typedef Channel_Info[NOFCHANEL];
//__IO uint16_t q[NOFCHANEL][LOFCHANEL] = {0};
channel_info_stack_typedef pole_stack[6][20];
int stack_number[6];
uint8_t flag[6];//这个通道是否可用
float id[6];

void for_max_time(__IO uint16_t (*p)[NOFCHANEL])//数据的处理
{
    memset(stack_number,0,sizeof(int)*6);
    memset(flag,0,sizeof(uint8_t)*6);
    for(int i=2;i<LOFCHANEL-2;i++)
        for(int j=0;j<NOFCHANEL;j++)
        {
            if(p[i][j]>=p[i-1][j]&&p[i][j]>p[i-2][j]&&p[i][j]>=p[i+1][j]&&p[i][j]>p[i+2][j]&&!flag[j])
            {
                pole_stack[j][stack_number[j]].flag=1;
                pole_stack[j][stack_number[j]].id=i;
                stack_number[j]++;
            }
            if(p[i][j]<=p[i-1][j]&&p[i][j]<p[i-2][j]&&p[i][j]<=p[i+1][j]&&p[i][j]<p[i+2][j]&&!flag[j])
            {
                pole_stack[j][stack_number[j]].flag=0;
                pole_stack[j][stack_number[j]].id=i;
                stack_number[j]++;
            }
            if(stack_number[j]>=20)flag[j]=1;
        }
    for(int i=0;i<NOFCHANEL;i++)
    if(flag[i]==0&&stack_number[i]>=4)
    {
        uint8_t flag_temp=0;
        int number=0;
        if(abs(pole_stack[i][0].id-pole_stack[i][1].id-123)<10&&(pole_stack[i][0].flag+pole_stack[i][1].flag)==1)number++;
        for(int j=1;j<stack_number[i]-1;j++)
        {
            if((pole_stack[i][j].flag+pole_stack[i][j-1].flag)==1&&(pole_stack[i][j].flag+pole_stack[i][j+1].flag)==1&&\
            abs(pole_stack[i][j].id-pole_stack[i][j-1].id-123)<10&&abs(pole_stack[i][j].id-pole_stack[i][j+1].id-123)<10)
            number++;
            else number=0;
            if(number>=4)
            {
                float temp=pole_stack[i][j].id;
                if(pole_stack[i][j].flag==0)temp+=123;
                while(temp>0)temp-=123;
                id[i]=temp+123;
                flag_temp=1;
            }
        }
        if(abs(pole_stack[i][stack_number[i]-1].id-pole_stack[i][stack_number[i]-2].id-123)<10&&(pole_stack[i][stack_number[i]-2].flag+pole_stack[i][stack_number[i]-1].flag)==1)number++;
        else number++;
        if(number>=4)
        {
            float temp=pole_stack[i][stack_number[i]-1].id;
            if(pole_stack[i][stack_number[i]-1].flag==0)temp+=123;
            while(temp>0)temp-=123;
            id[i]=temp+123;
            flag_temp=1;
        }
        if(flag_temp==0)flag[i]=1;
    }
    else flag[i]=1;
    //id和flag是正确的
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

/**
 @brief             根据声音计算声音信标的位置，这是检测单峰值的版本
 @return    void    
 @param     McPhe   位置计算的相关数据，从右往左为0-5
 @param     Psi     最后得出的信标位置信息
 @param     value   拿到的麦克风声音信息,从右往左为0-5
 */

#ifdef Single                                               /* 测量单峰的形式 */

void search( Micophone_Typedef *McPhe, Position_Typedef *Psi , max_Typedef *value )
{
    

    uint8_t far_id = 0;
    uint8_t close_id = 0;
    uint8_t stader[6] = {0};

    for( uint8_t i=0;i<6;i++ )                              /* 判断是否是静默 */
    {
        if( value[i].max > Single_line )
            stader[i] = 1;
    }

    /* 找出最远的和最近的 */
    uint16_t max = 0;
    uint16_t min = 65535;
    for( uint8_t i=0;i<6;i++ )
    {
        if( stader[i] )
        {
            if(  value[i].id > max )
            {
                max = value[i].id;
                far_id = i;
            }
            if(  value[i].id < min )
            {
                min = value[i].id;
                close_id = i;
            }
                
        }
    }

    /* 计算距离 *//* id*6.75us 为实际的时间 *//* 距离计算出来的单位是 cm */
    McPhe[close_id].l = value[far_id].max*34*( (value[far_id].id - value[close_id].id)*6.75f + (far_id - close_id)*1.125f )/1000\
                        /( value[close_id].max - value[far_id].max );
    McPhe[far_id].l = McPhe[close_id].l + 34*( (value[far_id].id - value[close_id].id)*6.75f + (far_id - close_id)*1.125f )/1000;

    /* 计算位置 */
    if( close_id < far_id )                                     /* 说明声源在右边 */
    {
        float d = (McPhe[close_id].x - McPhe[far_id].x);        /* 两个麦克风之间的距离 */
        float Cos_a = ( d*d + McPhe[close_id].l*McPhe[close_id].l - McPhe[far_id].l*McPhe[far_id].l )\
                    /( 2*d*McPhe[close_id].l );                 /* 计算侧斜角的余弦值 */
        Psi->x = McPhe[close_id].x - McPhe[close_id].l*Cos_a;
        Psi->y = McPhe[close_id].l*sqrtf( 1 - Cos_a*Cos_a );
    }
    else                                                        /* 说明声源在左边 */
    {
        float d = (McPhe[far_id].x - McPhe[close_id].x);        /* 两个麦克风之间的距离 */
        float Cos_a = ( d*d + McPhe[close_id].l*McPhe[close_id].l - McPhe[far_id].l*McPhe[far_id].l )\
                    /( 2*d*McPhe[close_id].l );                 /* 计算侧斜角的余弦值 */
        Psi->x = McPhe[close_id].x + McPhe[close_id].l*Cos_a;
        Psi->y = McPhe[close_id].l*sqrtf( 1 - Cos_a*Cos_a );
    }

}

#endif


#ifdef Muti_wave                                            /* 测量多峰的形式 */

void search( Micophone_Typedef *McPhe, Position_Typedef *Psi, Channel_Info_Typedef *value )
{
    

    uint8_t far_id = 0;
    uint8_t middle_id = 0;
    uint8_t close_id = 0;
    uint8_t stader[6] = {0};

    for( uint8_t i=0;i<6;i++ )                              /* 判断是否是静默 */
    {
        if( value[i].id != -1 )
            stader[i] = 1;
    }

    /* 找出最远的，中间的和最近的 */
    uint16_t max = 0;
    uint16_t min = 65535;
    for( uint8_t i=0;i<6;i++ )
    {
        if( stader[i] )
        {
            if(  value[i].id > max )
            {
                max = value[i].id;
                middle_id = far_id;
                far_id = i;  
            }
            if(  value[i].id < min )
            {
                min = value[i].id;
                middle_id = close_id;   /* 两个里面都放一个是为了防止直接出现最值而不再迭代，同时这样也保证了它是中间的 */
                close_id = i;
            }
                
        }
    }

    float vt1 = 34*( (value[middle_id].id - value[close_id].id)*6.75f + (middle_id - close_id)*1.125f )/1000;
    float vt2 = 34*( (value[far_id].id - value[close_id].id)*6.75f + (far_id - close_id)*1.125f )/1000;
    if( close_id < far_id )                                         /* 说明声源在右边 */
    {
        float d1 = McPhe[middle_id].x - McPhe[far_id].x;
        float d2 = McPhe[close_id].x - McPhe[middle_id].x;
        float d12 = McPhe[close_id].x - McPhe[far_id].x;

        McPhe[close_id].l = ( (vt2*vt2 - d12*d12)/2*d12 - (vt1*vt1 - d2*d2)/2*d2 )/( vt1/d2 - vt2/d12 );
        McPhe[far_id].l = McPhe[close_id].l + vt2;

        float Cos_a = ( d12*d12 + McPhe[close_id].l*McPhe[close_id].l - McPhe[far_id].l*McPhe[far_id].l )\
                    /( 2*d12*McPhe[close_id].l );                   /* 计算侧斜角的余弦值 */
        Psi->x = McPhe[close_id].x - McPhe[close_id].l*Cos_a;
        Psi->y = McPhe[close_id].l*sqrtf( 1 - Cos_a*Cos_a );

    }
    else                                                            /* 说明声源在左边 */
    {
        float d1 = McPhe[far_id].x - McPhe[middle_id].x;
        float d2 = McPhe[middle_id].x - McPhe[close_id].x;
        float d12 = McPhe[far_id].x - McPhe[close_id].x;

        McPhe[close_id].l = ( (vt2*vt2 - d12*d12)/2*d12 - (vt1*vt1 - d2*d2)/2*d2 )/( vt1/d2 - vt2/d12 );
        McPhe[far_id].l = McPhe[close_id].l + vt2;

        float Cos_a = ( d12*d12 + McPhe[close_id].l*McPhe[close_id].l - McPhe[far_id].l*McPhe[far_id].l )\
                    /( 2*d12*McPhe[close_id].l );                   /* 计算侧斜角的余弦值 */
        Psi->x = McPhe[close_id].x + McPhe[close_id].l*Cos_a;
        Psi->y = McPhe[close_id].l*sqrtf( 1 - Cos_a*Cos_a );
    }
    

}

#endif



void New_search( Micophone_Typedef *McPhe, Position_Typedef *Psi, INfection_Typedef *Out )
{

}




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
float low_adc[LOFCHANEL][NOFCHANEL];
int stack_number[6];
uint8_t flag[6];//这个通道是否可用
float id[6];
const float rad2degree = 180/3.1415926535;

int myabs(int a)
{
	return a<0?-a:a;
}


Micophone_Typedef micophone[6] = 
{
	{
		.x = 24.4
	},
	{
		.x = 14.4
	},
	{
		.x = 4.4
	},
	{
		.x = -4.4
	},
	{
		.x = -14.4
	},
	{
		.x = -24.4
	}
};

Position_Typedef taget = {0};
KalmanFilter_Typedef kalman_yaw = 
{
    .p = 10,
    .r = 2.5,
    .q = 0.1
};
KalmanFilter_Typedef kalman_pitch = 
{
    .p = 10,
    .r = 2.5,
    .q = 0.1
};

static float KalmanFilter(float inData, KalmanFilter_Typedef *filter)
{
 
    filter->p += filter->q;
    filter->kg = filter->p / ( filter->p + filter->r ); //计算卡尔曼增益
    filter->out = filter->out + (filter->kg * (inData - filter->out)); //计算本次滤波估计值
    filter->p = (1 - filter->kg) * filter->p; //更新测量方差
    
    return filter->out;
}



void for_max_time(__IO uint16_t (*p)[NOFCHANEL])//数据的处理
{
    memset(stack_number,0,sizeof(int)*6);
    memset(flag,0,sizeof(uint8_t)*6);
		for(int i=0;i<6;i++)
		low_adc[0][i]=p[0][i];
		for(int i=1;i<LOFCHANEL;i++)
        for(int j=0;j<NOFCHANEL;j++)
		low_adc[i][j]=low_adc[i-1][j]*0.99+0.01*p[i][j];
    for(int i=2;i<LOFCHANEL-2;i++)
        for(int j=0;j<NOFCHANEL;j++)
        {
            if(low_adc[i][j]>low_adc[i-2][j]&&low_adc[i][j]>=low_adc[i-1][j]&&low_adc[i][j]>low_adc[i+2][j]&&low_adc[i][j]>=low_adc[i+1][j]&&!flag[j])
            {
                pole_stack[j][stack_number[j]].flag=1;
                pole_stack[j][stack_number[j]].id=i;
                stack_number[j]++;
            }
            if(low_adc[i][j]<low_adc[i-2][j]&&low_adc[i][j]<=low_adc[i-1][j]&&low_adc[i][j]<low_adc[i+2][j]&&low_adc[i][j]>=low_adc[i+1][j]&&!flag[j])
            {
                pole_stack[j][stack_number[j]].flag=0;
                pole_stack[j][stack_number[j]].id=i;
                stack_number[j]++;
            }
            if(stack_number[j]>=20)flag[j]=1;
        }
    for(int i=0;i<NOFCHANEL;i++)
    if(flag[i]==0&&stack_number[i]>=2)
    {
        for(int j=0;j<stack_number[i]-1;j++)
        {
					if(myabs(pole_stack[i][j+1].id-pole_stack[i][j].id-123)<20&&pole_stack[i][j].flag+pole_stack[i][j+1].flag==1)
					{
						int temp=pole_stack[i][j].id;
						if(pole_stack[i][j].flag==0)temp+=123;
						while(temp>0)temp-=246;
						temp+=246;
						flag[i]=0;
						id[i]=temp;
						break;
					}
					flag[i]=1;
        }
    }
    else flag[i]=1;

    int number=0;
    static float Last_data_p = 0;
    static float Last_data_y = 0;
    static float my_k = 0.25;
    static int Y = 0;
    static int angle = 0;
    static uint8_t my_times = 0;
    static float Yaw = 0;
    static float Pitch = 0;
    for(int i=0;i<NOFCHANEL;i++)
    number+=flag[i];
    if(number>3)return;
    New_search(micophone,&taget,id,flag);

    Yaw = location_2_Yaw( taget.x, taget.y );
    Pitch = location_2_Pitch( taget.x, taget.y );

    angle = (int)atanf( taget.x/taget.y )*rad2degree;
    Y = (int)(taget.y);
    print_plus("%d",Y);
    //  /* 对 X 轴进行低通滤波 */
    // if (Last_data_p == 0)
    // {
    //     Last_data_p = Pitch;
    // }
    // if( Pitch - Last_data_p > 5 )
    //     Pitch  = Last_data_p + 5;
    // Pitch = my_k * Pitch + ( 1 - my_k ) * Last_data_p;
    // Last_data_p = Pitch;

    // /* 对 Y 轴进行低通滤波 */
    // if (Last_data_y == 0)
    // {
    //     Last_data_y = Yaw;
    // }
    // if( Yaw - Last_data_y > 5 )
    //     Yaw  = Last_data_y + 5;

    // Yaw = my_k * Yaw + ( 1 - my_k ) * Last_data_y;
    // Last_data_y = Yaw;

    Yaw   = KalmanFilter(Yaw,&kalman_yaw);
    Pitch = KalmanFilter(Pitch,&kalman_pitch);

    Servo_drive( Yaw, SERVO1_Low);
    Servo_drive( Pitch, SERVO1_High);
    //id和flag是正确的
    my_times++;
    
    if( my_times > 5 )
    {
        OLED_show6x8number(1, 1, angle);
        OLED_show6x8number(1, 3, Y);
    }
    if( my_times > 10 )
        ray_on();
}


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

/**
 @brief             用于计算声音信标的位置
 @return    void
 @param     McPhe   麦克风的位置信息
 @param     Psi     声音信标的位置信息
 @param     id      波形的峰值位置信息
 @param     Flag    波形的有效位，决定是否有效，为 1 为无效
 */

void New_search( Micophone_Typedef *McPhe, Position_Typedef *Psi, float *id, uint8_t *Flag )
{
    
    uint8_t far_id = 0;
    uint8_t middle_id = 0;
    uint8_t close_id = 0;
    uint8_t k = 0;                                          /* 最初是有几个通道的值是可用的，后来是有几个通道的值需要减一个周期 */
    //const uint8_t length = 246;                           /* 一个周期的数组长度为 246 */
    uint8_t rank[6] = {0};

    for( uint8_t i=0;i<6;i++ )
    {
        if( !Flag[i] )
        {
            rank[k] = i;                                    /* 存的是通道的编号 */
            k++;                                            /* 计算得出有几个通道是能用的 */
        }  
    }

    for( uint8_t i=0;i<k;i++ )                              /* 将所有值都化作一个区间内 */
    {
        for( uint8_t j=i+1;j<k;j++ )
        {
            if( id[rank[i]] - id[rank[j]] > 123 )
                id[rank[i]] -= 246;
            if( id[rank[i]] - id[rank[j]] < -123 )
                id[rank[j]] -= 246;
        }
        
    }


    /* 找出最远的，中间的和最近的 */
    uint16_t max = 0;
    uint16_t min = 65535;
    
    for( uint8_t i=0;i<6;i++ )
    {
        if( !Flag[i] )
        {
            if(  id[i] > max )
            {
                max = id[i];
                middle_id = far_id;
                far_id = i;  
            }
            if(  id[i] < min )
            {
                min = id[i];
                middle_id = close_id;
                close_id = i;
            }
                
        }
    }

    float vt1 = 34*( (id[middle_id] - id[close_id])*6.75f + (middle_id - close_id)*1.125f )/1000;
    float vt2 = 34*( (id[far_id] - id[close_id])*6.75f + (far_id - close_id)*1.125f )/1000;
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



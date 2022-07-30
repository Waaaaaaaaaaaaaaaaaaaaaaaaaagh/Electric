#ifndef __SOLUTION__
#define __SOLUTION__

#include "stm32f10x.h"
#include "adc.h"


#define Single_line         2500        /* 单峰值信号的触发阈值 */
//#define Single
#ifndef Single
#define Muti_wave
#endif

typedef struct 
{
    int id;
    uint16_t flag;
}channel_info_stack_typedef;

typedef struct 
{
    uint16_t max;
    uint16_t id;
    float average;
}max_Typedef;

typedef struct
{
    const uint16_t LOW;
    const uint16_t HIGH;
    const uint16_t average;
    const uint16_t NUMBERMUST;
    const uint16_t MAX;
    const uint16_t MIN;
    int number;
    uint16_t last_value;
    int id;
}Channel_Info_Typedef;


typedef struct 
{
    const float x;          /* 在横坐标上的位置 */
    float l;                /* 与声音信标的距离 */
    float h;                /* 振幅 *//* 用于初步判断距离信息 */
    float t;                /* 在时间戳上的绝对时间 *//* 单位为us */
}Micophone_Typedef;


typedef struct 
{
    float x;                /* 横坐标上的位置 */
    float y;                /* 纵坐标上的位置 */
}Position_Typedef;


typedef struct 
{
    float real;             /* 实部 */
	float imag;             /* 虚部 */
}INfection_Typedef;


void for_max_time(__IO uint16_t (*p)[NOFCHANEL]);
float location_2_Yaw( float x, float y );
float location_2_Pitch( float x, float y );
#ifdef Single
void search( Micophone_Typedef *McPhe, Position_Typedef *Psi , max_Typedef *value );
#endif

#ifdef Muti_wave
void search( Micophone_Typedef *McPhe, Position_Typedef *Psi , Channel_Info_Typedef *value );
#endif
void New_search( Micophone_Typedef *McPhe, Position_Typedef *Psi, float *id, uint8_t *Flag );

#endif


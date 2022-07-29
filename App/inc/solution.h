#ifndef __SOLUTION__
#define __SOLUTION__

#include "stm32f10x.h"
#include "adc.h"


#define Single_line         2500        /* ����ֵ�źŵĴ�����ֵ */
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
    const float x;          /* �ں������ϵ�λ�� */
    float l;                /* �������ű�ľ��� */
    float h;                /* ��� *//* ���ڳ����жϾ�����Ϣ */
    float t;                /* ��ʱ����ϵľ���ʱ�� *//* ��λΪus */
}Micophone_Typedef;


typedef struct 
{
    float x;                /* �������ϵ�λ�� */
    float y;                /* �������ϵ�λ�� */
}Position_Typedef;


typedef struct 
{
    float real;             /* ʵ�� */
	float imag;             /* �鲿 */
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


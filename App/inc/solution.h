#ifndef __SOLUTION__
#define __SOLUTION__

#include "stm32f10x.h"

typedef struct 
{
    uint16_t max;
    uint16_t id;
}max_Typedef;

typedef struct
{
    const uint16_t LOW;
    
}Channnel_Info;

void for_max_time(__IO uint16_t (*p)[6]);
float location_2_Yaw( float x, float y );
float location_2_Pitch( float x, float y );

#endif


#ifndef __SOLUTION__
#define __SOLUTION__

#include "stm32f10x.h"
#include "adc.h"

typedef struct 
{
    uint16_t max;
    uint16_t id;
}max_Typedef;

typedef struct
{
    const uint16_t LOW;
    const uint16_t HIGH;
    const uint16_t average;
    const uint16_t NUMBERMUST;
    float StdDeviation; 
    int number;
    int last_value;
    uint16_t id;
}Channel_Info_Typedef;

void for_max_time(__IO uint16_t (*p)[NOFCHANEL]);

#endif


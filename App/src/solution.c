#include "solution.h"
#include "include.h"
#include "usartplus.h"
#include "string.h"

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


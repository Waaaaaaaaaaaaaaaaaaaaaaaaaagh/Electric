#include "ray.h"
#include "include.h"


void ray_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure; //GPIO初始化结构体

    RCC_APB2PeriphClockCmd(RCC_APBPeriph_Ray_GPIO, ENABLE);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;            //通用推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;           //设置输出频率
    GPIO_InitStructure.GPIO_Pin = RAY_PIN;                      //设置GPIO管脚
    GPIO_Init(RAY_PORT, &GPIO_InitStructure);

}


void ray_on(void)
{
    GPIO_SetBits(RAY_PORT,RAY_PIN);
}


void ray_off(void)
{
    GPIO_ResetBits(RAY_PORT,RAY_PIN);;
}





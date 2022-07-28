#include "servo.h"
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "delay.h"

/**
  * @brief	      高级定时器初始化
  * @param        无
  * @retval       无
  * @author       718 Lab
  */
void TIM_SERVO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure; //GPIO初始化结构体
    //输出比较通道1、2、3 GPIO初始化
    RCC_APB2PeriphClockCmd(RCC_APBPeriph_TIMx_GPIOx1, ENABLE); //使能外设总线时钟
    RCC_APB2PeriphClockCmd(RCC_APBPeriph_TIMx_GPIOx2, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APBPeriph_TIMx_GPIOx3, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); //shut JTAG

    GPIO_PinRemapConfig(GPIO_FullRemap_TIM2, ENABLE);
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;                                                                  //复用推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                                                                //设置输出频率
    GPIO_InitStructure.GPIO_Pin = SERVO1_Low | SERVO1_High | SERVO1_TIM_CH3_PIN | SERVO1_TIM_CH4_PIN; //设置GPIO管脚
    GPIO_Init(SERVO1_PORT, &GPIO_InitStructure);                                                                     //初始化GPIO

    GPIO_InitStructure.GPIO_Pin = SERVO2_TIM_CH2_PIN | SERVO2_TIM_CH3_PIN | SERVO2_TIM_CH4_PIN; //设置GPIO管脚
    GPIO_Init(SERVO2_PORT, &GPIO_InitStructure);                                                //初始化GPIO

    GPIO_InitStructure.GPIO_Pin = SERVO3_TIM_CH1_PIN; //设置GPIO管脚
    GPIO_Init(SERVO3_PORT, &GPIO_InitStructure);      //初始化GPIO

    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure; //定时器时基结构体
    TIM_OCInitTypeDef TIM_OCInitStructure;         //定时器输出比较结构体

    RCC_APB2PeriphClockCmd(RCC_APBPeriph_TIMx1, ENABLE); //使能定时器时钟
    RCC_APB1PeriphClockCmd(RCC_APBPeriph_TIMx2, ENABLE); //使能定时器时钟
    RCC_APB1PeriphClockCmd(RCC_APBPeriph_TIMx3, ENABLE);

    /*--------------------时基结构体初始化-------------------------*/
    // 配置周期，这里配置为0.02s
    TIM_TimeBaseStructure.TIM_Period = 10000 - 1;               //自动重装载寄存器的值，累计TIM_Period+1个频率后产生一个更新或者中断
    TIM_TimeBaseStructure.TIM_Prescaler = 71;                   //驱动CNT计数器的时钟 = Fck_int/(psc+1)
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;     //时钟分频因子 ，输出互补脉冲配置死区时间需要用到
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //计数器计数模式，设置为向上计数
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;            //设置重复计数器的值为0，不设置重复计数
    TIM_TimeBaseInit(SERVO1_TIM, &TIM_TimeBaseStructure);       //初始化定时器
    TIM_TimeBaseInit(SERVO2_TIM, &TIM_TimeBaseStructure);       //初始化定时器
    TIM_TimeBaseInit(SERVO3_TIM, &TIM_TimeBaseStructure);       //初始化定时器
    /*--------------------输出比较结构体初始化-------------------*/
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;             //选择定时器模式:TIM脉冲宽度调制模式1
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //使能输出比较
    TIM_OCInitStructure.TIM_Pulse = 0;                            //设置初始占空比
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;     //输出极性:TIM输出比较极性高(高电平有效)

    TIM_OC1Init(SERVO1_TIM, &TIM_OCInitStructure);          //初始化输出比较通道1
    TIM_OC1PreloadConfig(SERVO1_TIM, TIM_OCPreload_Enable); //使能通道1的CCR4上的预装载寄存器

    TIM_OC2Init(SERVO1_TIM, &TIM_OCInitStructure);          //初始化输出比较通道2
    TIM_OC2PreloadConfig(SERVO1_TIM, TIM_OCPreload_Enable); //使能通道2的CCR4上的预装载寄存器

    TIM_OC3Init(SERVO1_TIM, &TIM_OCInitStructure);          //初始化输出比较通道3
    TIM_OC3PreloadConfig(SERVO1_TIM, TIM_OCPreload_Enable); //使能通道3的CCR4上的预装载寄存器

    TIM_OC4Init(SERVO1_TIM, &TIM_OCInitStructure);          //初始化输出比较通道3
    TIM_OC4PreloadConfig(SERVO1_TIM, TIM_OCPreload_Enable); //使能通道3的CCR4上的预装载寄存器

    TIM_OC1Init(SERVO2_TIM, &TIM_OCInitStructure);          //初始化输出比较通道1
    TIM_OC1PreloadConfig(SERVO2_TIM, TIM_OCPreload_Enable); //使能通道1的CCR4上的预装载寄存器

    TIM_OC2Init(SERVO2_TIM, &TIM_OCInitStructure);          //初始化输出比较通道2
    TIM_OC2PreloadConfig(SERVO2_TIM, TIM_OCPreload_Enable); //使能通道2的CCR4上的预装载寄存器

    TIM_OC3Init(SERVO2_TIM, &TIM_OCInitStructure);          //初始化输出比较通道3
    TIM_OC3PreloadConfig(SERVO2_TIM, TIM_OCPreload_Enable); //使能通道3的CCR4上的预装载寄存器

    TIM_OC4Init(SERVO2_TIM, &TIM_OCInitStructure);          //初始化输出比较通道3
    TIM_OC4PreloadConfig(SERVO2_TIM, TIM_OCPreload_Enable); //使能通道3的CCR4上的预装载寄存器

    TIM_OC1Init(SERVO3_TIM, &TIM_OCInitStructure);          //初始化输出比较通道1
    TIM_OC1PreloadConfig(SERVO3_TIM, TIM_OCPreload_Enable); //使能通道1的CCR4上的预装载寄存器

    TIM_OC2Init(SERVO3_TIM, &TIM_OCInitStructure);          //初始化输出比较通道2
    TIM_OC2PreloadConfig(SERVO3_TIM, TIM_OCPreload_Enable); //使能通道2的CCR4上的预装载寄存器

    TIM_OC3Init(SERVO3_TIM, &TIM_OCInitStructure);          //初始化输出比较通道3
    TIM_OC3PreloadConfig(SERVO3_TIM, TIM_OCPreload_Enable); //使能通道3的CCR4上的预装载寄存器

    TIM_OC4Init(SERVO3_TIM, &TIM_OCInitStructure);          //初始化输出比较通道3
    TIM_OC4PreloadConfig(SERVO3_TIM, TIM_OCPreload_Enable); //使能通道3的CCR4上的预装载寄存器

    TIM_Cmd(SERVO1_TIM, ENABLE);            //使能TIM
    TIM_CtrlPWMOutputs(SERVO1_TIM, ENABLE); //主输出使能，当使用的是通用定时器，不需要主输出使能

    TIM_Cmd(SERVO2_TIM, ENABLE);            //使能TIM
    TIM_CtrlPWMOutputs(SERVO2_TIM, ENABLE); //主输出使能，当使用的是通用定时器，不需要主输出使能

    TIM_Cmd(SERVO3_TIM, ENABLE);            //使能TIM
    TIM_CtrlPWMOutputs(SERVO3_TIM, ENABLE); //主输出使能，当使用的是通用定时器，不需要主输出使能
}

/**
  * @brief	      舵机控制函数
  * @param        无
  * @retval       无
  * @note         使用时注意一下舵机中值的占空比、PWM频率、舵机打角范围对应的占空比
  * @author       718 Lab
  */
void Servo_Open(void)
{
    TIM_SetCompare1(SERVO1_TIM, 2000);
    TIM_SetCompare2(SERVO1_TIM, 2000);
    TIM_SetCompare3(SERVO1_TIM, 2000);
    TIM_SetCompare4(SERVO1_TIM, 2000);
    TIM_SetCompare1(SERVO2_TIM, 2000);
    TIM_SetCompare2(SERVO2_TIM, 2000);
    TIM_SetCompare3(SERVO2_TIM, 2000);
    TIM_SetCompare4(SERVO2_TIM, 2000);
    TIM_SetCompare1(SERVO3_TIM, 2000);
    TIM_SetCompare2(SERVO3_TIM, 2000);
    TIM_SetCompare3(SERVO3_TIM, 2000);
    TIM_SetCompare4(SERVO3_TIM, 2000);
}

/**
  * @brief	      舵机控制函数
  * @param        无
  * @retval       无
  * @note         使用时注意一下舵机中值的占空比、PWM频率、舵机打角范围对应的占空比
  * @author       718 Lab
  */
void Servo_Close(void)
{
    TIM_SetCompare1(SERVO1_TIM, 1000);
    TIM_SetCompare2(SERVO1_TIM, 1000);
    TIM_SetCompare3(SERVO1_TIM, 1000);
    TIM_SetCompare4(SERVO1_TIM, 1000);
    TIM_SetCompare1(SERVO2_TIM, 2000);
    TIM_SetCompare2(SERVO2_TIM, 2000);
    TIM_SetCompare3(SERVO2_TIM, 2000);
    TIM_SetCompare4(SERVO2_TIM, 2000);
    TIM_SetCompare1(SERVO3_TIM, 2000);
    TIM_SetCompare2(SERVO3_TIM, 2000);
    TIM_SetCompare3(SERVO3_TIM, 2000);
    TIM_SetCompare4(SERVO3_TIM, 2000);
}

/**
  * @brief	      舵机测试函数
  * @param        无
  * @retval       无
  * @note         使用时注意一下舵机中值的占空比、PWM频率、舵机打角范围对应的占空比
  * @author       718 Lab
  */
void Test_Servo(void)
{
  float k = -90;
  while(1)
  {
    Servo_drive(k,SERVO1_Low);/* 底层舵机 *//* X 轴 *//* Yaw 轴 */
		Servo_drive(k,SERVO1_High);/* 顶层舵机 *//* Y 轴 *//* Pitch 轴 */
    k += 1;
    if( k == 91 )
			break;
		Delay_MS(100);
  }
    
		
}


/**
 @brief           用于驱动舵机
 @return          void
 @param angle     期望转动的角度，单位为度
 @param Ch        对应的引脚
*/

#ifdef fix

void Servo_drive(float angle,uint16_t Ch)
{
  static float upper = 0;
  static float lower = 0;
  uint16_t compare;
  if( Ch == SERVO1_Low )/* 下层舵机 *//* 对应的关系为 7 us/° *//* 左为负，右为正 *//* 最左为2112，中值为1478， */
  {
    if( angle >= lower)
    {
      compare = 1500 - (int)( (angle + 35)*7 );
      TIM_SetCompare1(SERVO1_TIM, compare);
      compare = 1500 - (int)( angle*7 );
      Delay_MS(250);
      TIM_SetCompare1(SERVO1_TIM, compare);
    }
    else
    {
      compare = 1500 - (int)( (angle - 20)*7 );
      TIM_SetCompare1(SERVO1_TIM, compare);
      compare = 1500 - (int)( angle*7 );
      Delay_MS(250);
      TIM_SetCompare1(SERVO1_TIM, compare);
    }

    lower = angle;
  }
  if( Ch == SERVO1_High )/* 下层舵机 *//* 对应的关系为 7 us/° *//* 下为负，上为正 */
  {
    if( angle >= upper )
    {
      compare = 1500 + (int)( (angle + 35)*95/9 );
      TIM_SetCompare2(SERVO1_TIM, compare);
      compare = 1500 + (int)( angle*95/9 );
      Delay_MS(250);
      TIM_SetCompare2(SERVO1_TIM, compare);
    }
    else
    {
      compare = 1500 + (int)( (angle - 20)*95/9 );
      TIM_SetCompare2(SERVO1_TIM, compare);
      compare = 1500 + (int)( angle*95/9 );
      Delay_MS(250);
      TIM_SetCompare2(SERVO1_TIM, compare);
    }

    upper = angle;
  }
}
#endif

#ifdef common

void Servo_drive(float angle,uint16_t Ch)
{
  uint16_t compare;
  if( Ch == SERVO1_Low )  /* 下层舵机 *//* 左为负，右为正 *//* 最左为2112，左45为1804，中值为1482，右45为1144，最右为820 */
  {
    if( angle < -45 )
    {
      compare = 1804 - (int)( (angle + 45)*308/45 );
      TIM_SetCompare1(SERVO1_TIM, compare);
    }
    else if( (angle >= -45)&&(angle < 0) )
    {
      compare = 1482 - (int)( angle *322/45 );
      TIM_SetCompare1(SERVO1_TIM, compare);
    }
    else if( (angle >= 0)&&(angle < 45) )
    {
      compare = 1482 - (int)( angle *338/45 );
      TIM_SetCompare1(SERVO1_TIM, compare);
    }
    else if( angle >= 45 )
    {
      compare = 1144 - (int)( (angle - 45) *324/45 );
      TIM_SetCompare1(SERVO1_TIM, compare);
    }
      
  }
  if( Ch == SERVO1_High )/* 下层舵机 *//* 下为负，上为正 *//* 下90为550，水平为1526，上90为2450 */
  {
    // if( angle < 0 )
    // {
    //   compare = 1526 + (int)( angle*976/90 );
    //   TIM_SetCompare2(SERVO1_TIM, compare);
    // }
    // else
    // {
    //   compare = 1526 + (int)( angle*924/90 );
    //   TIM_SetCompare2(SERVO1_TIM, compare);
    // }
     compare = 1500 + (int)( angle*95.5/9 );
     TIM_SetCompare2(SERVO1_TIM, compare);
  }
}

#endif

/**
 @brief           复位舵机
 @return          void
 */
void Servo_reset(void)
{
  TIM_SetCompare1(SERVO1_TIM, 1500);
  TIM_SetCompare2(SERVO1_TIM, 1500);
}

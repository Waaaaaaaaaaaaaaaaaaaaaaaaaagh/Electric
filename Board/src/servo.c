#include "servo.h"
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "delay.h"

/**
  * @brief	      �߼���ʱ����ʼ��
  * @param        ��
  * @retval       ��
  * @author       718 Lab
  */
void TIM_SERVO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure; //GPIO��ʼ���ṹ��
    //����Ƚ�ͨ��1��2��3 GPIO��ʼ��
    RCC_APB2PeriphClockCmd(RCC_APBPeriph_TIMx_GPIOx1, ENABLE); //ʹ����������ʱ��
    RCC_APB2PeriphClockCmd(RCC_APBPeriph_TIMx_GPIOx2, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APBPeriph_TIMx_GPIOx3, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); //shut JTAG

    GPIO_PinRemapConfig(GPIO_FullRemap_TIM2, ENABLE);
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;                                                                  //�����������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                                                                //�������Ƶ��
    GPIO_InitStructure.GPIO_Pin = SERVO1_Low | SERVO1_High | SERVO1_TIM_CH3_PIN | SERVO1_TIM_CH4_PIN; //����GPIO�ܽ�
    GPIO_Init(SERVO1_PORT, &GPIO_InitStructure);                                                                     //��ʼ��GPIO

    GPIO_InitStructure.GPIO_Pin = SERVO2_TIM_CH2_PIN | SERVO2_TIM_CH3_PIN | SERVO2_TIM_CH4_PIN; //����GPIO�ܽ�
    GPIO_Init(SERVO2_PORT, &GPIO_InitStructure);                                                //��ʼ��GPIO

    GPIO_InitStructure.GPIO_Pin = SERVO3_TIM_CH1_PIN; //����GPIO�ܽ�
    GPIO_Init(SERVO3_PORT, &GPIO_InitStructure);      //��ʼ��GPIO

    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure; //��ʱ��ʱ���ṹ��
    TIM_OCInitTypeDef TIM_OCInitStructure;         //��ʱ������ȽϽṹ��

    RCC_APB2PeriphClockCmd(RCC_APBPeriph_TIMx1, ENABLE); //ʹ�ܶ�ʱ��ʱ��
    RCC_APB1PeriphClockCmd(RCC_APBPeriph_TIMx2, ENABLE); //ʹ�ܶ�ʱ��ʱ��
    RCC_APB1PeriphClockCmd(RCC_APBPeriph_TIMx3, ENABLE);

    /*--------------------ʱ���ṹ���ʼ��-------------------------*/
    // �������ڣ���������Ϊ0.02s
    TIM_TimeBaseStructure.TIM_Period = 10000 - 1;               //�Զ���װ�ؼĴ�����ֵ���ۼ�TIM_Period+1��Ƶ�ʺ����һ�����»����ж�
    TIM_TimeBaseStructure.TIM_Prescaler = 71;                   //����CNT��������ʱ�� = Fck_int/(psc+1)
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;     //ʱ�ӷ�Ƶ���� ���������������������ʱ����Ҫ�õ�
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //����������ģʽ������Ϊ���ϼ���
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;            //�����ظ���������ֵΪ0���������ظ�����
    TIM_TimeBaseInit(SERVO1_TIM, &TIM_TimeBaseStructure);       //��ʼ����ʱ��
    TIM_TimeBaseInit(SERVO2_TIM, &TIM_TimeBaseStructure);       //��ʼ����ʱ��
    TIM_TimeBaseInit(SERVO3_TIM, &TIM_TimeBaseStructure);       //��ʼ����ʱ��
    /*--------------------����ȽϽṹ���ʼ��-------------------*/
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;             //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ1
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //ʹ������Ƚ�
    TIM_OCInitStructure.TIM_Pulse = 0;                            //���ó�ʼռ�ձ�
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;     //�������:TIM����Ƚϼ��Ը�(�ߵ�ƽ��Ч)

    TIM_OC1Init(SERVO1_TIM, &TIM_OCInitStructure);          //��ʼ������Ƚ�ͨ��1
    TIM_OC1PreloadConfig(SERVO1_TIM, TIM_OCPreload_Enable); //ʹ��ͨ��1��CCR4�ϵ�Ԥװ�ؼĴ���

    TIM_OC2Init(SERVO1_TIM, &TIM_OCInitStructure);          //��ʼ������Ƚ�ͨ��2
    TIM_OC2PreloadConfig(SERVO1_TIM, TIM_OCPreload_Enable); //ʹ��ͨ��2��CCR4�ϵ�Ԥװ�ؼĴ���

    TIM_OC3Init(SERVO1_TIM, &TIM_OCInitStructure);          //��ʼ������Ƚ�ͨ��3
    TIM_OC3PreloadConfig(SERVO1_TIM, TIM_OCPreload_Enable); //ʹ��ͨ��3��CCR4�ϵ�Ԥװ�ؼĴ���

    TIM_OC4Init(SERVO1_TIM, &TIM_OCInitStructure);          //��ʼ������Ƚ�ͨ��3
    TIM_OC4PreloadConfig(SERVO1_TIM, TIM_OCPreload_Enable); //ʹ��ͨ��3��CCR4�ϵ�Ԥװ�ؼĴ���

    TIM_OC1Init(SERVO2_TIM, &TIM_OCInitStructure);          //��ʼ������Ƚ�ͨ��1
    TIM_OC1PreloadConfig(SERVO2_TIM, TIM_OCPreload_Enable); //ʹ��ͨ��1��CCR4�ϵ�Ԥװ�ؼĴ���

    TIM_OC2Init(SERVO2_TIM, &TIM_OCInitStructure);          //��ʼ������Ƚ�ͨ��2
    TIM_OC2PreloadConfig(SERVO2_TIM, TIM_OCPreload_Enable); //ʹ��ͨ��2��CCR4�ϵ�Ԥװ�ؼĴ���

    TIM_OC3Init(SERVO2_TIM, &TIM_OCInitStructure);          //��ʼ������Ƚ�ͨ��3
    TIM_OC3PreloadConfig(SERVO2_TIM, TIM_OCPreload_Enable); //ʹ��ͨ��3��CCR4�ϵ�Ԥװ�ؼĴ���

    TIM_OC4Init(SERVO2_TIM, &TIM_OCInitStructure);          //��ʼ������Ƚ�ͨ��3
    TIM_OC4PreloadConfig(SERVO2_TIM, TIM_OCPreload_Enable); //ʹ��ͨ��3��CCR4�ϵ�Ԥװ�ؼĴ���

    TIM_OC1Init(SERVO3_TIM, &TIM_OCInitStructure);          //��ʼ������Ƚ�ͨ��1
    TIM_OC1PreloadConfig(SERVO3_TIM, TIM_OCPreload_Enable); //ʹ��ͨ��1��CCR4�ϵ�Ԥװ�ؼĴ���

    TIM_OC2Init(SERVO3_TIM, &TIM_OCInitStructure);          //��ʼ������Ƚ�ͨ��2
    TIM_OC2PreloadConfig(SERVO3_TIM, TIM_OCPreload_Enable); //ʹ��ͨ��2��CCR4�ϵ�Ԥװ�ؼĴ���

    TIM_OC3Init(SERVO3_TIM, &TIM_OCInitStructure);          //��ʼ������Ƚ�ͨ��3
    TIM_OC3PreloadConfig(SERVO3_TIM, TIM_OCPreload_Enable); //ʹ��ͨ��3��CCR4�ϵ�Ԥװ�ؼĴ���

    TIM_OC4Init(SERVO3_TIM, &TIM_OCInitStructure);          //��ʼ������Ƚ�ͨ��3
    TIM_OC4PreloadConfig(SERVO3_TIM, TIM_OCPreload_Enable); //ʹ��ͨ��3��CCR4�ϵ�Ԥװ�ؼĴ���

    TIM_Cmd(SERVO1_TIM, ENABLE);            //ʹ��TIM
    TIM_CtrlPWMOutputs(SERVO1_TIM, ENABLE); //�����ʹ�ܣ���ʹ�õ���ͨ�ö�ʱ��������Ҫ�����ʹ��

    TIM_Cmd(SERVO2_TIM, ENABLE);            //ʹ��TIM
    TIM_CtrlPWMOutputs(SERVO2_TIM, ENABLE); //�����ʹ�ܣ���ʹ�õ���ͨ�ö�ʱ��������Ҫ�����ʹ��

    TIM_Cmd(SERVO3_TIM, ENABLE);            //ʹ��TIM
    TIM_CtrlPWMOutputs(SERVO3_TIM, ENABLE); //�����ʹ�ܣ���ʹ�õ���ͨ�ö�ʱ��������Ҫ�����ʹ��
}

/**
  * @brief	      ������ƺ���
  * @param        ��
  * @retval       ��
  * @note         ʹ��ʱע��һ�¶����ֵ��ռ�ձȡ�PWMƵ�ʡ������Ƿ�Χ��Ӧ��ռ�ձ�
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
  * @brief	      ������ƺ���
  * @param        ��
  * @retval       ��
  * @note         ʹ��ʱע��һ�¶����ֵ��ռ�ձȡ�PWMƵ�ʡ������Ƿ�Χ��Ӧ��ռ�ձ�
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
  * @brief	      ������Ժ���
  * @param        ��
  * @retval       ��
  * @note         ʹ��ʱע��һ�¶����ֵ��ռ�ձȡ�PWMƵ�ʡ������Ƿ�Χ��Ӧ��ռ�ձ�
  * @author       718 Lab
  */
void Test_Servo(void)
{
  float k = -90;
  while(1)
  {
    Servo_drive(k,SERVO1_Low);/* �ײ��� *//* X �� *//* Yaw �� */
		Servo_drive(k,SERVO1_High);/* ������ *//* Y �� *//* Pitch �� */
    k += 1;
    if( k == 91 )
			break;
		Delay_MS(100);
  }
    
		
}


/**
 @brief           �����������
 @return          void
 @param angle     ����ת���ĽǶȣ���λΪ��
 @param Ch        ��Ӧ������
*/

#ifdef fix

void Servo_drive(float angle,uint16_t Ch)
{
  static float upper = 0;
  static float lower = 0;
  uint16_t compare;
  if( Ch == SERVO1_Low )/* �²��� *//* ��Ӧ�Ĺ�ϵΪ 7 us/�� *//* ��Ϊ������Ϊ�� *//* ����Ϊ2112����ֵΪ1478�� */
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
  if( Ch == SERVO1_High )/* �²��� *//* ��Ӧ�Ĺ�ϵΪ 7 us/�� *//* ��Ϊ������Ϊ�� */
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
  if( Ch == SERVO1_Low )  /* �²��� *//* ��Ϊ������Ϊ�� *//* ����Ϊ2112����45Ϊ1804����ֵΪ1482����45Ϊ1144������Ϊ820 */
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
  if( Ch == SERVO1_High )/* �²��� *//* ��Ϊ������Ϊ�� *//* ��90Ϊ550��ˮƽΪ1526����90Ϊ2450 */
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
 @brief           ��λ���
 @return          void
 */
void Servo_reset(void)
{
  TIM_SetCompare1(SERVO1_TIM, 1500);
  TIM_SetCompare2(SERVO1_TIM, 1500);
}

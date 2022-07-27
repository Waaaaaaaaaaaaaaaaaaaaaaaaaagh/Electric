#ifndef _RAY_H_
#define _RAY_H_


#define	RCC_APBPeriph_Ray_GPIO			    RCC_APB2Periph_GPIOC
#define	RAY_PORT							GPIOC
#define RAY_PIN					            GPIO_Pin_4



void ray_init(void);
void ray_on(void);
void ray_off(void);


#endif /* _RAY_H_ */

#ifndef __FFT_H
#define __FFT_H


#include "include.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdint.h>
#include "stm32f10x.h"


#define N 1024 
typedef struct
{
	float real;
	float imag;
}complex;


void FFT_change( __IO uint16_t q[] );
void ifft(int n, int k,complex In[],INfection_Typedef Out[]);


#endif /* __FFT_H */

#include "fft.h"


complex x[N], *W;
int size = 1024;
const float PI=3.1415926535f;
static void change()
{
	complex temp;
	unsigned short i=0,j=0,k=0;
	float t;
	for(i=0;i<size;i++)
	{
		k=i;
		j=0;
		t=(log(size)/log(2));
		while( (t--)>0 )
		{
			j=j<<1;
			j|=(k & 1);
			k=k>>1;
		}
		if(j>i)
		{
			temp=x[i];
			x[i]=x[j];
			x[j]=temp;
		}
	}
}
static void transform()
{
	int i;
	W=(complex *)malloc(sizeof(complex) * size);
	for(i=0;i<size;i++)
	{
		W[i].real=cos(2*PI/size*i);
		W[i].imag=-1*sin(2*PI/size*i);
	}
}
static void add(complex a,complex b,complex *c)
{
	c->real=a.real+b.real;
	c->imag=a.imag+b.imag;
}
static void sub(complex a,complex b,complex *c)
{
	c->real=a.real-b.real;
	c->imag=a.imag-b.imag;
}
static void mul(complex a,complex b,complex *c)
{
	c->real=a.real*b.real - a.imag*b.imag;
	c->imag=a.real*b.imag + a.imag*b.real;
}
static void fft()
{
	int i=0,j=0,k=0,m=0;
	complex q,y,z;
	change();
	for(i=0;i<log(size)/log(2) ;i++)
	{
		m=1<<i;
		for(j=0;j<size;j+=2*m)
		{
			for(k=0;k<m;k++)
			{
				mul(x[k+j+m],W[size*k/2/m],&q);
				add(x[j+k],q,&y);
				sub(x[j+k],q,&z);
				x[j+k]=y;
				x[j+k+m]=z;
			}
		}
	}
}


void FFT_change( __IO uint16_t q[] )
{

    for (uint16_t i = 0; i < 1024; i++)
    {
        x[i].real = q[i];
        x[i].imag = 0;
    }
    transform();
	fft();

}



// n 为信号长度， n=2^k
// pr 为输入信号实部，pi 为输入信号虚部
// fr 为输出信号实部，fi 为输出信号虚部
// 函数执行后，pr 变为输出信号的模
void ifft(int n, int k,complex In[],INfection_Typedef Out[])
{
	int it, m, is, i, j, nv, l0;
	float p, q, s, vr, vi, poddr, poddi;

	for (i = 0; i < n; i++)	// 取共轭
	{
		In[i].imag = -In[i].imag;
	}

	for (it = 0; it <= n - 1; it++)  //将pr[0]和pi[0]循环赋值给fr[]和fi[]
	{
		m = it;
		is = 0;
		for (i = 0; i <= k - 1; i++)
		{
			j = m / 2;
			is = 2 * is + (m - 2 * j);
			m = j;
		}
		Out[it].real = In[is].real;
		Out[it].imag = In[is].imag;
	}
	In[0].real = 1.0;
	In[0].imag = 0.0;
	p = 6.283185306 / (1.0 * n);
	In[1].real = cos(p); //将w=e^-j2pi/n用欧拉公式表示
	In[1].imag = -sin(p);

	for (i = 2; i <= n - 1; i++)  //计算pr[]
	{
		p = In[i - 1].real * In[1].real;
		q = In[i - 1].imag * In[1].imag;
		s = (In[i - 1].real + In[i - 1].imag) * (In[1].real + In[1].imag);
		In[i].real = p - q; In[i].imag = s - p - q;
	}
	for (it = 0; it <= n - 2; it = it + 2)
	{
		vr = Out[it].real;
		vi = Out[it].imag;
		Out[it].real = vr + Out[it + 1].real;
		Out[it].imag = vi + Out[it + 1].imag;
		Out[it + 1].real = vr - Out[it + 1].real;
		Out[it + 1].imag = vi - Out[it + 1].imag;
	}
	m = n / 2;
	nv = 2;
	for (l0 = k - 2; l0 >= 0; l0--) //蝴蝶操作
	{
		m = m / 2;
		nv = 2 * nv;
		for (it = 0; it <= (m - 1) * nv; it = it + nv)
			for (j = 0; j <= (nv / 2) - 1; j++)
			{
				p = In[m * j].real * Out[it + j + nv / 2].real;
				q = In[m * j].imag * Out[it + j + nv / 2].imag;
				s = In[m * j].real + In[m * j].imag;
				s = s * (Out[it + j + nv / 2].real + Out[it + j + nv / 2].imag);
				poddr = p - q;
				poddi = s - p - q;
				Out[it + j + nv / 2].real = Out[it + j].real - poddr;
				Out[it + j + nv / 2].imag = Out[it + j].imag - poddi;
				Out[it + j].real = Out[it + j].real + poddr;
				Out[it + j].imag = Out[it + j].imag + poddi;
			}
	}

	for (i = 0; i < n; i++)	// 取共轭
	{
		Out[i].real = Out[i].real / n;
		Out[i].imag = -Out[i].imag / n;
	}

	// for (i = 0; i <= n - 1; i++)
	// {
	// 	pr[i] = sqrt(fr[i] * fr[i] + fi[i] * fi[i]);  //幅值计算
	// }

}






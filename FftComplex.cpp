// FftComplex.cpp: implementation of the FftComplex class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "FftComplex_1D.h"
#include "FftComplex.h"


//////////////////////////////////////////////////////////////////////
//#include <stdio.h>
#include <math.h>
//#include <stdlib.h>
//////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

FftComplex::FftComplex()
{
	m_nWindowMode = WINFUN_RECT;							//加窗函数
	m_nSampleFreq = 100000000;							//输入信号的采样率
	m_nPower = 9	;								//2的n次幂
	m_nOperSize = 512 ;

	m_nOutputMode = YAXIS_UNIT_V;
	m_nImpedance = 1000;


	InitialWArray();
	m_OperSignal = (complex *)calloc(m_nOperSize,sizeof(complex));
}

FftComplex::~FftComplex()
{
	//释放内存
	free(m_OperSignal);
	free(m_WArray);
}

int FftComplex::FftRun(	double* pSignalArray, int nSignalLen, double* pXAxisArray, int* pXAxisLen, double* pYAxisArray, int* pYAxisLen	)
{
	/*比较nSignalLen和m_nOperSize长度，同时进行补0或者抽取*/
	int nCount = 0;
	int nIndex = 0;

	if ( nSignalLen <= m_nOperSize)
	{
		//进行补0
		for( nCount=0 ; nCount<m_nOperSize ; nCount++)
		{
			if (nCount < nSignalLen)
			{
				m_OperSignal[nCount].real = pSignalArray[nCount];
				m_OperSignal[nCount].img = 0;
			}
			else{
				m_OperSignal[nCount].real = 0;
				m_OperSignal[nCount].img = 0;
			}
		}
	}
	else{
		//nSignalLen个数据抽取出m_nOperSize个来
		for( nCount=0 ; nCount<m_nOperSize ; nCount++)
		{
			nIndex = (int)(nCount*nSignalLen/m_nOperSize);
		
			m_OperSignal[nCount].real = pSignalArray[nIndex];
			m_OperSignal[nCount].img = 0;
		
		}
	}

	/*进行加窗*/
	AddWinFun();

	/*初始化变化核*/
//	InitialWArray();

	/*对原始信号进行变址*/
	ChangeAddress();

	/*进行基-2 FFT运算*/
	int   i=0,j=0,k=0,l=0;   
	complex up,down,product;   

	int nButterflyOp = (int)(log(m_nOperSize)/log(2));
  
	for( i=0 ; i< nButterflyOp ; i++) /*一级蝶形运算*/ 
	{       
		l=1<<i;   
		for(j=0 ; j<m_nOperSize ; j+=2*l ) /*一组蝶形运算*/ 
		{                           
			for(k=0;k<l;k++) /*一个蝶形运算*/ 
			{                 
				Mul(m_OperSignal[j+k+l]	,m_WArray[m_nOperSize*k/2/l],	&product);   
				Add(m_OperSignal[j+k]	,product,	&up);   
				Sub(m_OperSignal[j+k]	,product,	&down);   
				m_OperSignal[j+k]	= up;   
				m_OperSignal[j+k+l]	= down;   
			}   
		}   
	 }   

	/*计算结果的横纵坐标*/
	GenerateResult(pXAxisArray, pXAxisLen, pYAxisArray, pYAxisLen);


	return 1;
}

int FftComplex::FftConf(int nWindowMode, int nSampleFreq, int nOperSize, int nOutputMode)
{
	//加窗函数
	if ( nWindowMode != 0 )
	{
		m_nWindowMode = nWindowMode;						
	}
	//输入信号的采样率
	if ( nSampleFreq != 0 )
	{
		m_nSampleFreq = nSampleFreq;							
	}

	//运算数组长度
	if ( nOperSize==m_nOperSize || nOperSize==0 )
	{
		//和原来相等或者为0就不用变
	}
	else{
		if ( nOperSize != 0)
		{
			m_nOperSize = nOperSize ;
		}
		if ( m_OperSignal!= NULL)
		{
			free(m_OperSignal);
		}		
		m_OperSignal = (complex *)calloc(m_nOperSize,sizeof(complex));
		if ( m_WArray!= NULL)
		{
			free(m_WArray);
			InitialWArray();
		}
	}
	

	if (nOutputMode != 0)
	{
		m_nOutputMode = nOutputMode;
	}
	return 1;
}

int FftComplex::ChangeAddress()
{
	complex   temp;   
	unsigned   short   i=0,j=0,k=0;   
	double   t;   

	//int nSizeX;

	for(i=0;i<m_nOperSize;i++)   
	{   
		k=i;j=0;   
		t=(log(m_nOperSize)/log(2));   
		while(   (t--)>0   )   
		{   
			j=j<<1;   
			j|=(k&1);   
			k=k>>1;   
		}   
		if(j>i)   
		{   
			temp.real = m_OperSignal[i].real;  
			temp.img  = m_OperSignal[i].img;
			m_OperSignal[i].real= m_OperSignal[j].real;
			m_OperSignal[i].img	=m_OperSignal[j].img;  
			m_OperSignal[j].real=temp.real;   
			m_OperSignal[j].img	=temp.img; 
		}   
	}   

	return 1;
}

int FftComplex::InitialWArray()
{
	int i;
	m_WArray=(complex *)calloc(m_nOperSize,sizeof(complex));
	for(i=0;i<m_nOperSize;i++)
	{   
		m_WArray[i].real= cos(2*PI/m_nOperSize*i);   
		m_WArray[i].img	= -1*sin(2*PI/m_nOperSize*i);   
	}
	return 1;
}

/*
#define WINFUN_RECT					(1)
#define WINFUN_HANNING				(2)
#define WINFUN_HAMMING				(3)
#define WINFUN_FLATTOP				(4)
#define WINFUN_TRIANGLE				(5)
*/

int FftComplex::AddWinFun()
{
	int nCount = 0;
	//double fCosValue;

	switch ( m_nWindowMode )
	{
	case WINFUN_RECT:
		//WINFUN_RECT	W[n]=1.0	区分频域和振幅接近的信号瞬时信号宽度小于窗
		//矩形窗就是乘以1.0，因此不做任何处理
		break;
	case WINFUN_HANNING:
		//WINFUN_HANNING	W[n]=0.5cos(2n*PI/N)	瞬时信号宽度大于窗 普通目的的应用
		for ( nCount=0 ; nCount<m_nOperSize ;nCount++)
		{
			m_OperSignal[nCount].real = m_OperSignal[nCount].real * 0.5 * cos( 2*nCount*PI/m_nOperSize );
			m_OperSignal[nCount].img = m_OperSignal[nCount].img * 0.5 * cos( 2*nCount*PI/m_nOperSize );
		}
		break;
	case WINFUN_HAMMING:
		//WINFUN_HAMMING	W[n]=0.54-0.46cos(2n*PI/N)	声音处理
		for ( nCount=0 ; nCount<m_nOperSize ;nCount++)
		{
			m_OperSignal[nCount].real = m_OperSignal[nCount].real * (0.54 - 0.46*cos(	2*nCount*PI/m_nOperSize	)) ;
			m_OperSignal[nCount].img = m_OperSignal[nCount].img * (0.54 - 0.46*cos(	2*nCount*PI/m_nOperSize	)) ;
		}
		break;
	case WINFUN_FLATTOP:
		//WINFUN_FLATTOP	W[n]=0.2810639-0.5208972cos(2n*PI/N) +0.1980399cos(2n*PI/N)	分析无精确参照物且要求精确测量的信号  
		for ( nCount=0 ; nCount<m_nOperSize ;nCount++)
		{			
			m_OperSignal[nCount].real = m_OperSignal[nCount].real * (0.2810639 - 0.5208972*cos(2*nCount*PI/m_nOperSize) +0.1980399*cos(2*nCount*PI/m_nOperSize)) ;
			m_OperSignal[nCount].img = m_OperSignal[nCount].img * (0.2810639 - 0.5208972*cos(2*nCount*PI/m_nOperSize) +0.1980399*cos(2*nCount*PI/m_nOperSize)) ;
		}
		break;
	case WINFUN_TRIANGLE:
		//WINFUN_TRIANGLE	W[n]=1-｜（2n-N）/N｜	无特殊应用
		for ( nCount=0 ; nCount<m_nOperSize ;nCount++)
		{			
			m_OperSignal[nCount].real = m_OperSignal[nCount].real * (1 - abs( (2*nCount-m_nOperSize)/m_nOperSize )) ;
			m_OperSignal[nCount].img = m_OperSignal[nCount].img * (1 - abs( (2*nCount-m_nOperSize)/m_nOperSize )) ;
		}
		break;
	default:
		break;
	}

	return 1;
}


void FftComplex::Add(complex   a,complex   b,complex   *c)   
{   
	c->real=a.real+b.real;   
	c->img=a.img+b.img;   
}   

void FftComplex::Mul(complex   a,complex   b,complex   *c)   
{   
	c->real=a.real*b.real   -   a.img*b.img;   
	c->img=a.real*b.img   +   a.img*b.real;   
}   
void FftComplex::Sub(complex   a,complex   b,complex   *c)   
{   
	c->real=a.real-b.real;   
	c->img=a.img-b.img;   
}   
void FftComplex::Divi(complex   a,complex   b,complex   *c)   
{   
	c->real=(   a.real*b.real+a.img*b.img   )/(   
		b.real*b.real+b.img*b.img);   
	c->img=(   a.img*b.real-a.real*b.img)/(b.real*b.real+b.img*b.img);   
} 

int FftComplex::GenerateResult(double* pXAxisArray, int* pXAxisLen, double* pYAxisArray, int* pYAxisLen)
{
	/*这里注意，输出的结果应该是FFT结果的一半，这也符合奈奎斯特采样定律*/
	*pXAxisLen = m_nOperSize / 2;
	*pYAxisLen = m_nOperSize / 2;

	int nCount = 0;
	/*X轴*/
	for ( nCount=0 ; nCount<m_nOperSize / 2 ; nCount++)
	{
		pXAxisArray[nCount] = floor((m_nSampleFreq+0.0)*nCount/m_nOperSize);
	}


	/*Y轴*/


	for ( nCount=0 ; nCount<m_nOperSize / 2 ; nCount++)
	{
		pYAxisArray[nCount] = sqrt(m_OperSignal[nCount].real * m_OperSignal[nCount].real + m_OperSignal[nCount].img * m_OperSignal[nCount].img);	//这个结果算出来单位是 幅值
		if ( nCount==0 )
		{
			pYAxisArray[nCount] = pYAxisArray[nCount] / m_nOperSize ;			//这个结果算出来单位是V
		}
		else{
			pYAxisArray[nCount] = pYAxisArray[nCount] / m_nOperSize / 2 ;
		}	
	}
	
	switch ( m_nOutputMode )
	{
	case YAXIS_UNIT_V:
		//上面的FOR循环结果就是V了
		break;
	case YAXIS_UNIT_DBV:
		for ( nCount=0 ; nCount<m_nOperSize/2 ; nCount++)
		{
			pYAxisArray[nCount] = 20 * log10( pYAxisArray[nCount] );
		}
		break;
	case YAXIS_UNIT_DBu:
		for ( nCount=0 ; nCount<m_nOperSize/2 ; nCount++)
		{
			pYAxisArray[nCount] = 20 * log10( pYAxisArray[nCount] * 1000000 );
		}
		break;
	case YAXIS_UNIT_DBm:
		for ( nCount=0 ; nCount<m_nOperSize/2 ; nCount++)
		{
			pYAxisArray[nCount] = 10 * log10( pYAxisArray[nCount] * pYAxisArray[nCount] / m_nImpedance * 1000 );
		}
		break;
	default:
		break;
	}
/**/
	
	return 1;
}

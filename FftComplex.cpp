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
	m_nWindowMode = WINFUN_RECT;							//�Ӵ�����
	m_nSampleFreq = 100000000;							//�����źŵĲ�����
	m_nPower = 9	;								//2��n����
	m_nOperSize = 512 ;

	m_nOutputMode = YAXIS_UNIT_V;
	m_nImpedance = 1000;


	InitialWArray();
	m_OperSignal = (complex *)calloc(m_nOperSize,sizeof(complex));
}

FftComplex::~FftComplex()
{
	//�ͷ��ڴ�
	free(m_OperSignal);
	free(m_WArray);
}

int FftComplex::FftRun(	double* pSignalArray, int nSignalLen, double* pXAxisArray, int* pXAxisLen, double* pYAxisArray, int* pYAxisLen	)
{
	/*�Ƚ�nSignalLen��m_nOperSize���ȣ�ͬʱ���в�0���߳�ȡ*/
	int nCount = 0;
	int nIndex = 0;

	if ( nSignalLen <= m_nOperSize)
	{
		//���в�0
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
		//nSignalLen�����ݳ�ȡ��m_nOperSize����
		for( nCount=0 ; nCount<m_nOperSize ; nCount++)
		{
			nIndex = (int)(nCount*nSignalLen/m_nOperSize);
		
			m_OperSignal[nCount].real = pSignalArray[nIndex];
			m_OperSignal[nCount].img = 0;
		
		}
	}

	/*���мӴ�*/
	AddWinFun();

	/*��ʼ���仯��*/
//	InitialWArray();

	/*��ԭʼ�źŽ��б�ַ*/
	ChangeAddress();

	/*���л�-2 FFT����*/
	int   i=0,j=0,k=0,l=0;   
	complex up,down,product;   

	int nButterflyOp = (int)(log(m_nOperSize)/log(2));
  
	for( i=0 ; i< nButterflyOp ; i++) /*һ����������*/ 
	{       
		l=1<<i;   
		for(j=0 ; j<m_nOperSize ; j+=2*l ) /*һ���������*/ 
		{                           
			for(k=0;k<l;k++) /*һ����������*/ 
			{                 
				Mul(m_OperSignal[j+k+l]	,m_WArray[m_nOperSize*k/2/l],	&product);   
				Add(m_OperSignal[j+k]	,product,	&up);   
				Sub(m_OperSignal[j+k]	,product,	&down);   
				m_OperSignal[j+k]	= up;   
				m_OperSignal[j+k+l]	= down;   
			}   
		}   
	 }   

	/*�������ĺ�������*/
	GenerateResult(pXAxisArray, pXAxisLen, pYAxisArray, pYAxisLen);


	return 1;
}

int FftComplex::FftConf(int nWindowMode, int nSampleFreq, int nOperSize, int nOutputMode)
{
	//�Ӵ�����
	if ( nWindowMode != 0 )
	{
		m_nWindowMode = nWindowMode;						
	}
	//�����źŵĲ�����
	if ( nSampleFreq != 0 )
	{
		m_nSampleFreq = nSampleFreq;							
	}

	//�������鳤��
	if ( nOperSize==m_nOperSize || nOperSize==0 )
	{
		//��ԭ����Ȼ���Ϊ0�Ͳ��ñ�
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
		//WINFUN_RECT	W[n]=1.0	����Ƶ�������ӽ����ź�˲ʱ�źſ��С�ڴ�
		//���δ����ǳ���1.0����˲����κδ���
		break;
	case WINFUN_HANNING:
		//WINFUN_HANNING	W[n]=0.5cos(2n*PI/N)	˲ʱ�źſ�ȴ��ڴ� ��ͨĿ�ĵ�Ӧ��
		for ( nCount=0 ; nCount<m_nOperSize ;nCount++)
		{
			m_OperSignal[nCount].real = m_OperSignal[nCount].real * 0.5 * cos( 2*nCount*PI/m_nOperSize );
			m_OperSignal[nCount].img = m_OperSignal[nCount].img * 0.5 * cos( 2*nCount*PI/m_nOperSize );
		}
		break;
	case WINFUN_HAMMING:
		//WINFUN_HAMMING	W[n]=0.54-0.46cos(2n*PI/N)	��������
		for ( nCount=0 ; nCount<m_nOperSize ;nCount++)
		{
			m_OperSignal[nCount].real = m_OperSignal[nCount].real * (0.54 - 0.46*cos(	2*nCount*PI/m_nOperSize	)) ;
			m_OperSignal[nCount].img = m_OperSignal[nCount].img * (0.54 - 0.46*cos(	2*nCount*PI/m_nOperSize	)) ;
		}
		break;
	case WINFUN_FLATTOP:
		//WINFUN_FLATTOP	W[n]=0.2810639-0.5208972cos(2n*PI/N) +0.1980399cos(2n*PI/N)	�����޾�ȷ��������Ҫ��ȷ�������ź�  
		for ( nCount=0 ; nCount<m_nOperSize ;nCount++)
		{			
			m_OperSignal[nCount].real = m_OperSignal[nCount].real * (0.2810639 - 0.5208972*cos(2*nCount*PI/m_nOperSize) +0.1980399*cos(2*nCount*PI/m_nOperSize)) ;
			m_OperSignal[nCount].img = m_OperSignal[nCount].img * (0.2810639 - 0.5208972*cos(2*nCount*PI/m_nOperSize) +0.1980399*cos(2*nCount*PI/m_nOperSize)) ;
		}
		break;
	case WINFUN_TRIANGLE:
		//WINFUN_TRIANGLE	W[n]=1-����2n-N��/N��	������Ӧ��
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
	/*����ע�⣬����Ľ��Ӧ����FFT�����һ�룬��Ҳ�����ο�˹�ز�������*/
	*pXAxisLen = m_nOperSize / 2;
	*pYAxisLen = m_nOperSize / 2;

	int nCount = 0;
	/*X��*/
	for ( nCount=0 ; nCount<m_nOperSize / 2 ; nCount++)
	{
		pXAxisArray[nCount] = floor((m_nSampleFreq+0.0)*nCount/m_nOperSize);
	}


	/*Y��*/


	for ( nCount=0 ; nCount<m_nOperSize / 2 ; nCount++)
	{
		pYAxisArray[nCount] = sqrt(m_OperSignal[nCount].real * m_OperSignal[nCount].real + m_OperSignal[nCount].img * m_OperSignal[nCount].img);	//�������������λ�� ��ֵ
		if ( nCount==0 )
		{
			pYAxisArray[nCount] = pYAxisArray[nCount] / m_nOperSize ;			//�������������λ��V
		}
		else{
			pYAxisArray[nCount] = pYAxisArray[nCount] / m_nOperSize / 2 ;
		}	
	}
	
	switch ( m_nOutputMode )
	{
	case YAXIS_UNIT_V:
		//�����FORѭ���������V��
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

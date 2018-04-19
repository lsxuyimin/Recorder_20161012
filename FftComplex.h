// FftComplex.h: interface for the FftComplex class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FFTCOMPLEX_H__E7E5160E_A17B_42C3_987C_65E711BE5DBB__INCLUDED_)
#define AFX_FFTCOMPLEX_H__E7E5160E_A17B_42C3_987C_65E711BE5DBB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


////////////////////////////��������start//////////////////////////////////////////////
typedef struct
{
	double real;//����ʵ��
	double img;//�����鲿imaginary part
}complex;
////////////////////////////��������end//////////////////////////////////////////////



////////////////////////////�Ӵ���������start//////////////////////////////////////////////
#define WINFUN_RECT					(1)
#define WINFUN_HANNING				(2)
#define WINFUN_HAMMING				(3)
#define WINFUN_FLATTOP				(4)
#define WINFUN_TRIANGLE				(5)
#define WINFUN_INDEX					(6)						//ָ������֪����ôʵ��  ��ֵ��֪��ʲô��˼
#define WINFUN_KAISERBESSEL				(7)						//Kaiser-Bessel��  ��ʽ������
////////////////////////////�Ӵ���������end//////////////////////////////////////////////

////////////////////////////Y�������λ����start//////////////////////////////////////////////
#define YAXIS_UNIT_V					(1)
#define YAXIS_UNIT_DBV					(2)
#define YAXIS_UNIT_DBu					(3)
#define YAXIS_UNIT_DBm					(4)
////////////////////////////Y�������λ����end//////////////////////////////////////////////
#define PI (3.1415926)


class FftComplex  
{
public:
	int GenerateResult(double* pXAxisArray, int* pXAxisLen, double* pYAxisArray, int* pYAxisLen);
	int AddWinFun();
	int InitialWArray();
	int ChangeAddress();
	void Add(complex   a,complex   b,complex   *c);	
	void Mul(complex   a,complex   b,complex   *c);   
	void Sub(complex   a,complex   b,complex   *c);  
	void Divi(complex   a,complex   b,complex   *c);
	
	/*ԭʼ�ź����飬ԭʼ�ź����鳤�ȣ����X�����飬���X�����鳤�ȣ����Y�����飬���Y�����鳤��*/
	int FftRun(double* pSignalArray, int nSignalLen, double* pXAxisArray, int* pXAxisLen, double* pYAxisArray, int* pYAxisLen	);
	/*�Ӵ������������ʣ����������С*/
	int FftConf(int nWindowMode, int nSampleFreq, int nOperSize,int nOutputMode);

	int m_nWindowMode;							//�Ӵ�����
	int m_nSampleFreq;							//�����źŵĲ�����		��λHz
	int m_nPower;								//2��n����
	int m_nOperSize;							//������źŵĳ��ȣ�������2��n�η�

	int m_nOutputMode;							//���ģʽ����ӦYAXIS_UNIT_DBV��YAXIS_UNIT_DBu��YAXIS_UNIT_DBm
	int m_nImpedance;							//�迹����YAXIS_UNIT_DBm������ʹ��	��λ��ŷķ

	complex* m_OperSignal;				//������ԭʼ�ź� ͬʱҲ��Ƶ�����
	complex* m_WArray;


	FftComplex();
	virtual ~FftComplex();

};

#endif // !defined(AFX_FFTCOMPLEX_H__E7E5160E_A17B_42C3_987C_65E711BE5DBB__INCLUDED_)

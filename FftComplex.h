// FftComplex.h: interface for the FftComplex class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FFTCOMPLEX_H__E7E5160E_A17B_42C3_987C_65E711BE5DBB__INCLUDED_)
#define AFX_FFTCOMPLEX_H__E7E5160E_A17B_42C3_987C_65E711BE5DBB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


////////////////////////////复数定义start//////////////////////////////////////////////
typedef struct
{
	double real;//复数实部
	double img;//复数虚部imaginary part
}complex;
////////////////////////////复数定义end//////////////////////////////////////////////



////////////////////////////加窗函数定义start//////////////////////////////////////////////
#define WINFUN_RECT					(1)
#define WINFUN_HANNING				(2)
#define WINFUN_HAMMING				(3)
#define WINFUN_FLATTOP				(4)
#define WINFUN_TRIANGLE				(5)
#define WINFUN_INDEX					(6)						//指数窗不知道怎么实现  终值不知道什么意思
#define WINFUN_KAISERBESSEL				(7)						//Kaiser-Bessel窗  公式看不懂
////////////////////////////加窗函数定义end//////////////////////////////////////////////

////////////////////////////Y轴输出单位定义start//////////////////////////////////////////////
#define YAXIS_UNIT_V					(1)
#define YAXIS_UNIT_DBV					(2)
#define YAXIS_UNIT_DBu					(3)
#define YAXIS_UNIT_DBm					(4)
////////////////////////////Y轴输出单位定义end//////////////////////////////////////////////
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
	
	/*原始信号数组，原始信号数组长度，输出X轴数组，输出X轴数组长度，输出Y轴数组，输出Y轴数组长度*/
	int FftRun(double* pSignalArray, int nSignalLen, double* pXAxisArray, int* pXAxisLen, double* pYAxisArray, int* pYAxisLen	);
	/*加窗函数，采样率，运算数组大小*/
	int FftConf(int nWindowMode, int nSampleFreq, int nOperSize,int nOutputMode);

	int m_nWindowMode;							//加窗函数
	int m_nSampleFreq;							//输入信号的采样率		单位Hz
	int m_nPower;								//2的n次幂
	int m_nOperSize;							//输出的信号的长度，长度是2的n次方

	int m_nOutputMode;							//输出模式，对应YAXIS_UNIT_DBV，YAXIS_UNIT_DBu，YAXIS_UNIT_DBm
	int m_nImpedance;							//阻抗，和YAXIS_UNIT_DBm输出配合使用	单位：欧姆

	complex* m_OperSignal;				//真正的原始信号 同时也是频域输出
	complex* m_WArray;


	FftComplex();
	virtual ~FftComplex();

};

#endif // !defined(AFX_FFTCOMPLEX_H__E7E5160E_A17B_42C3_987C_65E711BE5DBB__INCLUDED_)

// Product.h: interface for the CProduct class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PRODUCT_H__100BE4B7_F8A7_4C58_893B_6FA68A56803C__INCLUDED_)
#define AFX_PRODUCT_H__100BE4B7_F8A7_4C58_893B_6FA68A56803C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



class CWaveData;
class CRecorderView;
class FftComplex;

#include "VirtualCard.h"
#include "PhyCard.h"


/////////////自定义消息/////////////////////////////////////////////////////////////
#define WM_ACQEND		WM_USER + 100
#define WM_INACQ		WM_USER + 101
#define WM_WAITACQ		WM_USER + 102

#define WM_UPDATE_CACHE	WM_USER + 103
#define WM_UPDATE_PLAY	WM_USER + 104

////////////重定义函数原型//////////////////////////////////////////////////////////////

//读取缓冲区数据(通道号,数据指针,重采样模式,压缩间隔,采样起点,读取数据长度)
typedef int		(__stdcall *pREC_Pack_Ptr)(int, short *, int, int, long, long);

//设置采样率(采样率,reserved)
typedef double	(__stdcall *pREC_SetAcqRate_Ptr)(double arate, double amrate);
//读取采样率(采样率,采样率数值)
typedef void	(__stdcall *pREC_GetAcqRate_Ptr)(double *arate, double *amrate);

//设置采样帧信息(采样长度<样点>,相对与触发信号的延时值)
typedef void	(__stdcall *pREC_SetAcqFrame_Ptr)(int alength, int adelay);
//读取采样帧信息(采样长度<样点>,相对与触发信号的延时值)
typedef void	(__stdcall *pREC_GetAcqFrame_Ptr)(int *alength, int *adelay);

//设置触发条件(触发模式,触发通道<内触发有效>,触发电平<内触发有效>)
typedef void	(__stdcall *pREC_SetAcqTrigger_Ptr)(int tmode, int tch,int tlevel);
//读取触发条件(触发模式,触发通道<内触发有效>,触发电平<内触发有效>)
typedef void	(__stdcall *pREC_GetAcqTrigger_Ptr)(int *tmode, int *tch,int *tlevel);

//设置通道控制参数(通道号,通道量程代号<range_tab索引>,耦合模式)
typedef void	(__stdcall *pREC_SetChannelParam_Ptr)(int chidx, int rgidx,int cpl);
//读取通道控制参数(通道号,通道量程代号<range_tab索引>,耦合模式)
typedef void	(__stdcall *pREC_GetChannelParam_Ptr)(int *chidx, int *rgidx,int *cpl);

//启动采集
typedef void	(__stdcall *pREC_Acq_Ptr)(void);
//强制终止采集
typedef void	(__stdcall *pREC_StopAcq_Ptr)(void);
//采集状态检测
typedef int		(__stdcall *pREC_StatusCheck_Ptr)(void);

/*

//系统自检(产品信息结构,reserved)*
typedef	int		(__stdcall *pIDTS_SysInit_Ptr)(_SysInfo *SysInfo_ptr, int);
//退出采集系统
typedef int		(__stdcall *pIDTS_SysFinally_Ptr)(void); 

//??
typedef void	(__stdcall *pIDTS_ManualTrg_Ptr)(void);

//读I/O口数据(采集卡卡号)
typedef unsigned short(__stdcall *pIDTS_DI_Ptr)(short card);
//写I/O口数据(采集卡卡号,输出数据16Bit)
typedef unsigned short(__stdcall *pIDTS_DO_Ptr)(short card, unsigned short data);

*/




//该类存放所有的数据 是DOC类的成员变量
class CProduct  
{
public:
	void PlayEnvInitialize();							
	void GetDataFromHDD(int nChNum,long nPointStartPos, int nInterval, short* pDataBuffer , int nDataLen);
	void AcqEnvInitialize();							//该采集初始化由点击了采集按钮后调用
	void WriteInSetupDoc();
	void OpenStorageDoc();
	void OnAcqEnd();
	int GetACQStatus();
	void Get_AI();
	void Set_AI();
	void FunPtrInitialize(int nMode);
	void StartCapture();
	void StopCapture();
	
	void DataInitialize(int ChannelNum);			//ChannelNum为包含的通道数
	void Initialize();								//初始化数据 新建CView中的各个窗口(CWatchDlg,CBgDlg)
	int m_nOpenChannelNum;							//基于1
	int m_nRecorderMode;							//0:physical   1:demo    2:play
	CTypedPtrList<CObList, CWaveData*> m_WaveList;	//所有直接通道波形数据
	CTypedPtrList<CObList, CWaveData*> m_CombWaveList;	//所有组合波形数据
	int m_nCombWaveNum;

	int ChechCard();

	CRecorderView* m_pRecView;
	CProduct();
	virtual ~CProduct();
	
	bool m_bAcqing;
	bool m_bPlaying;
	bool m_bTransing;
	bool m_bExistData;

	AcqInfo m_AcqInfo;

	CVirtualCard m_VirtualCard;
	CPhyCard m_PhyCard;

	PlayCtrlInfo m_PlayCtrlInfo;
	

	FILE* m_pArrayWriteFile[MAX_CHANNUM+1];			//一个Setup.inf和X个(最多64)txt
	FILE* m_pArrayReadFile[MAX_CHANNUM+1];		
	
	short m_BufferData[10240];
	unsigned char m_BufferDataForPhy[8][10240*CH_NUM_IN_PER_CARD*2];	//第一个8是八张卡的意思，懒得用new动态分配了
	unsigned short **m_ShortData;
	int m_nSegmentNum;							//一共有多少个数据分段 值为 数据量/最小数据分段数据量 (这里是10K个点)
	int m_nSegmentValidIndex;
	int m_nSegmentPlayIndex;
	int m_nSegmentPlayWndWidth;
	int m_nOffsetInSegment;

	int m_nPlayStartIndex;						//代表在采样点中开始播放的起始点
	int m_nPlayWndWidth;						//播放窗口的宽度 配合m_nPlayStartIndex使用
												//m_nPlayStartIndex+m_nPlayWndWidth <= m_AcqInfo.samplingLength
	
	CString m_strCurWorkPath;
	CString m_strWorkPlace;
	CString m_strSavePath;

	/*FFT相关*/
	FftComplex* m_pFftModule;						//FFT计算模块
	FftConf m_FftConf;								//FFT变换用到的设定值 所有信号公用同一组设置
	double m_aFftResult[64][65536];				//FFT变换后的结果，因为在MeasureDlg中也要用到FFT的结果，所以存起来避免二次计算
									//造成了32M的内存占用，这里慎用


/*
	pREC_Pack_Ptr				REC_Pack_Ptr;
	pREC_SetAcqRate_Ptr			REC_SetAcqRate_Ptr;
	pREC_GetAcqRate_Ptr			REC_GetAcqRate_Ptr;
	pREC_SetAcqFrame_Ptr		REC_SetAcqFrame_Ptr;
	pREC_GetAcqFrame_Ptr		REC_GetAcqFrame_Ptr;
	pREC_SetAcqTrigger_Ptr		REC_SetAcqTrigger_Ptr;
	pREC_GetAcqTrigger_Ptr		REC_GetAcqTrigger_Ptr;
	pREC_SetChannelParam_Ptr	REC_SetChannelParam_Ptr;
	pREC_GetChannelParam_Ptr	REC_GetChannelParam_Ptr;
	pREC_Acq_Ptr				REC_Acq_Ptr;
	pREC_StopAcq_Ptr			REC_StopAcq_Ptr;
	pREC_StatusCheck_Ptr		REC_StatusCheck_Ptr;
*/


	int		Pack(int chNum, short *pData, int mode, int interval, long start, long dataLen);	//读取缓冲区数据(通道号,数据指针,重采样模式,压缩间隔,采样起点,读取数据长度)
	double	SetAcqRate(double arate, double amrate);
	void	GetAcqRate(double *arate, double *amrate);
	void	SetAcqFrame(int alength, int adelay);
	void	GetAcqFrame(int *alength, int *adelay);
	void	SetAcqTrigger(int tmode, int tch,int tlevel);
	void	GetAcqTrigger(int *tmode, int *tch,int *tlevel);
	void	SetChannelParam(int chidx, int rgidx,int cpl);
	void	GetChannelParam(int *chidx, int *rgidx,int *cpl);
	
	void	Acq(void);
	void	StopAcq(void);
	int		StatusCheck(void);

};

#endif // !defined(AFX_PRODUCT_H__100BE4B7_F8A7_4C58_893B_6FA68A56803C__INCLUDED_)

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


/////////////�Զ�����Ϣ/////////////////////////////////////////////////////////////
#define WM_ACQEND		WM_USER + 100
#define WM_INACQ		WM_USER + 101
#define WM_WAITACQ		WM_USER + 102

#define WM_UPDATE_CACHE	WM_USER + 103
#define WM_UPDATE_PLAY	WM_USER + 104

////////////�ض��庯��ԭ��//////////////////////////////////////////////////////////////

//��ȡ����������(ͨ����,����ָ��,�ز���ģʽ,ѹ�����,�������,��ȡ���ݳ���)
typedef int		(__stdcall *pREC_Pack_Ptr)(int, short *, int, int, long, long);

//���ò�����(������,reserved)
typedef double	(__stdcall *pREC_SetAcqRate_Ptr)(double arate, double amrate);
//��ȡ������(������,��������ֵ)
typedef void	(__stdcall *pREC_GetAcqRate_Ptr)(double *arate, double *amrate);

//���ò���֡��Ϣ(��������<����>,����봥���źŵ���ʱֵ)
typedef void	(__stdcall *pREC_SetAcqFrame_Ptr)(int alength, int adelay);
//��ȡ����֡��Ϣ(��������<����>,����봥���źŵ���ʱֵ)
typedef void	(__stdcall *pREC_GetAcqFrame_Ptr)(int *alength, int *adelay);

//���ô�������(����ģʽ,����ͨ��<�ڴ�����Ч>,������ƽ<�ڴ�����Ч>)
typedef void	(__stdcall *pREC_SetAcqTrigger_Ptr)(int tmode, int tch,int tlevel);
//��ȡ��������(����ģʽ,����ͨ��<�ڴ�����Ч>,������ƽ<�ڴ�����Ч>)
typedef void	(__stdcall *pREC_GetAcqTrigger_Ptr)(int *tmode, int *tch,int *tlevel);

//����ͨ�����Ʋ���(ͨ����,ͨ�����̴���<range_tab����>,���ģʽ)
typedef void	(__stdcall *pREC_SetChannelParam_Ptr)(int chidx, int rgidx,int cpl);
//��ȡͨ�����Ʋ���(ͨ����,ͨ�����̴���<range_tab����>,���ģʽ)
typedef void	(__stdcall *pREC_GetChannelParam_Ptr)(int *chidx, int *rgidx,int *cpl);

//�����ɼ�
typedef void	(__stdcall *pREC_Acq_Ptr)(void);
//ǿ����ֹ�ɼ�
typedef void	(__stdcall *pREC_StopAcq_Ptr)(void);
//�ɼ�״̬���
typedef int		(__stdcall *pREC_StatusCheck_Ptr)(void);

/*

//ϵͳ�Լ�(��Ʒ��Ϣ�ṹ,reserved)*
typedef	int		(__stdcall *pIDTS_SysInit_Ptr)(_SysInfo *SysInfo_ptr, int);
//�˳��ɼ�ϵͳ
typedef int		(__stdcall *pIDTS_SysFinally_Ptr)(void); 

//??
typedef void	(__stdcall *pIDTS_ManualTrg_Ptr)(void);

//��I/O������(�ɼ�������)
typedef unsigned short(__stdcall *pIDTS_DI_Ptr)(short card);
//дI/O������(�ɼ�������,�������16Bit)
typedef unsigned short(__stdcall *pIDTS_DO_Ptr)(short card, unsigned short data);

*/




//���������е����� ��DOC��ĳ�Ա����
class CProduct  
{
public:
	void PlayEnvInitialize();							
	void GetDataFromHDD(int nChNum,long nPointStartPos, int nInterval, short* pDataBuffer , int nDataLen);
	void AcqEnvInitialize();							//�òɼ���ʼ���ɵ���˲ɼ���ť�����
	void WriteInSetupDoc();
	void OpenStorageDoc();
	void OnAcqEnd();
	int GetACQStatus();
	void Get_AI();
	void Set_AI();
	void FunPtrInitialize(int nMode);
	void StartCapture();
	void StopCapture();
	
	void DataInitialize(int ChannelNum);			//ChannelNumΪ������ͨ����
	void Initialize();								//��ʼ������ �½�CView�еĸ�������(CWatchDlg,CBgDlg)
	int m_nOpenChannelNum;							//����1
	int m_nRecorderMode;							//0:physical   1:demo    2:play
	CTypedPtrList<CObList, CWaveData*> m_WaveList;	//����ֱ��ͨ����������
	CTypedPtrList<CObList, CWaveData*> m_CombWaveList;	//������ϲ�������
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
	

	FILE* m_pArrayWriteFile[MAX_CHANNUM+1];			//һ��Setup.inf��X��(���64)txt
	FILE* m_pArrayReadFile[MAX_CHANNUM+1];		
	
	short m_BufferData[10240];
	unsigned char m_BufferDataForPhy[8][10240*CH_NUM_IN_PER_CARD*2];	//��һ��8�ǰ��ſ�����˼��������new��̬������
	unsigned short **m_ShortData;
	int m_nSegmentNum;							//һ���ж��ٸ����ݷֶ� ֵΪ ������/��С���ݷֶ������� (������10K����)
	int m_nSegmentValidIndex;
	int m_nSegmentPlayIndex;
	int m_nSegmentPlayWndWidth;
	int m_nOffsetInSegment;

	int m_nPlayStartIndex;						//�����ڲ������п�ʼ���ŵ���ʼ��
	int m_nPlayWndWidth;						//���Ŵ��ڵĿ�� ���m_nPlayStartIndexʹ��
												//m_nPlayStartIndex+m_nPlayWndWidth <= m_AcqInfo.samplingLength
	
	CString m_strCurWorkPath;
	CString m_strWorkPlace;
	CString m_strSavePath;

	/*FFT���*/
	FftComplex* m_pFftModule;						//FFT����ģ��
	FftConf m_FftConf;								//FFT�任�õ����趨ֵ �����źŹ���ͬһ������
	double m_aFftResult[64][65536];				//FFT�任��Ľ������Ϊ��MeasureDlg��ҲҪ�õ�FFT�Ľ�������Դ�����������μ���
									//�����32M���ڴ�ռ�ã���������


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


	int		Pack(int chNum, short *pData, int mode, int interval, long start, long dataLen);	//��ȡ����������(ͨ����,����ָ��,�ز���ģʽ,ѹ�����,�������,��ȡ���ݳ���)
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

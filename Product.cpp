// Product.cpp: implementation of the CProduct class.
//
//////////////////////////////////////////////////////////////////////




#include "stdafx.h"
#include "recorder.h"
#include "Product.h"


#include "CheckCardDlg.h"
#include "RecorderView.h"
#include "WaveData.h"
#include <math.h>
#include "MainFrm.h"
#include "StoragePathDlg.h"
#include "DlgBarExBottomPlay.h"
#include "PlayBar.h"
#include "WndManage.h"
#include "FftComplex.h"



#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
#include "WCDPcie//WCDPcieDriver.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

extern int g_nScreenWidth;
extern int g_nScreenHeight;

CEvent g_TransAndPlayEvent(FALSE,FALSE);
CEvent g_PlayPauseEvent(FALSE,TRUE);


CProduct::CProduct()
{
	m_bAcqing = false;
	m_bExistData = false;
	m_bPlaying = false;
	m_bTransing = false;

/*
	g_nScreenWidth = GetSystemMetrics ( SM_CXSCREEN ); 
	g_nScreenHeight= GetSystemMetrics ( SM_CYSCREEN ); 
*/
	g_nScreenWidth = 4096;						//�Ƚ�ÿ�����ɵ��������赽���
	g_nScreenHeight= 10000; 

	m_nOpenChannelNum = 0;
	m_nCombWaveNum = 0;

	m_AcqInfo.range=10;											//��ʼ���� 10v	
	m_AcqInfo.samplingLength = 40960;							//��ʼ�ɼ����� 40960	ָ����ÿ��ͨ���Ĳ�����
	m_AcqInfo.samplingRate = 1000000;							//��ʼ����Ƶ��1M
	m_AcqInfo.trigChan = 0;
	m_AcqInfo.trigLevel = 0;
	m_AcqInfo.trigMode = TRIMODE_MANUAL;
	m_AcqInfo.samplingDelay = 0;								//������ʱ ms
	m_AcqInfo.trigLowerLimit = 17873;
	m_AcqInfo.trigUpperLimit = 47663;

	/*��ʼ����ǰ�Ĺ���·��m_strWorkPath*/
	CString sPath;   
	GetModuleFileName(NULL,sPath.GetBufferSetLength(255+1),255);   
	sPath.ReleaseBuffer();   
	int nPos;   
	nPos=sPath.ReverseFind('\\');   
	sPath=sPath.Left(nPos);   
	
	CString prefixpath, childPath, temp;
	nPos = sPath.ReverseFind('\\');  
	childPath = sPath.Mid(nPos+1);
	if ("Debug" == childPath){
		prefixpath = sPath.Left(nPos);
	}
	else{
		prefixpath = sPath;
	}
	m_strWorkPlace = prefixpath + "\\AcqData";
	m_strCurWorkPath = prefixpath + "\\AcqData\\Temp";

	/*��ʼ��m_pArrayFile*/
	int nCount = 0;
	for (nCount=0 ; nCount<MAX_CHANNUM+1 ; nCount++)
	{
		m_pArrayWriteFile[nCount] = NULL;
	}
	for (nCount=0 ; nCount<MAX_CHANNUM+1 ; nCount++)
	{
		m_pArrayReadFile[nCount] = NULL;
	}

	m_nSegmentNum = 0;	
	m_nSegmentValidIndex = 0;
	m_nSegmentPlayIndex = 0;
	m_nSegmentPlayWndWidth = 1;
	m_nOffsetInSegment = 0;

	m_PlayCtrlInfo.nSpeed = 64;
	m_PlayCtrlInfo.nInterval = 1;

	m_nPlayStartIndex = 0;				
	m_nPlayWndWidth = DATASTORAGE_SEGMENT;

	/*m_ShortData*/
	int i = 0;
	m_ShortData=new unsigned short*[CH_NUM_IN_PER_CARD*8];
	for(i=0;i<CH_NUM_IN_PER_CARD*8;i++)
	{
		m_ShortData[i]=new unsigned short[10240];
	}

	/*��ʼ��m_FftConf*/
	m_FftConf.nFftBins = 2048;
	m_FftConf.nWindowMode = WINFUN_RECT;
	m_FftConf.nOutputMode = YAXIS_UNIT_V;
	m_FftConf.nImpedance = 500;

	/*�½�*/
	m_pFftModule = new FftComplex(); 

}

CProduct::~CProduct()
{
	delete m_pFftModule;
	m_pFftModule = NULL;

	if (m_WaveList.IsEmpty())
	{
		//m_WaveList�б� Ϊ��
	} 
	else
	{
		//m_WaveList�б� ��Ϊ��  �������
		int i=0;
		CWaveData* pWaveData;
		for (i=0 ; i<MAX_CHANNUM ;i ++)
		{
			pWaveData = (CWaveData*)m_WaveList.GetAt(m_WaveList.FindIndex(i));
			delete pWaveData;
			pWaveData = NULL;
		}
		
	}
}

int CProduct::ChechCard()
{
	
	CCheckCardDlg dlg(this);
	if (IDOK == dlg.DoModal())
	{
		//�����ж� �����ִ���
		switch (m_nRecorderMode)
		{
		case 0:
			//����
			Initialize();
			break;
		case 1:
			//ģ�⿨
			Initialize();
			break;
		case 2:
			//�����Ѿ������ļ�
			PlayEnvInitialize();
			break;
		}
	}
	
	return 1;
}

void CProduct::Initialize()
{
	//1.��ʼ��CProduct�е�����
	DataInitialize(m_nOpenChannelNum);
	//2.��ʼ������ָ��
//	FunPtrInitialize(m_nRecorderMode);
	//3.��ʼ��View
	m_pRecView->ViewInitialize(m_nRecorderMode);
	//��ʼ����ɺ����Ĭ���趨
	m_pRecView->m_WndManage.DefaultSetting();
}

void CProduct::DataInitialize(int ChannelNum)
{	
	POSITION pos= m_WaveList.GetHeadPosition();
	CWaveData* pWaveData;

	while(pos!=NULL)
	{
		pWaveData=m_WaveList.GetNext(pos);
		delete pWaveData;	
		pWaveData = NULL;
	}
	m_WaveList.RemoveAll();


	for (int i=0 ; i<MAX_CHANNUM ; i++)
	{
		pWaveData = new CWaveData(i);
//		pWaveData->m_nChnIndex = i;
//		pWaveData->GenerateColor(pWaveData->m_nChnIndex , 0);

		if (i<ChannelNum){
			pWaveData->m_nChanState = 1;
		}
		else{
			pWaveData->m_nChanState = 0;
		}
		CString str;
		str.Format("chan%d",i+1);
		pWaveData->m_strChanName = str;
		m_WaveList.AddTail(pWaveData);
	}
}

DWORD WINAPI ControlAcqProc3(  LPVOID lpParameter  )
{
	//	IDTS_Product * tProduct = ( IDTS_Product * )lpParameter;
	
	int i,j;
	double randomNum;
//	CRecorderDoc* pDoc = (CRecorderDoc*)(lpParameter);
	CProduct* pProduct = (CProduct*)(lpParameter);
	
	CTypedPtrList<CObList, CWaveData*>* pWaveList = ( CTypedPtrList<CObList, CWaveData*>* )(&(pProduct->m_WaveList));
	
	time_t t;
	time(&t);
	srand(t);
	
	pProduct->m_bExistData = TRUE;

	while( pProduct -> m_bAcqing )
	{
		for ( i = 0 ; i < MAX_CHANNUM ; i++)
		{
			CWaveData* pWave = pWaveList->GetAt(pWaveList->FindIndex(i));
			//pWave->m_WaveData.RemoveAll();
			randomNum = (rand()%1000) / 100.0;
			for ( j = 0 ; j < g_nScreenWidth ; j++)
			{
				//�������
				//pWave->m_WaveData.SetAt(j,rand()%10000-5000);						//�������� 0~100 ֮��
				//������Ҳ�
				int save = (int)(5000* sin((j%200-100.0)/100*2*3.14+randomNum));
				pWave->m_WaveData.SetAt(j, (int)(5000* sin((j%200-100.0)/100*2*3.14+randomNum)));
				
			}
		}
		//ÿ��ѭ������50ms  ����ϣ��ÿ50ms����һ������
		Sleep(50);
	}
	return 0;
}

DWORD WINAPI ControlAcqProc(  LPVOID lpParameter  )
{
	CProduct* pProduct = (CProduct*)(lpParameter);
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
		
	while( pProduct->m_bAcqing )
	{
		int tAcqStatus = pProduct->GetACQStatus( );
		//TRACE("-----pProduct->GetACQStatus( ): %d------\n",tAcqStatus);
		if ( tAcqStatus == REC_STA_ACQEND )		// �жϲɼ��Ƿ����
		{
			//pProduct->m_iDatFlg = ACQDAT;
			pFrame->SendMessage( WM_ACQEND, 0, 0 );
			pProduct->OnAcqEnd();
			break;
		}
		else if (tAcqStatus == REC_STA_INACQ){	
			//�жϲɼ�
			pFrame->SendMessage( WM_INACQ, 0, 0 );
		}
		else if (tAcqStatus == REC_STA_WAITTRG){
			//�жϵȴ�
			pFrame->SendMessage( WM_WAITACQ, 0, 0 );
		}
		else{
			Sleep(100);
		}
		Sleep(2000);
	}
	pProduct->m_bAcqing = false;

	return 0;
}




void CProduct::StartCapture()
{
	//����Ӧ����������ϴβ�����Ϣ ���������̣߳�PlayBar���ر�128���ļ����
	AcqEnvInitialize();

	//�޸����ݴ洢·��
	CStoragePathDlg stoPathDlg;
	stoPathDlg.DoModal();

	//�������ò���
	Set_AI();

	//����SegmentNum
/*
	if (m_nRecorderMode == 0)		//TEST
	{
		m_AcqInfo.samplingLength = 1024*1024*10;
	}
*/	m_nSegmentNum = (int)(ceil((double)m_AcqInfo.samplingLength / (double)DATASTORAGE_SEGMENT)) ;
	m_nSegmentPlayIndex = 0;
	m_nSegmentValidIndex = 0;
	m_nPlayStartIndex = 0;	

	//�����߳� �ȴ��ɼ�->�ɼ���->�򿪴洢�ļ��Լ�Setup.inf->���ݻ���
	SECURITY_ATTRIBUTES saAttr;
	saAttr.nLength = sizeof( SECURITY_ATTRIBUTES );
	saAttr.bInheritHandle = TRUE; 
	saAttr.lpSecurityDescriptor = NULL; 
	DWORD ThreadId ;
	
	m_bAcqing = TRUE;

	Acq();

	VERIFY( CreateThread(&saAttr, 1000, ControlAcqProc, (void *)(this), 0, &ThreadId ) );
	//���߳����ǽ��� ���򿪴����߳�,�����߳�

	
/*
	if (1 == m_nRecorderMode)
	{
		//demo ģʽ
		//�������
		//����64�鲨��  ����m_WaveList
		
		SECURITY_ATTRIBUTES saAttr;
		saAttr.nLength = sizeof( SECURITY_ATTRIBUTES );
		saAttr.bInheritHandle = TRUE; 
		saAttr.lpSecurityDescriptor = NULL; 
		DWORD ThreadId ;
		
		m_bAcqing = TRUE;
		VERIFY( CreateThread(&saAttr, 1000, ControlAcqProc, (void *)(this), 0, &ThreadId ) );
		
	} 
	else
	{
	}
*/
}

void CProduct::StopCapture()
{
	if (1 == m_nRecorderMode)
	{
		//demo ģʽ
		//ֹͣ�����������
		
		m_bAcqing = FALSE;
	} 
	else
	{
	}
}



void CProduct::FunPtrInitialize(int nMode)
{

	if (1 == m_nRecorderMode)
	{
		//demo ģʽ
	
		
	} 
	else if (0 == m_nRecorderMode)
	{
		//PhyCard
	}
}


int	CProduct::Pack(int chNum, short *pData, int mode, int interval, long start, long dataLen)	//��ȡ����������(ͨ����,����ָ��,�ز���ģʽ,ѹ�����,�������,��ȡ���ݳ���)
{
	if (0 == m_nRecorderMode){
		//PhyCard
		m_PhyCard.PhyCard_Pack(chNum , pData , mode , interval , start ,dataLen);
	}
	else if (1 == m_nRecorderMode){
		//demo
		m_VirtualCard.VirCard_Pack(chNum , pData , mode , interval , start ,dataLen);
	}
	else{
		//play
	}
	return 0;
}

double CProduct::SetAcqRate(double arate, double amrate)
{
	if (0 == m_nRecorderMode){
		//PhyCard
		m_PhyCard.PhyCard_SetAcqRate(arate , amrate);
	}
	else if (1 == m_nRecorderMode){
		//demo
		m_VirtualCard.VirCard_SetAcqRate(arate , amrate);
	}
	else{
		//play
	}
	
	return 0;
}

void CProduct::GetAcqRate(double *arate, double *amrate)
{
	if (0 == m_nRecorderMode){
		//PhyCard
	}
	else if (1 == m_nRecorderMode){
		//demo
		m_VirtualCard.VirCard_GetAcqRate(arate , amrate);
	}
	else{
		//play
	}
	
}

void CProduct::SetAcqFrame(int alength, int adelay)
{
	if (0 == m_nRecorderMode){
		//PhyCard
		m_PhyCard.PhyCard_SetAcqFrame(alength , adelay);
	}
	else if (1 == m_nRecorderMode){
		//demo
		m_VirtualCard.VirCard_SetAcqFrame(alength , adelay);
	}
	else{
		//play
	}
	
}

void CProduct::GetAcqFrame(int *alength, int *adelay)
{
	if (0 == m_nRecorderMode){
		//PhyCard
	}
	else if (1 == m_nRecorderMode){
		//demo
		m_VirtualCard.VirCard_GetAcqFrame(alength , adelay);
	}
	else{
		//play
	}
	
}

void CProduct::SetAcqTrigger(int tmode, int tLevelUpperLimit,int tLevelLowerLimit)
{
	if (0 == m_nRecorderMode){
		//PhyCard
		m_PhyCard.PhyCard_SetAcqTrigger(tmode , tLevelUpperLimit , tLevelLowerLimit);
	}
	else if (1 == m_nRecorderMode){
		//demo
		m_VirtualCard.VirCard_SetAcqTrigger(tmode , tLevelUpperLimit , tLevelLowerLimit);
	}
	else{
		//play
	}
	
}

void CProduct::GetAcqTrigger(int *tmode, int *tch,int *tlevel)
{
	if (0 == m_nRecorderMode){
		//PhyCard
	}
	else if (1 == m_nRecorderMode){
		//demo
		m_VirtualCard.VirCard_GetAcqTrigger(tmode , tch	, tlevel);
	}
	else{
		//play
	}
	
}

void CProduct::SetChannelParam(int chidx, int rgidx,int cpl)
{
	if (0 == m_nRecorderMode){
		//PhyCard
	}
	else if (1 == m_nRecorderMode){
		//demo
		m_VirtualCard.VirCard_SetChannelParam(chidx , rgidx , cpl);
	}
	else{
		//play
	}
	
}

void CProduct::GetChannelParam(int *chidx, int *rgidx,int *cpl)
{
	if (0 == m_nRecorderMode){
		//PhyCard
	}
	else if (1 == m_nRecorderMode){
		//demo
		m_VirtualCard.VirCard_GetChannelParam(chidx , rgidx , cpl);
	}
	else{
		//play
	}
	
}


void CProduct::Acq(void)
{
	if (0 == m_nRecorderMode){
		//PhyCard
		m_PhyCard.PhyCard_Acq();
	}
	else if (1 == m_nRecorderMode){
		//demo
		m_VirtualCard.VirCard_Acq();
	}
	else{
		//play
	}
	
}

void CProduct::StopAcq(void)
{
	if (0 == m_nRecorderMode){
		//PhyCard
	}
	else if (1 == m_nRecorderMode){
		//demo
		m_VirtualCard.VirCard_StopAcq();
	}
	else{
		//play
	}
	
}

int	CProduct::StatusCheck(void)
{
	if (0 == m_nRecorderMode){
		//PhyCard
	}
	else if (1 == m_nRecorderMode){
		//demo
		m_VirtualCard.VirCard_StatusCheck();
	}
	else{
		//play
	}
	
	return 0;
}

void CProduct::Set_AI()
{
	SetAcqRate((double)m_AcqInfo.samplingRate, 0);		//���ò�����(������,reserved)
	SetAcqFrame(m_AcqInfo.samplingLength, 0);			//���ò���֡��Ϣ(��������<����>,����봥���źŵ���ʱֵ)
	SetAcqTrigger(m_AcqInfo.trigMode, m_AcqInfo.trigUpperLimit , m_AcqInfo.trigLowerLimit);		//���ô�������(����ģʽ,��������<ͨ��������Ч>,��������<ͨ��������Ч>)
//	SetChannelParam(int chidx, int rgidx,int cpl);		//����ͨ�����Ʋ���(ͨ����,ͨ�����̴���<range_tab����>,���ģʽ)

}

void CProduct::Get_AI()
{

}

int CProduct::GetACQStatus()
{
	if (0 == m_nRecorderMode){
		//PhyCard
		return m_PhyCard.PhyCard_StatusCheck();
	}
	else if (1 == m_nRecorderMode){
		//demo
		return m_VirtualCard.VirCard_StatusCheck();
	}
	else{
		//play
	}
	
	return 0;
}


DWORD WINAPI TransCtrlProc(  LPVOID lpParameter  )
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CProduct* pProduct = (CProduct*)(lpParameter);

	int nCount = 0 ;
	int i=0;	
	long nCardIndex = 0;
	
	/*���������ɼ������ȰѲɼ���������*/
	if ( pProduct->m_nRecorderMode == 0 )
	{
		for(nCardIndex=0 ; nCardIndex<pProduct->m_PhyCard.m_nPhyCardNum ; nCardIndex++)
		{
			WCDPcie_StopAD(nCardIndex);
		}
	}
	
	while ( pProduct->m_bTransing )
	{
		//ÿ�ν�����ͨ��ÿ��ͨ����10K����д��Ӳ��
		if (pProduct->m_nRecorderMode == 1){
			//demo
			for(nCount=0 ; nCount<pProduct->m_nOpenChannelNum ; nCount++)
			{
				//ȡ�� ������ͨ���� Segment�� -> pProduct->m_BufferData
				pProduct->Pack(	nCount+1 , 
					pProduct->m_BufferData,
					-1,
					1,
					pProduct->m_nSegmentValidIndex,
					10240);
				//д��Ӳ��
				for (i=0 ; i<10240 ; i++)
				{
					fprintf(pProduct->m_pArrayWriteFile[nCount+1],"%6d\n",pProduct->m_BufferData[i]);
				}
				fflush(pProduct->m_pArrayWriteFile[nCount+1]);
			}
		}
		else{
			//physical
			nCardIndex = 0;
		
			for(nCardIndex=0 ; nCardIndex<pProduct->m_PhyCard.m_nPhyCardNum ; nCardIndex++)
			{
				WCDPcie_ReceiveReadDMA(nCardIndex , 10240*CH_NUM_IN_PER_CARD*2 , pProduct->m_BufferDataForPhy[nCardIndex]);
			}
			long j=0;
			for(j=0;j<10240;j++)
			{
				for(long CardID=0 ; CardID<pProduct->m_PhyCard.m_nPhyCardNum ; CardID++)
				{
					pProduct->m_ShortData[CardID*CH_NUM_IN_PER_CARD+0][j] = pProduct->m_BufferDataForPhy[CardID][(j*CH_NUM_IN_PER_CARD+0)*2] + (pProduct->m_BufferDataForPhy[CardID][(j*CH_NUM_IN_PER_CARD+0)*2+1]<<8);
					pProduct->m_ShortData[CardID*CH_NUM_IN_PER_CARD+1][j] = pProduct->m_BufferDataForPhy[CardID][(j*CH_NUM_IN_PER_CARD+1)*2] + (pProduct->m_BufferDataForPhy[CardID][(j*CH_NUM_IN_PER_CARD+1)*2+1]<<8);
					pProduct->m_ShortData[CardID*CH_NUM_IN_PER_CARD+2][j] = pProduct->m_BufferDataForPhy[CardID][(j*CH_NUM_IN_PER_CARD+2)*2] + (pProduct->m_BufferDataForPhy[CardID][(j*CH_NUM_IN_PER_CARD+2)*2+1]<<8);
					pProduct->m_ShortData[CardID*CH_NUM_IN_PER_CARD+3][j] = pProduct->m_BufferDataForPhy[CardID][(j*CH_NUM_IN_PER_CARD+3)*2] + (pProduct->m_BufferDataForPhy[CardID][(j*CH_NUM_IN_PER_CARD+3)*2+1]<<8);
					pProduct->m_ShortData[CardID*CH_NUM_IN_PER_CARD+4][j] = pProduct->m_BufferDataForPhy[CardID][(j*CH_NUM_IN_PER_CARD+4)*2] + (pProduct->m_BufferDataForPhy[CardID][(j*CH_NUM_IN_PER_CARD+4)*2+1]<<8);
					pProduct->m_ShortData[CardID*CH_NUM_IN_PER_CARD+5][j] = pProduct->m_BufferDataForPhy[CardID][(j*CH_NUM_IN_PER_CARD+5)*2] + (pProduct->m_BufferDataForPhy[CardID][(j*CH_NUM_IN_PER_CARD+5)*2+1]<<8);
					pProduct->m_ShortData[CardID*CH_NUM_IN_PER_CARD+6][j] = pProduct->m_BufferDataForPhy[CardID][(j*CH_NUM_IN_PER_CARD+6)*2] + (pProduct->m_BufferDataForPhy[CardID][(j*CH_NUM_IN_PER_CARD+6)*2+1]<<8);
					pProduct->m_ShortData[CardID*CH_NUM_IN_PER_CARD+7][j] = pProduct->m_BufferDataForPhy[CardID][(j*CH_NUM_IN_PER_CARD+7)*2] + (pProduct->m_BufferDataForPhy[CardID][(j*CH_NUM_IN_PER_CARD+7)*2+1]<<8);
					
					
					fprintf(pProduct->m_pArrayWriteFile[CardID*CH_NUM_IN_PER_CARD+0+1] , "%6d\n" , (int)((pProduct->m_ShortData[CardID*CH_NUM_IN_PER_CARD+0][j]-32768.0)/32768*2200) );
					fprintf(pProduct->m_pArrayWriteFile[CardID*CH_NUM_IN_PER_CARD+1+1] , "%6d\n" , (int)((pProduct->m_ShortData[CardID*CH_NUM_IN_PER_CARD+1][j]-32768.0)/32768*2200) );
					fprintf(pProduct->m_pArrayWriteFile[CardID*CH_NUM_IN_PER_CARD+2+1] , "%6d\n" , (int)((pProduct->m_ShortData[CardID*CH_NUM_IN_PER_CARD+2][j]-32768.0)/32768*2200) );
					fprintf(pProduct->m_pArrayWriteFile[CardID*CH_NUM_IN_PER_CARD+3+1] , "%6d\n" , (int)((pProduct->m_ShortData[CardID*CH_NUM_IN_PER_CARD+3][j]-32768.0)/32768*2200) );
					fprintf(pProduct->m_pArrayWriteFile[CardID*CH_NUM_IN_PER_CARD+4+1] , "%6d\n" , (int)((pProduct->m_ShortData[CardID*CH_NUM_IN_PER_CARD+4][j]-32768.0)/32768*2200) );
					fprintf(pProduct->m_pArrayWriteFile[CardID*CH_NUM_IN_PER_CARD+5+1] , "%6d\n" , (int)((pProduct->m_ShortData[CardID*CH_NUM_IN_PER_CARD+5][j]-32768.0)/32768*2200) );
					fprintf(pProduct->m_pArrayWriteFile[CardID*CH_NUM_IN_PER_CARD+6+1] , "%6d\n" , (int)((pProduct->m_ShortData[CardID*CH_NUM_IN_PER_CARD+6][j]-32768.0)/32768*2200) );
					fprintf(pProduct->m_pArrayWriteFile[CardID*CH_NUM_IN_PER_CARD+7+1] , "%6d\n" , (int)((pProduct->m_ShortData[CardID*CH_NUM_IN_PER_CARD+7][j]-32768.0)/32768*2200) );
					
				}
				
			}
		}
		


		pProduct->m_nSegmentValidIndex = pProduct->m_nSegmentValidIndex + 1;
		g_TransAndPlayEvent.PulseEvent();

		//��������
		pFrame->SendMessage( WM_UPDATE_CACHE, pProduct->m_nSegmentValidIndex , 0 );

		if (pProduct->m_nSegmentValidIndex >= pProduct->m_nSegmentNum){
			//ֹͣ�˳��������
			pProduct->m_bTransing = false;
			break;
		}
	}

	return 0;
}


DWORD WINAPI PlayCtrlProc(  LPVOID lpParameter  )
{
	CProduct* pProduct = (CProduct*)(lpParameter);
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CRecorderView* pView = (CRecorderView*)(pFrame->GetActiveView());
	CDlgBarExBottomPlay* pDlgBarExBottomPlay = pFrame->m_pDlgBarExBottomPlay;

	pProduct->m_bExistData = true;

	int nSegmentPlayIndexAdd=0;

	while ( pProduct->m_bPlaying )
	{	
		//��ͣ�ȴ�
		WaitForSingleObject(g_PlayPauseEvent,INFINITE);
		if ( pProduct->m_bPlaying == false )
		{
			break;
		}

//		TRACE("-***-pProduct->m_nPlayStartIndex : %d------\n",pProduct->m_nPlayStartIndex);
//		TRACE("-***-pProduct->m_nPlayWndWidth : %d------\n",pProduct->m_nPlayWndWidth);
//		TRACE("-***-pProduct->m_AcqInfo.samplingLength : %d------\n",pProduct->m_AcqInfo.samplingLength);

		if (pProduct->m_nPlayStartIndex + pProduct->m_nPlayWndWidth > pProduct->m_AcqInfo.samplingLength)
		{
			//�Ѿ��������
			//����Ӧ�ý�����ͣ״̬ ��Ϊ�п����û���Ҫ�϶������лطŹ���
			//pDlgBarExBottomPlay->OnRADIOPause();
			g_PlayPauseEvent.ResetEvent();
			((CButton *)(pDlgBarExBottomPlay->GetDlgItem(IDC_RADIO_Play)))->SetCheck(false);
			((CButton *)(pDlgBarExBottomPlay->GetDlgItem(IDC_RADIO_Pause)))->SetCheck(true);
			
			pProduct->m_nPlayStartIndex = 0;
			pFrame->SendMessage(WM_UPDATE_PLAY , pProduct->m_nPlayStartIndex , 0);
			
			WaitForSingleObject(g_PlayPauseEvent,INFINITE);
			continue;
		}

		if (pProduct->m_nPlayStartIndex + pProduct->m_nPlayWndWidth > pProduct->m_nSegmentValidIndex*DATASTORAGE_SEGMENT)
		{
			//�����˵�ǰ��������������
			WaitForSingleObject(g_TransAndPlayEvent,INFINITE);
			continue;
		}




		pFrame->SendMessage(WM_UPDATE_PLAY , pProduct->m_nPlayStartIndex , 0);
		pView->WndManageRefresh();
		TRACE("pProduct->m_nPlayStartIndex : %d\n",pProduct->m_nPlayStartIndex);

		pProduct->m_nPlayStartIndex = pProduct->m_nPlayStartIndex + pProduct->m_PlayCtrlInfo.nSpeed;



		Sleep(100);

		
	}
	return 0;
}


void CProduct::OnAcqEnd()
{
	m_bAcqing = false;			//add in 20160716  ��ߺ�������ǰһ��û�ҵ������� ����ControlAcqProc��ѭ��

//	TRACE("CProduct::OnAcqEnd()\n");
	CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();
	pFrame->m_pDlgBarExBottomPlay->m_pPlayBar->SetRange(0 , m_AcqInfo.samplingLength);
	pFrame->m_pDlgBarExBottomPlay->m_pPlayBar->SetPlayWndWidth(DATASTORAGE_SEGMENT);		//���ڳ�ʼ��Ϊ10240


	g_TransAndPlayEvent.ResetEvent();
	g_PlayPauseEvent.ResetEvent();
	//�ı乤������ť״̬
	pFrame->m_aPlayCtrlStatus[0] = 0;
	pFrame->m_aPlayCtrlStatus[1] = 1;
	//��X��txt 1��Setup.inf
	OpenStorageDoc();
	WriteInSetupDoc();
	//�����߳�
	SECURITY_ATTRIBUTES saAttr;
	saAttr.nLength = sizeof( SECURITY_ATTRIBUTES );
	saAttr.bInheritHandle = TRUE; 
	saAttr.lpSecurityDescriptor = NULL; 
	DWORD ThreadId ;

	m_bTransing = true;
	VERIFY( CreateThread(&saAttr, 1000, TransCtrlProc, (void*)(this), 0, &ThreadId ) );


	//�����߳�
	m_bPlaying = true;
	VERIFY( CreateThread(&saAttr, 1000, PlayCtrlProc, (void*)(this), 0, &ThreadId ) );
}

void CProduct::OpenStorageDoc()
{
	//�ȹر�
	int nCount = 0;
	for (nCount=0 ; nCount<MAX_CHANNUM+1 ; nCount++)
	{
		if (NULL != m_pArrayWriteFile[nCount])
		{
			fclose(m_pArrayWriteFile[nCount]);
			m_pArrayWriteFile[nCount] = NULL;
		}
	}
	for (nCount=0 ; nCount<MAX_CHANNUM+1 ; nCount++)
	{
		if (NULL != m_pArrayReadFile[nCount])
		{
			fclose(m_pArrayReadFile[nCount]);
			m_pArrayReadFile[nCount] = NULL;
		}
	}
	//���´�
	CString strTemp;
	m_pArrayWriteFile[0] = fopen(m_strCurWorkPath + "\\Setup.inf" , "w+");

	for (nCount=0 ; nCount<m_nOpenChannelNum ; nCount++)
	{
		strTemp.Format("Ch%d",nCount+1);
		m_pArrayWriteFile[nCount+1] = fopen(m_strCurWorkPath + "\\" + strTemp, "w+");
	}
	for (nCount=0 ; nCount<m_nOpenChannelNum ; nCount++)
	{
		strTemp.Format("Ch%d",nCount+1);
		m_pArrayReadFile[nCount+1] = fopen(m_strCurWorkPath + "\\" + strTemp, "r");
	}

}

void CProduct::WriteInSetupDoc()
{
	if (NULL == m_pArrayWriteFile[0])
	{
		AfxMessageBox("Setup.inf�ļ������Ч");
	}

	//д������
////////////д���ʽ//////////////////////////////////////////////////////////////
//	0.�������
//	1.ͨ����
//	2.����
//	3.��������
//	4.��������
//	5.������ʽ
//	6.����ͨ��
//	7.������ƽ
//	8.������ʱ
	//////////////////////////////////////////////////////////////////////////
	fprintf(m_pArrayWriteFile[0],"RECORDER SETTING FILE\n");
	fprintf(m_pArrayWriteFile[0],"%d\n",m_nOpenChannelNum);
	fprintf(m_pArrayWriteFile[0],"%d\n",m_AcqInfo.range);
	fprintf(m_pArrayWriteFile[0],"%d\n",m_AcqInfo.samplingLength);
	fprintf(m_pArrayWriteFile[0],"%d\n",m_AcqInfo.samplingRate);
	fprintf(m_pArrayWriteFile[0],"%d\n",m_AcqInfo.trigMode);
	fprintf(m_pArrayWriteFile[0],"%d\n",m_AcqInfo.trigChan);
	fprintf(m_pArrayWriteFile[0],"%d\n",m_AcqInfo.trigLevel);
	fprintf(m_pArrayWriteFile[0],"%d\n",m_AcqInfo.samplingDelay);

	fclose(m_pArrayWriteFile[0]);
}

void CProduct::AcqEnvInitialize()
{
	
	CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();
	CRecorderView* pView= (CRecorderView*)(pFrame->GetActiveView());
//	CRecorderDoc *pDoc = (CRecorderDoc*)(pFrame->GetActiveDocument());
//	CProduct* pProduct = &(pDoc->m_Product);
	CWndManage* pWndMange = &(pView->m_WndManage);	
	WavePlayCtrl* pWPC = &(pWndMange->m_WPCUnified);
	//��������ϴβ�����Ϣ ���������̣߳�PlayBar

	m_bAcqing = false;
	m_bTransing = false;
	m_bPlaying = false;

	//����ͬ��500MS ��Ϊ����PlayPro��TransPro�ߵ���WaitForSingleObject�ĵط�	
	Sleep(500);
	g_PlayPauseEvent.SetEvent();
	g_TransAndPlayEvent.SetEvent();
	Sleep(500);

	m_nSegmentPlayIndex = 0;

	m_nSegmentValidIndex = 0;
	m_nPlayStartIndex = 0;
	m_nPlayWndWidth = DATASTORAGE_SEGMENT;
//	pWPC->fRatioX = DATASTORAGE_SEGMENT / 


	pFrame->m_pDlgBarExBottomPlay->m_pPlayBar->SetCachePos(0);
	pFrame->m_pDlgBarExBottomPlay->m_pPlayBar->SetPlayPos(0);
	pFrame->m_pDlgBarExBottomPlay->m_pPlayBar->SetPlayWndWidth(DATASTORAGE_SEGMENT);
	

	((CButton*)(pFrame->m_pDlgBarExBottomPlay->GetDlgItem(IDC_RADIO_Play)))->SetCheck(false);
	((CButton*)(pFrame->m_pDlgBarExBottomPlay->GetDlgItem(IDC_RADIO_Pause)))->SetCheck(TRUE);
	pFrame->SendMessage( WM_UPDATE_CACHE, m_nSegmentValidIndex , 0 );
	pFrame->SendMessage( WM_UPDATE_PLAY , m_nPlayStartIndex , 0);

	int nCount = 0;
	for (nCount=0 ; nCount<MAX_CHANNUM ; nCount++)
	{
		if (NULL != m_pArrayWriteFile[nCount+1])
		{
			fclose(m_pArrayWriteFile[nCount+1]);
			m_pArrayWriteFile[nCount+1] = NULL;
		}
	}
	for (nCount=0 ; nCount<MAX_CHANNUM ; nCount++)
	{
		if (NULL != m_pArrayReadFile[nCount+1])
		{
			fclose(m_pArrayReadFile[nCount+1]);
			m_pArrayReadFile[nCount+1] = NULL;
		}
	}

}

//nPointStartPos = nSegmentPlayPos*10240 + nOffsetInSegment
//nChNum�ǻ���0��
void CProduct::GetDataFromHDD(int nChNum, long nPointStartPos, int nInterval, short *pDataBuffer, int nDataLen)
{
	CString str;
	int nCount;
	char line[100];
	//*8����Ϊÿ������6���ֽڼ��ϻس�2���ֽ�һ��8���ֽ�

	//���if������������ɼ���ťʱ������m_pArrayReadFile�ᱻ�رն�������Э����
	if (	NULL == m_pArrayReadFile[nChNum+1]	)
	{
		for (nCount=0 ; nCount<nDataLen ; nCount++)
		{
			*(pDataBuffer+nCount) = 0;				
		}
		return;
	}


	fseek(m_pArrayReadFile[nChNum+1], nPointStartPos*8 , SEEK_SET);	
	for (nCount=0 ; nCount<nDataLen ; nCount++)
	{
		fgets( line, 100, m_pArrayReadFile[nChNum+1] );    
		str.Format("%s", line);
		str.TrimLeft();
		str.TrimRight();

		*(pDataBuffer+nCount) = atoi(str);

		fseek(m_pArrayReadFile[nChNum+1], (nInterval-1)*8 , SEEK_CUR);
	}
}

void CProduct::PlayEnvInitialize()
{
	Initialize();
	//д�빤����
	
	//��Trans��ֵ��������
	m_nSegmentNum = (int)(ceil((double)m_AcqInfo.samplingLength / (double)DATASTORAGE_SEGMENT)) ;
//	m_nSegmentPlayIndex = 0;
	m_nSegmentValidIndex = m_nSegmentNum;
	m_nPlayStartIndex = 0;	


	CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();
	pFrame->m_pDlgBarExBottomPlay->m_pPlayBar->SetRange(0 , m_AcqInfo.samplingLength);
	pFrame->m_pDlgBarExBottomPlay->m_pPlayBar->SetPlayWndWidth(DATASTORAGE_SEGMENT);		//���ڳ�ʼ��Ϊ10240
	pFrame->m_pDlgBarExBottomPlay->m_pPlayBar->SetCachePos(m_nSegmentValidIndex);
	pFrame->m_pDlgBarExBottomPlay->m_pPlayBar->SetPlayPos(m_nPlayStartIndex);


	g_TransAndPlayEvent.ResetEvent();
	g_PlayPauseEvent.ResetEvent();
	//�ı乤������ť״̬
	pFrame->m_aPlayCtrlStatus[0] = 0;
	pFrame->m_aPlayCtrlStatus[1] = 1;

	
	//��cproduct�еĶ��ļ����
	int nCount;
	for (nCount=0 ; nCount<MAX_CHANNUM+1 ; nCount++)
	{
		if (NULL != m_pArrayReadFile[nCount])
		{
			fclose(m_pArrayReadFile[nCount]);
			m_pArrayReadFile[nCount] = NULL;
		}
	}
	CString strTemp;
	for (nCount=0 ; nCount<m_nOpenChannelNum ; nCount++)
	{
		strTemp.Format("Ch%d",nCount+1);
		m_pArrayReadFile[nCount+1] = fopen(m_strCurWorkPath + "\\" + strTemp, "r");
	}

	//�𲥷��߳�
	SECURITY_ATTRIBUTES saAttr;
	saAttr.nLength = sizeof( SECURITY_ATTRIBUTES );
	saAttr.bInheritHandle = TRUE; 
	saAttr.lpSecurityDescriptor = NULL; 
	DWORD ThreadId ;

	m_bPlaying = true;
	VERIFY( CreateThread(&saAttr, 1000, PlayCtrlProc, (void*)(this), 0, &ThreadId ) );


}

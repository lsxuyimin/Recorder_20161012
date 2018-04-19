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
	g_nScreenWidth = 4096;						//先将每次生成的数据量设到最大
	g_nScreenHeight= 10000; 

	m_nOpenChannelNum = 0;
	m_nCombWaveNum = 0;

	m_AcqInfo.range=10;											//初始量程 10v	
	m_AcqInfo.samplingLength = 40960;							//初始采集长度 40960	指的是每个通道的采样点
	m_AcqInfo.samplingRate = 1000000;							//初始采样频率1M
	m_AcqInfo.trigChan = 0;
	m_AcqInfo.trigLevel = 0;
	m_AcqInfo.trigMode = TRIMODE_MANUAL;
	m_AcqInfo.samplingDelay = 0;								//采样延时 ms
	m_AcqInfo.trigLowerLimit = 17873;
	m_AcqInfo.trigUpperLimit = 47663;

	/*初始化当前的工作路径m_strWorkPath*/
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

	/*初始化m_pArrayFile*/
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

	/*初始化m_FftConf*/
	m_FftConf.nFftBins = 2048;
	m_FftConf.nWindowMode = WINFUN_RECT;
	m_FftConf.nOutputMode = YAXIS_UNIT_V;
	m_FftConf.nImpedance = 500;

	/*新建*/
	m_pFftModule = new FftComplex(); 

}

CProduct::~CProduct()
{
	delete m_pFftModule;
	m_pFftModule = NULL;

	if (m_WaveList.IsEmpty())
	{
		//m_WaveList列表 为空
	} 
	else
	{
		//m_WaveList列表 不为空  进行清空
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
		//进行判断 做三种处理
		switch (m_nRecorderMode)
		{
		case 0:
			//物理卡
			Initialize();
			break;
		case 1:
			//模拟卡
			Initialize();
			break;
		case 2:
			//播放已经存在文件
			PlayEnvInitialize();
			break;
		}
	}
	
	return 1;
}

void CProduct::Initialize()
{
	//1.初始化CProduct中的数据
	DataInitialize(m_nOpenChannelNum);
	//2.初始化函数指针
//	FunPtrInitialize(m_nRecorderMode);
	//3.初始化View
	m_pRecView->ViewInitialize(m_nRecorderMode);
	//初始化完成后进行默认设定
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
				//随机波形
				//pWave->m_WaveData.SetAt(j,rand()%10000-5000);						//存入数字 0~100 之间
				//随机正弦波
				int save = (int)(5000* sin((j%200-100.0)/100*2*3.14+randomNum));
				pWave->m_WaveData.SetAt(j, (int)(5000* sin((j%200-100.0)/100*2*3.14+randomNum)));
				
			}
		}
		//每次循环休眠50ms  这里希望每50ms产生一组数据
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
		if ( tAcqStatus == REC_STA_ACQEND )		// 判断采集是否结束
		{
			//pProduct->m_iDatFlg = ACQDAT;
			pFrame->SendMessage( WM_ACQEND, 0, 0 );
			pProduct->OnAcqEnd();
			break;
		}
		else if (tAcqStatus == REC_STA_INACQ){	
			//判断采集
			pFrame->SendMessage( WM_INACQ, 0, 0 );
		}
		else if (tAcqStatus == REC_STA_WAITTRG){
			//判断等待
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
	//首先应该清除所有上次采样信息 包括所有线程，PlayBar，关闭128个文件句柄
	AcqEnvInitialize();

	//修改数据存储路径
	CStoragePathDlg stoPathDlg;
	stoPathDlg.DoModal();

	//发送设置参数
	Set_AI();

	//计算SegmentNum
/*
	if (m_nRecorderMode == 0)		//TEST
	{
		m_AcqInfo.samplingLength = 1024*1024*10;
	}
*/	m_nSegmentNum = (int)(ceil((double)m_AcqInfo.samplingLength / (double)DATASTORAGE_SEGMENT)) ;
	m_nSegmentPlayIndex = 0;
	m_nSegmentValidIndex = 0;
	m_nPlayStartIndex = 0;	

	//控制线程 等待采集->采集中->打开存储文件以及Setup.inf->数据缓存
	SECURITY_ATTRIBUTES saAttr;
	saAttr.nLength = sizeof( SECURITY_ATTRIBUTES );
	saAttr.bInheritHandle = TRUE; 
	saAttr.lpSecurityDescriptor = NULL; 
	DWORD ThreadId ;
	
	m_bAcqing = TRUE;

	Acq();

	VERIFY( CreateThread(&saAttr, 1000, ControlAcqProc, (void *)(this), 0, &ThreadId ) );
	//该线程若是结束 即打开传输线程,播放线程

	
/*
	if (1 == m_nRecorderMode)
	{
		//demo 模式
		//随机波形
		//产生64组波形  存入m_WaveList
		
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
		//demo 模式
		//停止产生随机波形
		
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
		//demo 模式
	
		
	} 
	else if (0 == m_nRecorderMode)
	{
		//PhyCard
	}
}


int	CProduct::Pack(int chNum, short *pData, int mode, int interval, long start, long dataLen)	//读取缓冲区数据(通道号,数据指针,重采样模式,压缩间隔,采样起点,读取数据长度)
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
	SetAcqRate((double)m_AcqInfo.samplingRate, 0);		//设置采样率(采样率,reserved)
	SetAcqFrame(m_AcqInfo.samplingLength, 0);			//设置采样帧信息(采样长度<样点>,相对与触发信号的延时值)
	SetAcqTrigger(m_AcqInfo.trigMode, m_AcqInfo.trigUpperLimit , m_AcqInfo.trigLowerLimit);		//设置触发条件(触发模式,触发上限<通道触发有效>,触发下限<通道触发有效>)
//	SetChannelParam(int chidx, int rgidx,int cpl);		//设置通道控制参数(通道号,通道量程代号<range_tab索引>,耦合模式)

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
	
	/*如果是物理采集卡，先把采集卡都关了*/
	if ( pProduct->m_nRecorderMode == 0 )
	{
		for(nCardIndex=0 ; nCardIndex<pProduct->m_PhyCard.m_nPhyCardNum ; nCardIndex++)
		{
			WCDPcie_StopAD(nCardIndex);
		}
	}
	
	while ( pProduct->m_bTransing )
	{
		//每次将所有通道每个通道的10K个点写入硬盘
		if (pProduct->m_nRecorderMode == 1){
			//demo
			for(nCount=0 ; nCount<pProduct->m_nOpenChannelNum ; nCount++)
			{
				//取数 参数：通道号 Segment号 -> pProduct->m_BufferData
				pProduct->Pack(	nCount+1 , 
					pProduct->m_BufferData,
					-1,
					1,
					pProduct->m_nSegmentValidIndex,
					10240);
				//写入硬盘
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

		//画进度条
		pFrame->SendMessage( WM_UPDATE_CACHE, pProduct->m_nSegmentValidIndex , 0 );

		if (pProduct->m_nSegmentValidIndex >= pProduct->m_nSegmentNum){
			//停止退出传输进程
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
		//暂停等待
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
			//已经播放完毕
			//这里应该进入暂停状态 因为有可能用户还要拖动光标进行回放过程
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
			//超过了当前缓存下来的数据
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
	m_bAcqing = false;			//add in 20160716  这边好像是以前一个没找到的问题 导致ControlAcqProc死循环

//	TRACE("CProduct::OnAcqEnd()\n");
	CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();
	pFrame->m_pDlgBarExBottomPlay->m_pPlayBar->SetRange(0 , m_AcqInfo.samplingLength);
	pFrame->m_pDlgBarExBottomPlay->m_pPlayBar->SetPlayWndWidth(DATASTORAGE_SEGMENT);		//窗口初始化为10240


	g_TransAndPlayEvent.ResetEvent();
	g_PlayPauseEvent.ResetEvent();
	//改变工具栏按钮状态
	pFrame->m_aPlayCtrlStatus[0] = 0;
	pFrame->m_aPlayCtrlStatus[1] = 1;
	//打开X个txt 1个Setup.inf
	OpenStorageDoc();
	WriteInSetupDoc();
	//传输线程
	SECURITY_ATTRIBUTES saAttr;
	saAttr.nLength = sizeof( SECURITY_ATTRIBUTES );
	saAttr.bInheritHandle = TRUE; 
	saAttr.lpSecurityDescriptor = NULL; 
	DWORD ThreadId ;

	m_bTransing = true;
	VERIFY( CreateThread(&saAttr, 1000, TransCtrlProc, (void*)(this), 0, &ThreadId ) );


	//播放线程
	m_bPlaying = true;
	VERIFY( CreateThread(&saAttr, 1000, PlayCtrlProc, (void*)(this), 0, &ThreadId ) );
}

void CProduct::OpenStorageDoc()
{
	//先关闭
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
	//重新打开
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
		AfxMessageBox("Setup.inf文件句柄无效");
	}

	//写入设置
////////////写入格式//////////////////////////////////////////////////////////////
//	0.检查用行
//	1.通道数
//	2.量程
//	3.采样长度
//	4.采样速率
//	5.触发方式
//	6.触发通道
//	7.触发电平
//	8.采样延时
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
	//清除所有上次采样信息 包括所有线程，PlayBar

	m_bAcqing = false;
	m_bTransing = false;
	m_bPlaying = false;

	//这里同步500MS 是为了让PlayPro和TransPro走到有WaitForSingleObject的地方	
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
//nChNum是基于0的
void CProduct::GetDataFromHDD(int nChNum, long nPointStartPos, int nInterval, short *pDataBuffer, int nDataLen)
{
	CString str;
	int nCount;
	char line[100];
	//*8是因为每个数据6个字节加上回车2个字节一共8个字节

	//这个if是在连续点击采集按钮时，由于m_pArrayReadFile会被关闭而做的妥协方法
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
	//写入工具栏
	
	//将Trans的值都设置下
	m_nSegmentNum = (int)(ceil((double)m_AcqInfo.samplingLength / (double)DATASTORAGE_SEGMENT)) ;
//	m_nSegmentPlayIndex = 0;
	m_nSegmentValidIndex = m_nSegmentNum;
	m_nPlayStartIndex = 0;	


	CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();
	pFrame->m_pDlgBarExBottomPlay->m_pPlayBar->SetRange(0 , m_AcqInfo.samplingLength);
	pFrame->m_pDlgBarExBottomPlay->m_pPlayBar->SetPlayWndWidth(DATASTORAGE_SEGMENT);		//窗口初始化为10240
	pFrame->m_pDlgBarExBottomPlay->m_pPlayBar->SetCachePos(m_nSegmentValidIndex);
	pFrame->m_pDlgBarExBottomPlay->m_pPlayBar->SetPlayPos(m_nPlayStartIndex);


	g_TransAndPlayEvent.ResetEvent();
	g_PlayPauseEvent.ResetEvent();
	//改变工具栏按钮状态
	pFrame->m_aPlayCtrlStatus[0] = 0;
	pFrame->m_aPlayCtrlStatus[1] = 1;

	
	//打开cproduct中的读文件句柄
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

	//起播放线程
	SECURITY_ATTRIBUTES saAttr;
	saAttr.nLength = sizeof( SECURITY_ATTRIBUTES );
	saAttr.bInheritHandle = TRUE; 
	saAttr.lpSecurityDescriptor = NULL; 
	DWORD ThreadId ;

	m_bPlaying = true;
	VERIFY( CreateThread(&saAttr, 1000, PlayCtrlProc, (void*)(this), 0, &ThreadId ) );


}

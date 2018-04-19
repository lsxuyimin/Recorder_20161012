// PhyCard.cpp: implementation of the CPhyCard class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "recorder.h"
#include "PhyCard.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
#include "WCDPcie//WCDPcieDriver.h"


#include "Product.h"
#include "RecorderDoc.h"
#include "MainFrm.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPhyCard::CPhyCard()
{
	m_PhyCardAcqInfo.range = 0;
	m_PhyCardAcqInfo.samplingLength = 0;
	m_PhyCardAcqInfo.samplingRate = 0;
	m_PhyCardAcqInfo.samplingTime = 0;
	m_PhyCardAcqInfo.trigMode = 0;
	m_PhyCardAcqInfo.samplingDelay = 0;
	m_PhyCardAcqInfo.trigChan = 0;
	m_PhyCardAcqInfo.trigLevel = 0;
	
	m_bPhyCardAcqing = false;
	
	m_nPhyCardAcqStatus = REC_STA_IDLE;

	m_nPhyCardNum = 0 ;
	m_nOpenChannelNum = 0 ;

	bTestFlag = FALSE;
	m_Data=new unsigned char[1024*1024*40];
}

CPhyCard::~CPhyCard()
{

}

DWORD WINAPI ChangePhyCardStaProc(  LPVOID lpParameter  )
{
	TRACE("im here!!!!!!!!!!111\n");
	CPhyCard* pPhyCard = (CPhyCard*)(lpParameter);
	
/*
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	TRACE("im here!!!!!!!!!!222\n");
	CRecorderDoc *pDoc = (CRecorderDoc*)(pFrame->GetActiveDocument());
	TRACE("im here!!!!!!!!!!333\n");
	CProduct* pProduct = &(pDoc->m_Product);
	TRACE("im here!!!!!!!!!!444\n");
*/

	pPhyCard->m_nPhyCardAcqStatus = REC_STA_WAITTRG;

/*
	while(pProduct->m_bAcqing == true)
	{
		int nStatus = pPhyCard->PhyCard_TrigStatusCheck();
		pPhyCard->m_nPhyCardAcqStatus = nStatus;
		Sleep(1000);
	}
*/
	int nStatus = REC_STA_WAITTRG;
	while( nStatus != REC_STA_ACQEND)
	{
		nStatus = pPhyCard->PhyCard_TrigStatusCheck();
		pPhyCard->m_nPhyCardAcqStatus = nStatus;
		Sleep(1000);
	}


	
	return 0;
}

//读取缓冲区数据(通道号,数据指针,重采样模式,压缩间隔,采样起点,读取数据长度)
//通道号 基于1
//取数 参数：通道号 Segment号 -> pProduct->m_BufferData
//pProduct->Pack(	nCount+1 , 
//			   pProduct->m_BufferData,
//			   -1,
//			   1,
//			   pProduct->m_nSegmentValidIndex,
//							10240);

int		__stdcall CPhyCard::PhyCard_Pack(int nChNum, short* pDataBuffer, int nMode, int nInterval, long nSegmentStartPos, long nDataLen)
{
/*
	if (bTestFlag == FALSE)
	{
		//cai
		WCDPcie_ReceiveReadDMA(1*1024*1024,m_Data);
		bTestFlag = TRUE;
	}
*/



	return 0;
}

double	__stdcall CPhyCard::PhyCard_SetAcqRate(double arate, double amrate)
{
	long nCardIndex=0 ;
	for ( nCardIndex=0 ; nCardIndex<m_nPhyCardNum ; nCardIndex++){
		WCDPcie_SetCardSampleFrequent(nCardIndex , 100000000);			//TEST 采样频率固定死了
	}
	
	return 0;
}

void	__stdcall CPhyCard::PhyCard_GetAcqRate(double *arate, double *amrate)
{
	
}

void	__stdcall CPhyCard::PhyCard_SetAcqFrame(int alength, int adelay)
{
	//入参的采样长度是以采样点数计数的，但是提供驱动的采样长度是以char计数的，所以要乘上每张卡含有的通道数，以及一个采样点占用的char位数
		
	long nCardIndex=0 ;
	for ( nCardIndex=0 ; nCardIndex<m_nPhyCardNum ; nCardIndex++){
		WCDPcie_SetADSampleLen(nCardIndex , alength * CH_NUM_IN_PER_CARD * 2);	
	}
}

void	__stdcall CPhyCard::PhyCard_GetAcqFrame(int *alength, int *adelay)
{
	
}
	
void	__stdcall CPhyCard::PhyCard_SetAcqTrigger(int tmode, int tLevelUpperLimit,int tLevelLowerLimit)
{
	//循环设置所有卡
	unsigned long TriggerType=0;//0立即出发，6，外部连续触发 ，7 外部高电压控制出发
	if (tmode == TRIMODE_MANUAL){
		TriggerType=0;
	} 
	else if ( tmode == TRIMODE_CHANNEL ){	
		TriggerType=15;
	}
	unsigned long PreTriggerLen=0;
	unsigned long TriggerVoltMax=tLevelUpperLimit;			//对应tLevelUpperLimit
	unsigned long TriggerVoltMin=tLevelLowerLimit;			//对应tLevelLowerLimit
	unsigned long ContinueLen=2000;
	unsigned long ContinueStep=2000;
	unsigned long ContinueTimes=40000;

	int nCardIndex = 0;
	for ( nCardIndex=0 ; nCardIndex<m_nPhyCardNum ; nCardIndex++){
		WCDPcie_SetADParamter(nCardIndex,TriggerType,PreTriggerLen,TriggerVoltMax,TriggerVoltMin,0xFF,ContinueLen,ContinueStep,ContinueTimes);
		WCDPcie_SetC0C1Ctl(nCardIndex,0,0,1);
		WCDPcie_Reset_AD_DDR(nCardIndex);	
		Sleep(10);
		WCDPcie_SetC0C1Ctl(nCardIndex,1,1,1);
		WCDPcie_InitReadMem(nCardIndex,0,2,10*1024*CH_NUM_IN_PER_CARD*2,0,NULL,NULL);	//每次拿10k个采样点，每张卡8个通道，一个采样点数据占2个char位
	}
	
}


void	__stdcall CPhyCard::PhyCard_GetAcqTrigger(int *tmode, int *tch,int *tlevel)
{
	
}

void	__stdcall CPhyCard::PhyCard_SetChannelParam(int chidx, int rgidx,int cpl)
{
	
}

void	__stdcall CPhyCard::PhyCard_GetChannelParam(int *chidx, int *rgidx,int *cpl)
{
	
}
	

void	__stdcall CPhyCard::PhyCard_Acq(void)
{
	SECURITY_ATTRIBUTES saAttr;
	saAttr.nLength = sizeof( SECURITY_ATTRIBUTES );
	saAttr.bInheritHandle = TRUE; 
	saAttr.lpSecurityDescriptor = NULL; 
	DWORD ThreadId ;
	VERIFY( CreateThread(&saAttr, 1000, ChangePhyCardStaProc, (void *)(this), 0, &ThreadId ) );


	long nCardIndex = 0;
	for(nCardIndex=0 ; nCardIndex<m_nPhyCardNum ; nCardIndex++)
	{
		WCDPcie_StartAD(nCardIndex);
	}


}

void	__stdcall CPhyCard::PhyCard_StopAcq(void)
{
	
}

int		__stdcall CPhyCard::PhyCard_StatusCheck(void)
{
	return m_nPhyCardAcqStatus;
//	return 0;
}	



void CPhyCard::PhyCard_Initialize(int nChnNum)
{
	int nCount = 0;
	for (nCount=0 ; nCount<m_nPhyCardNum ; nCount++)
	{
		WCDPcie_OpenDevice(0x10ee,0x0007,m_BusIDArray[nCount],m_SlotIDArray[nCount]);
	}
}

int CPhyCard::PhyCard_TrigStatusCheck()
{
	//检测当前的采集状态  0:等待触发 1:正在采集 2:采集完毕
	unsigned long TriggerOK;
	unsigned long SoftTriggerOK;
	unsigned long CHTriggerOK;
	unsigned long OutTriggerOK;
	unsigned long RealSampleFrequent;
	unsigned long DDRCHLenthC0;
	unsigned long DDRCHLenthC1;
	
	unsigned long nTrigStatus[8] = {0,0,0,0,0,0,0,0}; 
	unsigned long nSingleTrigStatus = 0;

	//联合采集测试
	for(long nCardIndex=0;nCardIndex<m_nPhyCardNum;nCardIndex++)
	{
		WCDPcie_GetADStatus(nCardIndex,TriggerOK,SoftTriggerOK,CHTriggerOK,OutTriggerOK);	
		nTrigStatus[nCardIndex] = TriggerOK+SoftTriggerOK+CHTriggerOK+OutTriggerOK;
	}
	
	int nSumStatus = 0 ;
	for(nCardIndex=0;nCardIndex<m_nPhyCardNum;nCardIndex++)
	{
		if ( 0 != nTrigStatus[nCardIndex])
		{
			nSumStatus = nSumStatus+1;
		}
	}
	
	if (m_nPhyCardNum == nSumStatus )
	{
		//全部触发 采集完成
		Sleep(1000);
		return REC_STA_ACQEND;
	}
	else if (0 == nSumStatus )
	{
		//没触发 等待触发
		return REC_STA_WAITTRG;
	}
	else if (m_nPhyCardNum > nSumStatus )
	{
		//部分触发 正在采集
		return REC_STA_INACQ;
	}
}

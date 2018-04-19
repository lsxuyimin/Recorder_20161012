// VirtualCard.cpp: implementation of the CVirtualCard class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "recorder.h"
#include "VirtualCard.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////////
#include "Product.h"
#include <math.h>
#include "RecorderDoc.h"
#include "MainFrm.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CVirtualCard::CVirtualCard()
{
	m_VirCardAcqInfo.range = 0;
	m_VirCardAcqInfo.samplingLength = 0;
	m_VirCardAcqInfo.samplingRate = 0;
	m_VirCardAcqInfo.samplingTime = 0;
	m_VirCardAcqInfo.trigMode = 0;
	m_VirCardAcqInfo.samplingDelay = 0;
	m_VirCardAcqInfo.trigChan = 0;
	m_VirCardAcqInfo.trigLevel = 0;

	m_bVirCardAcqing = false;

	m_nVirCardAcqStatus = REC_STA_IDLE;

	
}

CVirtualCard::~CVirtualCard()
{

}


DWORD WINAPI ChangeVirCardStaProc(  LPVOID lpParameter  )
{
//	TRACE("im here!!!!!!!!!!\n");
	CVirtualCard* pVirtualCard = (CVirtualCard*)(lpParameter);
	pVirtualCard->m_nVirCardAcqStatus = REC_STA_WAITTRG;
	Sleep(1000);	
	pVirtualCard->m_nVirCardAcqStatus = REC_STA_INACQ;
	//可以在这里准备数据
	Sleep(1000);
	pVirtualCard->m_nVirCardAcqStatus = REC_STA_ACQEND;
	
	
	return 0;
}

//读取缓冲区数据(通道号,数据指针,重采样模式,压缩间隔,采样起点,读取数据长度)
//通道号 基于1
int	__stdcall CVirtualCard::VirCard_Pack(int nChNum, short* pDataBuffer, int nMode, int nInterval, long nSegmentStartPos, long nDataLen)
{
/*
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CRecorderDoc *pDoc = (CRecorderDoc*)(pFrame->GetActiveDocument());
	CProduct* pProduct = &(pDoc->m_Product);
*/

	long nPointStartPos = nSegmentStartPos * DATASTORAGE_SEGMENT;
	
	long nCount=0;
	short nData;
	float fRatio;
	int nAmp;
	float fPointInCycle;		//一个周期内的点数 有可能小于0（当采样率大于波形频率时）
	fPointInCycle = m_VirCardAcqInfo.samplingRate / (m_VirtualCardChnInfo[nChNum-1].nFrequency * 1000);
	int nPointInCycle;

	if (fPointInCycle < 1.0)
	{
		nPointInCycle = 1;
	}
	else{
		nPointInCycle = (int)fPointInCycle;
	}

	switch (	m_VirtualCardChnInfo[nChNum-1].nWaveForm	)
	{
	case 0:
		//正弦波
		for (nCount=0 ; nCount<10240 ; nCount++)
		{
			//由于振幅会变化，先计算出当前周期的振幅
			fRatio = (float)(((nPointStartPos+nCount)/nPointInCycle)%5 +1 ) / 5;
			nAmp = fRatio*(	m_VirtualCardChnInfo[nChNum-1].nAmpMax - m_VirtualCardChnInfo[nChNum-1].nAmpMin) + m_VirtualCardChnInfo[nChNum-1].nAmpMin;
//			TRACE("----三角函数-%f---%d---\n",sin(((nPointStartPos+nCount)%nPointInCycle)/nPointInCycle*2*3.14),((nPointStartPos+nCount)%nPointInCycle)/nPointInCycle*2*3.14);
			nData = (short)(nAmp * sin(((float)((nPointStartPos+nCount)%nPointInCycle))/((float)nPointInCycle)*2*3.14) );
			*(pDataBuffer+nCount) = nData;
		}
		break;
	case 1:
		//方波
		for (nCount=0 ; nCount<10240 ; nCount++)
		{
			//由于振幅会变化，先计算出当前周期的振幅
			fRatio = (float)(((nPointStartPos+nCount)/nPointInCycle)%5 +1 ) / 5;
			nAmp = fRatio*(	m_VirtualCardChnInfo[nChNum-1].nAmpMax - m_VirtualCardChnInfo[nChNum-1].nAmpMin) + m_VirtualCardChnInfo[nChNum-1].nAmpMin;
			nData = (short)(nAmp * (((nPointStartPos+nCount)/(nPointInCycle/2))%2-0.5)*2 );
			*(pDataBuffer+nCount) = nData;
		}
		break;
	}


/*
	long nPointStartPos = nSegmentStartPos * DATASTORAGE_SEGMENT;
	
	long nCount=0;
	short nData;
	for (nCount=0 ; nCount<10240 ; nCount++)
	{
		float fRatio = (float)(((nPointStartPos+nCount)/4000)%5 + 5) / 10;
		nData = (short)(5000* sin(((nPointStartPos+nCount)%2000-1000.0)/1000*2*3.14) * fRatio );
		*(pDataBuffer+nCount) = nData;
	}
*/

	return 0;
}	


double	__stdcall CVirtualCard::VirCard_SetAcqRate(double arate, double amrate)
{
//	AfxMessageBox("VirCard_SetAcqRate");
	m_VirCardAcqInfo.samplingRate = (int)arate;
	return 0.0;
}

void	__stdcall CVirtualCard::VirCard_GetAcqRate(double *arate, double *amrate)
{
	
}

void	__stdcall CVirtualCard::VirCard_SetAcqFrame(int alength, int adelay)
{
	m_VirCardAcqInfo.samplingLength = alength;
	m_VirCardAcqInfo.samplingDelay = adelay;
}

void	__stdcall CVirtualCard::VirCard_GetAcqFrame(int *alength, int *adelay)
{

}
	
void	__stdcall CVirtualCard::VirCard_SetAcqTrigger(int tmode, int tLevelUpperLimit,int tLevelLowerLimit)
{
	m_VirCardAcqInfo.trigMode = tmode;
	m_VirCardAcqInfo.trigUpperLimit = tLevelUpperLimit;
	m_VirCardAcqInfo.trigLowerLimit = tLevelLowerLimit;
}

void	__stdcall CVirtualCard::VirCard_GetAcqTrigger(int *tmode, int *tch,int *tlevel)
{

}	

void	__stdcall CVirtualCard::VirCard_SetChannelParam(int chidx, int rgidx,int cpl)
{

}

void	__stdcall CVirtualCard::VirCard_GetChannelParam(int *chidx, int *rgidx,int *cpl)
{

}	

void	__stdcall CVirtualCard::VirCard_Acq(void)
{
	SECURITY_ATTRIBUTES saAttr;
	saAttr.nLength = sizeof( SECURITY_ATTRIBUTES );
	saAttr.bInheritHandle = TRUE; 
	saAttr.lpSecurityDescriptor = NULL; 
	DWORD ThreadId ;
	VERIFY( CreateThread(&saAttr, 1000, ChangeVirCardStaProc, (void *)(this), 0, &ThreadId ) );
}

void	__stdcall CVirtualCard::VirCard_StopAcq(void)
{

}

int		__stdcall CVirtualCard::VirCard_StatusCheck(void)
{
	return m_nVirCardAcqStatus;
}

void CVirtualCard::VirCard_Initialize(int nChnNum)
{
	int nCount=0;
	for (nCount=0 ; nCount<MAX_CHANNUM ; nCount++)
	{
		if (nCount < nChnNum)
		{
			m_VirtualCardChnInfo[nCount].nIndex = nCount;
			m_VirtualCardChnInfo[nCount].bOpened = TRUE;
			if ( 0 == nCount%2)	{
				m_VirtualCardChnInfo[nCount].nWaveForm = 0;
			}
			else{				
				m_VirtualCardChnInfo[nCount].nWaveForm = 1;
			}
			m_VirtualCardChnInfo[nCount].nFrequency = 1;
			m_VirtualCardChnInfo[nCount].nAmpMax = 10000;
			m_VirtualCardChnInfo[nCount].nAmpMin = 5000;
		}
		else{
			m_VirtualCardChnInfo[nCount].nIndex = nCount;
			m_VirtualCardChnInfo[nCount].bOpened = false;
			if ( 0 == nCount%2)	{
				m_VirtualCardChnInfo[nCount].nWaveForm = 0;
			}
			else{				
				m_VirtualCardChnInfo[nCount].nWaveForm = 1;
			}
			m_VirtualCardChnInfo[nCount].nFrequency = 1;
			m_VirtualCardChnInfo[nCount].nAmpMax = 10000;
			m_VirtualCardChnInfo[nCount].nAmpMin = 5000;
		}
		
	}
}




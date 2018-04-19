// PhyCard.h: interface for the CPhyCard class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PHYCARD_H__4FFC9D51_0DB6_48D7_9326_0696C6592F62__INCLUDED_)
#define AFX_PHYCARD_H__4FFC9D51_0DB6_48D7_9326_0696C6592F62__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPhyCard  
{
public:
	int PhyCard_TrigStatusCheck();
	void PhyCard_Initialize(int nChnNum);			//通过WCDPcie_OpenDevice打开m_nPhyCardNum张卡，好像和参数nChnNum没啥关系
	CPhyCard();
	virtual ~CPhyCard();

	///////////供函数指针指向的函数///////////////////////////////////////////////////////////////
	int		__stdcall PhyCard_Pack(int nChNum, short* pDataBuffer, int nMode, int nInterval, long nSegmentStartPos, long nDataLen);	
	
	double	__stdcall PhyCard_SetAcqRate(double arate, double amrate);
	void	__stdcall PhyCard_GetAcqRate(double *arate, double *amrate);
	void	__stdcall PhyCard_SetAcqFrame(int alength, int adelay);
	void	__stdcall PhyCard_GetAcqFrame(int *alength, int *adelay);	
	void	__stdcall PhyCard_SetAcqTrigger(int tmode, int tch,int tlevel);
	void	__stdcall PhyCard_GetAcqTrigger(int *tmode, int *tch,int *tlevel);	
	void	__stdcall PhyCard_SetChannelParam(int chidx, int rgidx,int cpl);
	void	__stdcall PhyCard_GetChannelParam(int *chidx, int *rgidx,int *cpl);	
	
	void	__stdcall PhyCard_Acq(void);
	void	__stdcall PhyCard_StopAcq(void);
	int		__stdcall PhyCard_StatusCheck(void);
	
	
	
	AcqInfo m_PhyCardAcqInfo;
	
	bool m_bPhyCardAcqing;
	int m_nPhyCardAcqStatus;

	int m_nPhyCardNum;
	int m_nOpenChannelNum;
	unsigned long m_BusIDArray[32];
	unsigned long m_SlotIDArray[32];
	
//	VirtualCardChnInfo m_PhytualCardChnInfo[MAX_CHANNUM];

	
	BOOL bTestFlag;
	unsigned char *m_Data;

};

#endif // !defined(AFX_PHYCARD_H__4FFC9D51_0DB6_48D7_9326_0696C6592F62__INCLUDED_)

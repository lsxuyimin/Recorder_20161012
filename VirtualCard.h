// VirtualCard.h: interface for the CVirtualCard class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VIRTUALCARD_H__5F739A92_9C6D_4274_A231_E69C4CF6AD70__INCLUDED_)
#define AFX_VIRTUALCARD_H__5F739A92_9C6D_4274_A231_E69C4CF6AD70__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CVirtualCard  
{
public:
	void VirCard_Initialize(int nChnNum);
	CVirtualCard();
	virtual ~CVirtualCard();


///////////供函数指针指向的函数///////////////////////////////////////////////////////////////
	int		__stdcall VirCard_Pack(int nChNum, short* pDataBuffer, int nMode, int nInterval, long nSegmentStartPos, long nDataLen);	

	double	__stdcall VirCard_SetAcqRate(double arate, double amrate);
	void	__stdcall VirCard_GetAcqRate(double *arate, double *amrate);
	void	__stdcall VirCard_SetAcqFrame(int alength, int adelay);
	void	__stdcall VirCard_GetAcqFrame(int *alength, int *adelay);	
	void	__stdcall VirCard_SetAcqTrigger(int tmode, int tch,int tlevel);
	void	__stdcall VirCard_GetAcqTrigger(int *tmode, int *tch,int *tlevel);	
	void	__stdcall VirCard_SetChannelParam(int chidx, int rgidx,int cpl);
	void	__stdcall VirCard_GetChannelParam(int *chidx, int *rgidx,int *cpl);	

	void	__stdcall VirCard_Acq(void);
	void	__stdcall VirCard_StopAcq(void);
	int		__stdcall VirCard_StatusCheck(void);



	AcqInfo m_VirCardAcqInfo;

	bool m_bVirCardAcqing;
	int m_nVirCardAcqStatus;

	VirtualCardChnInfo m_VirtualCardChnInfo[MAX_CHANNUM];




};

#endif // !defined(AFX_VIRTUALCARD_H__5F739A92_9C6D_4274_A231_E69C4CF6AD70__INCLUDED_)

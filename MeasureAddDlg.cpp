// MeasureAddDlg.cpp : implementation file
//

#include "stdafx.h"
#include "recorder.h"
#include "MeasureAddDlg.h"


#include "WatchDlg.h"
#include "WaveData.h"
#include "MeasureItem.h"
#include "MeasureDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMeasureAddDlg dialog


CMeasureAddDlg::CMeasureAddDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMeasureAddDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMeasureAddDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pOwnedWatchDlg = (CWatchDlg*)pParent;
//	m_pOwnedWatchDlg = NULL;
}


void CMeasureAddDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMeasureAddDlg)
	DDX_Control(pDX, IDC_COMBO_MeaType, m_comboMeaType);
	DDX_Control(pDX, IDC_COMBO_MeaPart, m_comboMeaPart);
	DDX_Control(pDX, IDC_COMBO_MeaChannel, m_comboMeaChannel);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMeasureAddDlg, CDialog)
	//{{AFX_MSG_MAP(CMeasureAddDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMeasureAddDlg message handlers

BOOL CMeasureAddDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	//=======================初始化===================================
	//初始化	m_comboMeaChannel;
	int nCount;
	CWaveData* pWavaData;
	CTypedPtrList<CObList , CWaveData*>* pWaveDataList = &(m_pOwnedWatchDlg->m_WaveDataList);
	for (nCount=0 ; nCount<m_pOwnedWatchDlg->m_nWaveDataNum ; nCount++)
	{
		pWavaData = pWaveDataList->GetAt(pWaveDataList->FindIndex(nCount));
		m_comboMeaChannel.AddString(pWavaData->m_strChanName);
	}
	m_comboMeaChannel.SetCurSel(0);

	//初始化	m_comboMeaType;
/*
	#define MEATYPE_NORMAL_MAX		0
	#define MEATYPE_NORMAL_MIN		1
	#define MEATYPE_NORMAL_ACAVG	2
		
	#define MEATYPE_FFT_PEAKAMP		0
	#define MEATYPE_FFT_SNR			1
*/
	if (0 == m_pOwnedWatchDlg->m_nMode)
	{
		//NORMAL
		m_comboMeaType.AddString("最大");
		m_comboMeaType.AddString("最小");
		m_comboMeaType.AddString("AC均方根");
	} 
	else
	{
		//FFT
		m_comboMeaType.AddString("振幅峰值");
		m_comboMeaType.AddString("频率峰值");
	}
	m_comboMeaType.SetCurSel(0);

	//初始化	m_comboMeaPart;
/*
	#define MEAPART_ALL				0
	#define MEAPART_BET1AND2		1
	#define MEAPART_LOOPBY1			2
	#define MEAPART_LOOPBY2			3
*/
	m_comboMeaPart.AddString("整个轨迹");
	m_comboMeaPart.AddString("标尺之间");
	m_comboMeaPart.AddString("标尺1循环");
	m_comboMeaPart.AddString("标尺2循环");
	m_comboMeaPart.SetCurSel(0);

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMeasureAddDlg::OnOK() 
{
	// TODO: Add extra validation here
	

	//======================初始化pMeaItem=========================================
	//pWaveData
	CWaveData* pWaveDataParam;

	CString strChanName;
	int nCurSeleted = m_comboMeaChannel.GetCurSel();
	m_comboMeaChannel.GetLBText(nCurSeleted,strChanName);

	int nCount;
	CWaveData* pWavaData;
	CTypedPtrList<CObList , CWaveData*>* pWaveDataList = &(m_pOwnedWatchDlg->m_WaveDataList);
	for (nCount=0 ; nCount<m_pOwnedWatchDlg->m_nWaveDataNum ; nCount++)
	{
		pWavaData = pWaveDataList->GetAt(pWaveDataList->FindIndex(nCount));
		if (strChanName == pWavaData->m_strChanName)
		{
			pWaveDataParam = pWavaData;
		}
	}

	//nMeaType
	int nMeaTypeParam = m_comboMeaType.GetCurSel();
	if ( m_pOwnedWatchDlg->m_nMode == DISPLAY_MODE_FREQ)
	{
		nMeaTypeParam = nMeaTypeParam + 50;
	}

	//nMeaPart
	int nMeaPartParam = m_comboMeaPart.GetCurSel();

	//======================加入WatchDlg===================================================
//m_pOwnedWatchDlg->m_pMeasureDlg->m_MeasureItemList.AddTail(pMeaItem);
	m_pOwnedWatchDlg->AddToMeasureDlg(pWaveDataParam , nMeaTypeParam , nMeaPartParam);
	m_pOwnedWatchDlg->ShowWindow(SW_SHOW);
	
	CDialog::OnOK();
}

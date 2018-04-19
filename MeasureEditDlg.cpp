// MeasureEditDlg.cpp : implementation file
//

#include "stdafx.h"
#include "recorder.h"
#include "MeasureEditDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



#include "WatchDlg.h"
#include "WaveData.h"
#include "MeasureItem.h"
#include "MeasureDlg.h"



/////////////////////////////////////////////////////////////////////////////
// CMeasureEditDlg dialog


CMeasureEditDlg::CMeasureEditDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMeasureEditDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMeasureEditDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pOwnedWatchDlg = (CWatchDlg*)pParent;
}


void CMeasureEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMeasureEditDlg)
	DDX_Control(pDX, IDC_COMBO_MeaType, m_comboMeaType);
	DDX_Control(pDX, IDC_COMBO_MeaPart, m_comboMeaPart);
	DDX_Control(pDX, IDC_COMBO_MeaChannel, m_comboMeaChannel);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMeasureEditDlg, CDialog)
	//{{AFX_MSG_MAP(CMeasureEditDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMeasureEditDlg message handlers

BOOL CMeasureEditDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here


	//=======================获取当前选中的CMeasureItem===================================

	CMeasureItem* pMeasureItem = NULL;	
	CTypedPtrList<CObList , CMeasureItem*>* pMeasureItemList;
	pMeasureItemList = &(m_pOwnedWatchDlg->m_pMeasureDlg->m_MeasureItemList);
	pMeasureItem = (CMeasureItem*)pMeasureItemList->GetAt(pMeasureItemList->FindIndex(m_pOwnedWatchDlg->m_pMeasureDlg->m_nSelItem));
	

	//=======================初始化===================================
	//初始化	m_comboMeaChannel;
	int nCount;
	int nSelCount=0;
	CWaveData* pWavaData;
	CTypedPtrList<CObList , CWaveData*>* pWaveDataList = &(m_pOwnedWatchDlg->m_WaveDataList);
	for (nCount=0 ; nCount<m_pOwnedWatchDlg->m_nWaveDataNum ; nCount++)
	{
		pWavaData = pWaveDataList->GetAt(pWaveDataList->FindIndex(nCount));
		m_comboMeaChannel.AddString(pWavaData->m_strChanName);
		if (pWavaData->m_strChanName == pMeasureItem->pWaveData->m_strChanName)
		{
			nSelCount = nCount;
		}
	}
	m_comboMeaChannel.SetCurSel(nSelCount);



	//初始化	m_comboMeaType;

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
		m_comboMeaType.AddString("SNR");
	}
	m_comboMeaType.SetCurSel(pMeasureItem->nMeaType);

	m_comboMeaPart.AddString("整个轨迹");
	m_comboMeaPart.AddString("标尺之间");
	m_comboMeaPart.AddString("标尺1循环");
	m_comboMeaPart.AddString("标尺2循环");
	m_comboMeaPart.SetCurSel(pMeasureItem->nMeaPart);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMeasureEditDlg::OnOK() 
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
	
	//nMeaPart
	int nMeaPartParam = m_comboMeaPart.GetCurSel();
	
/*
	//======================加入WatchDlg===================================================
	//m_pOwnedWatchDlg->m_pMeasureDlg->m_MeasureItemList.AddTail(pMeaItem);
	m_pOwnedWatchDlg->AddToMeasureDlg(pWaveDataParam , nMeaTypeParam , nMeaPartParam);
	m_pOwnedWatchDlg->ShowWindow(SW_SHOW);
*/
	//======================修改===================================================
	CMeasureItem* pMeasureItem = NULL;	
	CTypedPtrList<CObList , CMeasureItem*>* pMeasureItemList;
	pMeasureItemList = &(m_pOwnedWatchDlg->m_pMeasureDlg->m_MeasureItemList);
	pMeasureItem = (CMeasureItem*)pMeasureItemList->GetAt(pMeasureItemList->FindIndex(m_pOwnedWatchDlg->m_pMeasureDlg->m_nSelItem));
	
	pMeasureItem->pWaveData = pWaveDataParam;
	pMeasureItem->nMeaPart = nMeaPartParam;
	pMeasureItem->nMeaType = nMeaTypeParam;
	
	m_pOwnedWatchDlg->m_pMeasureDlg->Invalidate();
	m_pOwnedWatchDlg->ShowWindow(SW_SHOW);



	CDialog::OnOK();
}

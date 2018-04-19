// PopupPropertyDlg.cpp : implementation file
//

#include "stdafx.h"
#include "recorder.h"
#include "PopupPropertyDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
#include "MainFrm.h"
#include "RecorderView.h"
#include "RecorderDoc.h"
#include "Product.h"
#include "BgDlg.h"
#include "WatchDlg.h"
#include "WndManage.h"
#include "WaveData.h"

/////////////////////////////////////////////////////////////////////////////
// CPopupPropertyDlg dialog


CPopupPropertyDlg::CPopupPropertyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPopupPropertyDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPopupPropertyDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CPopupPropertyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPopupPropertyDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPopupPropertyDlg, CDialog)
	//{{AFX_MSG_MAP(CPopupPropertyDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPopupPropertyDlg message handlers

BOOL CPopupPropertyDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CRecorderDoc *pDoc = (CRecorderDoc*)(pFrame->GetActiveDocument());
	CProduct* pProduct = &(pDoc->m_Product);
	CRecorderView* pView = (CRecorderView*)(pFrame->GetActiveView());

	CBgDlg* pBgDlg = &(pView->m_WndManage.m_pBgDlg[pView->m_WndManage.m_nCurSelect]);
	
	int nCurSeleted;
	nCurSeleted = pBgDlg->m_TabBgDlg.GetCurSel();
	CWatchDlg* pWatchDlgCurSel;
	pWatchDlgCurSel = (CWatchDlg*)pBgDlg->m_WatchDlgList.GetAt(pBgDlg->m_WatchDlgList.FindIndex(nCurSeleted));

	//初始化几个Static
	CString strTemp;
	switch (pProduct->m_nRecorderMode)
	{
	case 0:
		strTemp = "物理采集卡";
		break;
	case 1:
		strTemp = "模拟信号";
		break;
	case 2:
		strTemp = "已有数据";
		break;
	}
	GetDlgItem(IDC_STATIC_Source)->SetWindowText(strTemp);

	strTemp = "";
	int nCount;
	CWaveData* pWavaData;
	for (nCount=0 ; nCount<pWatchDlgCurSel->m_nWaveDataNum ; nCount++)
	{
		pWavaData = pWatchDlgCurSel->m_WaveDataList.GetAt(pWatchDlgCurSel->m_WaveDataList.FindIndex(nCount));
		strTemp = strTemp + pWavaData->m_strChanName;
		if (nCount<pWatchDlgCurSel->m_nWaveDataNum-1)
		{
			strTemp = strTemp + ",";
		}
		
	}
	GetDlgItem(IDC_STATIC_ChNum)->SetWindowText(strTemp);

	switch (pProduct->m_AcqInfo.samplingRate)
	{
	case 50000000:
		strTemp = "50M";
		break;
	case 10000000:
		strTemp = "10M";
		break;
	case 1000000:
		strTemp = "1M";
		break;
	case 100000:
		strTemp = "100k";
		break;
	case 10000:
		strTemp = "10K";
		break;

	}
	GetDlgItem(IDC_STATIC_SamplingRate)->SetWindowText(strTemp);

	strTemp.Format("%d",pProduct->m_AcqInfo.samplingLength);
	GetDlgItem(IDC_STATIC_SamplingLen)->SetWindowText(strTemp);

	double fTime = (double)pProduct->m_AcqInfo.samplingLength / (double)pProduct->m_AcqInfo.samplingRate;
	double fResult;
	CString strUnit;
	if (fTime <0.001)
	{
		fResult = fTime * 1000000;
		strUnit.Format("ns"); 
	}
	else if (fTime <1)
	{
		fResult = fTime * 1000;
		strUnit.Format("ms"); 
	}
	else if (fTime <1000)
	{
		fResult = fTime;
		strUnit.Format("s"); 
	}
	else 
	{
		fResult = fTime / 1000;
		strUnit.Format("ks"); 
	}
	strTemp.Format("%.3f",fResult);
	strTemp = strTemp + strUnit;
	GetDlgItem(IDC_STATIC_SamplingTime)->SetWindowText(strTemp);

	strTemp.Format("%dV",pProduct->m_AcqInfo.range);
	GetDlgItem(IDC_STATIC_Range)->SetWindowText(strTemp);




	//移出动画

	CRect rcView;
	pView->GetClientRect(&rcView);
	CRect rcDlg;
	GetClientRect(&rcDlg);
	int nWidth = rcDlg.Width();
	rcDlg.bottom = rcView.bottom;
	rcDlg.right = rcView.right;
	rcDlg.left = rcDlg.right - nWidth;
	pView->ClientToScreen(rcDlg);
	MoveWindow(rcDlg);
	
	AnimateWindow(GetSafeHwnd(),300,AW_SLIDE|AW_HOR_NEGATIVE); 


	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

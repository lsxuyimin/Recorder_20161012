// PopupRightDlg.cpp : implementation file
//

#include "stdafx.h"
#include "recorder.h"
#include "PopupRightDlg.h"



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

//////////////////////////////////////////////////////////////////////////
// CPopupRightDlg dialog


CPopupRightDlg::CPopupRightDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPopupRightDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPopupRightDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CPopupRightDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPopupRightDlg)
	DDX_Control(pDX, IDC_EDIT_AmpMin, m_editAmpMin);
	DDX_Control(pDX, IDC_EDIT_AmpMax, m_editAmpMax);
	DDX_Control(pDX, IDC_COMBO_ChnIndex, m_comboChnIndex);
	DDX_Control(pDX, IDC_COMBO_Frequency, m_comboFrequency);
	DDX_Control(pDX, IDC_COMBO_WavefForm, m_comboWaveForm);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPopupRightDlg, CDialog)
	//{{AFX_MSG_MAP(CPopupRightDlg)
	ON_CBN_SELCHANGE(IDC_COMBO_ChnIndex, OnSelchangeCOMBOChnIndex)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPopupRightDlg message handlers

BOOL CPopupRightDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CRecorderDoc *pDoc = (CRecorderDoc*)(pFrame->GetActiveDocument());
	CProduct* pProduct = &(pDoc->m_Product);
	CRecorderView* pView = (CRecorderView*)(pFrame->GetActiveView());

	//默认读入Chn1的信息
	int nCount=0;
	CString strTemp;
	for (nCount=0 ; nCount<pProduct->m_nOpenChannelNum ; nCount++)
	{
		strTemp.Format("Ch%d",nCount+1);
		m_comboChnIndex.AddString(strTemp);
	}
	m_comboChnIndex.SetCurSel(0);

	m_comboWaveForm.AddString("正弦波");
	m_comboWaveForm.AddString("方波");
	m_comboWaveForm.SetCurSel(pProduct->m_VirtualCard.m_VirtualCardChnInfo[0].nWaveForm);

	m_comboFrequency.AddString("50M");
	m_comboFrequency.AddString("20M");
	m_comboFrequency.AddString("10M");
	m_comboFrequency.AddString("1M");
	m_comboFrequency.AddString("100K");
	m_comboFrequency.AddString("10K");
	m_comboFrequency.AddString("1K");
	switch (	pProduct->m_VirtualCard.m_VirtualCardChnInfo[0].nFrequency	)
	{
	case 1:
		m_comboFrequency.SetCurSel(6);
		break;
	case 10:
		m_comboFrequency.SetCurSel(5);
		break;
	case 100:
		m_comboFrequency.SetCurSel(4);
		break;
	case 1000:
		m_comboFrequency.SetCurSel(3);
		break;
	case 10000:
		m_comboFrequency.SetCurSel(2);
		break;
	case 20000:
		m_comboFrequency.SetCurSel(1);
		break;
	case 50000:
		m_comboFrequency.SetCurSel(0);
		break;
	}
	strTemp.Format("%d",pProduct->m_VirtualCard.m_VirtualCardChnInfo[0].nAmpMax );
	m_editAmpMax.SetWindowText(strTemp);
	strTemp.Format("%d",pProduct->m_VirtualCard.m_VirtualCardChnInfo[0].nAmpMin );
	m_editAmpMin.SetWindowText(strTemp);


	//移出动画

	CRect rcView;
	pView->GetClientRect(&rcView);
	CRect rcDlg;
	GetClientRect(&rcDlg);
	rcDlg.bottom = rcView.bottom;

	pView->ClientToScreen(rcDlg);
	MoveWindow(rcDlg);
	
	AnimateWindow(GetSafeHwnd(),300,AW_SLIDE|AW_HOR_POSITIVE); 
	


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPopupRightDlg::OnOK() 
{
	// TODO: Add extra validation here
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CRecorderDoc *pDoc = (CRecorderDoc*)(pFrame->GetActiveDocument());
	CProduct* pProduct = &(pDoc->m_Product);
	//更改ChnX的信息
	CString strTemp;
	int nSelChn = m_comboChnIndex.GetCurSel();
	
	int nSelWaveForm = m_comboWaveForm.GetCurSel();
	pProduct->m_VirtualCard.m_VirtualCardChnInfo[nSelChn].nWaveForm = nSelWaveForm;

	int nSelFrequency = m_comboFrequency.GetCurSel();

	switch (	m_comboFrequency.GetCurSel()	)
	{
	case 0:
		pProduct->m_VirtualCard.m_VirtualCardChnInfo[nSelChn].nFrequency = 50000;
		break;
	case 1:
		pProduct->m_VirtualCard.m_VirtualCardChnInfo[nSelChn].nFrequency = 20000;
		break;
	case 2:
		pProduct->m_VirtualCard.m_VirtualCardChnInfo[nSelChn].nFrequency = 10000;
		break;
	case 3:
		pProduct->m_VirtualCard.m_VirtualCardChnInfo[nSelChn].nFrequency = 1000;
		break;
	case 4:
		pProduct->m_VirtualCard.m_VirtualCardChnInfo[nSelChn].nFrequency = 100;
		break;
	case 5:
		pProduct->m_VirtualCard.m_VirtualCardChnInfo[nSelChn].nFrequency = 10;
		break;
	case 6:
		pProduct->m_VirtualCard.m_VirtualCardChnInfo[nSelChn].nFrequency = 1;
		break;
	}

	m_editAmpMax.GetWindowText(strTemp);
	pProduct->m_VirtualCard.m_VirtualCardChnInfo[nSelChn].nAmpMax = atoi(strTemp);
	
	m_editAmpMin.GetWindowText(strTemp);
	pProduct->m_VirtualCard.m_VirtualCardChnInfo[nSelChn].nAmpMin = atoi(strTemp);


	
	CDialog::OnOK();
}

void CPopupRightDlg::OnSelchangeCOMBOChnIndex() 
{
	// TODO: Add your control notification handler code here
	
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CRecorderDoc *pDoc = (CRecorderDoc*)(pFrame->GetActiveDocument());
	CProduct* pProduct = &(pDoc->m_Product);
	//读入ChnX的信息
	CString strTemp;
	int nSelected = m_comboChnIndex.GetCurSel();
	
	m_comboWaveForm.SetCurSel(pProduct->m_VirtualCard.m_VirtualCardChnInfo[nSelected].nWaveForm);

	switch (	pProduct->m_VirtualCard.m_VirtualCardChnInfo[nSelected].nFrequency	)
	{
	case 1:
		m_comboFrequency.SetCurSel(6);
		break;
	case 10:
		m_comboFrequency.SetCurSel(5);
		break;
	case 100:
		m_comboFrequency.SetCurSel(4);
		break;
	case 1000:
		m_comboFrequency.SetCurSel(3);
		break;
	case 10000:
		m_comboFrequency.SetCurSel(2);
		break;
	case 20000:
		m_comboFrequency.SetCurSel(1);
		break;
	case 50000:
		m_comboFrequency.SetCurSel(0);
		break;
	}
	strTemp.Format("%d",pProduct->m_VirtualCard.m_VirtualCardChnInfo[nSelected].nAmpMax );
	m_editAmpMax.SetWindowText(strTemp);
	strTemp.Format("%d",pProduct->m_VirtualCard.m_VirtualCardChnInfo[nSelected].nAmpMin );
	m_editAmpMin.SetWindowText(strTemp);
}


// DlgBarEx.cpp : implementation file
//

#include "stdafx.h"
#include "Recorder.h"
#include "DlgBarEx.h"

#include "MainFrm.h"
#include "RecorderView.h"
#include "RecorderDoc.h"
#include "WndManage.h"
#include "BgDlg.h"
#include "WatchDlg.h"
#include "Product.h"
#include "FftComplex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgBarEx dialog


CDlgBarEx::CDlgBarEx(CWnd* pParent /*=NULL*/)
{
	//{{AFX_DATA_INIT(CDlgBarEx)
	//}}AFX_DATA_INIT
}


void CDlgBarEx::DoDataExchange(CDataExchange* pDX)
{
	//CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgBarEx)
	DDX_Control(pDX, IDC_SLIDER_TRIGGER_UpperLimit, m_SliderTrigUpperLimit);
	DDX_Control(pDX, IDC_SLIDER_TRIGGER_LowerLimit, m_SliderTrigLowerLimit);
	DDX_Control(pDX, IDC_SLIDER_TRIGGER_LEVEL, m_SliderTrigLevel);
	DDX_Control(pDX, IDC_COMBO_TRIGGER_CHANNEL, m_ComboTriggerChn);
	DDX_Control(pDX, IDC_COMBO_TRIGGER_MODE, m_ComboTrigMode);
	DDX_Control(pDX, IDC_COMBO_RATE, m_ComboRate);
	DDX_Control(pDX, IDC_COMBO_RANGE, m_ComboRange);
	DDX_Control(pDX, IDC_COMBO_LENGTH, m_ComboLength);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgBarEx, CDialogBar)
	//{{AFX_MSG_MAP(CDlgBarEx)
	ON_CBN_SELCHANGE(IDC_COMBO_LENGTH, OnSelchangeComboLength)
	ON_CBN_SELCHANGE(IDC_COMBO_RANGE, OnSelchangeComboRange)
	ON_CBN_SELCHANGE(IDC_COMBO_RATE, OnSelchangeComboRate)
	ON_CBN_SELCHANGE(IDC_COMBO_TRIGGER_CHANNEL, OnSelchangeComboTriggerChannel)
	ON_WM_HSCROLL()
	ON_MESSAGE(WM_INITDIALOG,OnInitDialog)
	ON_CBN_SELCHANGE(IDC_COMBO_TRIGGER_MODE, OnSelchangeComboTriggerMode)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgBarEx message handlers

BOOL CDlgBarEx::OnInitDialog(UINT wParam,LONG lParam) 
{
	//CDialogBar::OnInitDialog();
	
	// TODO: Add extra initialization here
	BOOL bRet = HandleInitDialog(wParam,lParam);
	if (!UpdateData(FALSE))
	{
		TRACE("InitCDataStatus Failed！");
	}

	//初始化DlgBarEx上的控件
	InitialControl();


	return TRUE;  // return TRUE unless you set the focus to a control
                 // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgBarEx::InitialControl()
{
	
	m_ComboRate.AddString("100M");
	m_ComboRate.AddString("50M");
	m_ComboRate.AddString("10M");
	m_ComboRate.AddString("1M");
	m_ComboRate.AddString("100K");
	m_ComboRate.AddString("10K");
	m_ComboRate.SetCurSel(3);
	
	m_ComboLength.AddString("67101720");
	m_ComboLength.AddString("41943040");
	m_ComboLength.AddString("10485760");
	m_ComboLength.AddString("2621440");
	m_ComboLength.AddString("655360");
	m_ComboLength.AddString("163840");
	m_ComboLength.AddString("40960");
	m_ComboLength.AddString("10240");
//	m_ComboLength.AddString("4096");
	m_ComboLength.SetCurSel(6);
	
	m_ComboRange.AddString("20V");
	m_ComboRange.AddString("10V");
	m_ComboRange.AddString("5V");
	m_ComboRange.AddString("1V");
	m_ComboRange.SetCurSel(1);
	
	m_ComboTrigMode.AddString("手动触发");
	m_ComboTrigMode.AddString("通道触发");
	m_ComboTrigMode.SetCurSel(0);


	m_SliderTrigLevel.SetRange(-10000,10000,TRUE);//设置滑动条范围	
	m_SliderTrigLevel.SetPos(0);
	m_SliderTrigLowerLimit.SetRange(0,65536,TRUE);//设置滑动条范围	
	m_SliderTrigLowerLimit.SetPos(17873);		//设置上限为-1v
	GetDlgItem(IDC_STATIC_TRIGGER_UpperLimit)->SetWindowText("1v");
	m_SliderTrigUpperLimit.SetRange(0,65536,TRUE);//设置滑动条范围	
	m_SliderTrigUpperLimit.SetPos(47663);		//设置下限为1v
	GetDlgItem(IDC_STATIC_TRIGGER_LowerLimit)->SetWindowText("-1v");

//	RefreshParaSetting();
}

void CDlgBarEx::OnSelchangeComboLength() 
{
	// TODO: Add your control notification handler code here
/*
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CWndManage* pWndManage = &((CRecorderView*)(pFrame->GetActiveView()))->m_WndManage;

	CBgDlg* pCurBgDlg = &(pWndManage->m_pBgDlg[pWndManage->m_nCurSelect]);
	int nCurWatchDLgInBgDlg = pCurBgDlg->m_TabBgDlg.GetCurSel();
	CWatchDlg* pWatchDlgCurSel;
	pWatchDlgCurSel = (CWatchDlg*)pCurBgDlg->m_WatchDlgList.GetAt(pCurBgDlg->m_WatchDlgList.FindIndex(nCurWatchDLgInBgDlg));

	int selected = 	m_ComboLength.GetCurSel();
	switch(selected){
	case  0:
		pWatchDlgCurSel->m_AcqInfo.samplingLength = 1048576;
		break;
	case  1:
		pWatchDlgCurSel->m_AcqInfo.samplingLength = 262144;
		break;
	case  2:
		pWatchDlgCurSel->m_AcqInfo.samplingLength = 65536;
		break;
	case  3:
		pWatchDlgCurSel->m_AcqInfo.samplingLength = 16384;
		break;
	case  4:
		pWatchDlgCurSel->m_AcqInfo.samplingLength = 4096;
		break;
	default:
		break;
	}
	((CRecorderView*)(pFrame->GetActiveView()))->WndManageRefresh();
*/
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CRecorderDoc *pDoc = (CRecorderDoc*)(pFrame->GetActiveDocument());
	CProduct* pProduct = &(pDoc->m_Product);

	int selected = 	m_ComboLength.GetCurSel();
	switch(selected){
	case  0:
		pProduct->m_AcqInfo.samplingLength = 67108864;
		break;
	case  1:
		pProduct->m_AcqInfo.samplingLength = 33554432;
		break;
	case  2:
		pProduct->m_AcqInfo.samplingLength = 16777216;
		break;
	case  3:
		pProduct->m_AcqInfo.samplingLength = 4194304;
		break;
	case  4:
		pProduct->m_AcqInfo.samplingLength = 1048576;
		break;
	case  5:
		pProduct->m_AcqInfo.samplingLength = 262144;
		break;
	case  6:
		pProduct->m_AcqInfo.samplingLength = 65536;
		break;
	case  7:
		pProduct->m_AcqInfo.samplingLength = 16384;
		break;
	case  8:
		pProduct->m_AcqInfo.samplingLength = 4096;
		break;
	default:
		break;
	}
	((CRecorderView*)(pFrame->GetActiveView()))->WndManageRefresh_Complete();


}

void CDlgBarEx::OnSelchangeComboRange() 
{
	// TODO: Add your control notification handler code here
/*
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CWndManage* pWndManage = &((CRecorderView*)(pFrame->GetActiveView()))->m_WndManage;
	
	CBgDlg* pCurBgDlg = &(pWndManage->m_pBgDlg[pWndManage->m_nCurSelect]);
	int nCurWatchDLgInBgDlg = pCurBgDlg->m_TabBgDlg.GetCurSel();
	CWatchDlg* pWatchDlgCurSel;
	pWatchDlgCurSel = (CWatchDlg*)pCurBgDlg->m_WatchDlgList.GetAt(pCurBgDlg->m_WatchDlgList.FindIndex(nCurWatchDLgInBgDlg));
	
	int selected = 	m_ComboRange.GetCurSel();
	switch(selected){
	case  0:
		pWatchDlgCurSel->m_AcqInfo.range = 20;
		break;
	case  1:
		pWatchDlgCurSel->m_AcqInfo.range = 10;
		break;
	case  2:
		pWatchDlgCurSel->m_AcqInfo.range = 5;
		break;
	case  3:
		pWatchDlgCurSel->m_AcqInfo.range = 1;
		break;
	default:
		break;
	}
	((CRecorderView*)(pFrame->GetActiveView()))->WndManageRefresh();
*/
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CRecorderDoc *pDoc = (CRecorderDoc*)(pFrame->GetActiveDocument());
	CProduct* pProduct = &(pDoc->m_Product);
	
	int selected = 	m_ComboRange.GetCurSel();
	switch(selected){
	case  0:
		pProduct->m_AcqInfo.range = 20;
		break;
	case  1:
		pProduct->m_AcqInfo.range = 10;
		break;
	case  2:
		pProduct->m_AcqInfo.range = 5;
		break;
	case  3:
		pProduct->m_AcqInfo.range = 1;
		break;
	default:
		break;
	}

	m_SliderTrigLevel.SetRange((pProduct->m_AcqInfo.range)*(-1000),(pProduct->m_AcqInfo.range)*(1000));
	m_SliderTrigLevel.SetPos(0);
	GetDlgItem(IDC_STATIC_TRIGGER_LEVEL)->SetWindowText("0.0V");

	((CRecorderView*)(pFrame->GetActiveView()))->WndManageRefresh_Complete();

}

void CDlgBarEx::OnSelchangeComboRate() 
{
	// TODO: Add your control notification handler code here
/*
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CWndManage* pWndManage = &((CRecorderView*)(pFrame->GetActiveView()))->m_WndManage;
	
	CBgDlg* pCurBgDlg = &(pWndManage->m_pBgDlg[pWndManage->m_nCurSelect]);
	int nCurWatchDLgInBgDlg = pCurBgDlg->m_TabBgDlg.GetCurSel();
	CWatchDlg* pWatchDlgCurSel;
	pWatchDlgCurSel = (CWatchDlg*)pCurBgDlg->m_WatchDlgList.GetAt(pCurBgDlg->m_WatchDlgList.FindIndex(nCurWatchDLgInBgDlg));
		
	int selected = 	m_ComboRate.GetCurSel();
	switch(selected){
	case  0:
		pWatchDlgCurSel->m_AcqInfo.samplingRate = 50000000;
		break;
	case  1:
		pWatchDlgCurSel->m_AcqInfo.samplingRate = 10000000;
		break;
	case  2:
		pWatchDlgCurSel->m_AcqInfo.samplingRate = 1000000;
		break;
	case  3:
		pWatchDlgCurSel->m_AcqInfo.samplingRate = 100000;
		break;
	case  4:
		pWatchDlgCurSel->m_AcqInfo.samplingRate = 10000;
		break;
	default:
		break;
	}
	((CRecorderView*)(pFrame->GetActiveView()))->WndManageRefresh();
*/


	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CRecorderDoc *pDoc = (CRecorderDoc*)(pFrame->GetActiveDocument());
	CProduct* pProduct = &(pDoc->m_Product);

	int selected = 	m_ComboRate.GetCurSel();
	switch(selected){
	case  0:
		pProduct->m_AcqInfo.samplingRate = 100000000;
		break;
	case  1:
		pProduct->m_AcqInfo.samplingRate = 50000000;
		break;
	case  2:
		pProduct->m_AcqInfo.samplingRate = 10000000;
		break;
	case  3:
		pProduct->m_AcqInfo.samplingRate = 1000000;
		break;
	case  4:
		pProduct->m_AcqInfo.samplingRate = 100000;
		break;
	case  5:
		pProduct->m_AcqInfo.samplingRate = 10000;
		break;
	default:
		break;
	}
	

//////当前采样率被固定到了100M////////////////////////////////////////////////////
//	pProduct->m_AcqInfo.samplingRate = 100000000;
//	m_ComboRate.SetCurSel(0);
////////////////////////////////////////////////////////////////////////////

	pProduct->m_pFftModule->m_nSampleFreq = pProduct->m_AcqInfo.samplingRate;

	((CRecorderView*)(pFrame->GetActiveView()))->WndManageRefresh_Complete();


}

void CDlgBarEx::OnSelchangeComboTriggerChannel() 
{
	// TODO: Add your control notification handler code here
	
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CRecorderDoc *pDoc = (CRecorderDoc*)(pFrame->GetActiveDocument());
	CProduct* pProduct = &(pDoc->m_Product);
	
	int selected = 	m_ComboTriggerChn.GetCurSel();
	pProduct->m_AcqInfo.trigChan = selected;


	((CRecorderView*)(pFrame->GetActiveView()))->WndManageRefresh_Complete();

}

void CDlgBarEx::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CRecorderDoc *pDoc = (CRecorderDoc*)(pFrame->GetActiveDocument());
	CProduct* pProduct = &(pDoc->m_Product);

	CWnd* pSliderTrigLevel = GetDlgItem(IDC_SLIDER_TRIGGER_LEVEL);
	CWnd* pSliderTrigUpperLimit = GetDlgItem(IDC_SLIDER_TRIGGER_UpperLimit);
	CWnd* pSliderTrigLowerLimit = GetDlgItem(IDC_SLIDER_TRIGGER_LowerLimit);

	if (pSliderTrigLevel == pScrollBar)
	{
		//触发电平滑块
		int nSliderTrigLevelPos = m_SliderTrigLevel.GetPos();
		CString strTemp;
		strTemp.Format("%.2fV",((float)(nSliderTrigLevelPos))/1000);
		GetDlgItem(IDC_STATIC_TRIGGER_LEVEL)->SetWindowText(strTemp);
		
		pProduct->m_AcqInfo.trigLevel = nSliderTrigLevelPos;
	} 
	else if (pSliderTrigUpperLimit == pScrollBar)
	{
		//通道触发 触发上限
		int nSliderTrigUpperLimitPos = m_SliderTrigUpperLimit.GetPos();
		CString strTemp;
		strTemp.Format("%.2fV",((float)(nSliderTrigUpperLimitPos))/65536*4.4-2.2);			//TEST 根据0到65536对应电压的关系要改
		GetDlgItem(IDC_STATIC_TRIGGER_UpperLimit)->SetWindowText(strTemp);
		
		pProduct->m_AcqInfo.trigUpperLimit = nSliderTrigUpperLimitPos;
	} 
	else if (pSliderTrigLowerLimit == pScrollBar)
	{
		//通道触发 触发下限
		int nSliderTrigLowerLimitPos = m_SliderTrigLowerLimit.GetPos();
		CString strTemp;
		strTemp.Format("%.2fV",((float)(nSliderTrigLowerLimitPos))/65536*4.4-2.2);			//TEST 根据0到65536对应电压的关系要改
		GetDlgItem(IDC_STATIC_TRIGGER_LowerLimit)->SetWindowText(strTemp);
		
		pProduct->m_AcqInfo.trigLowerLimit = nSliderTrigLowerLimitPos;
	} 
	else
	{
		MessageBox("there is no match slider!!!");
	}
	
	CDialogBar::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CDlgBarEx::OnSelchangeComboTriggerMode() 
{
	// TODO: Add your control notification handler code here
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CRecorderDoc *pDoc = (CRecorderDoc*)(pFrame->GetActiveDocument());
	CProduct* pProduct = &(pDoc->m_Product);
	
	int selected = 	m_ComboTrigMode.GetCurSel();
	
	switch (selected)
	{
	case 0:
		pProduct->m_AcqInfo.trigMode = TRIMODE_MANUAL;
		break;
	case 1:
		pProduct->m_AcqInfo.trigMode = TRIMODE_CHANNEL;
		break;
	default:
		break;
	}

	((CRecorderView*)(pFrame->GetActiveView()))->WndManageRefresh_Complete();
}

int CDlgBarEx::RefreshParaSetting()
{

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	
	CRecorderDoc *pDoc = (CRecorderDoc*)(pFrame->GetActiveDocument());
	CProduct* pProduct = &(pDoc->m_Product);
	//采样率
	switch(pProduct->m_AcqInfo.samplingRate){
	case  100000000:
		m_ComboRate.SetCurSel(0);
		break;
	case  50000000:
		m_ComboRate.SetCurSel(1);
		break;
	case  10000000:
		m_ComboRate.SetCurSel(2);
		break;
	case  1000000:
		m_ComboRate.SetCurSel(3);
		break;
	case  100000:
		m_ComboRate.SetCurSel(4);
		break;
	case  10000:
		m_ComboRate.SetCurSel(5);
		break;
	default:
		break;
	}
	//采样长度
	switch(pProduct->m_AcqInfo.samplingLength){
	case  67108864:
		m_ComboLength.SetCurSel(0);
		break;
	case  33554432:
		m_ComboLength.SetCurSel(1);
		break;
	case  16777216:
		m_ComboLength.SetCurSel(2);
		break;
	case  4194304:
		m_ComboLength.SetCurSel(3);
		break;
	case  1048576:
		m_ComboLength.SetCurSel(4);
		break;
	case  262144:
		m_ComboLength.SetCurSel(5);
		break;
	case  65536:
		m_ComboLength.SetCurSel(6);
		break;
	case  16384:
		m_ComboLength.SetCurSel(7);
		break;
	default:
		break;
	}
	//量程
	switch(pProduct->m_AcqInfo.range){
	case  20:
		m_ComboRange.SetCurSel(0);
		break;
	case  10:
		m_ComboRange.SetCurSel(1);
		break;
	case  5:
		m_ComboRange.SetCurSel(2);
		break;
	case  1:
		m_ComboRange.SetCurSel(3);
		break;
	default:
		break;
	}


	return 0;
}

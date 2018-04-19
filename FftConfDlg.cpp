// FftConfDlg.cpp : implementation file
//

#include "stdafx.h"
#include "recorder.h"
#include "FftConfDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
#include "MainFrm.h"
#include "RecorderDoc.h"
#include "RecorderView.h"
#include "Product.h"
#include "BgDlg.h"
#include "WatchDlg.h"
#include "FftComplex.h"
/////////////////////////////////////////////////////////////////////////////
// CFftConfDlg dialog


CFftConfDlg::CFftConfDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFftConfDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFftConfDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CFftConfDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFftConfDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFftConfDlg, CDialog)
	//{{AFX_MSG_MAP(CFftConfDlg)
	ON_WM_KILLFOCUS()
	ON_WM_ACTIVATE()
	ON_BN_CLICKED(IDC_RADIO_Linear, OnRADIOLinear)
	ON_BN_CLICKED(IDC_RADIO_Logarithm, OnRADIOLogarithm)
	ON_CBN_SELCHANGE(IDC_COMBO_LogUnit, OnSelchangeCOMBOLogUnit)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFftConfDlg message handlers

void CFftConfDlg::OnKillFocus(CWnd* pNewWnd) 
{
	CDialog::OnKillFocus(pNewWnd);
	
	// TODO: Add your message handler code here
	
	TRACE("CFftConfDlg::OnKillFocus(CWnd* pNewWnd)  \n	");
}

void CFftConfDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	CDialog::OnActivate(nState, pWndOther, bMinimized);
	
	// TODO: Add your message handler code here
	//获得焦点
	if (nState==WA_ACTIVE )
	{
		
	}
	//失去焦点
	else if(nState==WA_INACTIVE) 
	{ 
		TRACE("CFftConfDlg::OnActivate	nState==WA_INACTIVE \n	");
		//DestroyWindow();
		SendMessage(WM_CLOSE);
	} 
}

void CFftConfDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	DestroyWindow(); 
	//CDialog::OnCancel();
}

void CFftConfDlg::OnOK() 
{
	// TODO: Add extra validation here
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
//	CRecorderView* pView = (CRecorderView*)(pFrame->GetActiveView());
	CRecorderDoc *pDoc = (CRecorderDoc*)(pFrame->GetActiveDocument());
	CProduct* pProduct = &(pDoc->m_Product);


	CComboBox* pComboBox = NULL;
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_Impedance);

	int nImpedance = -1;
	int nWindowMode = -1;
	int nOutputMode = -1;
	int nFftBins = -1;

	/*首先进行阻抗的检测*/
	CString strImpedance;
	pEdit->GetWindowText(strImpedance);
	nImpedance = _ttoi(strImpedance);
	if ( nImpedance<=0 || nImpedance>= 10000)
	{
		GetDlgItem(IDC_STATIC_Tips)->ShowWindow(SW_SHOW);
		return;
	}

	/*将所有的值付给*/
/*
	int m_nFftBins;				//输出的信号的长度，长度是2的n次方
	int m_nWindowMode;			//加窗函数
	int m_nOutputMode;			//输出模式，对应YAXIS_UNIT_DBV，YAXIS_UNIT_DBu，YAXIS_UNIT_DBm
	int m_nImpedance;			//阻抗，和YAXIS_UNIT_DBm输出配合使用	单位：欧姆
*/
	

	int nCurSel = -1;
	CString strTemp;
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_FftBins);
	nCurSel = pComboBox->GetCurSel();
	pComboBox->GetLBText(nCurSel,strTemp);
	nFftBins = _ttoi(strTemp);

	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_WindowMode);
	nCurSel = pComboBox->GetCurSel();
	nWindowMode = nCurSel+1;

	if ( ((CButton*)GetDlgItem(IDC_RADIO_Linear))->GetCheck() == TRUE )
	{
		/*选中的线性*/
		nOutputMode = YAXIS_UNIT_V;
	}
	else{
		/*选中的是对数*/
		pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_LogUnit);
		nCurSel = pComboBox->GetCurSel();
		switch (nCurSel)
		{
		case 0:
			nOutputMode = YAXIS_UNIT_DBV;
			break;
		case 1:
			nOutputMode = YAXIS_UNIT_DBu;
			break;
		case 2:
			nOutputMode = YAXIS_UNIT_DBm;
			break;
		default :
			break;
		}
	}
	
	CWatchDlg* pCWatchDlg;
	pFrame->GetCurSelWatchDlg(&pCWatchDlg);
	bool bNeedRedraw = false;
	bool bNeedSwitchUnit = false;

	if (	  pProduct->m_FftConf.nFftBins != nFftBins
			||pProduct->m_FftConf.nWindowMode != nWindowMode
			||pProduct->m_FftConf.nOutputMode != nOutputMode
			||pProduct->m_FftConf.nImpedance != nImpedance
		)
	{
		bNeedRedraw = true;
	}

	if ( pProduct->m_FftConf.nOutputMode != nOutputMode )
	{
		bNeedSwitchUnit = true;
	}
	
	pProduct->m_FftConf.nFftBins = nFftBins;
	pProduct->m_FftConf.nWindowMode = nWindowMode;
	pProduct->m_FftConf.nOutputMode = nOutputMode;
	pProduct->m_FftConf.nImpedance = nImpedance;
	
	if (bNeedSwitchUnit)
	{		
		pCWatchDlg->SwitchTimeAndFreq(2);
		pCWatchDlg->AdjustAllToolBar();
	}

	/*令当前的CWatchDlg进行重绘*/
	if ( bNeedRedraw )
	{
		if ( !pProduct->m_bAcqing )
		{
			pCWatchDlg->Invalidate();
			pCWatchDlg->UpdateWindow();
		}
	}
	

	/*关闭该非模态对话框*/
	DestroyWindow(); 
	//CDialog::OnOK();
}

void CFftConfDlg::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CDialog::PostNcDestroy();
	delete this; 
}

int CFftConfDlg::InitialControl()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CRecorderView* pView = (CRecorderView*)(pFrame->GetActiveView());
	CRecorderDoc *pDoc = (CRecorderDoc*)(pFrame->GetActiveDocument());
	CProduct* pProduct = &(pDoc->m_Product);

	CEdit* pEdit;
	CComboBox* pComboBox;
	int nCurSel = -1;
	int nCount = 0;

	GetDlgItem(IDC_STATIC_Tips)->ShowWindow(SW_HIDE);
/*
	int m_nFftBins;				//输出的信号的长度，长度是2的n次方
	int m_nWindowMode;			//加窗函数
	int m_nOutputMode;			//输出模式，对应YAXIS_UNIT_DBV，YAXIS_UNIT_DBu，YAXIS_UNIT_DBm
	int m_nImpedance;			//阻抗，和YAXIS_UNIT_DBm输出配合使用	单位：欧姆
*/

	/*频域Bins*/
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_FftBins);	
	nCurSel = -1;
	nCount = -1;
	int nPower = 128;
	CString strPower;
	while( nPower <= 65536 ){
		nCount = nCount + 1;

		strPower.Format("%d",nPower);
		pComboBox->AddString(strPower);
		if (nPower == pProduct->m_FftConf.nFftBins)
		{
			nCurSel = nCount;
		}
		nPower = nPower*2 ;		
	}
	pComboBox->SetCurSel(nCurSel);
	
/*
	int nIndexBgDlg = pView->m_WndManage.m_nCurSelect;
	CBgDlg* pBgDlg = &(pView->m_WndManage.m_pBgDlg[nIndexBgDlg]);
	
	int nIndexWatchDlg = pBgDlg->m_TabBgDlg.GetCurSel();
	if ( nIndexWatchDlg < 0 )
	{
		TRACE("Bunny Tips : CFftConfDlg::InitialControl() nIndexWatchDlg error!!!\n");
		return -1;
	}
	CWatchDlg* pWatchDlg = (CWatchDlg*)(pBgDlg->m_WatchDlgList.GetAt(pBgDlg->m_WatchDlgList.FindIndex(nIndexWatchDlg)));
	int nPixelWidth = pWatchDlg->m_WDW.rcDisplay.Width();
	TRACE("nPixelWidth is %d \n",nPixelWidth);
*/

	/*加窗函数*/
/*
	#define WINFUN_RECT					(1)
	#define WINFUN_HANNING				(2)
	#define WINFUN_HAMMING				(3)
	#define WINFUN_FLATTOP				(4)
	#define WINFUN_TRIANGLE				(5)
*/
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_WindowMode);
	nCurSel = -1;
	pComboBox->AddString("矩形窗");
	pComboBox->AddString("海宁窗");
	pComboBox->AddString("海明窗");
	pComboBox->AddString("平顶窗");
	pComboBox->AddString("三角形窗");
	nCurSel = pProduct->m_FftConf.nWindowMode - 1;					//窗口的计数是base on 1的，所以这里要减一
	pComboBox->SetCurSel(nCurSel);							


	/*输出模式*/
/*
	#define YAXIS_UNIT_V					(1)			线性
	#define YAXIS_UNIT_DBV					(2)		对数
	#define YAXIS_UNIT_DBu					(3)		对数
	#define YAXIS_UNIT_DBm					(4)		对数
*/
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_LogUnit);
	pEdit = (CEdit*)GetDlgItem(IDC_EDIT_Impedance);
	nCurSel = -1;
	nCurSel = pProduct->m_FftConf.nWindowMode;
	
//	pComboBox->AddString("V");		//线性关系不应该出现在对数单元的COMBOBOX里面，自己知道就行
	pComboBox->AddString("DBV");
	pComboBox->AddString("DBu");
	pComboBox->AddString("DBm");

	CString strImpedance;
	strImpedance.Format("%d",pProduct->m_FftConf.nImpedance);
	pEdit->SetWindowText(strImpedance);

	switch ( pProduct->m_FftConf.nOutputMode )
	{
	case YAXIS_UNIT_V:
		((CButton*)GetDlgItem(IDC_RADIO_Linear))->SetCheck(TRUE);
		((CButton*)GetDlgItem(IDC_RADIO_Logarithm))->SetCheck(FALSE);
		pEdit->EnableWindow(FALSE);
		pComboBox->SetCurSel(0);
		pComboBox->EnableWindow(FALSE);
		break;
	case YAXIS_UNIT_DBV:
		((CButton*)GetDlgItem(IDC_RADIO_Linear))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO_Logarithm))->SetCheck(TRUE);
		pEdit->EnableWindow(FALSE);
		pComboBox->SetCurSel(0);
		pComboBox->EnableWindow(TRUE);
		break;
	case YAXIS_UNIT_DBu:
		((CButton*)GetDlgItem(IDC_RADIO_Linear))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO_Logarithm))->SetCheck(TRUE);
		pEdit->EnableWindow(FALSE);
		pComboBox->SetCurSel(1);
		pComboBox->EnableWindow(TRUE);
		break;
	case YAXIS_UNIT_DBm:
		((CButton*)GetDlgItem(IDC_RADIO_Linear))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO_Logarithm))->SetCheck(TRUE);
		pEdit->EnableWindow(TRUE);
		pComboBox->SetCurSel(2);
		pComboBox->EnableWindow(TRUE);
		break;
	default:
		break;
	}


	return 1;
}

void CFftConfDlg::OnRADIOLinear() 
{
	// TODO: Add your control notification handler code here
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_LogUnit);
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_Impedance);

	((CButton*)GetDlgItem(IDC_RADIO_Linear))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_RADIO_Logarithm))->SetCheck(FALSE);
	pComboBox->EnableWindow(FALSE);
	pEdit->EnableWindow(FALSE);



}

void CFftConfDlg::OnRADIOLogarithm() 
{
	// TODO: Add your control notification handler code here
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_LogUnit);
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_Impedance);

	((CButton*)GetDlgItem(IDC_RADIO_Linear))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO_Logarithm))->SetCheck(TRUE);
	
	pComboBox->EnableWindow(TRUE);
	pComboBox->SetCurSel(0);
	pEdit->EnableWindow(FALSE);


}

void CFftConfDlg::OnSelchangeCOMBOLogUnit() 
{
	// TODO: Add your control notification handler code here
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_LogUnit);
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_Impedance);
	int nCurSel = -1;

	nCurSel = pComboBox->GetCurSel();
	switch (nCurSel)
	{
	case 0:
		/*YAXIS_UNIT_DBV*/		
		pEdit->EnableWindow(FALSE);
		break;
	case 1:
		/*YAXIS_UNIT_DBu*/
		pEdit->EnableWindow(FALSE);
		break;
	case 2:
		/*YAXIS_UNIT_DBm*/
		pEdit->EnableWindow(TRUE);
		break;
	default :
		break;
	}

}

HBRUSH CFftConfDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	
	if ( nCtlColor==CTLCOLOR_STATIC )
	{
		if ( pWnd->GetDlgCtrlID() == IDC_STATIC_Tips )
		{
			pDC->SetTextColor(RGB(255,0,0));
		//	pDC->SetBkColor(TRANSPARENT);
		}
	
	}
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

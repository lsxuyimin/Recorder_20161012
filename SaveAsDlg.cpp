// SaveAsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "recorder.h"
#include "SaveAsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
#include "MainFrm.h"
#include "Product.h"
#include "RecorderDoc.h"
/////////////////////////////////////////////////////////////////////////////
// CSaveAsDlg dialog


CSaveAsDlg::CSaveAsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSaveAsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSaveAsDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CSaveAsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSaveAsDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSaveAsDlg, CDialog)
	//{{AFX_MSG_MAP(CSaveAsDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSaveAsDlg message handlers

BOOL CSaveAsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CRecorderDoc *pDoc = (CRecorderDoc*)(pFrame->GetActiveDocument());
	CProduct* pProduct = &(pDoc->m_Product);
	
	GetDlgItem(IDC_STATIC_WorkPlace)->SetWindowText(pProduct->m_strWorkPlace + "\\");


	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSaveAsDlg::OnOK() 
{
	// TODO: Add extra validation here
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CRecorderDoc *pDoc = (CRecorderDoc*)(pFrame->GetActiveDocument());
	CProduct* pProduct = &(pDoc->m_Product);
	
	
	CString strTemp;
	GetDlgItem(IDC_EDIT_SavePath)->GetWindowText(strTemp);
	pProduct->m_strSavePath = pProduct->m_strWorkPlace + "\\" + strTemp;
	
	
	if(!CreateDirectory(pProduct->m_strSavePath , NULL))
	{
		AfxMessageBox("输入为空或文件夹已存在！");
		return;
	}
	

	CDialog::OnOK();
}

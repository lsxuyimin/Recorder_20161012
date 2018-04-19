// SaveDlg.cpp : implementation file
//

#include "stdafx.h"
#include "recorder.h"
#include "SaveDlg.h"

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
// CSaveDlg dialog


CSaveDlg::CSaveDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSaveDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSaveDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CSaveDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSaveDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSaveDlg, CDialog)
	//{{AFX_MSG_MAP(CSaveDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSaveDlg message handlers

BOOL CSaveDlg::OnInitDialog() 
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

void CSaveDlg::OnOK() 
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
		AfxMessageBox("����Ϊ�ջ��ļ����Ѵ��ڣ�");
		return;
	}







	CDialog::OnOK();
}

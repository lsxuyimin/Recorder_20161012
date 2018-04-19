// DlgBarExBottomAcq.cpp : implementation file
//

#include "stdafx.h"
#include "recorder.h"
#include "DlgBarExBottomAcq.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgBarExBottomAcq dialog


CDlgBarExBottomAcq::CDlgBarExBottomAcq(CWnd* pParent /*=NULL*/)
//	: CDialog(CDlgBarExBottomAcq::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgBarExBottomAcq)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgBarExBottomAcq::DoDataExchange(CDataExchange* pDX)
{
//	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgBarExBottomAcq)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgBarExBottomAcq, CDialogBar)
	//{{AFX_MSG_MAP(CDlgBarExBottomAcq)
	ON_MESSAGE(WM_INITDIALOG,OnInitDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgBarExBottomAcq message handlers

BOOL CDlgBarExBottomAcq::OnInitDialog(UINT wParam,LONG lParam) 
{
	//CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	BOOL bRet = HandleInitDialog(wParam,lParam);
	if (!UpdateData(FALSE))
	{
		TRACE("InitCDataStatus Failed£¡");
	}


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

// DlgBarExBottomPlay.cpp : implementation file
//

#include "stdafx.h"
#include "recorder.h"
#include "DlgBarExBottomPlay.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
#include "RecorderDoc.h"
#include "MainFrm.h"
#include "Product.h"
#include "PlayBar.h"
/////////////////////////////////////////////////////////////////////////////
extern CEvent g_TransAndPlayEvent;
extern CEvent g_PlayPauseEvent;
//////////////////////////////////////////////////////////////////////////
// CDlgBarExBottomPlay dialog


CDlgBarExBottomPlay::CDlgBarExBottomPlay(CWnd* pParent /*=NULL*/)
{
	//{{AFX_DATA_INIT(CDlgBarExBottomPlay)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pPlayBar = NULL;

}


void CDlgBarExBottomPlay::DoDataExchange(CDataExchange* pDX)
{

	//{{AFX_DATA_MAP(CDlgBarExBottomPlay)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgBarExBottomPlay, CDialogBar)
	//{{AFX_MSG_MAP(CDlgBarExBottomPlay)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_RADIO_Play, OnRADIOPlay)
	ON_BN_CLICKED(IDC_RADIO_Pause, OnRADIOPause)
	ON_BN_CLICKED(IDC_BUTTON_SpeedUp, OnBUTTONSpeedUp)
	ON_MESSAGE(WM_INITDIALOG,OnInitDialog)
	ON_BN_CLICKED(IDC_BUTTON_SpeedDown, OnBUTTONSpeedDown)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgBarExBottomPlay message handlers

BOOL CDlgBarExBottomPlay::OnInitDialog(UINT wParam,LONG lParam) 
{
	//CDialogBar::OnInitDialog();
	
	// TODO: Add extra initialization here
	BOOL bRet = HandleInitDialog(wParam,lParam);
	if (!UpdateData(FALSE))
	{
		TRACE("InitCDataStatus Failed！");
	}

/////////////////////初始化其上控件/////////////////////////////////////////////////////
	HBITMAP hBitmap;   
	hBitmap = (HBITMAP)::LoadImage(NULL,
		_T("bmp\\bmp_PlayBar\\PlayBar_Play.bmp"),  //资源号或本地文件名
		IMAGE_BITMAP,       //装载位图 IMAGE_CURSOR光标 IMAGE_ICON图标
        0,                  //宽度 0为默认大小
        0,                  //高度 像素为单位
        LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);   							
	((CButton *)GetDlgItem(IDC_RADIO_Play))->SetBitmap(hBitmap);  
  
	hBitmap = (HBITMAP)::LoadImage(NULL,
		_T("bmp\\bmp_PlayBar\\PlayBar_Pause.bmp"),  //资源号或本地文件名
		IMAGE_BITMAP,       //装载位图 IMAGE_CURSOR光标 IMAGE_ICON图标
        0,                  //宽度 0为默认大小
        0,                  //高度 像素为单位
        LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);   	
	((CButton *)GetDlgItem(IDC_RADIO_Pause))->SetBitmap(hBitmap);  
	((CButton *)GetDlgItem(IDC_RADIO_Pause))->SetCheck(true);


	hBitmap = (HBITMAP)::LoadImage(NULL,
		_T("bmp\\bmp_PlayBar\\PlayBar_SpeedUp.bmp"),  //资源号或本地文件名
		IMAGE_BITMAP,       //装载位图 IMAGE_CURSOR光标 IMAGE_ICON图标
        0,                  //宽度 0为默认大小
        0,                  //高度 像素为单位
        LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);   		
	((CButton *)GetDlgItem(IDC_BUTTON_SpeedUp))->SetBitmap(hBitmap);  

	hBitmap = (HBITMAP)::LoadImage(NULL,
		_T("bmp\\bmp_PlayBar\\PlayBar_SpeedDown.bmp"),  //资源号或本地文件名
		IMAGE_BITMAP,       //装载位图 IMAGE_CURSOR光标 IMAGE_ICON图标
        0,                  //宽度 0为默认大小
        0,                  //高度 像素为单位
        LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);   	
	((CButton *)GetDlgItem(IDC_BUTTON_SpeedDown))->SetBitmap(hBitmap);  


	//播放条
	CRect rect;  
	GetClientRect(&rect);
	int nWidth = rect.Width();
	rect.left = (long)(nWidth*0.3) ;
	rect.right = (long)(nWidth*0.8) ;
	rect.top = rect.top+5;
	rect.bottom = rect.bottom-5;

	m_pPlayBar=new CPlayBar ;  
    m_pPlayBar->Create("non",NULL,WS_CHILDWINDOW | WS_VISIBLE, rect,this,457);  

	//状态文字
	rect.left = (long)(nWidth*0.85) ;
	rect.right = nWidth ;
	rect.top = rect.top+5;
	rect.bottom = rect.bottom-5;
	GetDlgItem(IDC_STATIC_AcqStatus)->MoveWindow(rect);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgBarExBottomPlay::OnSize(UINT nType, int cx, int cy) 
{
	CDialogBar::OnSize(nType, cx, cy);
	// TODO: Add your message handler code here
	CRect Rect; 
	if (m_pPlayBar != NULL)
	{
		m_pPlayBar->GetWindowRect(Rect);
		ScreenToClient(Rect);
		
		Rect.left = 130;
		Rect.right = cx - 200;
		
		m_pPlayBar->MoveWindow(Rect);
	}

	if (GetDlgItem(IDC_STATIC_AcqStatus) != NULL)
	{
		Rect.left = cx - 180 ;
		Rect.right = cx ;
		Rect.top = Rect.top+5;
		Rect.bottom = Rect.bottom-5;
		GetDlgItem(IDC_STATIC_AcqStatus)->MoveWindow(Rect);
	}

}

void CDlgBarExBottomPlay::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	if (m_pPlayBar != NULL)
	{
		delete m_pPlayBar;
		m_pPlayBar = NULL;
	}

	CDialogBar::PostNcDestroy();
}

void CDlgBarExBottomPlay::OnRADIOPlay() 
{
	// TODO: Add your control notification handler code here
	g_PlayPauseEvent.SetEvent();
	((CButton *)GetDlgItem(IDC_RADIO_Play))->SetCheck(true);
	((CButton *)GetDlgItem(IDC_RADIO_Pause))->SetCheck(false);
}

void CDlgBarExBottomPlay::OnRADIOPause() 
{
	// TODO: Add your control notification handler code here	
	g_PlayPauseEvent.ResetEvent();
	((CButton *)GetDlgItem(IDC_RADIO_Play))->SetCheck(false);
	((CButton *)GetDlgItem(IDC_RADIO_Pause))->SetCheck(true);
}

void CDlgBarExBottomPlay::OnBUTTONSpeedUp() 
{
	// TODO: Add your control notification handler code here
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CRecorderDoc* pDoc = (CRecorderDoc*)(pFrame->GetActiveDocument());
	CProduct* pProduct = &(pDoc->m_Product);

	pProduct->m_PlayCtrlInfo.nSpeed = pProduct->m_PlayCtrlInfo.nSpeed * 2;
	if (pProduct->m_PlayCtrlInfo.nSpeed > 1024)
	{
		pProduct->m_PlayCtrlInfo.nSpeed = 1024;
	}
}

void CDlgBarExBottomPlay::OnBUTTONSpeedDown() 
{
	// TODO: Add your control notification handler code here
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CRecorderDoc* pDoc = (CRecorderDoc*)(pFrame->GetActiveDocument());
	CProduct* pProduct = &(pDoc->m_Product);
	
	pProduct->m_PlayCtrlInfo.nSpeed = pProduct->m_PlayCtrlInfo.nSpeed / 2;
	if (pProduct->m_PlayCtrlInfo.nSpeed < 1)
	{
		pProduct->m_PlayCtrlInfo.nSpeed = 1;
	}
}

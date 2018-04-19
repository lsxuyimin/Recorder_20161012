// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "Recorder.h"

#include "MainFrm.h"

#include "RecorderDoc.h"
#include "RecorderView.h"
#include "PopupRightDlg.h"
#include "DlgBarEx.h"
#include "DlgBarExBottomPlay.h"
#include "Product.h"
#include "CheckCardDlg.h"
#include "SaveDlg.h"
#include "SaveAsDlg.h"
#include "PlayBar.h"
#include "WndManage.h"
#include "BgDlg.h"
#include "WatchDlg.h"
#include "MeasureDlg.h"
#include "DlgBarExTopChnSel.h"
#include "FftConfDlg.h"
#include "MarkDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
extern CEvent g_PlayPauseEvent;
//////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_CTRL_StartAcq, OnCTRLStartAcq)
	ON_COMMAND(ID_CTRL_StopAcq, OnCTRLStopAcq)
	ON_COMMAND(ID_LAYOUT_Mode1, OnLAYOUTMode1)
	ON_COMMAND(ID_LAYOUT_Mode2, OnLAYOUTMode2)
	ON_COMMAND(ID_LAYOUT_Mode3, OnLAYOUTMode3)
	ON_COMMAND(ID_LAYOUT_Mode4, OnLAYOUTMode4)
	ON_COMMAND(ID_WATCH_NextPage, OnWATCHNextPage)
	ON_COMMAND(ID_WATCH_PrePage, OnWATCHPrePage)
	ON_COMMAND(ID_WATCH_Setting, OnWATCHSetting)
	ON_UPDATE_COMMAND_UI(ID_WATCH_ZoomIn, OnUpdateWATCHZoomIn)
	ON_COMMAND(ID_WATCH_ZoomIn, OnWATCHZoomIn)
	ON_COMMAND(ID_WATCH_ZoomOut, OnWATCHZoomOut)
	ON_UPDATE_COMMAND_UI(ID_WATCH_ZoomOut, OnUpdateWATCHZoomOut)
	ON_COMMAND(ID_WATCH_None, OnWATCHNone)
	ON_UPDATE_COMMAND_UI(ID_WATCH_None, OnUpdateWATCHNone)
	ON_UPDATE_COMMAND_UI(ID_CTRL_StartAcq, OnUpdateCTRLStartAcq)
	ON_UPDATE_COMMAND_UI(ID_CTRL_StopAcq, OnUpdateCTRLStopAcq)
	ON_COMMAND(ID_WATCH_ZoomDrag, OnWATCHZoomDrag)
	ON_UPDATE_COMMAND_UI(ID_WATCH_ZoomDrag, OnUpdateWATCHZoomDrag)
	ON_MESSAGE(WM_ACQEND, OnAcqEnd)
	ON_MESSAGE(WM_INACQ, OnInAcq)
	ON_MESSAGE(WM_WAITACQ, OnWaitAcq)
	ON_MESSAGE(WM_UPDATE_CACHE, OnUpdateCache)
	ON_MESSAGE(WM_UPDATE_PLAY, OnUpdatePlay)
	ON_COMMAND(ID_APP_CONNECT, OnAppConnect)
	ON_UPDATE_COMMAND_UI(ID_WATCH_Setting, OnUpdateWATCHSetting)
	ON_COMMAND(ID_APP_Help, OnAPPHelp)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	ON_COMMAND(ID_FILE_SAVE_Costum, OnFILESAVECostum)
	ON_COMMAND(ID_FILE_SAVE_AS_Costum, OnFILESAVEASCostum)
	ON_COMMAND(ID_WATCH_ZoomReStore, OnWATCHZoomReStore)
	ON_UPDATE_COMMAND_UI(ID_WATCH_ZoomReStore, OnUpdateWATCHZoomReStore)
	ON_COMMAND(ID_MEA_ADDMEASURE, OnMeaAddmeasure)
	ON_COMMAND(ID_MEA_DELMEASURE, OnMeaDelmeasure)
	ON_COMMAND(ID_MEA_EDITMEASURE, OnMeaEditmeasure)
	ON_UPDATE_COMMAND_UI(ID_MEA_EDITMEASURE, OnUpdateMeaEditmeasure)
	ON_UPDATE_COMMAND_UI(ID_MEA_DELMEASURE, OnUpdateMeaDelmeasure)
	ON_COMMAND(ID_WATCH_FreqDomain, OnWATCHFreqDomain)
	ON_COMMAND(ID_WATCH_FreqDomainConf, OnWATCHFreqDomainConf)
	ON_COMMAND(ID_WATCH_TimeDomain, OnWATCHTimeDomain)
	ON_UPDATE_COMMAND_UI(ID_WATCH_FreqDomain, OnUpdateWATCHFreqDomain)
	ON_UPDATE_COMMAND_UI(ID_WATCH_TimeDomain, OnUpdateWATCHTimeDomain)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	m_aMouseModeStatus[0] = 1;
	m_aMouseModeStatus[1] = 0;
	m_aMouseModeStatus[2] = 0;
	m_aMouseModeStatus[3] = 0;
	m_aMouseModeStatus[4] = 0;
	m_nMouseMode = MOUSEMODE_NONE;

	m_aPlayCtrlStatus[0] = 0;
	m_aPlayCtrlStatus[1] = 1;
//	m_aPlayCtrlStatus[2] = 0;

	m_aWatchMode[0] = 1;
	m_aWatchMode[1] = 0;



	m_bTip = false;

	m_pDlgBarEx = new CDlgBarEx(this);
	m_pDlgBarExTopChnSel = new CDlgBarExTopChnSel(this);
	m_pDlgBarExBottomPlay = new CDlgBarExBottomPlay;
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	//CDialogBar & CReBar    add by HANCHEN   20141124
	//客户区上方的CReBar
	if (!m_pDlgBarEx->Create(this,IDD_DIALOG_BarEx,WS_VISIBLE|WS_CHILD
		|CBRS_SIZE_DYNAMIC|CBRS_BOTTOM,IDD_DIALOG_BarEx))
	{
		TRACE0("Failed to create CDataStatus bar!");
		return -1;
	}

	if (!m_pDlgBarExTopChnSel->Create(this,IDD_DIALOG_BarExTopChnSel,WS_VISIBLE|WS_CHILD
		|CBRS_SIZE_DYNAMIC|CBRS_BOTTOM,IDD_DIALOG_BarExTopChnSel))
	{
		TRACE0("Failed to create CDataStatus bar!");
		return -1;
	}

	if (!m_wndReBar.Create(this,RBS_BANDBORDERS,WS_CHILD |
		WS_VISIBLE| CBRS_TOP|WS_CLIPSIBLINGS|WS_CLIPCHILDREN))
	{
		TRACE0("Failed to create  Rebar /n");
		return -1;
    }
	m_wndReBar.AddBar(m_pDlgBarEx,NULL,NULL,RBBS_GRIPPERALWAYS|RBBS_FIXEDBMP);
	m_wndReBar.AddBar(m_pDlgBarExTopChnSel,NULL,NULL,RBBS_GRIPPERALWAYS|RBBS_FIXEDBMP);
	//客户区下方的第一个CReBar
/*
	if (!m_DlgBarExBottomAcq.Create(this,IDD_DIALOG_BarExBottomAcq,WS_VISIBLE|WS_CHILD
		|CBRS_SIZE_DYNAMIC|CBRS_BOTTOM,IDD_DIALOG_BarEx))
	{
		TRACE0("Failed to create CDataStatus bar!");
		return -1;
	}
	if (!m_wndReBarBottomAcq.Create(this,RBS_BANDBORDERS,WS_CHILD |
		WS_VISIBLE| CBRS_BOTTOM|WS_CLIPSIBLINGS|WS_CLIPCHILDREN))
	{
		TRACE0("Failed to create  Rebar /n");
		return -1;
    }
	m_wndReBarBottomAcq.AddBar(&m_DlgBarExBottomAcq,NULL,NULL,RBBS_GRIPPERALWAYS|RBBS_FIXEDBMP);
	m_DlgBarExBottomAcq.ShowWindow(SW_HIDE);
*/
	//客户区下方的第二个CReBar
	if (!m_pDlgBarExBottomPlay->Create(this,IDD_DIALOG_BarExBottomPlay,WS_VISIBLE|WS_CHILD
		|CBRS_SIZE_DYNAMIC|CBRS_BOTTOM,IDD_DIALOG_BarEx))
	{
		TRACE0("Failed to create CDataStatus bar!");
		return -1;
	}
/**/
	if (!m_wndReBarBottomPlay.Create(this,RBS_BANDBORDERS,WS_CHILD |
		WS_VISIBLE| CBRS_BOTTOM|WS_CLIPSIBLINGS|WS_CLIPCHILDREN))
	{
		TRACE0("Failed to create  Rebar /n");
		return -1;
    }

	m_wndReBarBottomPlay.AddBar(m_pDlgBarExBottomPlay,NULL,NULL,RBBS_GRIPPERALWAYS|RBBS_FIXEDBMP);
	m_pDlgBarExBottomPlay->ShowWindow(SW_SHOW);

	//初始化菜单中的图标
	InitialMenuIcon();

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	cs.lpszName = _T("测试标题");
	cs.cx = 1200 ;
	cs.cy = 1200 ; 
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers


void CMainFrame::OnCTRLStartAcq() 
{
	CProduct* pProduct = &(((CRecorderDoc*)GetActiveDocument())->m_Product);

	if (1 == m_aPlayCtrlStatus[0])
	{
		//采集按钮已经是按下状态
		return;
	}
	// TODO: Add your command handler code here
	if (m_bTip == false && pProduct->m_nRecorderMode==1)
	{
		MessageBox("当前使用模拟波形！","提示",MB_ICONWARNING);
		m_bTip = true;
	}
	m_aPlayCtrlStatus[0] = 1;
	m_aPlayCtrlStatus[1] = 0;
//	m_aPlayCtrlStatus[2] = 0;
	//start
	//产生数据
	((CRecorderDoc*)GetActiveDocument())->m_Product.StartCapture();		//打开传输线程 播放线程
	//设置定时器
//	((CRecorderView*)GetActiveView())->SetTimer(1,200,NULL);
}

void CMainFrame::OnCTRLStopAcq() 
{
	// TODO: Add your command handler code here
	if (1 == m_aPlayCtrlStatus[1])
	{
		//停止按钮已经是按下状态
		return;
	}

	m_aPlayCtrlStatus[0] = 0;
	m_aPlayCtrlStatus[1] = 1;
//	m_aPlayCtrlStatus[2] = 0;
	//end
	//停止产生数据
	((CRecorderDoc*)GetActiveDocument())->m_Product.StopCapture();
	//关闭定时器
	((CRecorderView*)GetActiveView())->KillTimer(1);

}

void CMainFrame::OnLAYOUTMode1() 
{
	// TODO: Add your command handler code here
	((CRecorderView*)GetActiveView())->m_WndManage.Display(1);
}

void CMainFrame::OnLAYOUTMode2() 
{
	// TODO: Add your command handler code here
	((CRecorderView*)GetActiveView())->m_WndManage.Display(2);
}

void CMainFrame::OnLAYOUTMode3() 
{
	// TODO: Add your command handler code here
	((CRecorderView*)GetActiveView())->m_WndManage.Display(3);
	
}

void CMainFrame::OnLAYOUTMode4() 
{
	// TODO: Add your command handler code here
	((CRecorderView*)GetActiveView())->m_WndManage.Display(4);
	
}

void CMainFrame::OnWATCHNextPage() 
{
	// TODO: Add your command handler code here
	CRecorderView* pView = ((CRecorderView*)GetActiveView());
	int mode = pView->m_WndManage.m_nCurMode;
	pView->m_WndManage.m_nFirst += mode * mode;
	if (pView->m_WndManage.m_nFirst > MAX_CHANNUM - mode * mode)
	{
		pView->m_WndManage.m_nFirst = MAX_CHANNUM - mode * mode;
	}
	pView->m_WndManage.m_nCurSelect = pView->m_WndManage.m_nFirst;
	pView->m_WndManage.Postion();

	//找到当前的选中的CWatchDlg,并将它的信息更新到toolbar上
	RefreshRangeToolBar();

}

void CMainFrame::OnWATCHPrePage() 
{
	// TODO: Add your command handler code here
	CRecorderView* pView = ((CRecorderView*)GetActiveView());
	int mode = pView->m_WndManage.m_nCurMode;
	pView->m_WndManage.m_nFirst -= mode * mode;
	if (pView->m_WndManage.m_nFirst < 0)
	{
		pView->m_WndManage.m_nFirst = 0;
	}
	pView->m_WndManage.m_nCurSelect = pView->m_WndManage.m_nFirst;
	pView->m_WndManage.Postion();

	//找到当前的选中的CWatchDlg,并将它的信息更新到toolbar上
	RefreshRangeToolBar();
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (m_pDlgBarEx->OnCmdMsg(nID,nCode,pExtra,pHandlerInfo))
		return TRUE;
/*
	if (m_DlgBarExBottomAcq.OnCmdMsg(nID,nCode,pExtra,pHandlerInfo))
		return TRUE;
*/
	if (m_pDlgBarExTopChnSel->OnCmdMsg(nID,nCode,pExtra,pHandlerInfo))
		return TRUE;

	if (m_pDlgBarExBottomPlay->OnCmdMsg(nID,nCode,pExtra,pHandlerInfo))
		return TRUE;

	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CMainFrame::OnWATCHSetting() 
{
	// TODO: Add your command handler code here
	//弹出左侧对话框
	m_pDlgPopupRight = new CPopupRightDlg;
	m_pDlgPopupRight->DoModal();
	delete m_pDlgPopupRight;
	m_pDlgPopupRight = NULL;
}

void CMainFrame::OnUpdateWATCHZoomIn(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_aMouseModeStatus[1]);
}

void CMainFrame::OnWATCHZoomIn() 
{
	// TODO: Add your command handler code here
	m_aMouseModeStatus[1] = 1;
	m_nMouseMode = MOUSEMODE_ZOOMIN;
	
	m_aMouseModeStatus[0] = 0;
	m_aMouseModeStatus[2] = 0;
	m_aMouseModeStatus[3] = 0;
	m_aMouseModeStatus[4] = 0;
}

void CMainFrame::OnWATCHZoomOut() 
{
	// TODO: Add your command handler code here
	m_aMouseModeStatus[2] = 1;
	m_nMouseMode = MOUSEMODE_ZOOMOUT;
	
	m_aMouseModeStatus[0] = 0;
	m_aMouseModeStatus[1] = 0;
	m_aMouseModeStatus[3] = 0;
	m_aMouseModeStatus[4] = 0;
}

void CMainFrame::OnUpdateWATCHZoomOut(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_aMouseModeStatus[2]);
}

void CMainFrame::OnWATCHNone() 
{
	// TODO: Add your command handler code here
	m_aMouseModeStatus[0] = 1;
	m_nMouseMode = MOUSEMODE_NONE;

	m_aMouseModeStatus[1] = 0;
	m_aMouseModeStatus[2] = 0;
	m_aMouseModeStatus[3] = 0;
	m_aMouseModeStatus[4] = 0;
	
}

void CMainFrame::OnUpdateWATCHNone(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_aMouseModeStatus[0]);
}

void CMainFrame::OnUpdateCTRLStartAcq(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_aPlayCtrlStatus[0]);
	
	/*如果是Recorder模式 就没有播放暂停*/
	int nMode = ((CRecorderDoc*)(((CMainFrame *)AfxGetMainWnd())->GetActiveDocument()))->m_Product.m_nRecorderMode;
	pCmdUI->Enable(2 != nMode);
}

void CMainFrame::OnUpdateCTRLStopAcq(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_aPlayCtrlStatus[1]);

	/*如果是Recorder模式 就没有播放暂停*/
	int nMode = ((CRecorderDoc*)(((CMainFrame *)AfxGetMainWnd())->GetActiveDocument()))->m_Product.m_nRecorderMode;
	pCmdUI->Enable(2 != nMode);
}

void CMainFrame::ChangeToolBar(int mode)
{
	//更新CDlgBarEx上的 触发通道
	int OpenChannelNum = ((CRecorderDoc*)(((CMainFrame *)AfxGetMainWnd())->GetActiveDocument()))->m_Product.m_nOpenChannelNum;
	int nCount = 0;
	CString strTemp;
	m_pDlgBarEx->m_ComboTriggerChn.Clear();
	for (nCount=0 ; nCount<OpenChannelNum ; nCount++)
	{
		strTemp.Format("ch%d",nCount+1);
		m_pDlgBarEx->m_ComboTriggerChn.AddString(strTemp);
	}
	m_pDlgBarEx->m_ComboTriggerChn.SetCurSel(0);

	//更新CDlgBarTopChnSel
	//先将“关闭”写入，再将m_WaveList和m_CombWaveList中的通道写入
/*
	CComboBox* pComboDiaplayWave = NULL;
	for (nCount=0 ; nCount<4 ; nCount++)
	{
	}
*/


	//更新工具栏上的按钮的状态

	switch (mode)
	{
	case 0:
		TRACE("ChangeToolBar MODE PHYCARD\n");
		
		break;
	case 1:
		TRACE("ChangeToolBar MODE DEMO\n");

		break;
	case 2:
		TRACE("ChangeToolBar MODE PLAY\n");

		break;
	default:
		TRACE("Initialize MODE NODEFINE\n");
		break;
	}

	

}

void CMainFrame::OnWATCHZoomDrag() 
{
	// TODO: Add your command handler code here
	m_aMouseModeStatus[3] = 1;
	m_nMouseMode = MOUSEMODE_ZOOMDRAG;
	
	m_aMouseModeStatus[0] = 0;
	m_aMouseModeStatus[1] = 0;
	m_aMouseModeStatus[2] = 0;
	m_aMouseModeStatus[4] = 0;

}

void CMainFrame::OnUpdateWATCHZoomDrag(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_aMouseModeStatus[3]);

	pCmdUI->Enable(FALSE);
}

LRESULT CMainFrame::OnAcqEnd(WPARAM wParam, LPARAM lParam)
{
	m_pDlgBarExBottomPlay->GetDlgItem(IDC_STATIC_AcqStatus)->SetWindowText("缓存数据");
	return 0;
}


LRESULT CMainFrame::OnInAcq(WPARAM wParam, LPARAM lParam)
{
	m_pDlgBarExBottomPlay->GetDlgItem(IDC_STATIC_AcqStatus)->SetWindowText("数据采集中");
	return 0;
}

LRESULT CMainFrame::OnWaitAcq(WPARAM wParam, LPARAM lParam)
{
	m_pDlgBarExBottomPlay->GetDlgItem(IDC_STATIC_AcqStatus)->SetWindowText("等待采集...");
	return 0;
}

LRESULT CMainFrame::OnUpdateCache(WPARAM wParam, LPARAM lParam)
{
	//wParam为m_nSegmentValidIndex
	int nCachePos = (int)wParam;
//	TRACE("-------OnUpdateCache:wParam--%d------------------\n",nCachePos);
	m_pDlgBarExBottomPlay->m_pPlayBar->SetCachePos(nCachePos);
	return 0;
}

LRESULT CMainFrame::OnUpdatePlay(WPARAM wParam, LPARAM lParam)
{	
	int nPlayPos = (int)wParam;
//	TRACE("-------OnUpdatePlay:wParam--%d------------------\n",nPlayPos);
	m_pDlgBarExBottomPlay->m_pPlayBar->SetPlayPos(nPlayPos);
	return 0;
}

void CMainFrame::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	if (m_pDlgBarEx != NULL)
	{
		delete m_pDlgBarEx;
	}
	m_pDlgBarEx = NULL;

	if (m_pDlgBarExTopChnSel != NULL)
	{
		delete m_pDlgBarExTopChnSel;
	}
	m_pDlgBarExTopChnSel = NULL;

	if (m_pDlgBarExBottomPlay != NULL)
	{
		delete m_pDlgBarExBottomPlay;
	}
	m_pDlgBarExBottomPlay = NULL;

	CFrameWnd::PostNcDestroy();
}

void CMainFrame::OnAppConnect() 
{
	// TODO: Add your command handler code here

//	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CRecorderDoc *pDoc = (CRecorderDoc*)(GetActiveDocument());
	CProduct* pProduct = &(pDoc->m_Product);

	pProduct->ChechCard();
//	pProduct->Initialize();
}

void CMainFrame::OnUpdateWATCHSetting(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	int nMode = ((CRecorderDoc*)(((CMainFrame *)AfxGetMainWnd())->GetActiveDocument()))->m_Product.m_nRecorderMode;
	pCmdUI->Enable(1 == nMode);
}

void CMainFrame::OnAPPHelp() 
{
	// TODO: Add your command handler code here
	
	//弹出帮助文档
	CRecorderDoc *pDoc = (CRecorderDoc*)(GetActiveDocument());
	CProduct* pProduct = &(pDoc->m_Product);


	CString strTemp;
	int nPos;   
	nPos=pProduct->m_strWorkPlace.ReverseFind('\\');   
	strTemp=pProduct->m_strWorkPlace.Left(nPos); 
//	strTemp = "/c md " + strTemp + "\\help.docx";
	strTemp = strTemp + "\\help.doc";

//	ShellExecute(NULL,"open","cmd.exe","/c md d:\\123123","",SW_HIDE);
	ShellExecute(NULL,"open",strTemp , "" ,"",SW_SHOW);
}

void CMainFrame::OnFileOpen() 
{
	// TODO: Add your command handler code here
	CRecorderDoc *pDoc = (CRecorderDoc*)(GetActiveDocument());
	CProduct* pProduct = &(pDoc->m_Product);

	CCheckCardDlg dlg(pProduct ,NULL , 2);



	if (IDOK == dlg.DoModal())
	{
		//进行判断 做三种处理
		switch (pProduct->m_nRecorderMode)
		{
		case 0:
			//物理卡
			pProduct->Initialize();
			break;
		case 1:
			//模拟卡
			pProduct->Initialize();
			break;
		case 2:
			//播放已经存在文件
			pProduct->PlayEnvInitialize();
			break;
		}
	}
}

void CMainFrame::OnFileSave() 
{
	// TODO: Add your command handler code here
	CRecorderDoc *pDoc = (CRecorderDoc*)(GetActiveDocument());
	CProduct* pProduct = &(pDoc->m_Product);


	CSaveDlg saveDlg(this);
	if (IDOK == saveDlg.DoModal())
	{
		//将temp文件夹中的文件全部拷贝到m_strSavePath下
		//m_strCurWorkPath -> m_strSavePath
		CString strTemp;
		CString strSrc;
		CString strDest;
		int nCount=0;
		for(nCount=0 ; nCount<pProduct->m_nOpenChannelNum ; nCount++)
		{
			strTemp.Format("Ch%d",nCount+1);
			strSrc = pProduct->m_strCurWorkPath + "\\" + strTemp;
			strDest = pProduct->m_strSavePath + "\\" + strTemp;
			CopyFile(strSrc, strDest , FALSE);
		}
		
		strTemp.Format("Setup.inf");
		strSrc = pProduct->m_strCurWorkPath + "\\" + strTemp;
		strDest = pProduct->m_strSavePath + "\\" + strTemp;
		CopyFile(strSrc, strDest , FALSE);


	}


}

void CMainFrame::OnFILESAVECostum() 
{
	// TODO: Add your command handler code here
	CRecorderDoc *pDoc = (CRecorderDoc*)(GetActiveDocument());
	CProduct* pProduct = &(pDoc->m_Product);
	
	
	CSaveDlg saveDlg(this);
	if (IDOK == saveDlg.DoModal())
	{
		//将temp文件夹中的文件全部拷贝到m_strSavePath下
		//m_strCurWorkPath -> m_strSavePath
		CString strTemp;
		CString strSrc;
		CString strDest;
		int nCount=0;
		for(nCount=0 ; nCount<pProduct->m_nOpenChannelNum ; nCount++)
		{
			strTemp.Format("Ch%d",nCount+1);
			strSrc = pProduct->m_strCurWorkPath + "\\" + strTemp;
			strDest = pProduct->m_strSavePath + "\\" + strTemp;
			CopyFile(strSrc, strDest , FALSE);
		}
		
		strTemp.Format("Setup.inf");
		strSrc = pProduct->m_strCurWorkPath + "\\" + strTemp;
		strDest = pProduct->m_strSavePath + "\\" + strTemp;
		CopyFile(strSrc, strDest , FALSE);
		
		
	}

}

void CMainFrame::OnFILESAVEASCostum() 
{
	// TODO: Add your command handler code here
	CRecorderDoc *pDoc = (CRecorderDoc*)(GetActiveDocument());
	CProduct* pProduct = &(pDoc->m_Product);
	
	
	CSaveAsDlg saveDlg(this);
	if (IDOK == saveDlg.DoModal())
	{
		//将m_strCurWorkPath文件夹中的文件全部拷贝到m_strSavePath下
		//m_strCurWorkPath -> m_strSavePath
		CString strTemp;
		CString strSrc;
		CString strDest;
		int nCount=0;
		for(nCount=0 ; nCount<pProduct->m_nOpenChannelNum ; nCount++)
		{
			strTemp.Format("Ch%d",nCount+1);
			strSrc = pProduct->m_strCurWorkPath + "\\" + strTemp;
			strDest = pProduct->m_strSavePath + "\\" + strTemp;
			CopyFile(strSrc, strDest , FALSE);
		}
		
		strTemp.Format("Setup.inf");
		strSrc = pProduct->m_strCurWorkPath + "\\" + strTemp;
		strDest = pProduct->m_strSavePath + "\\" + strTemp;
		CopyFile(strSrc, strDest , FALSE);
		
		
	}

}

void CMainFrame::ViewWndManageRefresh()
{	
	((CRecorderView*)(GetActiveView()))->WndManageRefresh();

}

void CMainFrame::OnWATCHZoomReStore() 
{
	// TODO: Add your command handler code here
	m_aMouseModeStatus[4] = 1;
	m_nMouseMode = MOUSEMODE_RESTORE;
	
	m_aMouseModeStatus[0] = 0;
	m_aMouseModeStatus[1] = 0;
	m_aMouseModeStatus[2] = 0;
	m_aMouseModeStatus[3] = 0;
	

}

void CMainFrame::OnUpdateWATCHZoomReStore(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_aMouseModeStatus[4]);
}

void CMainFrame::OnMeaAddmeasure() 
{
	// TODO: Add your command handler code here
	
	CRecorderView* pView = (CRecorderView*)(GetActiveView());
	CBgDlg* pBgDlg = &(pView->m_WndManage.m_pBgDlg[pView->m_WndManage.m_nCurSelect]);
	
	int nCurSeleted;
	nCurSeleted = pBgDlg->m_TabBgDlg.GetCurSel();
	CWatchDlg* pWatchDlgCurSel;
	pWatchDlgCurSel = (CWatchDlg*)pBgDlg->m_WatchDlgList.GetAt(pBgDlg->m_WatchDlgList.FindIndex(nCurSeleted));
	pWatchDlgCurSel->MeasureAdd();


}

void CMainFrame::OnMeaDelmeasure() 
{
	// TODO: Add your command handler code here
	CRecorderView* pView = (CRecorderView*)(GetActiveView());
	CBgDlg* pBgDlg = &(pView->m_WndManage.m_pBgDlg[pView->m_WndManage.m_nCurSelect]);
	
	int nCurSeleted;
	nCurSeleted = pBgDlg->m_TabBgDlg.GetCurSel();
	CWatchDlg* pWatchDlgCurSel;
	pWatchDlgCurSel = (CWatchDlg*)pBgDlg->m_WatchDlgList.GetAt(pBgDlg->m_WatchDlgList.FindIndex(nCurSeleted));
	pWatchDlgCurSel->m_pMeasureDlg->MeasureDelete();
	
}

void CMainFrame::OnMeaEditmeasure() 
{
	// TODO: Add your command handler code here
	CRecorderView* pView = (CRecorderView*)(GetActiveView());
	CBgDlg* pBgDlg = &(pView->m_WndManage.m_pBgDlg[pView->m_WndManage.m_nCurSelect]);
	
	int nCurSeleted;
	nCurSeleted = pBgDlg->m_TabBgDlg.GetCurSel();
	CWatchDlg* pWatchDlgCurSel;
	pWatchDlgCurSel = (CWatchDlg*)pBgDlg->m_WatchDlgList.GetAt(pBgDlg->m_WatchDlgList.FindIndex(nCurSeleted));
	pWatchDlgCurSel->MeasureEdit();
}

void CMainFrame::OnUpdateMeaEditmeasure(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CRecorderView* pView = (CRecorderView*)(GetActiveView());
	CBgDlg* pBgDlg = &(pView->m_WndManage.m_pBgDlg[pView->m_WndManage.m_nCurSelect]);
	
	int nCurSeleted;
	nCurSeleted = pBgDlg->m_TabBgDlg.GetCurSel();
	CWatchDlg* pWatchDlgCurSel;
	pWatchDlgCurSel = (CWatchDlg*)pBgDlg->m_WatchDlgList.GetAt(pBgDlg->m_WatchDlgList.FindIndex(nCurSeleted));
	
	
	pCmdUI->Enable(pWatchDlgCurSel->m_pMeasureDlg != NULL);
}

void CMainFrame::OnUpdateMeaDelmeasure(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CRecorderView* pView = (CRecorderView*)(GetActiveView());
	CBgDlg* pBgDlg = &(pView->m_WndManage.m_pBgDlg[pView->m_WndManage.m_nCurSelect]);
	
	int nCurSeleted;
	nCurSeleted = pBgDlg->m_TabBgDlg.GetCurSel();
	CWatchDlg* pWatchDlgCurSel;
	pWatchDlgCurSel = (CWatchDlg*)pBgDlg->m_WatchDlgList.GetAt(pBgDlg->m_WatchDlgList.FindIndex(nCurSeleted));
	
	
	pCmdUI->Enable(pWatchDlgCurSel->m_pMeasureDlg != NULL);
}


void CMainFrame::InitialMenuIcon()
{
//	CBitmap bmpTemp;
//	bmp.LoadBitmap( LPCTSTR lpszResourceName );
//	m_bmpTest.LoadBitmap(IDB_BITMAP1);
	m_bmpPrePage.Attach((HBITMAP)LoadImage(NULL, _T("bmp\\菜单图标\\上一页.bmp"), IMAGE_BITMAP, NULL, NULL, LR_LOADFROMFILE));	
	GetMenu()->GetSubMenu(3)->SetMenuItemBitmaps(6,MF_BYPOSITION, &m_bmpPrePage, &m_bmpPrePage);
	m_bmpNextPage.Attach((HBITMAP)LoadImage(NULL, _T("bmp\\菜单图标\\下一页.bmp"), IMAGE_BITMAP, NULL, NULL, LR_LOADFROMFILE));	
	GetMenu()->GetSubMenu(3)->SetMenuItemBitmaps(7,MF_BYPOSITION, &m_bmpNextPage, &m_bmpNextPage);
}

int CMainFrame::RefreshRangeToolBar()
{

	CRecorderView* pView = ((CRecorderView*)GetActiveView());
	
	int nIndexBgDlg = pView->m_WndManage.m_nCurSelect;
	CBgDlg* pBgDlg = &(pView->m_WndManage.m_pBgDlg[nIndexBgDlg]);

	int nIndexWatchDlg = pBgDlg->m_TabBgDlg.GetCurSel();
	if ( nIndexWatchDlg < 0 )
	{
		TRACE("Bunny Tips : CMainFrame::RefreshRangeToolBar() nIndexWatchDlg error!!!\n");
		return -1;
	}
	CWatchDlg* pWatchDlg = pBgDlg->m_WatchDlgList.GetAt(pBgDlg->m_WatchDlgList.FindIndex(nIndexWatchDlg));

	m_pDlgBarExTopChnSel->ChangeToolBarDisplayChn(pWatchDlg);


	return 0;
}


void CMainFrame::OnWATCHTimeDomain() 
{
	// TODO: Add your command handler code here
	if ( m_aWatchMode[0] == 1 )
	{
		//时域分析的按钮已经被按下了
		return;
	}
	
	m_aWatchMode[0] = 1;
	m_aWatchMode[1] = 0;


	/*1. 如果在播放过程中，暂停播放*/
	CRecorderDoc *pDoc = (CRecorderDoc*)(GetActiveDocument());
	CProduct* pProduct = &(pDoc->m_Product);
	if (pProduct->m_bPlaying)
	{
		g_PlayPauseEvent.ResetEvent();
		Sleep(SLEEP_TIME);
	}

	/*2. 找到当前的CWatchDlg，改变它的m_nMode*/
	CWatchDlg* pWatchDlg;
	GetCurSelWatchDlg(&pWatchDlg);
	pWatchDlg->m_nMode = 0 ;
	
	pWatchDlg->SwitchTimeAndFreq(pWatchDlg->m_nMode);		//改变量程
	
	m_pDlgBarExTopChnSel->ChangeToolBarDisplayChn(pWatchDlg);

	/*清除CMarkDlg和CMeasureDlg的信息*/
	if ( pWatchDlg->m_pMarkDlg != NULL )
	{
		pWatchDlg->m_pMarkDlg->ClearMark();		
		pWatchDlg->m_pMarkDlg->ShowWindow(SW_HIDE);
	}
	if ( pWatchDlg->m_pMeasureDlg != NULL )
	{
		pWatchDlg->m_pMeasureDlg->SendMessage(WM_CLOSE);
		pWatchDlg->m_pMeasureDlg = NULL;
		pWatchDlg->ComputeFrame();
		pWatchDlg->ModifyPos();
	}


	if (!pProduct->m_bPlaying)
	{
		pWatchDlg->Invalidate();		//如果没有播放，那么立刻重绘
	}

	/*3. 继续播放*/
	if (pProduct->m_bPlaying)
	{
		g_PlayPauseEvent.SetEvent();
	}
}


void CMainFrame::OnWATCHFreqDomain() 
{
	// TODO: Add your command handler code here
	if ( m_aWatchMode[1] == 1 )
	{
		//频域分析的按钮已经被按下了
		return;
	}

	m_aWatchMode[0] = 0;
	m_aWatchMode[1] = 1;
	

	/*1. 如果在播放过程中，暂停播放*/
	CRecorderDoc *pDoc = (CRecorderDoc*)(GetActiveDocument());
	CProduct* pProduct = &(pDoc->m_Product);
	if (pProduct->m_bPlaying)
	{
		g_PlayPauseEvent.ResetEvent();
		Sleep(SLEEP_TIME);
	}

	/*2.找到当前的CWatchDlg，改变它的m_nMode*/
	CWatchDlg* pWatchDlg;
	GetCurSelWatchDlg(&pWatchDlg);
	pWatchDlg->m_nMode = 1 ;

	pWatchDlg->SwitchTimeAndFreq(pWatchDlg->m_nMode);		//改变量程

	m_pDlgBarExTopChnSel->ChangeToolBarDisplayChn(pWatchDlg);

	/*清除CMarkDlg和CMeasureDlg的信息*/
	if ( pWatchDlg->m_pMarkDlg != NULL )
	{
		pWatchDlg->m_pMarkDlg->ClearMark();		
		pWatchDlg->m_pMarkDlg->ShowWindow(SW_HIDE);
	}
	if ( pWatchDlg->m_pMeasureDlg != NULL )
	{
		pWatchDlg->m_pMeasureDlg->SendMessage(WM_CLOSE);
		pWatchDlg->m_pMeasureDlg = NULL;
		pWatchDlg->ComputeFrame();
		pWatchDlg->ModifyPos();
	}
	

	if (!pProduct->m_bPlaying)
	{
		pWatchDlg->Invalidate();		//如果没有播放，那么立刻重绘
	}

	/*3. 继续播放*/
	if (pProduct->m_bPlaying)
	{
		g_PlayPauseEvent.SetEvent();
	}
}

void CMainFrame::OnWATCHFreqDomainConf() 
{
	// TODO: Add your command handler code here

	CFftConfDlg *pFftConfDlg = new CFftConfDlg();
	pFftConfDlg->Create(IDD_DIALOG_FftConf,this);
	pFftConfDlg->ShowWindow(SW_SHOW);
	pFftConfDlg->InitialControl();

	CRect rcFftConfDlg;
	pFftConfDlg->GetClientRect(rcFftConfDlg);
	CRect rcMainFrame;
	
	this->GetClientRect(rcMainFrame);
	ClientToScreen(rcMainFrame);
	pFftConfDlg->MoveWindow(rcMainFrame.left+204 , rcMainFrame.top+39 , rcFftConfDlg.Width(), rcFftConfDlg.Height(), TRUE);

}



void CMainFrame::OnUpdateWATCHFreqDomain(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_aWatchMode[1]);
	
}

void CMainFrame::OnUpdateWATCHTimeDomain(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_aWatchMode[0]);
}

int CMainFrame::GetCurSelWatchDlg(CWatchDlg **ppWatchDlg)
{

	CWndManage* pWndManage = &((CRecorderView*)(GetActiveView()))->m_WndManage;
	CBgDlg* pCurBgDlg = &(pWndManage->m_pBgDlg[pWndManage->m_nCurSelect]);
	int nCurWatchDLgInBgDlg = pCurBgDlg->m_TabBgDlg.GetCurSel();
	CWatchDlg* pWatchDlgCurSel;
	pWatchDlgCurSel = (CWatchDlg*)pCurBgDlg->m_WatchDlgList.GetAt(pCurBgDlg->m_WatchDlgList.FindIndex(nCurWatchDLgInBgDlg));

	*ppWatchDlg = pWatchDlgCurSel;

	return 1;
}

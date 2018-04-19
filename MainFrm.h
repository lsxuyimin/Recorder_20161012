// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__29D07DFC_653F_47C6_AAED_D85C63F370D2__INCLUDED_)
#define AFX_MAINFRM_H__29D07DFC_653F_47C6_AAED_D85C63F370D2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "DlgBarEx.h"
//#include "DlgBarExBottomAcq.h"
//#include "DlgBarExBottomPlay.h"

class CDlgBarEx;
class CPopupRightDlg;
class CDlgBarExBottomPlay;
class CDlgBarExTopChnSel;
class CWatchDlg;

/////////////自定义消息/////////////////////////////////////////////////////////////
#define WM_ACQEND		WM_USER + 100
#define WM_INACQ		WM_USER + 101
#define WM_WAITACQ		WM_USER + 102

#define WM_UPDATE_CACHE	WM_USER + 103
#define WM_UPDATE_PLAY	WM_USER + 104

//////////////////////////////////////////////////////////////////////////

class CMainFrame : public CFrameWnd
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:
	CDlgBarEx* m_pDlgBarEx;
	CDlgBarExTopChnSel* m_pDlgBarExTopChnSel;
	CReBar	m_wndReBar;

//	CDlgBarExBottomAcq m_DlgBarExBottomAcq;
//	CReBar	m_wndReBarBottomAcq;

	CDlgBarExBottomPlay* m_pDlgBarExBottomPlay;
	CReBar	m_wndReBarBottomPlay;

	CPopupRightDlg* m_pDlgPopupRight;

	int m_aMouseModeStatus[5];			//0 1 2 3 4 分别代表none zoomin zoomout drag restore
	int m_aPlayCtrlStatus[3];			//0 1 2   分别代表play plause record
	int m_nMouseMode;
//	int m_aPlayCtrlEnable[3];			//0 1 2   分别代表play plause setting
	int m_aWatchMode[2];				//0 1	分别代表 时域分析 和 频域分析
	
	bool m_bTip;


	CBitmap m_bmpPrePage;
	CBitmap m_bmpNextPage;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	protected:
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
public:
	int GetCurSelWatchDlg(CWatchDlg** ppWatchDlg);
	int RefreshRangeToolBar();								//找到当前选中的BgDlg中选中的WatchDlg，并以它为参数更新toolbar
	void InitialMenuIcon();
	void ViewWndManageRefresh();
	void ChangeToolBar(int mode);							//0:phycard 1:demo 2:play
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnCTRLStartAcq();
	afx_msg void OnCTRLStopAcq();
	afx_msg void OnLAYOUTMode1();
	afx_msg void OnLAYOUTMode2();
	afx_msg void OnLAYOUTMode3();
	afx_msg void OnLAYOUTMode4();
	afx_msg void OnWATCHNextPage();
	afx_msg void OnWATCHPrePage();
	afx_msg void OnWATCHSetting();
	afx_msg void OnUpdateWATCHZoomIn(CCmdUI* pCmdUI);
	afx_msg void OnWATCHZoomIn();
	afx_msg void OnWATCHZoomOut();
	afx_msg void OnUpdateWATCHZoomOut(CCmdUI* pCmdUI);
	afx_msg void OnWATCHNone();
	afx_msg void OnUpdateWATCHNone(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCTRLStartAcq(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCTRLStopAcq(CCmdUI* pCmdUI);
	afx_msg void OnWATCHZoomDrag();
	afx_msg void OnUpdateWATCHZoomDrag(CCmdUI* pCmdUI);
	afx_msg LRESULT OnAcqEnd(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnInAcq(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnWaitAcq(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUpdateCache(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUpdatePlay(WPARAM wParam, LPARAM lParam);
	afx_msg void OnAppConnect();
	afx_msg void OnUpdateWATCHSetting(CCmdUI* pCmdUI);
	afx_msg void OnAPPHelp();
	afx_msg void OnFileOpen();
	afx_msg void OnFileSave();
	afx_msg void OnFILESAVECostum();
	afx_msg void OnFILESAVEASCostum();
	afx_msg void OnWATCHZoomReStore();
	afx_msg void OnUpdateWATCHZoomReStore(CCmdUI* pCmdUI);
	afx_msg void OnMeaAddmeasure();
	afx_msg void OnMeaDelmeasure();
	afx_msg void OnMeaEditmeasure();
	afx_msg void OnUpdateMeaEditmeasure(CCmdUI* pCmdUI);
	afx_msg void OnUpdateMeaDelmeasure(CCmdUI* pCmdUI);
	afx_msg void OnWATCHFreqDomain();
	afx_msg void OnWATCHFreqDomainConf();
	afx_msg void OnWATCHTimeDomain();
	afx_msg void OnUpdateWATCHFreqDomain(CCmdUI* pCmdUI);
	afx_msg void OnUpdateWATCHTimeDomain(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__29D07DFC_653F_47C6_AAED_D85C63F370D2__INCLUDED_)

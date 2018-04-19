// WndManage.h: interface for the CWndManage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WNDMANAGE_H__DC181133_6B54_455A_957B_E4724E6DD26E__INCLUDED_)
#define AFX_WNDMANAGE_H__DC181133_6B54_455A_957B_E4724E6DD26E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CWatchDlg;
class CBgDlg;

class CRecorderView;
class CRecorderDoc;
class CMainFrame;
class CProduct;

class CWndManage  
{
public:
	void AllWndRefresh_Complete();
	void AllWndRefresh();								//更新所有窗口显示图像 仅限于其中的WDW矩形框
	void DefaultSetting();								//默认设置，可以以后用在“默认设置”这个按钮上，将打开的X个通道依次加入到前X个BgDlg中，每个BgDlg加一个
	bool GetVideoRect(int nSub, CRect& rc);
	int m_nModeSave;
	int m_nCurMode;
	int m_nSubWnd;										//包含子窗口数
	int m_nFirst;
	int m_nLast;
	void Postion();
	CRect m_rcFrameWnd;									//用于显示的框架CRECT
	void Display(int nMode=0);
	CRecorderView* m_pOwnedViex;							//所有DLG的父窗口指针 也就是View窗口
	void Create(int nSubWnd, const RECT &rect, CRecorderView* pOwnedViex);
	int m_nCurSelect;

	CWatchDlg* m_pWatchDlg;// can be delete
	
	CBgDlg* m_pBgDlg;

	WavePlayCtrl  m_WPCUnified;						//20150507版本以后 所有watchdlg的放大缩小是同步的 所以将管理显示的结构体放在了这里

	CWndManage();
	virtual ~CWndManage();

};

#endif // !defined(AFX_WNDMANAGE_H__DC181133_6B54_455A_957B_E4724E6DD26E__INCLUDED_)

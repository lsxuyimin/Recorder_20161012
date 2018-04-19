#if !defined(AFX_BGDLG_H__7B376689_30B2_47AD_9C54_437D0862E10E__INCLUDED_)
#define AFX_BGDLG_H__7B376689_30B2_47AD_9C54_437D0862E10E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BgDlg.h : header file
//


class CWaveData;
class CWatchDlg;
class CWndManage;



/////////////////////////////////////////////////////////////////////////////
// CBgDlg dialog

class CBgDlg : public CDialog
{
// Construction
public:
	void AddWaveDataToBgDlg(int nWatchDlgIndex, CWatchDlg *pWatchDlg , int mode);
	void CloseCurSelWatchDlg();
	void WndRefresh_Complete();
	void SetBgDlgSeleted();
	void SetWndManage(CWndManage* pWndManage);
	void DisplayWndRefresh();
	void AddWatchDlgToTab(CWatchDlg* pWatchDlg);
	void ClearAll();
	void AddWaveDataToBgDlg(int nWatchDlgIndex,CWaveData* pWaveData , int mode);		//nWatchDlgIndex: -1新建 0-9序号	pWaveData:NULL	mode:	-1根据加入的pWaveData  0正常显示  1FFT (nWatchDlgIndex,mode不可同时为-1)
	CRect m_rcBgDlgFrame;
	void SetBgDlgFrame(CRect rc);							//入参rc 是BgDlg在View中的位置 将被赋值给m_rcBgDlgFrame
	void SetIndex(int index);
	int m_nIndex;								//CBgDlg的序号
	CWndManage* m_pWndManage;
	CBgDlg(CWnd* pParent = NULL);   // standard constructor

	int m_nWatchDlgNum;										//本BgDlg下包含的WatchDlg的数量
	int m_nWatchDlgHasBeenNew;								//本BgDlg下已经新建的WatchDlg的数量，用来确定WatchDlg的名字
	CTypedPtrList<CObList, CWatchDlg*> m_WatchDlgList;		//本BgDlg下包含的WatchDlg指针数组

// Dialog Data
	//{{AFX_DATA(CBgDlg)
	enum { IDD = IDD_DIALOG_BGDLG };
	CTabCtrl	m_TabBgDlg;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBgDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBgDlg)
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg void OnSelchangeTABBgDlg(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickTABBgDlg(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BGDLG_H__7B376689_30B2_47AD_9C54_437D0862E10E__INCLUDED_)

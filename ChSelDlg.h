#if !defined(AFX_CHSELDLG_H__64E240C4_436B_4B13_B931_6B497D4502B5__INCLUDED_)
#define AFX_CHSELDLG_H__64E240C4_436B_4B13_B931_6B497D4502B5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ChSelDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
class CWatchDlg;
/////////////////////////////////////////////////////////////////////////////
// CChSelDlg dialog

class CChSelDlg : public CDialog
{
// Construction
public:
	int ChangeDisplayChn();							//改变当前选中的CWatchDlg中显示的通道
	CChSelDlg(CWnd* pParent = NULL);   // standard constructor

	CWatchDlg* m_pOwnedWatchDlg;

// Dialog Data
	//{{AFX_DATA(CChSelDlg)
	enum { IDD = IDD_DIALOG_ChSel };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChSelDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CChSelDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnClickListCh(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHSELDLG_H__64E240C4_436B_4B13_B931_6B497D4502B5__INCLUDED_)

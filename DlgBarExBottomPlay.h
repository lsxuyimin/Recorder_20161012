#if !defined(AFX_DLGBAREXBOTTOMPLAY_H__798DFE1C_83F3_4E73_B03D_3DCEE6FF7B44__INCLUDED_)
#define AFX_DLGBAREXBOTTOMPLAY_H__798DFE1C_83F3_4E73_B03D_3DCEE6FF7B44__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgBarExBottomPlay.h : header file
//

/////////////////////////////////////////////////////////////////////////////

class CPlayBar;

//////////////////////////////////////////////////////////////////////////
// CDlgBarExBottomPlay dialog

class CDlgBarExBottomPlay : public CDialogBar
{
// Construction
public:
	CDlgBarExBottomPlay(CWnd* pParent = NULL);		// standard constructor

//	int m_nPlayCtrlStatus[2];						//0:PlayButton 1:PauseButton  0代表弹起 1代表按下
	CPlayBar* m_pPlayBar;

// Dialog Data
	//{{AFX_DATA(CDlgBarExBottomPlay)
	enum { IDD = IDD_DIALOG_BarExBottomPlay };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgBarExBottomPlay)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgBarExBottomPlay)
	virtual BOOL OnInitDialog(UINT wParam,LONG lParam);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnRADIOPlay();
	afx_msg void OnRADIOPause();
	afx_msg void OnBUTTONSpeedUp();
	afx_msg void OnBUTTONSpeedDown();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGBAREXBOTTOMPLAY_H__798DFE1C_83F3_4E73_B03D_3DCEE6FF7B44__INCLUDED_)

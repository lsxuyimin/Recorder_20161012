#if !defined(AFX_POPUPRIGHTDLG_H__0F5123C0_A24B_443A_8895_A94662DACE9C__INCLUDED_)
#define AFX_POPUPRIGHTDLG_H__0F5123C0_A24B_443A_8895_A94662DACE9C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PopupRightDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPopupRightDlg dialog

class CPopupRightDlg : public CDialog
{
// Construction
public:
	CPopupRightDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPopupRightDlg)
	enum { IDD = IDD_DIALOG_PopupRight };
	CEdit	m_editAmpMin;
	CEdit	m_editAmpMax;
	CComboBox	m_comboChnIndex;
	CComboBox	m_comboFrequency;
	CComboBox	m_comboWaveForm;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPopupRightDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPopupRightDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnSelchangeCOMBOChnIndex();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_POPUPRIGHTDLG_H__0F5123C0_A24B_443A_8895_A94662DACE9C__INCLUDED_)

#if !defined(AFX_POPUPPROPERTYDLG_H__C7423D63_C98D_41F3_A455_D328F643C784__INCLUDED_)
#define AFX_POPUPPROPERTYDLG_H__C7423D63_C98D_41F3_A455_D328F643C784__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PopupPropertyDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPopupPropertyDlg dialog

class CPopupPropertyDlg : public CDialog
{
// Construction
public:
	CPopupPropertyDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPopupPropertyDlg)
	enum { IDD = IDD_DIALOG_PopupProperty };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPopupPropertyDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPopupPropertyDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_POPUPPROPERTYDLG_H__C7423D63_C98D_41F3_A455_D328F643C784__INCLUDED_)

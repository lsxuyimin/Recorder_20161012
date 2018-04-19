#if !defined(AFX_SAVEASDLG_H__78345832_D87C_44EE_8E06_1F2C0E50F1EA__INCLUDED_)
#define AFX_SAVEASDLG_H__78345832_D87C_44EE_8E06_1F2C0E50F1EA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SaveAsDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSaveAsDlg dialog

class CSaveAsDlg : public CDialog
{
// Construction
public:
	CSaveAsDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSaveAsDlg)
	enum { IDD = IDD_DIALOG_SaveAs };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSaveAsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSaveAsDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SAVEASDLG_H__78345832_D87C_44EE_8E06_1F2C0E50F1EA__INCLUDED_)

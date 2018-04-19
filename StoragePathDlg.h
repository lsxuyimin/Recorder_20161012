#if !defined(AFX_STORAGEPATHDLG_H__F01DB8BC_F47D_44BB_A950_F7A1B8146F52__INCLUDED_)
#define AFX_STORAGEPATHDLG_H__F01DB8BC_F47D_44BB_A950_F7A1B8146F52__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StoragePathDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStoragePathDlg dialog

class CStoragePathDlg : public CDialog
{
// Construction
public:
	CStoragePathDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CStoragePathDlg)
	enum { IDD = IDD_DIALOG_StoragePath };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStoragePathDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CStoragePathDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnRADIOOverrideDoc();
	afx_msg void OnRADIONewDoc();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STORAGEPATHDLG_H__F01DB8BC_F47D_44BB_A950_F7A1B8146F52__INCLUDED_)

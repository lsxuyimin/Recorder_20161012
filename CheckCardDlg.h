#if !defined(AFX_CHECKCARDDLG_H__31F99A14_1CFB_4A9E_BC37_82818528CD0F__INCLUDED_)
#define AFX_CHECKCARDDLG_H__31F99A14_1CFB_4A9E_BC37_82818528CD0F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CheckCardDlg.h : header file
//

#include "Product.h"

/////////////////////////////////////////////////////////////////////////////
// CCheckCardDlg dialog

class CCheckCardDlg : public CDialog
{
// Construction
public:
	int ReadInSettingFile();

	CCheckCardDlg(CWnd* pParent = NULL);   // standard constructor
	CCheckCardDlg(CProduct* pProduct , CWnd* pParent, int nInitialMode);   
	CCheckCardDlg(CProduct* pProduct , CWnd* pParent = NULL);

	CProduct* m_pProduct;
	int m_nInitialMode;

	CString m_strFilePath;

	int m_nCardNum;
	unsigned long m_BusIDArray[32];
	unsigned long m_SlotIDArray[32];



// Dialog Data
	//{{AFX_DATA(CCheckCardDlg)
	enum { IDD = IDD_DIALOG_CheckCard };
	CButton	m_buttonBrowse;
	CButton	m_RadioMode;
	CListBox	m_ListPhyCard;
	CComboBox	m_ComboDemoChn;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCheckCardDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCheckCardDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnRADIOPhyCard();
	afx_msg void OnRADIOMode();
	virtual void OnOK();
	afx_msg void OnBUTTONBrowse();
	afx_msg void OnRADIOOpenExistData();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHECKCARDDLG_H__31F99A14_1CFB_4A9E_BC37_82818528CD0F__INCLUDED_)

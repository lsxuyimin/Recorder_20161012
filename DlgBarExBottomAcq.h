#if !defined(AFX_DLGBAREXBOTTOMACQ_H__6F69FB2C_7B72_4669_91A8_25F0ADF9B4B6__INCLUDED_)
#define AFX_DLGBAREXBOTTOMACQ_H__6F69FB2C_7B72_4669_91A8_25F0ADF9B4B6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgBarExBottomAcq.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgBarExBottomAcq dialog

class CDlgBarExBottomAcq : public CDialogBar
{
// Construction
public:
	CDlgBarExBottomAcq(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgBarExBottomAcq)
	enum { IDD = IDD_DIALOG_BarExBottomAcq };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgBarExBottomAcq)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgBarExBottomAcq)
	virtual BOOL OnInitDialog(UINT wParam,LONG lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGBAREXBOTTOMACQ_H__6F69FB2C_7B72_4669_91A8_25F0ADF9B4B6__INCLUDED_)

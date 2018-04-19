#if !defined(AFX_MEASUREEDITDLG_H__4FE6289E_8302_4518_A052_76A79D24DB23__INCLUDED_)
#define AFX_MEASUREEDITDLG_H__4FE6289E_8302_4518_A052_76A79D24DB23__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MeasureEditDlg.h : header file
//

class CWatchDlg;
class CWaveData;
class CMeasureItem;


/////////////////////////////////////////////////////////////////////////////
// CMeasureEditDlg dialog

class CMeasureEditDlg : public CDialog
{
// Construction
public:
	CWatchDlg* m_pOwnedWatchDlg;
	CMeasureEditDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMeasureEditDlg)
	enum { IDD = IDD_DIALOG_MeasureEdit };
	CComboBox	m_comboMeaType;
	CComboBox	m_comboMeaPart;
	CComboBox	m_comboMeaChannel;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMeasureEditDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMeasureEditDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MEASUREEDITDLG_H__4FE6289E_8302_4518_A052_76A79D24DB23__INCLUDED_)

#if !defined(AFX_MEASUREADDDLG_H__1DC25E45_4573_4417_A5BE_7CB791262D82__INCLUDED_)
#define AFX_MEASUREADDDLG_H__1DC25E45_4573_4417_A5BE_7CB791262D82__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MeasureAddDlg.h : header file
//

class CWatchDlg;
class CWaveData;
class CMeasureItem;



/////////////////////////////////////////////////////////////////////////////
// CMeasureAddDlg dialog

class CMeasureAddDlg : public CDialog
{
// Construction
public:
	CWatchDlg* m_pOwnedWatchDlg;
	CMeasureAddDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMeasureAddDlg)
	enum { IDD = IDD_DIALOG_MeasureAdd };
	CComboBox	m_comboMeaType;
	CComboBox	m_comboMeaPart;
	CComboBox	m_comboMeaChannel;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMeasureAddDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMeasureAddDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MEASUREADDDLG_H__1DC25E45_4573_4417_A5BE_7CB791262D82__INCLUDED_)

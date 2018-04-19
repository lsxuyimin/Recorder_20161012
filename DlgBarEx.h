#if !defined(AFX_DLGBAREX_H__F88B5CA4_D52C_40FC_8988_026DB09E763F__INCLUDED_)
#define AFX_DLGBAREX_H__F88B5CA4_D52C_40FC_8988_026DB09E763F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgBarEx.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgBarEx dialog

class CDlgBarEx : public CDialogBar
{
// Construction
public:
	int RefreshParaSetting();
	void InitialControl();
	CDlgBarEx(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgBarEx)
	enum { IDD = IDD_DIALOG_BarEx };
	CSliderCtrl	m_SliderTrigUpperLimit;
	CSliderCtrl	m_SliderTrigLowerLimit;
	CSliderCtrl	m_SliderTrigLevel;
	CComboBox	m_ComboTriggerChn;
	CComboBox	m_ComboTrigMode;
	CComboBox	m_ComboRate;
	CComboBox	m_ComboRange;
	CComboBox	m_ComboLength;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgBarEx)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgBarEx)
	virtual BOOL OnInitDialog(UINT wParam,LONG lParam);
	afx_msg void OnSelchangeComboLength();
	afx_msg void OnSelchangeComboRange();
	afx_msg void OnSelchangeComboRate();
	afx_msg void OnSelchangeComboTriggerChannel();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnEditchangeComboTriggerMode();
	afx_msg void OnSelchangeComboTriggerMode();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGBAREX_H__F88B5CA4_D52C_40FC_8988_026DB09E763F__INCLUDED_)

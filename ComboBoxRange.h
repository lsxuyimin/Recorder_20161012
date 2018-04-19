#if !defined(AFX_COMBOBOXRANGE_H__C75E51CC_A44F_40E5_B173_1F25B16D8EEF__INCLUDED_)
#define AFX_COMBOBOXRANGE_H__C75E51CC_A44F_40E5_B173_1F25B16D8EEF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ComboBoxRange.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CComboBoxRange window

class CComboBoxRange : public CComboBox
{
// Construction
public:
	CComboBoxRange();

// Attributes
	COLORREF m_clrText;		//记录文本颜色
	COLORREF m_clrBkgnd;	//记录背景颜色
	CBrush m_brBkgnd;		//用于绘制背景的刷子

public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CComboBoxRange)
	//}}AFX_VIRTUAL

// Implementation
public:
	int ChangeTextColor(COLORREF clrText );
	virtual ~CComboBoxRange();

	// Generated message map functions
protected:
	//{{AFX_MSG(CComboBoxRange)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMBOBOXRANGE_H__C75E51CC_A44F_40E5_B173_1F25B16D8EEF__INCLUDED_)

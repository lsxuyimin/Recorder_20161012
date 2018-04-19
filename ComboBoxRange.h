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
	COLORREF m_clrText;		//��¼�ı���ɫ
	COLORREF m_clrBkgnd;	//��¼������ɫ
	CBrush m_brBkgnd;		//���ڻ��Ʊ�����ˢ��

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

#if !defined(AFX_PLAYBAR_H__8D19F3EF_A73A_42B6_94EE_B9B998A64C4E__INCLUDED_)
#define AFX_PLAYBAR_H__8D19F3EF_A73A_42B6_94EE_B9B998A64C4E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PlayBar.h : header file
//

/////////////////////////////////////////////////////////////////////////////
class CProduct;
//////////////////////////////////////////////////////////////////////////
#define PRONUM_WIDTH 60
//////////////////////////////////////////////////////////////////////////
// CPlayBar window

class CPlayBar : public CWnd
{
// Construction
public:
	CPlayBar();

	DECLARE_DYNCREATE(CPlayBar) 
// Attributes
public:

	int m_nPlayPos;
	int m_nPlayWndWidth;
	int m_nCachePos;

	int m_nMinPos;
	int m_nMaxPos;
	int m_nRange;
	
	bool m_bPressed;

	int m_nWndLeftToClickPoint;

	CProduct* m_pProduct;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPlayBar)
	public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetPlayWndWidth(int nWidth);
	void SetRange(int nMin, int nMax);
	void SetPlayPos(int nPlayPos);
	void SetCachePos(int nCachePos);



	virtual ~CPlayBar();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPlayBar)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PLAYBAR_H__8D19F3EF_A73A_42B6_94EE_B9B998A64C4E__INCLUDED_)

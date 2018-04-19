#if !defined(AFX_MARKDLG_H__62F1A4E4_EEDC_4915_A537_720338DCC917__INCLUDED_)
#define AFX_MARKDLG_H__62F1A4E4_EEDC_4915_A537_720338DCC917__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MarkDlg.h : header file
//



class CWatchDlg;


/////////////////////////////////////////////////////////////////////////////
// CMarkDlg dialog

class CMarkDlg : public CDialog
{
// Construction
public:
	CString ComputeHorUnitFreq(double *pfResult, double fInput);			//fInput为真实采样率
	CString ComputeHorUnitTime(double *pfResult, double fInput);
	void ClearMark();
	void DrawBtn(CDC* pDC);
	void DrawGrid(CDC* pDC);
	void DrawItem(CDC* pDC);
	void DrawTitleBar(CDC* pDC);
	void ModifyValue(int nMarkMode , float fChangeValue);
	CMarkDlg(CWnd* pParent = NULL);   // standard constructor


	CWatchDlg* m_pOwnedDlg;
	float m_aValue[12];					//每个值代表一个MARK块，一共12个，其中4个通道每个通道2个，X轴还有4个
	int m_nRowNum;

	int m_nMarkDlgHeight;
	int m_nMarkDlgWidth;
	int m_nMarkDlgWidthNormal;

	CBitmap m_bmpMarkX;
	CBitmap m_bmpMarkY;
	CBitmap m_bmpScale;

	CRect m_rcBtnClose;
	CRect m_rcBtnMinOrRe;
	bool m_bMinFlag;

	CRect m_rcMarkDlgInWatchDlg;

	CPoint m_ptMoveMouse;
	bool m_bIsPick;

	int m_nClickPointToLeft;
	int m_nClickPointToTop;
	int m_nClickPointToRight;
	int m_nClickPointToBottom;


// Dialog Data
	//{{AFX_DATA(CMarkDlg)
	enum { IDD = IDD_DIALOG_Mark };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMarkDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMarkDlg)
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg UINT OnNcHitTest(CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnNcLButtonUp(UINT nHitTest, CPoint point);
	afx_msg void OnNcMouseMove(UINT nHitTest, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MARKDLG_H__62F1A4E4_EEDC_4915_A537_720338DCC917__INCLUDED_)

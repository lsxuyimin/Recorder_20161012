#if !defined(AFX_ZOOMDLG_H__3F86B1B8_BA7C_4D2E_A806_6738FB25289D__INCLUDED_)
#define AFX_ZOOMDLG_H__3F86B1B8_BA7C_4D2E_A806_6738FB25289D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ZoomDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
class CWatchDlg;


//////////////////////////////////////////////////////////////////////////
// CZoomDlg dialog

class CZoomDlg : public CDialog
{
// Construction
public:
	void ModifyZoomRectRatio(	float fRatioStartX,
								float fRatioStartY,
								float fRatioEndX,
								float fRatioEndY
							);							//修改ZoomDlg上放大位置的方框
	void DrawZoomDlg(CDC* pDC);
	void DrawGrid(CDC *pDC);
	void DrawBtn(CDC *pDC);
	void DrawTitle(CDC *pDC);
	void DrawCurBorder(CDC* pDC);
	void DrawWave(CDC* pDC);
	CZoomDlg(CWnd* pParent = NULL);   // standard constructor

	//绘图相关
	float m_fRatioStartX;
	float m_fRatioStartY;
	float m_fRatioEndX;
	float m_fRatioEndY;


	CWatchDlg* m_pOwnedDlg;

	int m_nZoomDlgHeight;
	int m_nZoomDlgWidth;	

	CRect m_rcBtnClose;
	CRect m_rcBtnMinOrRe;
	CRect m_rcDisplayFrame;
	bool m_bMinFlag;
	
	CRect m_rcZoomDlgInWatchDlg;									//ZoomDlg在WatchDlg中的位置
	CRect m_rcBorder;												//Border标志ZoomDlg放大的是WatchDlg中的哪一部分 就是ZoomDlg中可以拉动的小框
	
	CPoint m_ptMoveMouse;
	bool m_bIsPick;
	
	int m_nClickPointToLeft;
	int m_nClickPointToTop;
	int m_nClickPointToRight;
	int m_nClickPointToBottom;

// Dialog Data
	//{{AFX_DATA(CZoomDlg)
	enum { IDD = IDD_DIALOG_Zoom };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CZoomDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CZoomDlg)
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg UINT OnNcHitTest(CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ZOOMDLG_H__3F86B1B8_BA7C_4D2E_A806_6738FB25289D__INCLUDED_)

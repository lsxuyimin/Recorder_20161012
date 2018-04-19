#if !defined(AFX_MEASUREDLG_H__589419D7_FEC2_41B4_9379_A40F4C7E5CA7__INCLUDED_)
#define AFX_MEASUREDLG_H__589419D7_FEC2_41B4_9379_A40F4C7E5CA7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MeasureDlg.h : header file
//


class CWatchDlg;
class CMeasureItem;
class CWaveData;


/////////////////////////////////////////////////////////////////////////////
// CMeasureDlg dialog


class CMeasureDlg : public CDialog
{
// Construction
public:
	void ComputeItemFreq(int nChnIndex, double *pDataBuffer, int nDataLen);
	CString FreqComputeMax(double*pDataBuffer, int nDataLen);
	CString FreqComputeFindMaxIndex(double *pDataBuffer, int nDataLen);
	void MeasureDelete();
	
	CString TransMeaPart( int nMeaPart );
	CString TimeComputeACAVG(short *pDataBuffer, int nDataLen);
	CString TimeComputeMin(short *pDataBuffer, int nDataLen);
	CString TimeComputeMax(short *pDataBuffer, int nDataLen);
	void ComputeItemTime(int nChnIndex, short *pDataBuffer, int nDataLen );
	CString TransMeaType(int nMeaType);
	void DrawGrid(CDC* pDC);
	void DrawTitleBar(CDC* pDC);
	void DeleteHScroll();
	void AddHScroll(int nRangeStart,int nRangeEnd,CRect* rcMFinTab);
	void DrawItem(CDC* pDC);
	void DrawBtn(CPaintDC* pDC);
	void AddMeasureItem(CWaveData* pWaveData,int nMeaType ,int nMeaPart);
	void SetMatchWatchDlg(CWatchDlg* pWatchDlg);
	CWatchDlg* m_pMatchWatchDlg;
	bool m_bMinFlag;

	CMeasureDlg(CWnd* pParent = NULL);   // standard constructor


	int m_nMeasureNum;															//0代表没有MeasureItem
	CTypedPtrList<CObList , CMeasureItem*> m_MeasureItemList;					//MeasureItem列表

	CRect m_rcBtnMinOrRe;
	CRect m_rcBtnClose;

	int m_nSelItem;

//	int m_aColWidth[8];															//列宽度
	int m_nColWidthSum;

	CScrollBar* m_pHScroll;
	int m_nHScrollPos;

	int m_nBmpWidth;
	int m_nBmpHeight;

// Dialog Data
	//{{AFX_DATA(CMeasureDlg)
	enum { IDD = IDD_DIALOG_Measure };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMeasureDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMeasureDlg)
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnMeaAddmeasure();
	afx_msg void OnMeaDelmeasure();
	virtual void OnCancel();
	afx_msg void OnMeaEditmeasure();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MEASUREDLG_H__589419D7_FEC2_41B4_9379_A40F4C7E5CA7__INCLUDED_)

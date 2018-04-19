// RecorderView.h : interface of the CRecorderView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_RECORDERVIEW_H__49FBB71F_03F4_42FF_859A_4B83EEAD411E__INCLUDED_)
#define AFX_RECORDERVIEW_H__49FBB71F_03F4_42FF_859A_4B83EEAD411E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "WndManage.h"
//#include "RecorderDoc.h"

//////////////////////////////////////////////////////////////////////////
class CRecorderDoc;


//////////////////////////////////////////////////////////////////////////
class CRecorderView : public CFormView
{
protected: // create from serialization only
	CRecorderView();
	DECLARE_DYNCREATE(CRecorderView)

public:
	//{{AFX_DATA(CRecorderView)
	enum { IDD = IDD_RECORDER_FORM };
	CStatic	m_bInitialMark;
	//}}AFX_DATA

// Attributes
public:
	
	CWndManage m_WndManage;

	CRecorderDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRecorderView)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	void ViewInitialize(int mode);
	void WndManageRefresh_Complete();						//所有地方的刷新 DisplayWnd以及各个边框
	void WndManageRefresh();								//所有显示窗口的刷新 只包含DisplayWnd
	virtual ~CRecorderView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CRecorderView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in RecorderView.cpp
inline CRecorderDoc* CRecorderView::GetDocument()
   { return (CRecorderDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RECORDERVIEW_H__49FBB71F_03F4_42FF_859A_4B83EEAD411E__INCLUDED_)

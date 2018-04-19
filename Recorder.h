// Recorder.h : main header file for the RECORDER application
//

#if !defined(AFX_RECORDER_H__42A91F01_BC86_4F4E_A6DA_ACD27781BBB8__INCLUDED_)
#define AFX_RECORDER_H__42A91F01_BC86_4F4E_A6DA_ACD27781BBB8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CRecorderApp:
// See Recorder.cpp for the implementation of this class
//

class CRecorderApp : public CWinApp
{
public:
	HANDLE m_hUniqueMutex;
	CRecorderApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRecorderApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CRecorderApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RECORDER_H__42A91F01_BC86_4F4E_A6DA_ACD27781BBB8__INCLUDED_)

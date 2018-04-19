// ComboBoxRange.cpp : implementation file
//

#include "stdafx.h"
#include "recorder.h"
#include "ComboBoxRange.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CComboBoxRange

CComboBoxRange::CComboBoxRange()
{
	m_clrText = RGB( 0, 0, 0 );					//初始化时把文字颜色设置成黑色
	m_clrBkgnd = RGB( 255, 255, 255 );
	m_brBkgnd.CreateSolidBrush( m_clrBkgnd );
}

CComboBoxRange::~CComboBoxRange()
{
}


BEGIN_MESSAGE_MAP(CComboBoxRange, CComboBox)
	//{{AFX_MSG_MAP(CComboBoxRange)
	ON_WM_CTLCOLOR()
	ON_WM_CTLCOLOR_REFLECT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CComboBoxRange message handlers

HBRUSH CComboBoxRange::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CComboBox::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here

	pDC-> SetTextColor( m_clrText );
	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

HBRUSH CComboBoxRange::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	// TODO: Change any attributes of the DC here

	pDC->SetTextColor( m_clrText );		// text
	pDC->SetBkColor( m_clrBkgnd );		// text bkgnd
	return m_brBkgnd;					// ctl bkgnd
	
	// TODO: Return a non-NULL brush if the parent's handler should not be called
	return NULL;
}

int CComboBoxRange::ChangeTextColor(COLORREF clrText)
{
	m_clrText = clrText ;
	Invalidate();

	return 0;
}

// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__70E774B2_4914_4E3B_8542_18FFBB4BCA23__INCLUDED_)
#define AFX_STDAFX_H__70E774B2_4914_4E3B_8542_18FFBB4BCA23__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#undef WINVER
#define WINVER 0X500


#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT


//////////////////////////////////////////////////////////////////////////
//add by hanchen
#include "MyDef.h"

#include <afxtempl.h>  
#include <afxmt.h>



#define MAX_CHANNUM 64


//////////////////////////////////////////////////////////////////////////


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__70E774B2_4914_4E3B_8542_18FFBB4BCA23__INCLUDED_)

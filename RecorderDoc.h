// RecorderDoc.h : interface of the CRecorderDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_RECORDERDOC_H__A7B576C2_003B_4062_9029_835843AD2CED__INCLUDED_)
#define AFX_RECORDERDOC_H__A7B576C2_003B_4062_9029_835843AD2CED__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Product.h"

class CWaveData;
//class CProduct;

class CRecorderDoc : public CDocument
{
protected: // create from serialization only
	CRecorderDoc();
	DECLARE_DYNCREATE(CRecorderDoc)

// Attributes
public:

	CProduct m_Product;							//CTypedPtrList<CObList, CWaveData*> m_WaveList 放在其中
	CTypedPtrList<CObList, CWaveData*> m_WaveList;//待删除



// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRecorderDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	
	bool m_bAcqing;
	void StopCapture();
	void StartCapture();

	virtual ~CRecorderDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CRecorderDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RECORDERDOC_H__A7B576C2_003B_4062_9029_835843AD2CED__INCLUDED_)

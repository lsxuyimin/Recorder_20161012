#if !defined(AFX_MEASUREITEM_H__03EA8475_56C2_4568_9916_59E6C24FA555__INCLUDED_)
#define AFX_MEASUREITEM_H__03EA8475_56C2_4568_9916_59E6C24FA555__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MeasureItem.h : header file
//

class CWaveData;

/////////////////////////////////////////////////////////////////////////////
// CMeasureItem command target

class CMeasureItem : public CCmdTarget
{
	DECLARE_DYNCREATE(CMeasureItem)

	CMeasureItem();           // protected constructor used by dynamic creation

// Attributes
public:
	CWaveData* pWaveData;				//数据（通道）
	int	nMeaType;						//测量类型
	int nMeaPart;						//测量范围

/*
	int nMeaValue;						//测量计算结果
	int nMin;							
	int nMax;							
	int nAverage;						//计算平均值
	int nVariance;						//测量方差
*/
	CString strMeaValue;						//测量计算结果
	CString strMin;							
	CString strMax;							
	CString strAverage;							//计算平均值
	CString strVariance;						//测量方差

	
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMeasureItem)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMeasureItem();

	// Generated message map functions
	//{{AFX_MSG(CMeasureItem)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MEASUREITEM_H__03EA8475_56C2_4568_9916_59E6C24FA555__INCLUDED_)

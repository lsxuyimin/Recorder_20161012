#if !defined(AFX_WAVEDATA_H__1257AD04_B44A_47EB_9124_6230668051A9__INCLUDED_)
#define AFX_WAVEDATA_H__1257AD04_B44A_47EB_9124_6230668051A9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WaveData.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CWaveData command target

//CWaveData 中存储的是一个通道中的所有信息
class CWaveData : public CCmdTarget
{
	DECLARE_DYNCREATE(CWaveData)

	CWaveData();           // protected constructor used by dynamic creation
	CWaveData(int nChnIndex);
// Attributes
public:	
	int m_nChnIndex;								//基于0
	CString m_strChanName;						//该通道名称		e.g.:	chan1,chan2
	int m_nChanState;							//该通道当前状态	0:close 1:open
	CArray<int,int> m_WaveData;					//该通道的数据

	COLORREF m_LineColor;						//该波形的颜色
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWaveData)
	//}}AFX_VIRTUAL

// Implementation
public:
	int GenerateColor(int nRandomSeed , int nWaveMode=0);			//nWaveMode=0正常通道，nWaveMode=1合成通道，将nChnIndex作为入参nRandomSeed，保证生成的颜色都不一样，该函数尽量在构造时候，初始化了nChnIndex就调用
																//comb出来的通道可以考虑RGB值一样来区别
	virtual ~CWaveData();

	// Generated message map functions
	//{{AFX_MSG(CWaveData)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WAVEDATA_H__1257AD04_B44A_47EB_9124_6230668051A9__INCLUDED_)

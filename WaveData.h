#if !defined(AFX_WAVEDATA_H__1257AD04_B44A_47EB_9124_6230668051A9__INCLUDED_)
#define AFX_WAVEDATA_H__1257AD04_B44A_47EB_9124_6230668051A9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WaveData.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CWaveData command target

//CWaveData �д洢����һ��ͨ���е�������Ϣ
class CWaveData : public CCmdTarget
{
	DECLARE_DYNCREATE(CWaveData)

	CWaveData();           // protected constructor used by dynamic creation
	CWaveData(int nChnIndex);
// Attributes
public:	
	int m_nChnIndex;								//����0
	CString m_strChanName;						//��ͨ������		e.g.:	chan1,chan2
	int m_nChanState;							//��ͨ����ǰ״̬	0:close 1:open
	CArray<int,int> m_WaveData;					//��ͨ��������

	COLORREF m_LineColor;						//�ò��ε���ɫ
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWaveData)
	//}}AFX_VIRTUAL

// Implementation
public:
	int GenerateColor(int nRandomSeed , int nWaveMode=0);			//nWaveMode=0����ͨ����nWaveMode=1�ϳ�ͨ������nChnIndex��Ϊ���nRandomSeed����֤���ɵ���ɫ����һ�����ú��������ڹ���ʱ�򣬳�ʼ����nChnIndex�͵���
																//comb������ͨ�����Կ���RGBֵһ��������
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

#if !defined(AFX_DLGBAREXTOPCHNSEL_H__27F83046_3B34_4F9B_B46B_B0C57F44FFD0__INCLUDED_)
#define AFX_DLGBAREXTOPCHNSEL_H__27F83046_3B34_4F9B_B46B_B0C57F44FFD0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgBarExTopChnSel.h : header file
//

/////////////////////////////////////////////////////////////////////////////
#include "ComboBoxRange.h"

/////////////////////////////////////////////////////////////////////////////
class CWatchDlg;
class CWaveData;

/////////////////////////////////////////////////////////////////////////////
// CDlgBarExTopChnSel dialog

class CDlgBarExTopChnSel : public CDialogBar
{
// Construction
public:
	int InitializeDisplayChn(CWatchDlg *pWatchDlg);				//初始化变量，m_WaveDataList、m_pCurWatchDlg全部清零，根据是频域还是时域分析改变CComboBox中的内容，但是进行不选择
	int DrawStatic(CDC *pDC);
	int ChangeToolBarDisplayChn(CWatchDlg* pWatchDlg);				//同时改变CStatic和CComboBoxRange
	CDlgBarExTopChnSel(CWnd* pParent = NULL);   // standard constructor

	CStatic* m_pStaticChnSel[DRAW_CHNUM_MAX];			//存储用于显示通道的几个CStatic
	CComboBoxRange* m_pComboChnRange[DRAW_CHNUM_MAX];		//存储用于几个通道的量程选择的CCombo

	//WaveData的数据
	int m_nWaveDataNum;					//基于0
	CTypedPtrList<CObList , CWaveData*> m_WaveDataList;

	CWatchDlg* m_pCurWatchDlg;							//当前指向的WatchDlg，用于改变量程时候去反馈改变相应CWatchDlg的表m_nWaveDataRange

	int m_nModeCur;
	int m_nModeFreqRangeCur;


// Dialog Data
	//{{AFX_DATA(CDlgBarExTopChnSel)
	enum { IDD = IDD_DIALOG_BarExTopChnSel };
	CComboBoxRange	m_ComboChnRange4;
	CComboBoxRange	m_ComboChnRange3;
	CComboBoxRange	m_ComboChnRange2;
	CComboBoxRange	m_ComboChnRange1;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgBarExTopChnSel)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgBarExTopChnSel)
	virtual BOOL OnInitDialog(UINT wParam,LONG lParam);
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSelchangeCOMBOChnRange1();
	afx_msg void OnSelchangeCOMBOChnRange2();
	afx_msg void OnSelchangeCOMBOChnRange3();
	afx_msg void OnSelchangeCOMBOChnRange4();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGBAREXTOPCHNSEL_H__27F83046_3B34_4F9B_B46B_B0C57F44FFD0__INCLUDED_)

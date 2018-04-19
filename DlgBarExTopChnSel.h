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
	int InitializeDisplayChn(CWatchDlg *pWatchDlg);				//��ʼ��������m_WaveDataList��m_pCurWatchDlgȫ�����㣬������Ƶ����ʱ������ı�CComboBox�е����ݣ����ǽ��в�ѡ��
	int DrawStatic(CDC *pDC);
	int ChangeToolBarDisplayChn(CWatchDlg* pWatchDlg);				//ͬʱ�ı�CStatic��CComboBoxRange
	CDlgBarExTopChnSel(CWnd* pParent = NULL);   // standard constructor

	CStatic* m_pStaticChnSel[DRAW_CHNUM_MAX];			//�洢������ʾͨ���ļ���CStatic
	CComboBoxRange* m_pComboChnRange[DRAW_CHNUM_MAX];		//�洢���ڼ���ͨ��������ѡ���CCombo

	//WaveData������
	int m_nWaveDataNum;					//����0
	CTypedPtrList<CObList , CWaveData*> m_WaveDataList;

	CWatchDlg* m_pCurWatchDlg;							//��ǰָ���WatchDlg�����ڸı�����ʱ��ȥ�����ı���ӦCWatchDlg�ı�m_nWaveDataRange

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

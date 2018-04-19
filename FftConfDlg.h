#if !defined(AFX_FFTCONFDLG_H__126F5B1C_214E_48DC_B601_745CEAA0F116__INCLUDED_)
#define AFX_FFTCONFDLG_H__126F5B1C_214E_48DC_B601_745CEAA0F116__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FftConfDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFftConfDlg dialog

class CFftConfDlg : public CDialog
{
// Construction
public:
	int m_nFftBins;				//������źŵĳ��ȣ�������2��n�η�
	int m_nWindowMode;			//�Ӵ�����
	int m_nOutputMode;			//���ģʽ����ӦYAXIS_UNIT_DBV��YAXIS_UNIT_DBu��YAXIS_UNIT_DBm
	int m_nImpedance;			//�迹����YAXIS_UNIT_DBm������ʹ��	��λ��ŷķ

	int InitialControl();
	CFftConfDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFftConfDlg)
	enum { IDD = IDD_DIALOG_FftConf };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFftConfDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFftConfDlg)
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnRADIOLinear();
	afx_msg void OnRADIOLogarithm();
	afx_msg void OnSelchangeCOMBOLogUnit();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FFTCONFDLG_H__126F5B1C_214E_48DC_B601_745CEAA0F116__INCLUDED_)

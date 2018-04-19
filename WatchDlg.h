#if !defined(AFX_WATCHDLG_H__D0136C5D_0DF0_4EC2_8EAC_906CC9507EA7__INCLUDED_)
#define AFX_WATCHDLG_H__D0136C5D_0DF0_4EC2_8EAC_906CC9507EA7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WatchDlg.h : header file
//


class CWaveData;
class CMeasureDlg;
class CRecorderDoc;
class CProduct;
class CBgDlg;
class CWndManage;
class CMeasureItem;
class CMarkDlg;
class CZoomDlg;



//struct MeasureItem;

/////////////////////////////////////////////////////////////////////////////
// CWatchDlg dialog

typedef struct  
{
	CRect rcFrame;
	
}MeasureFrame;

typedef struct  
{
	CRect rcFrame;

}XAxisFrame;


typedef struct  
{
	CRect rcFrame;
	
}YAxisFrame;

/*
typedef struct  
{
	CRect rcFrame;

}YAxisFrameRight;


typedef struct  
{
	CRect rcFrame;

}YAxisFrameLeft;

*/

typedef struct  
{
	CRect rcXAxisMarkFrame;
	CRect rcPosMarkX_1;
	CRect rcPosMarkX_2;
	CRect rcPosScale_1;
	CRect rcPosScale_2;
	float fRatioMarkX_1;
	float fRatioMarkX_2;
	float fRatioScale_1;
	float fRatioScale_2;

	int nDisPickToLeftMarkX_1;
	int nDisPickToLeftMarkX_2;
	int nDisPickToLeftScale_1;
	int nDisPickToLeftScale_2;
	
}XAxisMarkFrame;


typedef struct  
{
	CRect rcYAxisMarkFrame;
	CRect rcPosMarkY_1;
	CRect rcPosMarkY_2;
	float fRatioMarkY_1;
	float fRatioMarkY_2;

	int nDisPickToTopMarkY_1;
	int nDisPickToTopMarkY_2;
	
}YAxisMarkFrame;



/*

////////////////////////AcqInfo�Ķ����Ѿ�����MyDef.h��//////////////////////////////////////////////////
typedef struct
{
	int range;
	int samplingLength;
	int samplingRate;
	double samplingTime;							//samplingTime = samplingLength/samplingRate
	int trigMode;
}AcqInfo;
*/







class CWatchDlg : public CDialog
{
// Construction
public:
	int ChangeAllToolBars();
	CString ComputeHorUnitFreq(double *pfResult, double fInput);
	int DrawXCoordinateFreq(CDC *pCdc);
	int DrawXCoordinateTime(CDC *pCdc);
	int DrawYCoordinateFreq(CDC *pCdc);
	int DrawYCoordinateTime(CDC *pCdc);
	int SwitchTimeAndFreq(int nDirection);									//�ı���������m_nWaveDataRange	nDirection������0 Freq->Time	1 Time->Freq	2 Freqһ�ֵ�λ->Freq����һ�ֵ�λ		
	int AdjustAllToolBar();
	int ChangeDisplayChn(const int* pChnSelected, int nChnSelectedNum);							//pChnSelected��һ�����飬����ͬDRAW_CHNUM_MAX��Ԫ������Ϊͨ������ţ�-1Ϊûѡ�У�ע������������̵��Ǹ�����
	int SynToolBar();																			//����ǰm_WaveDataList�е�ͨ������ͬ����CMainFrame��ToolBar��ȥ
	void AddWaveDataToWatchDlg(CWatchDlg* pAddWatchDlg);
	void UpdateWaveDataList(int nWaveIndexArray[] , int nWaveNum);
	void ChannelSelect();
	CString ComputeHorUnitTime(double* pfResult , double fInput);
	void ZoomRestoreModifyEveryWndRc(CPoint* point);
	void ChangeCursor(CPoint point);
	void DrawZoomDragBorder(CDC* pDC);
	void SetMarkByDefault();
	void DrawMark(CDC* pDC);
	void ModifyValueMarkDlg(int nMarkMode , float fChangeValue);
	void ModifyMarkPos(CPoint point);
	void ComputeMarkFrame();
	void MeasureEdit();
	void MeasureDelete();
	void ModifyPos();																			//������λ�ð���WatchDlg��MeasureDlg
	void AddToMeasureDlg(CWaveData* pWaveData ,int nMeaType ,int nMeaPart);						//������������MeasureDlg
	void MeasureAdd();
	void SetOwnedBgDlgSeleted();
	void SetOwnedBgDlg(CBgDlg* pBgDlg);
	void ComputeDisplayFrame();											//����m_WDW	 m_XF	 m_YFL	 m_YFR
	void ComputeFrame();												//���¼���	m_WDW	 m_XF	 m_YFL	 m_YFR	 /*m_MF*/	 m_rcWatchDlgFrame		 m_rcMFinTab;	
	virtual BOOL ShowWindow( int nCmdShow );							//������  showwindowͬʱ���WatchDlg,MeasureDlg
	void AddWaveDataToWatchDlg(CWaveData* pWaveData);
	void DrawCursorRead(CDC *pDC);

	void ZoomOutModifyEveryWndRc(CPoint *pt);
	void ZoomInModifyEveryWndRc(CPoint *pt);
	void ZoomOutModifyEveryWndRc2(CPoint *pt);							//�ꡰ2���ķŴ���С����Ϊ20150507��ǰ�汾ʹ��
	void ZoomInModifyEveryWndRc2(CPoint *pt);

	void RefreshParaDlgBar();
	void DrawSelectedBorder(CDC *pCdc);
	CFont m_editFont;
	void DrawTitle(CDC* pDC);
	void DrawBG(CDC* pCdc);
	void SetWatchDlgFrame(CRect rc);

	CRecorderDoc* m_pRecorderDoc;
	CProduct* m_pProduct;
	CBgDlg* m_pOwnedBgDlg;
	CWndManage* m_pWndManage;

	CWatchDlg(CWnd* pParent = NULL);   // standard constructor

	void SetIndex(int index);
	int m_nIndexInWaveList;				//ԭ�������  ��ɾ��
	void DrawWave(CDC* pCdc);
	
	CMeasureDlg* m_pMeasureDlg;				//���CWatchDlg���׵�CMeasureDlg
	XAxisFrame m_XF;
//	YAxisFrameLeft m_YFLeftOne;
//	YAxisFrameLeft m_YFLeftTwo;
//	YAxisFrameRight m_YFRightOne;
//	YAxisFrameRight m_YFRightTwo;
	YAxisFrame m_YF[4];						//��Ӧ��4����ʾ��ͨ����˳���ǣ� 0 2 WDW 3 1
	MeasureFrame m_MF;					//�Ȳ�ɾ 

	//MarkDlg���
	XAxisMarkFrame m_XMF;
	YAxisMarkFrame m_YMF[4];				//��Ӧ��4����ʾ��ͨ��,ÿ��ͨ��������������2��mark��
//	YAxisMarkFrame m_YMFLeftOne;
//	YAxisMarkFrame m_YMFLeftTwo;
//	YAxisMarkFrame m_YMFRightOne;
//	YAxisMarkFrame m_YMFRightTwo;
	int m_nMarkMode;					//0	δ��Mark��ѡ��	1 MarkY1��ѡ�� 2 MarkY2��ѡ�� 3 MarkX1��ѡ�� 4 MarkX2��ѡ�� 5 Sacle1��ѡ�� 6 Sacle2��ѡ��
	bool m_bMarkIsPick;						//�����������µı�־ ����ѡ����ĳ������
	CBitmap m_bmpMarkX;
	CBitmap m_bmpMarkY;
	CBitmap m_bmpScale;
	
	CMarkDlg* m_pMarkDlg;				//����Dialog
	
	//ZoomDlg���
	CZoomDlg* m_pZoomDlg;
	CRect m_rcZoomDrag;
	CPoint m_ptZoomDragStart;
	CPoint m_ptZoomDragEnd;

	bool m_bZoomDragIsPick;




	HCURSOR m_hCurCursor;
	
	CRect m_rcWatchDlgFrame;				//��rcָWatchDlg��Tab�е�λ��
	CRect m_rcMFinTab;						//��rcָMeasureDlg��Tab�е�λ��
	

	WaveDisplayWnd m_WDW;
	AcqInfo m_AcqInfo;						//�ñ����Ѿ����� ԭ����׼��ÿ��CWatchDlg��һ���ɼ����� ���Ǻ�����Ϊȫ��ͨ����ͬ���Ĳɼ�����
											//������m_AcqInfoӦ������CProduct����
	CPoint m_ptMouseMove;

	int m_nMode;							//0	ʱ��	1 Ƶ��
	CString m_strWatchDlgName;

	//WatchDlg�е�����
	int m_nWaveDataNum;					//����0
	CTypedPtrList<CObList , CWaveData*> m_WaveDataList;					//���DRAW_CHNUM_MAX��ͨ����
	int m_nWaveDataRange[DRAW_CHNUM_MAX];								//DRAW_CHNUM_MAX��ͨ�������̣�0:�ر� 1:1V 5:5V 10:10V 20:20V 

//	CRecorderView* pOwnView;

	virtual void OnOk();  
// Dialog Data
	//{{AFX_DATA(CWatchDlg)
	enum { IDD = IDD_DIALOG_WATCHDLG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWatchDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CWatchDlg)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	virtual BOOL OnInitDialog();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMeaAddmeasure();
	afx_msg void OnMeaDelmeasure();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLAYOUTMode1();
	afx_msg void OnLAYOUTMode2();
	afx_msg void OnLAYOUTMode3();
	afx_msg void OnLAYOUTMode4();
	afx_msg void OnWATCHProperty();
	afx_msg void OnWATCHAddOsc();
	afx_msg void OnWATCHAddFreDomain();
	afx_msg void OnWATCHChSel();
	afx_msg void OnWATCHCloseWatchDlg();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WATCHDLG_H__D0136C5D_0DF0_4EC2_8EAC_906CC9507EA7__INCLUDED_)

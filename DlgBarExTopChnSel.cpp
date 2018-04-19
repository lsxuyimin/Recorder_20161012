// DlgBarExTopChnSel.cpp : implementation file
//

#include "stdafx.h"
#include "recorder.h"
#include "DlgBarExTopChnSel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
#include "WatchDlg.h"
#include "WaveData.h"
#include "MainFrm.h"
#include "RecorderDoc.h"
#include "Product.h"
#include "FftComplex.h"
/////////////////////////////////////////////////////////////////////////////
extern CEvent g_PlayPauseEvent;
/////////////////////////////////////////////////////////////////////////////
// CDlgBarExTopChnSel dialog


CDlgBarExTopChnSel::CDlgBarExTopChnSel(CWnd* pParent /*=NULL*/)
{
	//{{AFX_DATA_INIT(CDlgBarExTopChnSel)
	//}}AFX_DATA_INIT
}


void CDlgBarExTopChnSel::DoDataExchange(CDataExchange* pDX)
{
	//CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgBarExTopChnSel)
	DDX_Control(pDX, IDC_COMBO_ChnRange4, m_ComboChnRange4);
	DDX_Control(pDX, IDC_COMBO_ChnRange3, m_ComboChnRange3);
	DDX_Control(pDX, IDC_COMBO_ChnRange2, m_ComboChnRange2);
	DDX_Control(pDX, IDC_COMBO_ChnRange1, m_ComboChnRange1);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgBarExTopChnSel, CDialogBar)
	//{{AFX_MSG_MAP(CDlgBarExTopChnSel)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_MESSAGE(WM_INITDIALOG,OnInitDialog)
	ON_CBN_SELCHANGE(IDC_COMBO_ChnRange1, OnSelchangeCOMBOChnRange1)
	ON_CBN_SELCHANGE(IDC_COMBO_ChnRange2, OnSelchangeCOMBOChnRange2)
	ON_CBN_SELCHANGE(IDC_COMBO_ChnRange3, OnSelchangeCOMBOChnRange3)
	ON_CBN_SELCHANGE(IDC_COMBO_ChnRange4, OnSelchangeCOMBOChnRange4)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgBarExTopChnSel message handlers

BOOL CDlgBarExTopChnSel::OnInitDialog(UINT wParam,LONG lParam) 
{
	//CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	BOOL bRet = HandleInitDialog(wParam,lParam);
	if (!UpdateData(FALSE))
	{
		TRACE("InitCDataStatus Failed��");
	}

	//��ȡ4��CStaitc��ָ��
	m_pStaticChnSel[0] = (CStatic*)GetDlgItem(IDC_STATIC_ChnSel1);				//�������Է���GetDlgItemֻ����ʱָ�룬�����Ժ�ʹ��
	m_pStaticChnSel[1] = (CStatic*)GetDlgItem(IDC_STATIC_ChnSel2);
	m_pStaticChnSel[2] = (CStatic*)GetDlgItem(IDC_STATIC_ChnSel3);
	m_pStaticChnSel[3] = (CStatic*)GetDlgItem(IDC_STATIC_ChnSel4);
	//CComboBox	
	m_pComboChnRange[0] = &m_ComboChnRange1;
	m_pComboChnRange[1] = &m_ComboChnRange2;
	m_pComboChnRange[2] = &m_ComboChnRange3;
	m_pComboChnRange[3] = &m_ComboChnRange4;

	//��ʼ��CComboBox�ϵ�����
	int nCount = 0;
	for (nCount=0 ; nCount<DRAW_CHNUM_MAX ; nCount++)
	{
		m_pComboChnRange[nCount]->AddString("�ر�");
		m_pComboChnRange[nCount]->AddString("��1V");
		m_pComboChnRange[nCount]->AddString("��5V");
		m_pComboChnRange[nCount]->AddString("��10V");
		m_pComboChnRange[nCount]->AddString("��20V");
		m_pComboChnRange[nCount]->SetCurSel(0);
	}


	int m_nModeOld = DISPLAY_MODE_TIME;
	int m_nModeFreqRangeOld = YAXIS_UNIT_V;


	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

int CDlgBarExTopChnSel::ChangeToolBarDisplayChn(CWatchDlg *pWatchDlg)
{
	InitializeDisplayChn(pWatchDlg);


	CWaveData* pWaveData = NULL;
	CString strTemp;

	m_nWaveDataNum = pWatchDlg->m_nWaveDataNum;
	int nCount = 0;
	m_pStaticChnSel[0] = (CStatic*)GetDlgItem(IDC_STATIC_ChnSel1);				//�������Է���GetDlgItemֻ����ʱָ�룬�����Ժ�ʹ��
	m_pStaticChnSel[1] = (CStatic*)GetDlgItem(IDC_STATIC_ChnSel2);
	m_pStaticChnSel[2] = (CStatic*)GetDlgItem(IDC_STATIC_ChnSel3);
	m_pStaticChnSel[3] = (CStatic*)GetDlgItem(IDC_STATIC_ChnSel4);


	/*1. �Ƚ���������pWatchDlg��Ϊ�µ�*/
	m_pCurWatchDlg = pWatchDlg ;

	/*2. ��CStatic*/
	for ( nCount=0 ; nCount<DRAW_CHNUM_MAX ; nCount++ )
	{
		if (nCount<m_nWaveDataNum)
		{
			pWaveData = (CWaveData*)pWatchDlg->m_WaveDataList.GetAt(pWatchDlg->m_WaveDataList.FindIndex(nCount));
			m_pComboChnRange[nCount]->ChangeTextColor(pWaveData->m_LineColor);

			m_WaveDataList.AddTail(pWaveData);

			(m_pStaticChnSel[nCount])->SetWindowText(pWaveData->m_strChanName);
		//	GetDlgItem(IDC_STATIC_ChnSel1)->SetWindowText("dfsf");
		}
		else{
			switch (nCount)
			{
			case 0:
				strTemp.Format("�ź�һ");
				break;
			case 1:
				strTemp.Format("�źŶ�");
				break;
			case 2:
				strTemp.Format("�ź���");
				break;
			case 3:
				strTemp.Format("�ź���");
				break;
			default:
				break;
			}
			m_pStaticChnSel[nCount]->SetWindowText(strTemp);
			m_pComboChnRange[nCount]->ChangeTextColor(RGB(0,0,0));
		}
	}


	/*3. ��CComboBoxRange������pWatchDlg�������m_nWaveDataRange*/
	/*Ҫ��ʱ���Ƶ��*/
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CRecorderDoc *pDoc = (CRecorderDoc*)(pFrame->GetActiveDocument());
	CProduct* pProduct = &(pDoc->m_Product);
	if ( pWatchDlg->m_nMode == DISPLAY_MODE_TIME )
	{
		//ʱ��
		for ( nCount=0 ; nCount<DRAW_CHNUM_MAX ; nCount++)
		{
			switch ( pWatchDlg->m_nWaveDataRange[nCount] )
			{
			case RANGE_CLOSED:
				m_pComboChnRange[nCount]->SetCurSel(0);
				break;
			case RANGE_TIME_pn1V:
				m_pComboChnRange[nCount]->SetCurSel(1);
				break;
			case RANGE_TIME_pn5V:
				m_pComboChnRange[nCount]->SetCurSel(2);
				break;
			case RANGE_TIME_pn10V:
				m_pComboChnRange[nCount]->SetCurSel(3);
				break;
			case RANGE_TIME_pn20V:
				m_pComboChnRange[nCount]->SetCurSel(4);
				break;
			default:
				break;
			}
		}	
	}
	else{
		//Ƶ��
		for ( nCount=0 ; nCount<DRAW_CHNUM_MAX ; nCount++)
		{
			//��Ҫ��4����������
			switch ( pProduct->m_FftConf.nOutputMode )
			{
			case YAXIS_UNIT_V:
				switch ( pWatchDlg->m_nWaveDataRange[nCount] )
				{
				case RANGE_CLOSED:
					m_pComboChnRange[nCount]->SetCurSel(0);
					break;
				case RANGE_FREQ_V_0_500:
					m_pComboChnRange[nCount]->SetCurSel(1);
					break;
				case RANGE_FREQ_V_0_1000:
					m_pComboChnRange[nCount]->SetCurSel(2);
					break;
				}
				break;
			case YAXIS_UNIT_DBV:
				switch ( pWatchDlg->m_nWaveDataRange[nCount] )
				{
				case RANGE_CLOSED:
					m_pComboChnRange[nCount]->SetCurSel(0);
					break;
				case RANGE_FREQ_DBV_n100_0:
					m_pComboChnRange[nCount]->SetCurSel(1);
					break;
				case RANGE_FREQ_DBV_n100_100:
					m_pComboChnRange[nCount]->SetCurSel(2);
					break;
				}
				break;
			case YAXIS_UNIT_DBu:
				switch ( pWatchDlg->m_nWaveDataRange[nCount] )
				{
				case RANGE_CLOSED:
					m_pComboChnRange[nCount]->SetCurSel(0);
					break;
				case RANGE_FREQ_DBu_0_100:
					m_pComboChnRange[nCount]->SetCurSel(1);
					break;
				case RANGE_FREQ_DBu_0_200:
					m_pComboChnRange[nCount]->SetCurSel(2);
					break;
				}
				break;
			case YAXIS_UNIT_DBm:
				switch ( pWatchDlg->m_nWaveDataRange[nCount] )
				{
				case RANGE_CLOSED:
					m_pComboChnRange[nCount]->SetCurSel(0);
					break;
				case RANGE_FREQ_DBm_n100_0:
					m_pComboChnRange[nCount]->SetCurSel(1);
					break;
				case RANGE_FREQ_DBm_n100_100:
					m_pComboChnRange[nCount]->SetCurSel(2);
					break;
				}
				break;
			}
			
		}	
	}



	
	

	
	Invalidate();

	return 0;
}

void CDlgBarExTopChnSel::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
//	DrawStatic(&dc);
	// Do not call CDialogBar::OnPaint() for painting messages
}

int CDlgBarExTopChnSel::DrawStatic(CDC *pDC)
{
	CWaveData* pWaveData = NULL;
	CRect rcDrawStatic;
	int nCount=0 ;
	for ( nCount=0 ; nCount<DRAW_CHNUM_MAX ; nCount )
	{
		if ( nCount< m_nWaveDataNum )
		{
			//�����ź�
			pWaveData = m_WaveDataList.GetAt(m_WaveDataList.FindIndex(nCount));
			//�����ԭ������
			m_pStaticChnSel[nCount]->SetWindowText("");
		}
		else{
			//�������źţ���ʾ���ź�X���رա�
		}


	}





	return 0;
}

HBRUSH CDlgBarExTopChnSel::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialogBar::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here

	
	if ( nCtlColor==CTLCOLOR_STATIC )
	{
		switch ( pWnd->GetDlgCtrlID() )
		{
		case IDC_STATIC_ChnSel1:
			if (m_nWaveDataNum>=1)			
			{
				pDC->SetTextColor( ((CWaveData*)m_WaveDataList.GetAt(m_WaveDataList.FindIndex(0)))->m_LineColor );
			//	pDC->SetTextColor( RGB(255,0,0) );
			}
			break;
		case IDC_STATIC_ChnSel2:
			if (m_nWaveDataNum>=2)			
			{
				pDC->SetTextColor( ((CWaveData*)m_WaveDataList.GetAt(m_WaveDataList.FindIndex(1)))->m_LineColor );
			}
			break;
		case IDC_STATIC_ChnSel3:
			if (m_nWaveDataNum>=3)			
			{
				pDC->SetTextColor( ((CWaveData*)m_WaveDataList.GetAt(m_WaveDataList.FindIndex(2)))->m_LineColor );
			}
			break;
		case IDC_STATIC_ChnSel4:
			if (m_nWaveDataNum>=4)			
			{
				pDC->SetTextColor( ((CWaveData*)m_WaveDataList.GetAt(m_WaveDataList.FindIndex(3)))->m_LineColor );
			}
			break;
		}
	//	pDC->SetTextColor( RGB(255,0,0) );
	//	pDC->SetBkMode(TRANSPARENT); //���ñ���͸��
	}
	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

int CDlgBarExTopChnSel::InitializeDisplayChn(CWatchDlg *pWatchDlg)
{
	m_WaveDataList.RemoveAll();
	m_pCurWatchDlg = NULL;
	
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	//	CRecorderView* pView = (CRecorderView*)(pFrame->GetActiveView());
	CRecorderDoc *pDoc = (CRecorderDoc*)(pFrame->GetActiveDocument());
	CProduct* pProduct = &(pDoc->m_Product);
	
	bool bNeedModify = false;
	if ( m_nModeCur == pWatchDlg->m_nMode )
	{
		//����ʱ�� ���� Ƶ�� �� ���������Ƶ�����п�����Ҫ�޸�
		if ( m_nModeCur == DISPLAY_MODE_FREQ)
		{
			if ( m_nModeFreqRangeCur == pProduct->m_FftConf.nOutputMode )
			{
				//Ƶ���У������ʽҲһ���ǾͲ����޸�
			}
			else{
				//�޸�Ƶ��������λ ������
				bNeedModify = true;
			}
		}
		else{
			//ʱ���� �����޸�
		}
	}
	else{
		//�޸�
		bNeedModify = true;
	}
	m_nModeCur = pWatchDlg->m_nMode;
	m_nModeFreqRangeCur = pProduct->m_FftConf.nOutputMode;

	if ( bNeedModify )
	{
		int nCount = 0;
/*
		CComboBox* pComboBox[DRAW_CHNUM_MAX];
		pComboBox[0] = &m_ComboChnRange1;
		pComboBox[1] = &m_ComboChnRange2;
		pComboBox[2] = &m_ComboChnRange3;
		pComboBox[3] = &m_ComboChnRange4;
*/

		for ( nCount=0 ; nCount<DRAW_CHNUM_MAX ; nCount++)
		{
			m_pComboChnRange[nCount]->ResetContent();
			if ( m_nModeCur == DISPLAY_MODE_TIME )
			{	//�ĳ�ʱ��
				m_pComboChnRange[nCount]->AddString("�ر�");
				m_pComboChnRange[nCount]->AddString("��1V");
				m_pComboChnRange[nCount]->AddString("��5V");
				m_pComboChnRange[nCount]->AddString("��10V");
				m_pComboChnRange[nCount]->AddString("��20V");
				//m_pComboChnRange[nCount]->SetCurSel(0);
			}
			else{
				//�ĳ�Ƶ��
				switch ( m_nModeFreqRangeCur )
				{
				case YAXIS_UNIT_V:
					m_pComboChnRange[nCount]->AddString("�ر�");
					m_pComboChnRange[nCount]->AddString("0 ~ 0.5V");
					m_pComboChnRange[nCount]->AddString("0 ~ 1V");
					//m_pComboChnRange[nCount]->SetCurSel(0);
					break;
				case YAXIS_UNIT_DBV:
					m_pComboChnRange[nCount]->AddString("�ر�");
					m_pComboChnRange[nCount]->AddString("-100 ~ 0");
					m_pComboChnRange[nCount]->AddString("-100 ~ 100");
					//m_pComboChnRange[nCount]->SetCurSel(0);
					break;
				case YAXIS_UNIT_DBu:
					m_pComboChnRange[nCount]->AddString("�ر�");
					m_pComboChnRange[nCount]->AddString("0 ~ 100");
					m_pComboChnRange[nCount]->AddString("0 ~ 200");
					//m_pComboChnRange[nCount]->SetCurSel(0);
					break;
				case YAXIS_UNIT_DBm:
					m_pComboChnRange[nCount]->AddString("�ر�");
					m_pComboChnRange[nCount]->AddString("-100 ~ 0");
					m_pComboChnRange[nCount]->AddString("-100 ~ 100");
					//m_pComboChnRange[nCount]->SetCurSel(0);
					break;
				default:
					break;
				}

			}
		}
	}


	return 0;
}

void CDlgBarExTopChnSel::OnSelchangeCOMBOChnRange1() 
{
	// TODO: Add your control notification handler code here
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CRecorderDoc *pDoc = (CRecorderDoc*)(pFrame->GetActiveDocument());
	CProduct* pProduct = &(pDoc->m_Product);

	//�ı�m_pCurWatchDlgָ���CWatchDlg�е�m_nWaveDataRange����
	int nSelect = m_ComboChnRange1.GetCurSel();
	int nRange = -1;
	
	/*Ҫ��ʱ���Ƶ��ֱ���*/
	if ( m_pCurWatchDlg->m_nMode == DISPLAY_MODE_TIME )
	{
		/*ʱ��*/
		switch ( nSelect )
		{
		case 0:
			nRange = RANGE_CLOSED;
			break;
		case 1:
			nRange = RANGE_TIME_pn1V;
			break;
		case 2:
			nRange = RANGE_TIME_pn5V;
			break;
		case 3:
			nRange = RANGE_TIME_pn10V;
			break;
		case 4:
			nRange = RANGE_TIME_pn20V;
			break;
		default:			
			TRACE("!!!ERROR : CDlgBarExTopChnSel::OnSelchangeCOMBOChnRange1() \n");
			break;
		}
	}
	else{
		/*Ƶ�� ����4�ֵ�λҪ����*/
		switch ( pProduct->m_FftConf.nOutputMode )
		{
		case YAXIS_UNIT_V:
			switch ( nSelect )
			{
			case 0:
				nRange = RANGE_CLOSED;
				break;
			case 1:
				nRange = RANGE_FREQ_V_0_500;
				break;
			case 2:
				nRange = RANGE_FREQ_V_0_1000;
				break;
			}
			break;
		case YAXIS_UNIT_DBV:
			switch ( nSelect )
			{
			case 0:
				nRange = RANGE_CLOSED;
				break;
			case 1:
				nRange = RANGE_FREQ_DBV_n100_0;
				break;
			case 2:
				nRange = RANGE_FREQ_DBV_n100_100;
				break;
			}
			break;
		case YAXIS_UNIT_DBu:
			switch ( nSelect )
			{
			case 0:
				nRange = RANGE_CLOSED;
				break;
			case 1:
				nRange = RANGE_FREQ_DBu_0_100;
				break;
			case 2:
				nRange = RANGE_FREQ_DBu_0_200;
				break;
			}
			break;
		case YAXIS_UNIT_DBm:
			switch ( nSelect )
			{
			case 0:
				nRange = RANGE_CLOSED;
				break;
			case 1:
				nRange = RANGE_FREQ_DBm_n100_0;
				break;
			case 2:
				nRange = RANGE_FREQ_DBm_n100_100;
				break;
			}
			break;
		default:
			TRACE("!!!ERROR : CDlgBarExTopChnSel::OnSelchangeCOMBOChnRange1() \n");
			break;
		}
	}
		
	//m_pCurWatchDlg->m_nWaveDataRange[0] = nRange;

	/*����������ڲ��ž������ػ�*/
	
	//if ( !(pProduct->m_bPlaying) )
	if ( WAIT_OBJECT_0 != WaitForSingleObject(g_PlayPauseEvent,0) )
	{
		//�������źţ�����ͣ״̬
		m_pCurWatchDlg->m_nWaveDataRange[0] = nRange;
		m_pCurWatchDlg->Invalidate();
	}
	else{
		g_PlayPauseEvent.ResetEvent();
		
		Sleep(SLEEP_TIME);
		m_pCurWatchDlg->m_nWaveDataRange[0] = nRange;
		g_PlayPauseEvent.SetEvent();
	}
}

void CDlgBarExTopChnSel::OnSelchangeCOMBOChnRange2() 
{
	// TODO: Add your control notification handler code here
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CRecorderDoc *pDoc = (CRecorderDoc*)(pFrame->GetActiveDocument());
	CProduct* pProduct = &(pDoc->m_Product);

	//�ı�m_pCurWatchDlgָ���CWatchDlg�е�m_nWaveDataRange����
	int nSelect = m_ComboChnRange2.GetCurSel();
	int nRange = -1;

	/*Ҫ��ʱ���Ƶ��ֱ���*/
	if ( m_pCurWatchDlg->m_nMode == DISPLAY_MODE_TIME )
	{
		/*ʱ��*/
		switch ( nSelect )
		{
		case 0:
			nRange = RANGE_CLOSED;
			break;
		case 1:
			nRange = RANGE_TIME_pn1V;
			break;
		case 2:
			nRange = RANGE_TIME_pn5V;
			break;
		case 3:
			nRange = RANGE_TIME_pn10V;
			break;
		case 4:
			nRange = RANGE_TIME_pn20V;
			break;
		default:			
			TRACE("!!!ERROR : CDlgBarExTopChnSel::OnSelchangeCOMBOChnRange1() \n");
			break;
		}
	}
	else{
		/*Ƶ�� ����4�ֵ�λҪ����*/
		switch ( pProduct->m_FftConf.nOutputMode )
		{
		case YAXIS_UNIT_V:
			switch ( nSelect )
			{
			case 0:
				nRange = RANGE_CLOSED;
				break;
			case 1:
				nRange = RANGE_FREQ_V_0_500;
				break;
			case 2:
				nRange = RANGE_FREQ_V_0_1000;
				break;
			}
			break;
			case YAXIS_UNIT_DBV:
				switch ( nSelect )
				{
				case 0:
					nRange = RANGE_CLOSED;
					break;
				case 1:
					nRange = RANGE_FREQ_DBV_n100_0;
					break;
				case 2:
					nRange = RANGE_FREQ_DBV_n100_100;
					break;
				}
				break;
				case YAXIS_UNIT_DBu:
					switch ( nSelect )
					{
					case 0:
						nRange = RANGE_CLOSED;
						break;
					case 1:
						nRange = RANGE_FREQ_DBu_0_100;
						break;
					case 2:
						nRange = RANGE_FREQ_DBu_0_200;
						break;
					}
					break;
					case YAXIS_UNIT_DBm:
						switch ( nSelect )
						{
						case 0:
							nRange = RANGE_CLOSED;
							break;
						case 1:
							nRange = RANGE_FREQ_DBm_n100_0;
							break;
						case 2:
							nRange = RANGE_FREQ_DBm_n100_100;
							break;
						}
						break;
						default:
							TRACE("!!!ERROR : CDlgBarExTopChnSel::OnSelchangeCOMBOChnRange1() \n");
							break;
		}
	}
	
	//m_pCurWatchDlg->m_nWaveDataRange[1] = nRange;
	
	//�������ػ�
	//if ( !(pProduct->m_bPlaying) )
	if ( WAIT_OBJECT_0 != WaitForSingleObject(g_PlayPauseEvent,0) )
	{
		//�������źţ�����ͣ״̬
		m_pCurWatchDlg->m_nWaveDataRange[1] = nRange;
		m_pCurWatchDlg->Invalidate();
	}
	else{
		g_PlayPauseEvent.ResetEvent();
		
		Sleep(SLEEP_TIME);
		m_pCurWatchDlg->m_nWaveDataRange[1] = nRange;
		g_PlayPauseEvent.SetEvent();
	}
}

void CDlgBarExTopChnSel::OnSelchangeCOMBOChnRange3() 
{
	// TODO: Add your control notification handler code here
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CRecorderDoc *pDoc = (CRecorderDoc*)(pFrame->GetActiveDocument());
	CProduct* pProduct = &(pDoc->m_Product);

	//�ı�m_pCurWatchDlgָ���CWatchDlg�е�m_nWaveDataRange����
	int nSelect = m_ComboChnRange3.GetCurSel();
	int nRange = -1;

	/*Ҫ��ʱ���Ƶ��ֱ���*/
	if ( m_pCurWatchDlg->m_nMode == DISPLAY_MODE_TIME )
	{
		/*ʱ��*/
		switch ( nSelect )
		{
		case 0:
			nRange = RANGE_CLOSED;
			break;
		case 1:
			nRange = RANGE_TIME_pn1V;
			break;
		case 2:
			nRange = RANGE_TIME_pn5V;
			break;
		case 3:
			nRange = RANGE_TIME_pn10V;
			break;
		case 4:
			nRange = RANGE_TIME_pn20V;
			break;
		default:			
			TRACE("!!!ERROR : CDlgBarExTopChnSel::OnSelchangeCOMBOChnRange1() \n");
			break;
		}
	}
	else{
		/*Ƶ�� ����4�ֵ�λҪ����*/
		switch ( pProduct->m_FftConf.nOutputMode )
		{
		case YAXIS_UNIT_V:
			switch ( nSelect )
			{
			case 0:
				nRange = RANGE_CLOSED;
				break;
			case 1:
				nRange = RANGE_FREQ_V_0_500;
				break;
			case 2:
				nRange = RANGE_FREQ_V_0_1000;
				break;
			}
			break;
			case YAXIS_UNIT_DBV:
				switch ( nSelect )
				{
				case 0:
					nRange = RANGE_CLOSED;
					break;
				case 1:
					nRange = RANGE_FREQ_DBV_n100_0;
					break;
				case 2:
					nRange = RANGE_FREQ_DBV_n100_100;
					break;
				}
				break;
				case YAXIS_UNIT_DBu:
					switch ( nSelect )
					{
					case 0:
						nRange = RANGE_CLOSED;
						break;
					case 1:
						nRange = RANGE_FREQ_DBu_0_100;
						break;
					case 2:
						nRange = RANGE_FREQ_DBu_0_200;
						break;
					}
					break;
					case YAXIS_UNIT_DBm:
						switch ( nSelect )
						{
						case 0:
							nRange = RANGE_CLOSED;
							break;
						case 1:
							nRange = RANGE_FREQ_DBm_n100_0;
							break;
						case 2:
							nRange = RANGE_FREQ_DBm_n100_100;
							break;
						}
						break;
						default:
							TRACE("!!!ERROR : CDlgBarExTopChnSel::OnSelchangeCOMBOChnRange1() \n");
							break;
		}
	}
	
	//m_pCurWatchDlg->m_nWaveDataRange[2] = nRange;
	
	//�������ػ�
	//if ( !(pProduct->m_bPlaying) )
	if ( WAIT_OBJECT_0 != WaitForSingleObject(g_PlayPauseEvent,0) )
	{
		//�������źţ�����ͣ״̬
		m_pCurWatchDlg->m_nWaveDataRange[2] = nRange;
		m_pCurWatchDlg->Invalidate();
	}
	else{
		g_PlayPauseEvent.ResetEvent();
		
		Sleep(SLEEP_TIME);
		m_pCurWatchDlg->m_nWaveDataRange[2] = nRange;
		g_PlayPauseEvent.SetEvent();
	}
}

void CDlgBarExTopChnSel::OnSelchangeCOMBOChnRange4() 
{
	// TODO: Add your control notification handler code here
	
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CRecorderDoc *pDoc = (CRecorderDoc*)(pFrame->GetActiveDocument());
	CProduct* pProduct = &(pDoc->m_Product);

	//�ı�m_pCurWatchDlgָ���CWatchDlg�е�m_nWaveDataRange����
	int nSelect = m_ComboChnRange4.GetCurSel();
	int nRange = -1;

	/*Ҫ��ʱ���Ƶ��ֱ���*/
	if ( m_pCurWatchDlg->m_nMode == DISPLAY_MODE_TIME )
	{
		/*ʱ��*/
		switch ( nSelect )
		{
		case 0:
			nRange = RANGE_CLOSED;
			break;
		case 1:
			nRange = RANGE_TIME_pn1V;
			break;
		case 2:
			nRange = RANGE_TIME_pn5V;
			break;
		case 3:
			nRange = RANGE_TIME_pn10V;
			break;
		case 4:
			nRange = RANGE_TIME_pn20V;
			break;
		default:			
			TRACE("!!!ERROR : CDlgBarExTopChnSel::OnSelchangeCOMBOChnRange1() \n");
			break;
		}
	}
	else{
		/*Ƶ�� ����4�ֵ�λҪ����*/
		switch ( pProduct->m_FftConf.nOutputMode )
		{
		case YAXIS_UNIT_V:
			switch ( nSelect )
			{
			case 0:
				nRange = RANGE_CLOSED;
				break;
			case 1:
				nRange = RANGE_FREQ_V_0_500;
				break;
			case 2:
				nRange = RANGE_FREQ_V_0_1000;
				break;
			}
			break;
			case YAXIS_UNIT_DBV:
				switch ( nSelect )
				{
				case 0:
					nRange = RANGE_CLOSED;
					break;
				case 1:
					nRange = RANGE_FREQ_DBV_n100_0;
					break;
				case 2:
					nRange = RANGE_FREQ_DBV_n100_100;
					break;
				}
				break;
				case YAXIS_UNIT_DBu:
					switch ( nSelect )
					{
					case 0:
						nRange = RANGE_CLOSED;
						break;
					case 1:
						nRange = RANGE_FREQ_DBu_0_100;
						break;
					case 2:
						nRange = RANGE_FREQ_DBu_0_200;
						break;
					}
					break;
					case YAXIS_UNIT_DBm:
						switch ( nSelect )
						{
						case 0:
							nRange = RANGE_CLOSED;
							break;
						case 1:
							nRange = RANGE_FREQ_DBm_n100_0;
							break;
						case 2:
							nRange = RANGE_FREQ_DBm_n100_100;
							break;
						}
						break;
						default:
							TRACE("!!!ERROR : CDlgBarExTopChnSel::OnSelchangeCOMBOChnRange1() \n");
							break;
		}
	}
	
	//m_pCurWatchDlg->m_nWaveDataRange[3] = nRange;
	
	//�������ػ�
	//if ( !(pProduct->m_bPlaying) )
	if ( WAIT_OBJECT_0 != WaitForSingleObject(g_PlayPauseEvent,0) )
	{
		//�������źţ�����ͣ״̬
		m_pCurWatchDlg->m_nWaveDataRange[3] = nRange;
		m_pCurWatchDlg->Invalidate();
	}
	else{
		g_PlayPauseEvent.ResetEvent();
		
		Sleep(SLEEP_TIME);
		m_pCurWatchDlg->m_nWaveDataRange[3] = nRange;
		g_PlayPauseEvent.SetEvent();
	}
}

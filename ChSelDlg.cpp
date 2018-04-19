// ChSelDlg.cpp : implementation file
//

#include "stdafx.h"
#include "recorder.h"
#include "ChSelDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
#include "WatchDlg.h"
#include "WaveData.h"
#include "Product.h"
#include "MainFrm.h"
#include "RecorderDoc.h"
#include "DlgBarExTopChnSel.h"
#include "MarkDlg.h"

/////////////////////////////////////////////////////////////////////////////
extern CEvent g_PlayPauseEvent;
/////////////////////////////////////////////////////////////////////////////
// CChSelDlg dialog


CChSelDlg::CChSelDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CChSelDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CChSelDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pOwnedWatchDlg = (CWatchDlg*)pParent;
}


void CChSelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CChSelDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CChSelDlg, CDialog)
	//{{AFX_MSG_MAP(CChSelDlg)
	ON_NOTIFY(NM_CLICK, IDC_LIST_CH, OnClickListCh)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChSelDlg message handlers

BOOL CChSelDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here	
	CString strTemp;
	strTemp.Format("��ѡ��Ҫ��ʾ��ͨ��������ͬʱ��ʾ%d��ͨ����",DRAW_CHNUM_MAX);
	GetDlgItem(IDC_STATIC_Tip)->SetWindowText(strTemp);
	//��ʼ�� LIST
	CListCtrl* pList;
	pList = (CListCtrl*)(GetDlgItem(IDC_LIST_CH));
	pList->SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_FLATSB|LVS_EX_CHECKBOXES );
	pList->InsertColumn(0,_T("Ϊ�˾��ж�����"),LVCFMT_CENTER, 0);
	pList->InsertColumn(1,_T("ͨ��"),LVCFMT_CENTER, 150);
	//	pList->InsertColumn(2,_T("ͨ��"),LVCFMT_CENTER, 100);
	pList->DeleteColumn(0);
	
	for (int nChNUm=0 ; nChNUm<64 ; nChNUm++ )
	{
		pList->InsertItem(nChNUm,"",0);
		CString strTemp;
		//strTemp.Format("%c", 0 );
		//pList->SetItemText(nChNUm,0,strTemp );
		strTemp.Format("Ch %d", nChNUm+1 );
		pList->SetItemText(nChNUm,0,strTemp);
	}

	CWatchDlg* pParentDlg = m_pOwnedWatchDlg;
	CWaveData* pWaveData = NULL;
	int nCount = 0;
	for (nCount=0 ; nCount<pParentDlg->m_nWaveDataNum ; nCount++)
	{
		pWaveData = (CWaveData*)pParentDlg->m_WaveDataList.GetAt(pParentDlg->m_WaveDataList.FindIndex(nCount));
		pList->SetCheck(pWaveData->m_nChnIndex);
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CChSelDlg::OnClickListCh(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here

	//�����CList�Ժ󣬽��в����Ƿ��ǵ����DRAW_CHNUM_MAX��ѡ��
/*
	CListCtrl* pList;
	pList = (CListCtrl*)(GetDlgItem(IDC_LIST_CH));
	CString str,str1=TEXT("��Check�������ֱ�Ϊ��");
	
    for(int i=0; i<pList->GetItemCount(); i++)
    {
        if( pList->GetCheck(i))
        {
            str.Format(TEXT("[%d]"),i);
            str1+=str;
        }
    }	
    AfxMessageBox(str1);
*/

	
	*pResult = 0;
}

void CChSelDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	//�ɿ��Լ�飺����ѡ���������1��DRAW_CHNUM_MAX֮��
	int nSelCount = 0;
	CListCtrl* pList;
	pList = (CListCtrl*)(GetDlgItem(IDC_LIST_CH));	
	for(int nCount=0; nCount<pList->GetItemCount(); nCount++)
	{
        if( pList->GetCheck(nCount))
        {
            nSelCount++;
        }
    }	
	if (0 == nSelCount)
	{
		MessageBox("��ѡ��һ��������ʾ��ͨ��");
		return;
	}
	if (nSelCount > DRAW_CHNUM_MAX)
	{
		CString strDisplay;
		strDisplay.Format("��ѡ���ͨ������%d���������������۲첨��",DRAW_CHNUM_MAX);
		MessageBox(strDisplay);
		return;
	}

	//�Ըı���ͨ����CWatchDlg����Ӧ�ĸı�
	ChangeDisplayChn();

	
	CDialog::OnOK();
}

int CChSelDlg::ChangeDisplayChn()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CRecorderDoc *pDoc = (CRecorderDoc*)(pFrame->GetActiveDocument());
	CProduct* pProduct = &(pDoc->m_Product);

	CWaveData* pWaveData = NULL;
	CListCtrl* pList;
	pList = (CListCtrl*)(GetDlgItem(IDC_LIST_CH));	


	if (m_pOwnedWatchDlg->m_pMarkDlg != NULL)
	{
		m_pOwnedWatchDlg->m_pMarkDlg->ClearMark();		
		m_pOwnedWatchDlg->m_pMarkDlg->ShowWindow(SW_HIDE);
		m_pOwnedWatchDlg->Invalidate();
	}
	//1. ����ڲ��Ź����У���ͣ����
	if (pProduct->m_bPlaying)
	{
		g_PlayPauseEvent.ResetEvent();
		Sleep(SLEEP_TIME);
	}

	//2. �ĸ�CChSelDlg������CWatchDlg�е�m_WaveDataList��m_nWaveDataNum
	int nChnNumSel = 0;
	int aChnSel[DRAW_CHNUM_MAX] = {-1,-1,-1,-1};				//��������Ż���0
	int nCount=0;

	for( nCount=0; nCount<pList->GetItemCount(); nCount++)				//�������ǰ�Ѿ�������������ˣ�����aChnSel����Խ��	
	{
        if( pList->GetCheck(nCount))
        {
			aChnSel[nChnNumSel] = nCount;
            nChnNumSel++;
        }
    }	
	m_pOwnedWatchDlg->ChangeDisplayChn(aChnSel , nChnNumSel);
/*
	m_pOwnedWatchDlg->m_nWaveDataNum = nChnNumSel;
	m_pOwnedWatchDlg->m_WaveDataList.RemoveAll();
	for ( nCount=0 ; nCount<DRAW_CHNUM_MAX ; nCount++)
	{
		if ( -1 != aChnSel[nCount] )
		{
			pWaveData = (CWaveData*)pProduct->m_WaveList.GetAt(pProduct->m_WaveList.FindIndex(aChnSel[nCount]));
			m_pOwnedWatchDlg->m_WaveDataList.AddTail(pWaveData);
		}
	}
*/
	

	//3. ���¼��������µ�ͨ���Ժ��ComputeFrame��ComputeMarkFrame
	m_pOwnedWatchDlg->ComputeFrame();
	m_pOwnedWatchDlg->Invalidate();


	//4. �ı乤��������ʾͨ����״̬
	pFrame->m_pDlgBarExTopChnSel->ChangeToolBarDisplayChn(m_pOwnedWatchDlg);

	//������ڲ���״̬�ͽ���һ��invalidate()
	


	//5. ��������
	if (pProduct->m_bPlaying)
	{
		g_PlayPauseEvent.SetEvent();
	}



	return 0;
}

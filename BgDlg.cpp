// BgDlg.cpp : implementation file
//

#include "stdafx.h"
#include "recorder.h"
#include "BgDlg.h"

#include "WatchDlg.h"
//#include "WaveData.h"
#include "WndManage.h"
#include "MeasureDlg.h"
#include "MarkDlg.h"
#include "ZoomDlg.h"
#include "MainFrm.h"
#include "DlgBarExTopChnSel.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBgDlg dialog


CBgDlg::CBgDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBgDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBgDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT


	m_pWndManage = NULL;
	m_nWatchDlgNum = 0;
	m_nWatchDlgHasBeenNew = 0;
}


void CBgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBgDlg)
	DDX_Control(pDX, IDC_TAB_BgDlg, m_TabBgDlg);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBgDlg, CDialog)
	//{{AFX_MSG_MAP(CBgDlg)
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_BgDlg, OnSelchangeTABBgDlg)
	ON_NOTIFY(NM_CLICK, IDC_TAB_BgDlg, OnClickTABBgDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBgDlg message handlers

void CBgDlg::SetIndex(int index)
{
	m_nIndex = index;

}

void CBgDlg::SetBgDlgFrame(CRect rc)
{
	//������Χ��ܾ��δ�С
	m_rcBgDlgFrame = rc;
	//�����ڲ�������ʾ��ܴ�СWatchDlgFrame
	int WatchDlgWidth = rc.Width();
	int WatchDlgHeight = rc.Height();
// 	m_WDW.rcDisplay.left = WatchDlgWidth * 1 / 10;
// 	m_WDW.rcDisplay.top = WatchDlgHeight * 2 / 10;
// 	m_WDW.rcDisplay.right = WatchDlgWidth * 19 / 20;
// 	m_WDW.rcDisplay.bottom = WatchDlgHeight * 9 / 10;
// 	
// 	m_WDW.rcReal = m_WDW.rcDisplay;
// 	m_WDW.fCurRatio = 1.0;
	
	CRect rcInter;												//rcInter��m_rcBgDlgFrameС
	GetClientRect(&rcInter);
	m_TabBgDlg.MoveWindow(2,2,rcInter.Width()-4,rcInter.Height()-4);
//	TRACE("=========================%d===%d",WatchDlgWidth,rcInter.Width());


	//�ı��BgDlg�¸���CWatchDlg��CMeasureDlg��λ��
	if (m_WatchDlgList.IsEmpty())
	{
		//WatchDlg�б� Ϊ��
	} 
	else
	{
		//WatchDlg�б� ��Ϊ��  
		int i=0;
		CWatchDlg* pWatchDlg;
		for (i=0 ; i<m_nWatchDlgNum ;i ++)
		{
			pWatchDlg = (CWatchDlg*)m_WatchDlgList.GetAt(m_WatchDlgList.FindIndex(i));
			pWatchDlg->ComputeFrame();
			pWatchDlg->MoveWindow(pWatchDlg->m_rcWatchDlgFrame);
			if (pWatchDlg->m_pMeasureDlg != NULL)
			{
				pWatchDlg->m_pMeasureDlg->MoveWindow(pWatchDlg->m_rcMFinTab);
			}
			
		}
	}


}

void CBgDlg::AddWaveDataToBgDlg(int nWatchDlgIndex, CWaveData *pRecWaveData , int mode)
{
	//nWatchDlgIndex: -1 �½� 0-9 ���		mode:	-1 ���ݼ����pWaveData  0 ������ʾ  1 FFT (nWatchDlgIndex,mode����ͬʱΪ-1)
	CWatchDlg* pWatchDlg;
	CWaveData* pWaveData;
	CString strTemp;
	if (-1 == nWatchDlgIndex)
	{	
		//-1		
		//�ø�pWaveData �½�һ��WatchDlg
		pWatchDlg = new CWatchDlg;
		pWatchDlg->Create(IDD_DIALOG_WATCHDLG,&m_TabBgDlg);
		pWatchDlg->m_nMode = mode;
		pWatchDlg->SetOwnedBgDlg(this);

		//���� WS_CLIPSIBLINGS ����
		LONG style = GetWindowLong(pWatchDlg->GetSafeHwnd(),GWL_STYLE);
		style = style | WS_CLIPCHILDREN ;
		SetWindowLong(pWatchDlg->GetSafeHwnd(),GWL_STYLE,style);

		strTemp.Format("ʾ����%d-%d", m_nIndex+1, ++m_nWatchDlgHasBeenNew);
		pWatchDlg->m_strWatchDlgName = strTemp;

		if ( NULL == pRecWaveData)	{
			//pWaveDataΪ�գ�˵��Ҫ�����pWaveDataӦ�ú͵�ǰѡ�е�CWatchDlg�е�WaveDataһ��
			//Ӧ�ò����������������������־ͼ��뵱ǰCWatchDlg�ĵ�һ��CWaveData
			TRACE(" NULL == pWaveData \n");
			int nCurSel = m_TabBgDlg.GetCurSel();
			CWatchDlg* pOriWatchDlg= m_WatchDlgList.GetAt(m_WatchDlgList.FindIndex(nCurSel));
			pWaveData = pOriWatchDlg->m_WaveDataList.GetAt(pOriWatchDlg->m_WaveDataList.FindIndex(0));
		}
		else{
			pWaveData = pRecWaveData;
		}
		pWatchDlg->AddWaveDataToWatchDlg(pWaveData);
		m_WatchDlgList.AddTail(pWatchDlg);
		pWatchDlg->ComputeFrame();

		AddWatchDlgToTab(pWatchDlg);

		m_nWatchDlgNum++;
	} 
	else if ( 0 <= nWatchDlgIndex)
	{
		//����0		
		//�����ø�pWaveData ������nWatchDlgIndexָ����WatchDlg��

		//�����nWatchDlgIndexָ����WatchDlg������pWaveData����
		//��	��������
		//��	���в���

	} 
	else
	{
	}

}

void CBgDlg::ClearAll()
{
	if (m_WatchDlgList.IsEmpty())
	{
		//WatchDlg�б� Ϊ��
		m_nWatchDlgNum = 0;
	} 
	else
	{
		//WatchDlg�б� ��Ϊ��  �Ƚ������
		int i=0;
		CWatchDlg* pWatchDlg;
		for (i=0 ; i<m_nWatchDlgNum ;i ++)
		{
			pWatchDlg = (CWatchDlg*)m_WatchDlgList.GetAt(m_WatchDlgList.FindIndex(i));
			pWatchDlg->DestroyWindow();
			delete pWatchDlg;
			pWatchDlg = NULL;
		}

		m_WatchDlgList.RemoveAll();
		m_TabBgDlg.DeleteAllItems();
		m_nWatchDlgNum = 0;

	}
}

void CBgDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	if (m_WatchDlgList.IsEmpty())
	{
		//WatchDlg�б� Ϊ��
	} 
	else
	{
		//WatchDlg�б� ��Ϊ��  �������
		int i=0;
		CWatchDlg* pWatchDlg;
		for (i=0 ; i<m_nWatchDlgNum ;i ++)
		{
			pWatchDlg = (CWatchDlg*)m_WatchDlgList.GetAt(m_WatchDlgList.FindIndex(i));
			pWatchDlg->DestroyWindow();
			delete pWatchDlg;
			pWatchDlg = NULL;
		}
		
	}
}

void CBgDlg::AddWatchDlgToTab(CWatchDlg* pWatchDlg)
{
	m_TabBgDlg.InsertItem(m_nWatchDlgNum , pWatchDlg->m_strWatchDlgName);	//��ʱm_nWatchDlgNum��δ��1 ����ֱ����
	

	/*�Ȱѵ�ǰ��ʾ��CWatchDlg����*/
	int nCurSeleted;
	nCurSeleted = m_TabBgDlg.GetCurSel();
	CWatchDlg* pWatchDlgCurSel;
	pWatchDlgCurSel = (CWatchDlg*)m_WatchDlgList.GetAt(m_WatchDlgList.FindIndex(nCurSeleted));
	pWatchDlgCurSel->ShowWindow(SW_HIDE);

	/*����Ĭ�ϵ�ѡ�*/
	m_TabBgDlg.SetCurSel(m_nWatchDlgNum);

	CRect rect;
	
	m_TabBgDlg.GetClientRect(&rect);
	/*�����ӶԻ����ڸ������е�λ��*/
	rect.top += 25;
	rect.bottom -= 5;
	rect.left += 1;
	rect.right -= 2;
	
	

	/*�����ӶԻ���ߴ粢�ƶ���ָ��λ��*/
	pWatchDlg->MoveWindow(&rect);
	pWatchDlg->ShowWindow(SW_SHOW);

	/*���ݵ�ǰָ���WatchDlg���¼���ToolBar*/
	pWatchDlg->AdjustAllToolBar();

}

void CBgDlg::DisplayWndRefresh()
{
	int nCurSeleted = m_TabBgDlg.GetCurSel();
	if ( -1 == nCurSeleted)
	{
		/*˵�����CBgDlgû���κ�CWatchDlg*/
		return;
	}

	CWatchDlg* pWatchDlgCurSel;

	CString strTrace;
	strTrace.Format("---CBgDlg::DisplayWndRefresh()------m_TabBgDlg.GetCurSel(): %d----------------\n",nCurSeleted);
	TRACE(strTrace);

	pWatchDlgCurSel = (CWatchDlg*)m_WatchDlgList.GetAt(m_WatchDlgList.FindIndex(nCurSeleted));
//	pWatchDlgCurSel->InvalidateRect(pWatchDlgCurSel->m_WDW.rcDisplay);
	pWatchDlgCurSel->Invalidate();
	pWatchDlgCurSel->UpdateWindow();

	if (pWatchDlgCurSel->m_pMarkDlg != NULL)
	{
		pWatchDlgCurSel->m_pMarkDlg->Invalidate();
		pWatchDlgCurSel->m_pMarkDlg->UpdateWindow();
	}

	if (pWatchDlgCurSel->m_pZoomDlg != NULL)
	{
		CZoomDlg* pZoomDlg = pWatchDlgCurSel->m_pZoomDlg;
		if (pZoomDlg->IsWindowVisible())
		{
			pZoomDlg->InvalidateRect(pZoomDlg->m_rcDisplayFrame);
		}
	}
}

void CBgDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	if (m_nIndex != m_pWndManage->m_nCurSelect)
	{
		//��ǰѡ�е�BgDlg�����Լ� ֱ���˳�
		return;
	} 


	CRect rcBgDlg;
	GetClientRect(&rcBgDlg);
	

	CBrush *pBrush = CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
	CBrush *pOldBrushp = dc.SelectObject(pBrush);
	CPen pen(PS_SOLID,3,RGB(0,0,0));
	CPen *pOldPen=dc.SelectObject(&pen);

	dc.Rectangle(&rcBgDlg);

	dc.SelectObject(pOldBrushp);
	dc.SelectObject(pOldPen);


	// Do not call CDialog::OnPaint() for painting messages
}

void CBgDlg::SetWndManage(CWndManage *pWndManage)
{
	m_pWndManage = pWndManage;
}

void CBgDlg::OnSelchangeTABBgDlg(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
//	SetBgDlgSeleted();
	int nTabLabelNum = m_TabBgDlg.GetItemCount();
	int nSelected = m_TabBgDlg.GetCurSel();
	int nCount = 0 ;
	CWatchDlg* pWatchDlgCurSel;
	for (nCount=0 ; nCount<nTabLabelNum ; nCount++)
	{
		pWatchDlgCurSel = (CWatchDlg*)m_WatchDlgList.GetAt(m_WatchDlgList.FindIndex(nCount));
		if ( nSelected == nCount){
			pWatchDlgCurSel->ShowWindow(SW_SHOW);
			//������ʾ���̵�DlgBar
			pWatchDlgCurSel->ChangeAllToolBars();
			//pFrame->m_pDlgBarExTopChnSel->ChangeToolBarDisplayChn(pWatchDlgCurSel);
		}
		else{			
			pWatchDlgCurSel->ShowWindow(SW_HIDE);
			//pWatchDlgCurSel->Invalidate();
		}
	}

	*pResult = 0;
}

void CBgDlg::SetBgDlgSeleted()
{
	int nOldSeleted;
	nOldSeleted = m_pWndManage->m_nCurSelect;
	if (m_nIndex == m_pWndManage->m_nCurSelect)
	{
		//����Ҫ�ı� WndManage�Ѿ�ѡ�е�ǰ��BgDlg
	} 
	else
	{
		m_pWndManage->m_nCurSelect = m_nIndex;
		Invalidate();
		UpdateWindow();
		m_pWndManage->m_pBgDlg[nOldSeleted].Invalidate();
		m_pWndManage->m_pBgDlg[nOldSeleted].UpdateWindow();
		DisplayWndRefresh();
	}
}

void CBgDlg::OnClickTABBgDlg(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	SetBgDlgSeleted();


	int nSelected = m_TabBgDlg.GetCurSel();
	CWatchDlg* pWatchDlgCurSel;
	pWatchDlgCurSel = (CWatchDlg*)m_WatchDlgList.GetAt(m_WatchDlgList.FindIndex(nSelected));
		
	pWatchDlgCurSel->ShowWindow(SW_SHOW);
	//������ʾ���̵�DlgBar
	pWatchDlgCurSel->ChangeAllToolBars();

	*pResult = 0;
}

void CBgDlg::WndRefresh_Complete()
{

	Invalidate();

//	m_TabBgDlg.Invalidate();

	int nCurSeleted = m_TabBgDlg.GetCurSel();
	CWatchDlg* pWatchDlgCurSel;
	pWatchDlgCurSel = (CWatchDlg*)m_WatchDlgList.GetAt(m_WatchDlgList.FindIndex(nCurSeleted));
	pWatchDlgCurSel->Invalidate();
	pWatchDlgCurSel->UpdateWindow();
	
	if (pWatchDlgCurSel->m_pMarkDlg != NULL)
	{
		pWatchDlgCurSel->m_pMarkDlg->Invalidate();
		pWatchDlgCurSel->m_pMarkDlg->UpdateWindow();
	}
	
	if (pWatchDlgCurSel->m_pZoomDlg != NULL)
	{
		CZoomDlg* pZoomDlg = pWatchDlgCurSel->m_pZoomDlg;
		if (pZoomDlg->IsWindowVisible())
		{
			pZoomDlg->InvalidateRect(pZoomDlg->m_rcDisplayFrame);
		}
	}
}

void CBgDlg::CloseCurSelWatchDlg()
{
	int nTabLabelNum = m_TabBgDlg.GetItemCount();
	
	//������ֻʣһ��CWatchDlg����ô�Ͳ��ٹر�
	if (nTabLabelNum <= 1)
	{
		TRACE("BUNNY TIPS : Here is only one tab in m_TabBgDlg!\n");
		return;
	}


	int nCurSeleted = m_TabBgDlg.GetCurSel();
	CWatchDlg* pWatchDlgCurSel;
	pWatchDlgCurSel = (CWatchDlg*)m_WatchDlgList.GetAt(m_WatchDlgList.FindIndex(nCurSeleted));

	//ɾ��CTypedPtrList�е�CWatchDlgָ��
	POSITION POS=m_WatchDlgList.FindIndex(nCurSeleted);
	m_WatchDlgList.RemoveAt(POS);

	pWatchDlgCurSel->ShowWindow(SW_HIDE);
	pWatchDlgCurSel->DestroyWindow();
	delete pWatchDlgCurSel;
	pWatchDlgCurSel = NULL;
	m_TabBgDlg.DeleteItem(nCurSeleted);
	m_nWatchDlgNum--;

	//����Ҫ�ı�m_TabBgDlg��ѡ�񣬲�ȻGetCurSel()�ķ��ؾͻ���-1
	int nNewSel = 0;
	if ( nCurSeleted > 0)
	{		
		//nCurSeleted֮ǰ���п�ѡ��tab
		nNewSel = nCurSeleted-1;
	} 
	else
	{
		nNewSel = 0;
	}
	m_TabBgDlg.SetCurSel(nNewSel);
			
	int nCount = 0 ;
	CWatchDlg* pWatchDlg;
	for (nCount=0 ; nCount<nTabLabelNum-1 ; nCount++)					//����Tab������һ��������������м�����ʱ��Ҫ��nTabLabelNum-1
	{
		pWatchDlg = (CWatchDlg*)m_WatchDlgList.GetAt(m_WatchDlgList.FindIndex(nCount));
		if ( nNewSel == nCount){
			pWatchDlg->ShowWindow(SW_SHOW);
		}
		else{			
			pWatchDlg->ShowWindow(SW_HIDE);
			//pWatchDlgCurSel->Invalidate();
		}
	}
	
}


/*
 *	AddWaveDataToBgDlg��һ������
 *	����ĵ���CWatchDlg��ָ�룬ͨ�������½�һ���µ�CWatchDlg������ָ��ָ���CWatchDlg������WaveData���Ƹ��½���CWatchDlg
 *
 *
 *
 */
void CBgDlg::AddWaveDataToBgDlg(int nWatchDlgIndex, CWatchDlg *pRecWatchDlg, int mode)
{
	//nWatchDlgIndex: -1 �½� 0-9 ���		mode:	-1 ���ݼ����pWaveData  0 ������ʾ  1 FFT (nWatchDlgIndex,mode����ͬʱΪ-1)
	CWatchDlg* pWatchDlg;		//�½���CWatchDlg
	CWatchDlg* pAddWatchDlg;	//����Ҫ�����CWatchDlg
//	CWaveData* pAddWaveData;
	CString strTemp;
	if (-1 == nWatchDlgIndex)
	{	
		//-1		
		//�ø�pWaveData �½�һ��WatchDlg
		pWatchDlg = new CWatchDlg;
		pWatchDlg->Create(IDD_DIALOG_WATCHDLG,&m_TabBgDlg);
		pWatchDlg->m_nMode = mode;
		pWatchDlg->SetOwnedBgDlg(this);
		
		//���� WS_CLIPSIBLINGS ����
		LONG style = GetWindowLong(pWatchDlg->GetSafeHwnd(),GWL_STYLE);
		style = style | WS_CLIPCHILDREN ;
		SetWindowLong(pWatchDlg->GetSafeHwnd(),GWL_STYLE,style);
		
		if (mode == DISPLAY_MODE_TIME)
		{
			strTemp.Format("ʾ����%d-%d", m_nIndex+1, ++m_nWatchDlgHasBeenNew);
		}
		else{
			strTemp.Format("Ƶ��%d-%d", m_nIndex+1, ++m_nWatchDlgHasBeenNew);
		}		
		pWatchDlg->m_strWatchDlgName = strTemp;
		
		if ( NULL == pRecWatchDlg)	{
			//pAddWatchDlgΪ�գ�˵��Ҫ�����WaveDataӦ�ú͵�ǰѡ�е�CWatchDlg�е�WaveDataһ��
			TRACE(" NULL == pRecWatchDlg \n");
			int nCurSel = m_TabBgDlg.GetCurSel();
			pAddWatchDlg= m_WatchDlgList.GetAt(m_WatchDlgList.FindIndex(nCurSel));
			
		}
		else{
			//pAddWatchDlg��Ϊ�գ��ͽ�pAddWatchDlgָ����CWatchDlg�е�����WaveData�����½���CWatchDlg
			pAddWatchDlg = pRecWatchDlg;
		}
		pWatchDlg->AddWaveDataToWatchDlg(pAddWatchDlg);

		m_WatchDlgList.AddTail(pWatchDlg);
		pWatchDlg->ComputeFrame();
		
		AddWatchDlgToTab(pWatchDlg);
		
		m_nWatchDlgNum++;
	} 
	else if ( 0 <= nWatchDlgIndex)
	{
		//����0		
		//�����ø�pWaveData ������nWatchDlgIndexָ����WatchDlg��
		
		//�����nWatchDlgIndexָ����WatchDlg������pWaveData����
		//��	��������
		//��	���в���
		
	} 
	else
	{
	}
}

// WndManage.cpp: implementation of the CWndManage class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Recorder.h"
#include "WndManage.h"

#include "MainFrm.h"
#include "Product.h"
#include "RecorderDoc.h"

#include "RecorderView.h"
#include "WatchDlg.h"
#include "BgDlg.h"
#include "DlgBarExTopChnSel.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWndManage::CWndManage()
{
	m_nCurSelect = 0;

}

CWndManage::~CWndManage()
{

	int i;
	for (i=0 ; i<MAX_CHANNUM ; i++ )
	{
		m_pBgDlg[i].DestroyWindow();
		//delete &m_pBgDlg[i];
	}

	delete[] m_pBgDlg;
}


void CWndManage::Create(int nSubWnd, const RECT &rect, CRecorderView* pOwnedViex)
{

	m_nSubWnd = nSubWnd;		//����������64
	m_pOwnedViex = pOwnedViex;
	m_rcFrameWnd = rect;
//	m_pWatchDlg = new CWatchDlg[MAX_CHANNUM];

	m_pBgDlg = new CBgDlg[MAX_CHANNUM];


	int i;
	for (i=0 ; i<MAX_CHANNUM ; i++ )
	{
		m_pBgDlg[i].Create(IDD_DIALOG_BGDLG , (CWnd*)m_pOwnedViex);
		m_pBgDlg[i].SetIndex(i);
		m_pBgDlg[i].SetWndManage(this);

		//���� WS_CLIPSIBLINGS ����
		LONG style = GetWindowLong(m_pBgDlg[i].GetSafeHwnd(),GWL_STYLE);
		style = style | WS_CLIPCHILDREN ;
		SetWindowLong(m_pBgDlg[i].GetSafeHwnd(),GWL_STYLE,style);
	}

	m_nCurSelect = 0;

	Display(2);

}

void CWndManage::Display(int nMode)
{
	if(m_nCurMode == nMode)
	{
//		return;
	}
	ASSERT((nMode>=1)&&(nMode<=4));
	
//	m_nSelected = CVideoDlg::m_nSelectedIndex;
	// ������һ�ε�ģʽ
	if(nMode != m_nCurMode)
		m_nModeSave = m_nCurMode;

	m_nCurMode  = nMode;

	//����first��last��ʾ��dlg


	if (m_nCurMode == 1)
	{
		m_nLast = m_nFirst = m_nCurSelect;
	}
	else
	{
		m_nFirst = m_nCurSelect;
		m_nFirst -= ( m_nFirst%(m_nModeSave*m_nModeSave) );		//m_nCurMode�ó���m_nModeSave    ���ԭ����ʾģʽ�µĵ�һ��������ţ�����0�ļ�����
		int n;
		if((MAX_CHANNUM - m_nFirst) < m_nCurMode*m_nCurMode)
		{
			n = m_nCurMode*m_nCurMode - (MAX_CHANNUM-m_nFirst);
			m_nFirst -= n;
		}
	}

	Postion();
}

void CWndManage::Postion()
{
	int i=0;
	CRect rc;
//	CRect rc2;

	m_nLast  = min (m_nFirst + m_nCurMode * m_nCurMode, m_nSubWnd);
	m_nLast --;
	

	for(i = m_nFirst; i <= m_nLast; i++)
	{
		VERIFY(GetVideoRect(i,rc));
		m_pBgDlg[i].ShowWindow(SW_HIDE);	
		m_pBgDlg[i].MoveWindow(rc);

		m_pBgDlg[i].SetBgDlgFrame(rc);
	}
	
	
	// ���ز��ɼ��Ӵ���,������������Ƶ������Ϊ����ȫ��״̬�²���˸����������
	for( i=0;i<m_nSubWnd;i++)
	{
		if(i<m_nFirst || i> m_nLast)
		{
			m_pBgDlg[i].ShowWindow(SW_HIDE);
			//			m_pWndSub[i].m_OsdDlg.ShowWindow(SW_HIDE);	
		}
		else
		{
			m_pBgDlg[i].ShowWindow(SW_SHOW);
		}
		
// 		if(g_arrPageInfo[m_nCurrentPageID].m_CamInfo[i].m_strSvrAddr.IsEmpty())
// 		{
// 			strTemp.Format(IDS_WINDOW_NUM, i + 1);
// 			m_pVideoDlg[i].m_stStatus.SetWindowText(strTemp);
// 		}
// 		else
// 		{                                                                                                     
// 			strTemp.Empty();
// 			strTemp += g_arrPageInfo[m_nCurrentPageID].m_CamInfo[i].m_strSvrDesc;
// 			strTemp += " ";
// 			strTemp += g_arrPageInfo[m_nCurrentPageID].m_CamInfo[i].m_strCamDesc;
// 			m_pVideoDlg[i].m_stStatus.SetWindowText(strTemp);
// 		}
	}

}

bool CWndManage::GetVideoRect(int nSub, CRect& rc)
{
	int i;
	CRect rcClient;
	int nSubWidth = 0;
	int nSubHeight = 0;
	CPoint pt;
	CWatchDlg* pWatchDlg;

	rcClient = m_rcFrameWnd;
	nSubWidth = rcClient.Width()/m_nCurMode;
	nSubHeight = rcClient.Height()/m_nCurMode;
	i = nSub - m_nFirst;
	pt.x  = m_rcFrameWnd.left + (i%m_nCurMode)*nSubWidth;
	pt.y  = m_rcFrameWnd.top + (i/m_nCurMode)*nSubHeight;

	rc.SetRect(pt.x,pt.y,pt.x + nSubWidth,pt.y + nSubHeight);
//	rc.DeflateRect(m_nGap,m_nGap);

	return TRUE;
}

void CWndManage::DefaultSetting()
{
	//Ĭ������
	//ƥ��ͨ����BgDlg

	// Doc	
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CRecorderDoc *pDoc = m_pOwnedViex->GetDocument();
	CProduct* pProduct = &(pDoc->m_Product);
	CTypedPtrList<CObList, CWaveData*>* pWaveList = &(pProduct->m_WaveList);

	TRACE("Product�д�ͨ����Ϊ%d\n",pProduct->m_nOpenChannelNum);

	//���򿪵�X��ͨ�����μ��뵽ǰX��BgDlg�У�ÿ��BgDlg��һ��
	int i;
	for (i=0 ; i<MAX_CHANNUM ; i++)
	{
		if (i<pProduct->m_nOpenChannelNum)
		{
			//���BgDlg������WatchDlg�б� 
			m_pBgDlg[i].ClearAll();
			//pWaveList->GetAt(pWaveList->FindIndex(i));
			m_pBgDlg[i].AddWaveDataToBgDlg(-1 , pWaveList->GetAt(pWaveList->FindIndex(i)) , 0);
		}
		else{
			m_pBgDlg[i].ClearAll();
		}
		
	}

	//��ȡ��һ��CBgDlg�ĵ�һ��CWatchDlg�����Ը�CWatchDlgΪ����������ToolBar����ʾ
	m_nCurSelect = 0;
	CWatchDlg* pWatchDlg = m_pBgDlg[m_nCurSelect].m_WatchDlgList.GetAt(m_pBgDlg[m_nCurSelect].m_WatchDlgList.FindIndex(0));
	//������ʾ���̵�DlgBar
	pFrame->m_pDlgBarExTopChnSel->ChangeToolBarDisplayChn(pWatchDlg);
	
	//��ʼ��m_WPCUnified
	int nWidth = ((CWatchDlg*)(m_pBgDlg[0].m_WatchDlgList.GetAt(m_pBgDlg[0].m_WatchDlgList.FindIndex(0))))->m_WDW.rcDisplay.Width();
	m_WPCUnified.fRatioX = DATASTORAGE_SEGMENT / nWidth;
	m_WPCUnified.fRatioY = 1.0;

}

void CWndManage::AllWndRefresh()
{
	int i = m_nFirst;
	for ( ; i <= m_nLast ; i++)
	{
		m_pBgDlg[i].DisplayWndRefresh();
	}
}

void CWndManage::AllWndRefresh_Complete()
{
	int i = m_nFirst;
	for ( ; i <= m_nLast ; i++)
	{
		m_pBgDlg[i].WndRefresh_Complete();
	}
}

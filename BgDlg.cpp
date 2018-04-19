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
	//复制外围框架矩形大小
	m_rcBgDlgFrame = rc;
	//计算内部波形显示框架大小WatchDlgFrame
	int WatchDlgWidth = rc.Width();
	int WatchDlgHeight = rc.Height();
// 	m_WDW.rcDisplay.left = WatchDlgWidth * 1 / 10;
// 	m_WDW.rcDisplay.top = WatchDlgHeight * 2 / 10;
// 	m_WDW.rcDisplay.right = WatchDlgWidth * 19 / 20;
// 	m_WDW.rcDisplay.bottom = WatchDlgHeight * 9 / 10;
// 	
// 	m_WDW.rcReal = m_WDW.rcDisplay;
// 	m_WDW.fCurRatio = 1.0;
	
	CRect rcInter;												//rcInter比m_rcBgDlgFrame小
	GetClientRect(&rcInter);
	m_TabBgDlg.MoveWindow(2,2,rcInter.Width()-4,rcInter.Height()-4);
//	TRACE("=========================%d===%d",WatchDlgWidth,rcInter.Width());


	//改变该BgDlg下各个CWatchDlg与CMeasureDlg的位置
	if (m_WatchDlgList.IsEmpty())
	{
		//WatchDlg列表 为空
	} 
	else
	{
		//WatchDlg列表 不为空  
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
	//nWatchDlgIndex: -1 新建 0-9 序号		mode:	-1 根据加入的pWaveData  0 正常显示  1 FFT (nWatchDlgIndex,mode不可同时为-1)
	CWatchDlg* pWatchDlg;
	CWaveData* pWaveData;
	CString strTemp;
	if (-1 == nWatchDlgIndex)
	{	
		//-1		
		//用该pWaveData 新建一个WatchDlg
		pWatchDlg = new CWatchDlg;
		pWatchDlg->Create(IDD_DIALOG_WATCHDLG,&m_TabBgDlg);
		pWatchDlg->m_nMode = mode;
		pWatchDlg->SetOwnedBgDlg(this);

		//设置 WS_CLIPSIBLINGS 属性
		LONG style = GetWindowLong(pWatchDlg->GetSafeHwnd(),GWL_STYLE);
		style = style | WS_CLIPCHILDREN ;
		SetWindowLong(pWatchDlg->GetSafeHwnd(),GWL_STYLE,style);

		strTemp.Format("示波器%d-%d", m_nIndex+1, ++m_nWatchDlgHasBeenNew);
		pWatchDlg->m_strWatchDlgName = strTemp;

		if ( NULL == pRecWaveData)	{
			//pWaveData为空，说明要加入的pWaveData应该和当前选中的CWatchDlg中的WaveData一样
			//应该不会有这样的情况，如果出现就加入当前CWatchDlg的第一个CWaveData
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
		//大于0		
		//将该用该pWaveData 加在由nWatchDlgIndex指定的WatchDlg上

		//检查由nWatchDlgIndex指定的WatchDlg上有无pWaveData数据
		//有	不做操作
		//无	进行插入

	} 
	else
	{
	}

}

void CBgDlg::ClearAll()
{
	if (m_WatchDlgList.IsEmpty())
	{
		//WatchDlg列表 为空
		m_nWatchDlgNum = 0;
	} 
	else
	{
		//WatchDlg列表 不为空  先进行清空
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
		//WatchDlg列表 为空
	} 
	else
	{
		//WatchDlg列表 不为空  进行清空
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
	m_TabBgDlg.InsertItem(m_nWatchDlgNum , pWatchDlg->m_strWatchDlgName);	//此时m_nWatchDlgNum还未加1 所以直接用
	

	/*先把当前显示的CWatchDlg隐藏*/
	int nCurSeleted;
	nCurSeleted = m_TabBgDlg.GetCurSel();
	CWatchDlg* pWatchDlgCurSel;
	pWatchDlgCurSel = (CWatchDlg*)m_WatchDlgList.GetAt(m_WatchDlgList.FindIndex(nCurSeleted));
	pWatchDlgCurSel->ShowWindow(SW_HIDE);

	/*设置默认的选项卡*/
	m_TabBgDlg.SetCurSel(m_nWatchDlgNum);

	CRect rect;
	
	m_TabBgDlg.GetClientRect(&rect);
	/*调整子对话框在父窗口中的位置*/
	rect.top += 25;
	rect.bottom -= 5;
	rect.left += 1;
	rect.right -= 2;
	
	

	/*设置子对话框尺寸并移动到指定位置*/
	pWatchDlg->MoveWindow(&rect);
	pWatchDlg->ShowWindow(SW_SHOW);

	/*根据当前指向的WatchDlg更新几个ToolBar*/
	pWatchDlg->AdjustAllToolBar();

}

void CBgDlg::DisplayWndRefresh()
{
	int nCurSeleted = m_TabBgDlg.GetCurSel();
	if ( -1 == nCurSeleted)
	{
		/*说明这个CBgDlg没有任何CWatchDlg*/
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
		//当前选中的BgDlg不是自己 直接退出
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
			//更新显示量程的DlgBar
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
		//不需要改变 WndManage已经选中当前的BgDlg
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
	//更新显示量程的DlgBar
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
	
	//如果最后只剩一个CWatchDlg，那么就不再关闭
	if (nTabLabelNum <= 1)
	{
		TRACE("BUNNY TIPS : Here is only one tab in m_TabBgDlg!\n");
		return;
	}


	int nCurSeleted = m_TabBgDlg.GetCurSel();
	CWatchDlg* pWatchDlgCurSel;
	pWatchDlgCurSel = (CWatchDlg*)m_WatchDlgList.GetAt(m_WatchDlgList.FindIndex(nCurSeleted));

	//删除CTypedPtrList中的CWatchDlg指针
	POSITION POS=m_WatchDlgList.FindIndex(nCurSeleted);
	m_WatchDlgList.RemoveAt(POS);

	pWatchDlgCurSel->ShowWindow(SW_HIDE);
	pWatchDlgCurSel->DestroyWindow();
	delete pWatchDlgCurSel;
	pWatchDlgCurSel = NULL;
	m_TabBgDlg.DeleteItem(nCurSeleted);
	m_nWatchDlgNum--;

	//这里要改变m_TabBgDlg的选择，不然GetCurSel()的返回就会变成-1
	int nNewSel = 0;
	if ( nCurSeleted > 0)
	{		
		//nCurSeleted之前还有可选的tab
		nNewSel = nCurSeleted-1;
	} 
	else
	{
		nNewSel = 0;
	}
	m_TabBgDlg.SetCurSel(nNewSel);
			
	int nCount = 0 ;
	CWatchDlg* pWatchDlg;
	for (nCount=0 ; nCount<nTabLabelNum-1 ; nCount++)					//由于Tab减少了一个，所以这里进行计数的时候要是nTabLabelNum-1
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
 *	AddWaveDataToBgDlg的一个重载
 *	加入的的是CWatchDlg的指针，通常用于新建一个新的CWatchDlg，并把指针指向的CWatchDlg的所有WaveData复制给新建的CWatchDlg
 *
 *
 *
 */
void CBgDlg::AddWaveDataToBgDlg(int nWatchDlgIndex, CWatchDlg *pRecWatchDlg, int mode)
{
	//nWatchDlgIndex: -1 新建 0-9 序号		mode:	-1 根据加入的pWaveData  0 正常显示  1 FFT (nWatchDlgIndex,mode不可同时为-1)
	CWatchDlg* pWatchDlg;		//新建的CWatchDlg
	CWatchDlg* pAddWatchDlg;	//真正要加入的CWatchDlg
//	CWaveData* pAddWaveData;
	CString strTemp;
	if (-1 == nWatchDlgIndex)
	{	
		//-1		
		//用该pWaveData 新建一个WatchDlg
		pWatchDlg = new CWatchDlg;
		pWatchDlg->Create(IDD_DIALOG_WATCHDLG,&m_TabBgDlg);
		pWatchDlg->m_nMode = mode;
		pWatchDlg->SetOwnedBgDlg(this);
		
		//设置 WS_CLIPSIBLINGS 属性
		LONG style = GetWindowLong(pWatchDlg->GetSafeHwnd(),GWL_STYLE);
		style = style | WS_CLIPCHILDREN ;
		SetWindowLong(pWatchDlg->GetSafeHwnd(),GWL_STYLE,style);
		
		if (mode == DISPLAY_MODE_TIME)
		{
			strTemp.Format("示波器%d-%d", m_nIndex+1, ++m_nWatchDlgHasBeenNew);
		}
		else{
			strTemp.Format("频域%d-%d", m_nIndex+1, ++m_nWatchDlgHasBeenNew);
		}		
		pWatchDlg->m_strWatchDlgName = strTemp;
		
		if ( NULL == pRecWatchDlg)	{
			//pAddWatchDlg为空，说明要加入的WaveData应该和当前选中的CWatchDlg中的WaveData一样
			TRACE(" NULL == pRecWatchDlg \n");
			int nCurSel = m_TabBgDlg.GetCurSel();
			pAddWatchDlg= m_WatchDlgList.GetAt(m_WatchDlgList.FindIndex(nCurSel));
			
		}
		else{
			//pAddWatchDlg不为空，就将pAddWatchDlg指代的CWatchDlg中的所有WaveData加入新建的CWatchDlg
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
		//大于0		
		//将该用该pWaveData 加在由nWatchDlgIndex指定的WatchDlg上
		
		//检查由nWatchDlgIndex指定的WatchDlg上有无pWaveData数据
		//有	不做操作
		//无	进行插入
		
	} 
	else
	{
	}
}

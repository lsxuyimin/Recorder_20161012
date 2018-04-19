// MeasureDlg.cpp : implementation file
//

#include "stdafx.h"
#include "recorder.h"
#include "MeasureDlg.h"

#include "MeasureAddDlg.h"
#include "MeasureItem.h"
#include "WatchDlg.h"
#include "WaveData.h"
#include "MainFrm.h"
#include "RecorderDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
#include <math.h>


//////////////////////////////////////////////////////////////////////////
int g_aMeaColWidth[MEA_COL_NUM]={50,50,50,50,50,50,50,50};
CString g_aMeaColName[MEA_COL_NUM]={"Channel","Name","Value","Min","Max","Average","σ","Span"};


/////////////////////////////////////////////////////////////////////////////
// CMeasureDlg dialog


CMeasureDlg::CMeasureDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMeasureDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMeasureDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_bMinFlag = false;

	m_nMeasureNum = 0;

	m_nSelItem = 0;

	int nCount = 0;
	m_nColWidthSum = 0;
	for (nCount=0 ; nCount<MEA_COL_NUM ; nCount++){
		m_nColWidthSum += g_aMeaColWidth[nCount];
	}

	m_pHScroll=NULL;
	m_nHScrollPos=0;
}


void CMeasureDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMeasureDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMeasureDlg, CDialog)
	//{{AFX_MSG_MAP(CMeasureDlg)
	ON_WM_RBUTTONDOWN()
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_DESTROY()
	ON_WM_HSCROLL() 
	ON_COMMAND(ID_MEA_ADDMEASURE, OnMeaAddmeasure)
	ON_COMMAND(ID_MEA_DELMEASURE, OnMeaDelmeasure)
	ON_COMMAND(ID_MEA_EDITMEASURE, OnMeaEditmeasure)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMeasureDlg message handlers

void CMeasureDlg::SetMatchWatchDlg(CWatchDlg* pWatchDlg)
{
	m_pMatchWatchDlg = pWatchDlg;
}

void CMeasureDlg::AddMeasureItem(CWaveData* pWaveData,int nMeaType ,int nMeaPart)
{

	CMeasureItem* pMeaItem = new CMeasureItem();
	pMeaItem->pWaveData = pWaveData;
	pMeaItem->nMeaPart = nMeaPart;
	pMeaItem->nMeaType = nMeaType;

	m_MeasureItemList.AddTail(pMeaItem);
	m_nMeasureNum++;

}

void CMeasureDlg::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	//首先将该CMeasureDlg匹配的WatchDlg所属的BgDlg变为选中
	m_pMatchWatchDlg->SetOwnedBgDlgSeleted();
	//==========================实现右键菜单==========================================================================
	CMenu menu,*pSubMenu;//定义下面要用到的cmenu对象  
    menu.LoadMenu(IDR_MENU_MeaDlgPopup);//装载自定义的右键菜单  
    pSubMenu=menu.GetSubMenu(0);//获取第一个弹出菜单，所以第一个菜单必须有子菜单  
    CPoint oPoint;//定义一个用于确定光标位置的位置  
    GetCursorPos(&oPoint);//获取当前光标的位置，以便使得菜单可以跟随光标  
    pSubMenu->TrackPopupMenu(TPM_LEFTALIGN,oPoint.x,oPoint.y,this);   //在指定位置显示弹出菜单


	CDialog::OnRButtonDown(nFlags, point);
}

void CMeasureDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here

	//建pDisplayMemDC
	CRect rcTitleAndItem;
	GetClientRect(&rcTitleAndItem);


	if (m_pHScroll == NULL){
		//无m_pHScroll
		m_nBmpWidth = rcTitleAndItem.Width();
		m_nBmpHeight = rcTitleAndItem.Height();
	} 
	else{
		m_nBmpWidth = m_nColWidthSum;												//400
		m_nBmpHeight = rcTitleAndItem.Height()-MEA_HSCROLL_HEIGHT;
	}
	CDC* pDisplayMemDC=new CDC;
	pDisplayMemDC->CreateCompatibleDC(&dc);
	CBitmap* pBitmap = new CBitmap;
	pBitmap->CreateCompatibleBitmap(&dc ,m_nBmpWidth ,m_nBmpHeight );
	pDisplayMemDC->SelectObject(pBitmap);
	//绘制pDisplayMemDC底色
	CBrush BgGrayBrush(RGB(255,255,255));
	CBrush* pOldBrush = pDisplayMemDC->SelectObject(&BgGrayBrush);
	pDisplayMemDC->Rectangle(0,0,m_nBmpWidth,m_nBmpHeight);
	pDisplayMemDC->SelectObject(pOldBrush);

	//黑底色标题栏
	DrawTitleBar(pDisplayMemDC);

	//绘制Item
	DrawItem(pDisplayMemDC);

	//绘制网格
	DrawGrid(pDisplayMemDC);

	//BitBlt: pDisplayMemDC to paintDC
	int nBitBltWidth,nBitBltHeight;
	if (m_pHScroll == NULL){
		//无m_pHScroll
		nBitBltWidth = m_nBmpWidth;
		nBitBltHeight = m_nBmpHeight;
		dc.BitBlt(0,0, nBitBltWidth, nBitBltHeight, pDisplayMemDC, 0, 0, SRCCOPY);
	} 
	else{
		nBitBltWidth = m_nColWidthSum;
		nBitBltHeight = m_nBmpHeight;
		dc.BitBlt(0,0, nBitBltWidth, nBitBltHeight, pDisplayMemDC, m_nHScrollPos, 0, SRCCOPY);
	}
	

	//标题栏按钮
	DrawBtn(&dc);


	delete pDisplayMemDC;
	delete pBitmap;
	// Do not call CDialog::OnPaint() for painting messages
}

BOOL CMeasureDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMeasureDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDC* pDC = GetWindowDC();
	CDC* pDisplayMemDC=new CDC;
	pDisplayMemDC->CreateCompatibleDC(pDC);
	CBitmap* pBitmapMinOrRe = new CBitmap;
	CBitmap* pBitmapClose = new CBitmap;
	CBitmap* pOldBitmap;
	CRect rtWnd, rtButton;
//	TRACE("=====mousemove position:%d , %d ===============\n",point.x ,point.y);
	
	if (pDC){
		if (m_bMinFlag == false)
		{
			if (m_rcBtnMinOrRe.PtInRect(point)){
				pBitmapMinOrRe->LoadBitmap(IDB_BMP_Minimize_Focus);
			} 
			else{
				pBitmapMinOrRe->LoadBitmap(IDB_BMP_Minimize_Normal);
			}
		} 
		else
		{
			if (m_rcBtnMinOrRe.PtInRect(point)){
				pBitmapMinOrRe->LoadBitmap(IDB_BMP_Restore_Focus);
			} 
			else{
				pBitmapMinOrRe->LoadBitmap(IDB_BMP_Restore_Normal);
			}
		}
		

	
		if (m_rcBtnClose.PtInRect(point)){
			pBitmapClose->LoadBitmap(IDB_BMP_Close_Focus);
		} 
		else{
			pBitmapClose->LoadBitmap(IDB_BMP_Close_Normal);
		}
	}



	BITMAP BmpInfo;
	pBitmapClose->GetBitmap(&BmpInfo);
	pOldBitmap=(CBitmap*)pDisplayMemDC->SelectObject(pBitmapClose);
	pDC->BitBlt(m_rcBtnClose.left,m_rcBtnClose.top, BmpInfo.bmWidth, BmpInfo.bmHeight, pDisplayMemDC, 0, 0, SRCCOPY);
	pDisplayMemDC->SelectObject(pOldBitmap);
	pBitmapClose->DeleteObject();

	pBitmapMinOrRe->GetBitmap(&BmpInfo);
	pOldBitmap=(CBitmap*)pDisplayMemDC->SelectObject(pBitmapMinOrRe);
	pDC->BitBlt(m_rcBtnMinOrRe.left,m_rcBtnMinOrRe.top, BmpInfo.bmWidth, BmpInfo.bmHeight, pDisplayMemDC, 0, 0, SRCCOPY);
	pDisplayMemDC->SelectObject(pOldBitmap);
	pBitmapMinOrRe->DeleteObject();

	ReleaseDC(pDisplayMemDC);
	ReleaseDC(pDC);
	delete pDisplayMemDC;
	delete pBitmapClose;
	delete pBitmapMinOrRe;

	CDialog::OnMouseMove(nFlags, point);
}

void CMeasureDlg::DrawBtn(CPaintDC *pDC)
{
	CRect rcClient;
	GetClientRect(&rcClient);
	int nWidth = rcClient.Width();
//	TRACE("=========width is %d==============\n",nWidth);
//	TRACE("=========rcClient is %d to %d==============\n",rcClient.left,rcClient.right);
	
	////////////////////更新m_rcBtnClose,m_rcBtnMinOrRe//////////////////////////////////////////////////////
	CRect rcTemp;
	rcTemp.right = nWidth;
	rcTemp.left = rcTemp.right - 19;
	rcTemp.top = 0;
	rcTemp.bottom = MEA_TITLE_HEIGHT;
	m_rcBtnClose = rcTemp;
	
	rcTemp.right = rcTemp.left;
	rcTemp.left = rcTemp.right - 19;
	m_rcBtnMinOrRe = rcTemp;

	/////////////////////draw button/////////////////////////////////////////////////////
	
//	CDC* pDC = GetWindowDC();
	CDC* pDisplayMemDC=new CDC;
	pDisplayMemDC->CreateCompatibleDC(pDC);
	CBitmap* pBitmapMinOrRe = new CBitmap;
	CBitmap* pBitmapClose = new CBitmap;
	CBitmap* pOldBitmap;
	
	if (m_bMinFlag == false)
	{
		pBitmapMinOrRe->LoadBitmap(IDB_BMP_Minimize_Normal);
	} 
	else
	{
		pBitmapMinOrRe->LoadBitmap(IDB_BMP_Restore_Normal);
	}
	pBitmapClose->LoadBitmap(IDB_BMP_Close_Normal);

	BITMAP BmpInfo;
	pBitmapClose->GetBitmap(&BmpInfo);
	pOldBitmap=(CBitmap*)pDisplayMemDC->SelectObject(pBitmapClose);
	pDC->BitBlt(m_rcBtnClose.left,m_rcBtnClose.top, BmpInfo.bmWidth, BmpInfo.bmHeight, pDisplayMemDC, 0, 0, SRCCOPY);
	pDisplayMemDC->SelectObject(pOldBitmap);
	pBitmapClose->DeleteObject();

	pBitmapMinOrRe->GetBitmap(&BmpInfo);
	pOldBitmap=(CBitmap*)pDisplayMemDC->SelectObject(pBitmapMinOrRe);
	pDC->BitBlt(m_rcBtnMinOrRe.left,m_rcBtnMinOrRe.top, BmpInfo.bmWidth, BmpInfo.bmHeight, pDisplayMemDC, 0, 0, SRCCOPY);
	pDisplayMemDC->SelectObject(pOldBitmap);
	pBitmapMinOrRe->DeleteObject();

	ReleaseDC(pDisplayMemDC);
//	ReleaseDC(pDC);
	delete pDisplayMemDC;
	delete pBitmapClose;
	delete pBitmapMinOrRe;
}

void CMeasureDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	if (m_rcBtnMinOrRe.PtInRect(point))
	{
		//鼠标点击了BtnMinOrRe按钮
		m_bMinFlag = !m_bMinFlag;
		
		m_pMatchWatchDlg->ComputeFrame();
		m_pMatchWatchDlg->ModifyPos();
	}
	else if (m_rcBtnClose.PtInRect(point))
	{
		//鼠标点击了BtnClose按钮
		m_pMatchWatchDlg->m_pMeasureDlg = NULL;
		SendMessage(WM_CLOSE);
		m_pMatchWatchDlg->ComputeFrame();
		m_pMatchWatchDlg->ModifyPos();

	}
	else
	{
		CRect rcClient;
		GetClientRect(&rcClient);
		CRect rcAllItem;
		rcAllItem.left = 0;
		rcAllItem.top = MEA_TITLE_HEIGHT;
		rcAllItem.right = rcClient.right;
		rcAllItem.bottom = rcClient.bottom;
		if (rcAllItem.PtInRect(point))
		{
			//鼠标点击了AllItem范围内
			m_nSelItem = (int)((point.y - MEA_TITLE_HEIGHT)/MEA_ITEM_HEIGHT);
			InvalidateRect(&rcAllItem);
		}
	}
	
	
	
	CDialog::OnLButtonUp(nFlags, point);
}

void CMeasureDlg::DrawItem(CDC* pDC)
{
	int nCount;
	CBrush lightGrayBrush(RGB(50,50,50));
	CBrush* pOldBrush = pDC->SelectObject(&lightGrayBrush);

	CPen transPen(PS_NULL, 0 , RGB(0,0,0));
	CPen* pOldPen = pDC->SelectObject(&transPen);

	int nOldMode = pDC->SetBkMode(TRANSPARENT);
	COLORREF clOldText=pDC->GetTextColor();		
	CFont itemFont;	
	itemFont.CreateFont( 10, // nHeight 
		5, // nWidth 
		0, // nEscapement文本行逆时针旋转角度 
		0, // nOrientation字体角度
		FW_BOLD, // nWeight字体粗细程度 
		FALSE, // bItalic 
		FALSE, // bUnderline 
		0, // cStrikeOut 删除线
		ANSI_CHARSET, // nCharSet 
		OUT_DEFAULT_PRECIS, // nOutPrecision 
		CLIP_DEFAULT_PRECIS, // nClipPrecision 
		DEFAULT_QUALITY, // nQuality 
		DEFAULT_PITCH | FF_SWISS, // nPitchAndFamily 
		_T("隶书")); // lpszFac		pDC->SelectStockObject(SYSTEM_FIXED_FONT);		
	CFont *pOldFont;
	pOldFont = pDC->SelectObject(&itemFont);


	CMeasureItem* pMeasureItem;
	CString strTemp;
	for(nCount=0 ; nCount<m_nMeasureNum ; nCount++)
	{
		
		pMeasureItem = (CMeasureItem*)m_MeasureItemList.GetAt(m_MeasureItemList.FindIndex(nCount));

		if (nCount == m_nSelItem)
		{
			//该行被选中 深色底 白色字
			CBrush lightGrayBrush(RGB(30,60,130));
			CBrush* pOldBrush = pDC->SelectObject(&lightGrayBrush);
			CPen transPen(PS_NULL, 0 , RGB(0,0,0));
			CPen* pOldPen = pDC->SelectObject(&transPen);
			pDC->Rectangle(0 , (nCount+1)*MEA_ITEM_HEIGHT , m_nBmpWidth , (nCount+2)*MEA_ITEM_HEIGHT);
			pDC->SelectObject(pOldBrush);
			pDC->SelectObject(pOldPen);
			pDC->SetTextColor(RGB(255,255,255));
			//写Item
			int nDrawX=10;
			int nColCount = 0;
			for (nColCount=0 ;nColCount<MEA_COL_NUM ;nColCount++ )
			{
				//pDC->TextOut(nDrawX , (nCount+1)*MEA_ITEM_HEIGHT+4,"9999" );
				switch (nColCount)
				{
				case 0:					
					pDC->TextOut(nDrawX , (nCount+1)*MEA_ITEM_HEIGHT+4,pMeasureItem->pWaveData->m_strChanName );
					break;
				case 1:
					//strTemp.Format("%d",pMeasureItem->nMeaType);
					pDC->TextOut(nDrawX , (nCount+1)*MEA_ITEM_HEIGHT+4,TransMeaType(pMeasureItem->nMeaType) );
					break;
				case 2:
					pDC->TextOut(nDrawX , (nCount+1)*MEA_ITEM_HEIGHT+4,pMeasureItem->strMeaValue );
					break;
				case 3:
					pDC->TextOut(nDrawX , (nCount+1)*MEA_ITEM_HEIGHT+4,pMeasureItem->strMin );
					break;
				case 4:
					pDC->TextOut(nDrawX , (nCount+1)*MEA_ITEM_HEIGHT+4,pMeasureItem->strMax );
					break;
				case 5:
					pDC->TextOut(nDrawX , (nCount+1)*MEA_ITEM_HEIGHT+4,pMeasureItem->strAverage );
					break;
				case 6:
					pDC->TextOut(nDrawX , (nCount+1)*MEA_ITEM_HEIGHT+4,pMeasureItem->strVariance );
					break;
				case 7:
					pDC->TextOut(nDrawX , (nCount+1)*MEA_ITEM_HEIGHT+4,TransMeaPart(pMeasureItem->nMeaPart) );
					break;
				default :
					break;
				}
				nDrawX += g_aMeaColWidth[nCount];
			}
			//pDC->TextOut(0 , (nCount+1)*MEA_ITEM_HEIGHT+4,"哈哈哈哈哈哈哈哈" );
		}
		else
		{
			pDC->SetTextColor(RGB(0,0,0));
			//写Item
			int nDrawX=10;
			int nColCount = 0;
			for (nColCount=0 ;nColCount<MEA_COL_NUM ;nColCount++ )
			{
				//pDC->TextOut(nDrawX , (nCount+1)*MEA_ITEM_HEIGHT+4,"9999" );
				switch (nColCount)
				{
				case 0:					
					pDC->TextOut(nDrawX , (nCount+1)*MEA_ITEM_HEIGHT+4,pMeasureItem->pWaveData->m_strChanName );
					break;
				case 1:
					//strTemp.Format("%d",pMeasureItem->nMeaType);
					pDC->TextOut(nDrawX , (nCount+1)*MEA_ITEM_HEIGHT+4,TransMeaType(pMeasureItem->nMeaType) );
					break;
				case 2:
					pDC->TextOut(nDrawX , (nCount+1)*MEA_ITEM_HEIGHT+4,pMeasureItem->strMeaValue  );
					break;
				case 3:
					pDC->TextOut(nDrawX , (nCount+1)*MEA_ITEM_HEIGHT+4,pMeasureItem->strMin );
					break;
				case 4:
					pDC->TextOut(nDrawX , (nCount+1)*MEA_ITEM_HEIGHT+4,pMeasureItem->strMax );
					break;
				case 5:
					pDC->TextOut(nDrawX , (nCount+1)*MEA_ITEM_HEIGHT+4,pMeasureItem->strAverage );
					break;
				case 6:
					pDC->TextOut(nDrawX , (nCount+1)*MEA_ITEM_HEIGHT+4,pMeasureItem->strVariance );
					break;
				case 7:
					pDC->TextOut(nDrawX , (nCount+1)*MEA_ITEM_HEIGHT+4,TransMeaPart(pMeasureItem->nMeaPart) );
					break;
				default :
					break;
				}
				nDrawX += g_aMeaColWidth[nCount];
			}
			//pDC->TextOut(0 , (nCount+1)*MEA_ITEM_HEIGHT+4,"哈哈哈哈哈哈哈哈哈哈" );
		}

	}
	

	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldFont);
	pDC->SetBkMode(nOldMode);



}

void CMeasureDlg::AddHScroll(int nRangeStart, int nRangeEnd , CRect* rcMFinTab)
{
	if (m_pHScroll != NULL)
	{
		//已经有HScroll 调整已经有HScroll大小
		m_pHScroll->MoveWindow(CRect(0,rcMFinTab->Height()-MEA_HSCROLL_HEIGHT,rcMFinTab->Width(),rcMFinTab->Height()));
		m_pHScroll->SetScrollRange(nRangeStart,nRangeEnd,TRUE); 
		m_pHScroll->SetScrollPos(m_nHScrollPos, TRUE ); 
		return;
	}
	CRect rc; 
    GetClientRect(&rc); 
    // Create a scroll bar 
    m_pHScroll = new CScrollBar(); 
    m_pHScroll->Create(WS_CHILD|WS_VISIBLE|SBS_HORZ, 
        CRect(0,rcMFinTab->Height()-MEA_HSCROLL_HEIGHT,rcMFinTab->Width(),rcMFinTab->Height()), 
        this, 
        100); 
    m_pHScroll->SetScrollRange(nRangeStart,nRangeEnd,TRUE); 
	m_nHScrollPos = 0;
}

void CMeasureDlg::DeleteHScroll()
{
	if (m_pHScroll != NULL)
	{
		delete m_pHScroll;
		m_pHScroll = NULL;
	}
}

void CMeasureDlg::OnDestroy() 
{
	CDialog::OnDestroy();
//	TRACE("=============CMeasureDlg::OnDestroy() =================================\n");

	// TODO: Add your message handler code here
	DeleteHScroll();
/*
	if (m_pMatchWatchDlg->m_pMeasureDlg != NULL)
	{
		delete m_pMatchWatchDlg->m_pMeasureDlg;
		m_pMatchWatchDlg->m_pMeasureDlg = NULL;
	}
*/
	
}


void CMeasureDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{ 
	int pos; 
	int nRangeStart,nRangeEnd;
	m_pHScroll->GetScrollRange(&nRangeStart , &nRangeEnd);
    pos = m_pHScroll->GetScrollPos(); 
    switch ( nSBCode ) 
    { 
	case SB_LINEUP: 
		pos -= 1; 
		break; 
	case SB_LINEDOWN: 
		pos += 1; 
		break; 
	case SB_PAGEUP: 
		pos -= 1; 
		break; 
	case SB_PAGEDOWN: 
		pos += 1; 
		break; 
	case SB_TOP: 
		pos = nRangeStart; 
		break; 
	case SB_BOTTOM: 
		pos = nRangeEnd; 
		break; 
		
	case SB_THUMBPOSITION: 
		pos = nPos; 
		break; 
	default: 
		return; 
    } 
    if ( pos < nRangeStart ) 
        pos = nRangeStart; 
    else if ( pos > nRangeEnd ) 
        pos = nRangeEnd; 
    m_pHScroll->SetScrollPos( pos, TRUE ); 
	m_nHScrollPos = pos;

	CRect rcReDraw;
	GetClientRect(&rcReDraw);
	if (m_pHScroll == NULL){
		//无m_pHScroll
	} 
	else{
		rcReDraw.bottom -= MEA_HSCROLL_HEIGHT;
	}
	InvalidateRect(&rcReDraw);
} 

void CMeasureDlg::DrawTitleBar(CDC *pDC)
{
	CBrush grayBrush(RGB(100,100,100));
	CBrush* pOldBrush = pDC->SelectObject(&grayBrush);

	CPen transPen(PS_NULL, 0 , RGB(0,0,0));
	CPen* pOldPen = pDC->SelectObject(&transPen);

	pDC->Rectangle(0,0,m_nBmpWidth,MEA_TITLE_HEIGHT);

	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);


	CFont itemFont;	
	itemFont.CreateFont( 12, // nHeight 
		5, // nWidth 
		0, // nEscapement文本行逆时针旋转角度 
		0, // nOrientation字体角度
		FW_BOLD, // nWeight字体粗细程度 
		FALSE, // bItalic 
		FALSE, // bUnderline 
		0, // cStrikeOut 删除线
		ANSI_CHARSET, // nCharSet 
		OUT_DEFAULT_PRECIS, // nOutPrecision 
		CLIP_DEFAULT_PRECIS, // nClipPrecision 
		DEFAULT_QUALITY, // nQuality 
		DEFAULT_PITCH | FF_SWISS, // nPitchAndFamily 
		_T("隶书")); // lpszFac		pDC->SelectStockObject(SYSTEM_FIXED_FONT);		
	CFont *pOldFont;
	pOldFont = pDC->SelectObject(&itemFont);

	int nOldMode = pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(RGB(255,255,255));
	//写Item
	int nDrawX=10;
	int nColCount = 0;
	for (nColCount=0 ;nColCount<MEA_COL_NUM ;nColCount++ )
	{
		pDC->TextOut(nDrawX , 3 , g_aMeaColName[nColCount] );
		nDrawX += g_aMeaColWidth[nColCount];
	}
	

	pDC->SelectObject(pOldFont);
	pDC->SetBkMode(nOldMode);

}

void CMeasureDlg::DrawGrid(CDC *pDC)
{
	int nCount;
	CPen grayPen(PS_SOLID, 0 , RGB(212,208,200));
	CPen whitePen(PS_SOLID, 0 , RGB(255,255,255));
	//表格横线
	for(nCount=0 ; nCount<m_nMeasureNum ; nCount++)
	{
		
		CPen* pOldPen = pDC->SelectObject(&grayPen);
		pDC->MoveTo(0 , (nCount+1)*MEA_ITEM_HEIGHT);
		pDC->LineTo(m_nBmpWidth , (nCount+1)*MEA_ITEM_HEIGHT);	
		pDC->SelectObject(pOldPen);
	}
	
	//表格竖线
	int nDrawX = 5;					
	for(nCount=0 ; nCount<MEA_COL_NUM ; nCount++)
	{
		if (nCount == 0)
		{
			//第一道线不画  为了美观
			nDrawX += g_aMeaColWidth[nCount];
			continue;
		}
		CPen* pOldPen = pDC->SelectObject(&whitePen);
		pDC->MoveTo(nDrawX , 0);
		pDC->LineTo(nDrawX , MEA_TITLE_HEIGHT);	
		pDC->SelectObject(pOldPen);

		pOldPen = pDC->SelectObject(&grayPen);
		pDC->MoveTo(nDrawX , MEA_TITLE_HEIGHT);
		pDC->LineTo(nDrawX , m_nBmpHeight);	
		pDC->SelectObject(pOldPen);

		nDrawX += g_aMeaColWidth[nCount];
	}


}

void CMeasureDlg::OnMeaAddmeasure() 
{
	// TODO: Add your command handler code here
	//MessageBeep(-1);
	m_pMatchWatchDlg->MeasureAdd();
}

void CMeasureDlg::OnMeaDelmeasure() 
{
	// TODO: Add your command handler code here
//	TRACE("=============CMeasureDlg::OnMeaDelmeasure() =================================\n");
//	m_pMatchWatchDlg->MeasureDelete();

	
	if (m_nMeasureNum <= 1)
	{
		//该MeasureDlg只有最后一个Item了 直接关闭
		
		m_pMatchWatchDlg->m_pMeasureDlg = NULL;
		SendMessage(WM_CLOSE);

	} 
	else
	{
		//删除当前Item
		CMeasureItem* pMeasureItem;

/*
		CTypedPtrList<CObList , CMeasureItem*>* pMeasureItemList;
		pMeasureItemList = &(m_pMeasureDlg->m_MeasureItemList);
		pMeasureItem = (CMeasureItem*)pMeasureItemList->GetAt(pMeasureItemList->FindIndex(m_pMeasureDlg->m_nSelItem));
		
		pMeasureItemList->RemoveAt(pMeasureItemList->FindIndex(m_pMeasureDlg->m_nSelItem));
*/
		pMeasureItem = (CMeasureItem*)m_MeasureItemList.GetAt(m_MeasureItemList.FindIndex(m_nSelItem));

//		m_pMatchWatchDlg->MeasureDelete();

		m_MeasureItemList.RemoveAt(m_MeasureItemList.FindIndex(m_nSelItem));


		delete pMeasureItem;

		m_nMeasureNum--;
		if (m_nMeasureNum == m_nSelItem)
		{
			//m_nSelItem选择的是最后一个Item 需要修改
			m_nSelItem--;
		}
	}
	
//	m_pMeasureDlg = NULL;

	m_pMatchWatchDlg->ComputeFrame();
	m_pMatchWatchDlg->ModifyPos();
}

void CMeasureDlg::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
//	TRACE("=============CMeasureDlg::PostNcDestroy()  =================================\n");

	int i=0;
	CMeasureItem* pMeasureItem;
	for (i=0 ; i<m_nMeasureNum ;i ++)
	{
		pMeasureItem = (CMeasureItem*)m_MeasureItemList.GetAt(m_MeasureItemList.FindIndex(0));		//每Remove一个 数量减一 因此这里0不能换成i
		
		m_MeasureItemList.RemoveAt(m_MeasureItemList.FindIndex(0));					//每Remove一个 数量减一 因此这里0不能换成i
	
		delete pMeasureItem;
		pMeasureItem = NULL;
	}

	delete this;

	CDialog::PostNcDestroy();
}

void CMeasureDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	DestroyWindow();
	//CDialog::OnCancel();
}

CString CMeasureDlg::TransMeaType(int nMeaType)
{
	CString strTemp;
	switch (nMeaType)
	{
	case MEATYPE_NORMAL_MAX:
		strTemp.Format("最大") ;
		break;
	case MEATYPE_NORMAL_MIN:
		strTemp.Format("最小") ;
		break;
	case MEATYPE_NORMAL_ACAVG:
		strTemp.Format("AC均方根") ;
		break;
	case MEATYPE_FFT_AMPPEAK:
		strTemp.Format("振幅峰值") ;
		break;
	case MEATYPE_FFT_FREQPEAK:
		strTemp.Format("频率峰值") ;
		break;
	}

	return strTemp;
}

void CMeasureDlg::OnMeaEditmeasure() 
{
	// TODO: Add your command handler code here
	m_pMatchWatchDlg->MeasureEdit();
}

void CMeasureDlg::ComputeItemTime(int nChnIndex, short *pDataBuffer, int nDataLen)
{	
	if (m_bMinFlag == true)
	{	
		//最小化的时候不用计算
		return;
	}

	int nCount = 0;
	CMeasureItem* pMeasureItem;
	for(nCount=0 ; nCount<m_nMeasureNum ; nCount++)
	{
		CString strTemp;
		pMeasureItem = (CMeasureItem*)m_MeasureItemList.GetAt(m_MeasureItemList.FindIndex(nCount));
		if ( pMeasureItem->pWaveData->m_nChnIndex == nChnIndex)
		{
			//通道数对应 进行更新
			switch (pMeasureItem->nMeaType)
			{
			/*时域的分析*/
			case MEATYPE_NORMAL_MAX:
				strTemp = TimeComputeMax(pDataBuffer,nDataLen);
				pMeasureItem->strMeaValue = strTemp;
				pMeasureItem->strMax = strTemp;
				pMeasureItem->strMin = strTemp;
				pMeasureItem->strAverage = strTemp;				
				break;
			case MEATYPE_NORMAL_MIN:
				strTemp = TimeComputeMin(pDataBuffer,nDataLen);
				pMeasureItem->strMeaValue = strTemp;
				pMeasureItem->strMax = strTemp;
				pMeasureItem->strMin = strTemp;
				pMeasureItem->strAverage = strTemp;

				break;
			case MEATYPE_NORMAL_ACAVG:
				strTemp = TimeComputeACAVG(pDataBuffer,nDataLen);
				pMeasureItem->strMeaValue = strTemp;
				pMeasureItem->strMax = strTemp;
				pMeasureItem->strMin = strTemp;
				pMeasureItem->strAverage = strTemp;
				break;
			}
		}

	}

	CRect rcClient;
	GetClientRect(&rcClient);
	rcClient.top=MEA_ITEM_HEIGHT;
	rcClient.left = g_aMeaColWidth[0] + g_aMeaColWidth[1];
	rcClient.right = rcClient.left + g_aMeaColWidth[2] + g_aMeaColWidth[3] + g_aMeaColWidth[4] + g_aMeaColWidth[5] + g_aMeaColWidth[6];

	InvalidateRect(rcClient);
}


void CMeasureDlg::ComputeItemFreq(int nChnIndex, double *pDataBuffer, int nDataLen)
{
	if (m_bMinFlag == true)
	{	
		//最小化的时候不用计算
		return;
	}
	
	int nCount = 0;
	CMeasureItem* pMeasureItem;
	for(nCount=0 ; nCount<m_nMeasureNum ; nCount++)
	{
		CString strTemp;
		pMeasureItem = (CMeasureItem*)m_MeasureItemList.GetAt(m_MeasureItemList.FindIndex(nCount));
		if ( pMeasureItem->pWaveData->m_nChnIndex == nChnIndex)
		{
			//通道数对应 进行更新
			switch (pMeasureItem->nMeaType)
			{
				/*频域的分析*/
			case MEATYPE_FFT_AMPPEAK:
				strTemp = FreqComputeMax(pDataBuffer,nDataLen);
				pMeasureItem->strMeaValue = strTemp;
				pMeasureItem->strMax = strTemp;
				pMeasureItem->strMin = strTemp;
				pMeasureItem->strAverage = strTemp;	
				break;
			case MEATYPE_FFT_FREQPEAK:
				strTemp = FreqComputeFindMaxIndex(pDataBuffer,nDataLen);
				pMeasureItem->strMeaValue = strTemp;
				pMeasureItem->strMax = strTemp;
				pMeasureItem->strMin = strTemp;
				pMeasureItem->strAverage = strTemp;	
				break;
			}
		}
		
	}
	
	CRect rcClient;
	GetClientRect(&rcClient);
	rcClient.top=MEA_ITEM_HEIGHT;
	rcClient.left = g_aMeaColWidth[0] + g_aMeaColWidth[1];
	rcClient.right = rcClient.left + g_aMeaColWidth[2] + g_aMeaColWidth[3] + g_aMeaColWidth[4] + g_aMeaColWidth[5] + g_aMeaColWidth[6];
	
	InvalidateRect(rcClient);
}


CString CMeasureDlg::FreqComputeFindMaxIndex(double *pDataBuffer, int nDataLen)
{
	CString strResult;
	
	int nMaxIndex = 0;
	int nCount=0;
	double nMax = *pDataBuffer;
	for (nCount=1 ; nCount<nDataLen ; nCount++)
	{
		if ( nMax < *(pDataBuffer+nCount) )		{
			nMax = *(pDataBuffer+nCount);
			nMaxIndex = nCount;
		}
	}
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CRecorderDoc *pDoc = (CRecorderDoc*)(pFrame->GetActiveDocument());
	CProduct* pProduct = &(pDoc->m_Product);
	
	double fMaxFreq;

	double fRange=0.0;
	CString strUnit;

	strUnit = m_pMatchWatchDlg->ComputeHorUnitFreq(&fRange , pProduct->m_AcqInfo.samplingRate/2 );

	strResult.Format("%.2f",fRange*nMaxIndex/nDataLen);
	strResult = strResult + strUnit;
	
	return strResult;
}

CString CMeasureDlg::FreqComputeMax(double *pDataBuffer, int nDataLen)
{
	CString strResult;
	
	int nCount=0;
	double fMax = *pDataBuffer;
	for (nCount=1 ; nCount<nDataLen ; nCount++)
	{
		if ( fMax < *(pDataBuffer+nCount) )		{
			fMax = *(pDataBuffer+nCount);
		}
	}
	strResult.Format("%.2f",fMax);
	
	return strResult;
}

CString CMeasureDlg::TimeComputeMax(short *pDataBuffer, int nDataLen)
{
	CString strResult;

	int nCount=0;
	short nMax = *pDataBuffer;
	for (nCount=1 ; nCount<nDataLen ; nCount++)
	{
		if ( nMax < *(pDataBuffer+nCount) )		{
			nMax = *(pDataBuffer+nCount);
		}
	}
	strResult.Format("%d",nMax);

	return strResult;
}	

CString CMeasureDlg::TimeComputeMin(short *pDataBuffer, int nDataLen)
{
	CString strResult;
	
	int nCount=0;
	short nMin = *pDataBuffer;
	for (nCount=1 ; nCount<nDataLen ; nCount++)
	{
		if ( nMin > *(pDataBuffer+nCount) )		{
			nMin = *(pDataBuffer+nCount);
		}
	}
	strResult.Format("%d",nMin);
	
	return strResult;
}

CString CMeasureDlg::TimeComputeACAVG(short *pDataBuffer, int nDataLen)
{
	CString strResult;
	
	int nCount=0;
	short nACAVG = 0;
	long nSum = 0;
	for (nCount=1 ; nCount<nDataLen ; nCount++)
	{
		nSum = nSum + (*(pDataBuffer+nCount)) * (*(pDataBuffer+nCount));
	}
	nACAVG = (short)sqrt((float)(nSum));
	strResult.Format("%d",(int)(nACAVG));
	
	return strResult;
}

CString CMeasureDlg::TransMeaPart(int nMeaPart)
{
	CString strTemp;
	switch (nMeaPart)
	{
	case MEAPART_ALL:
		strTemp.Format("整个轨迹") ;
		break;
	case MEAPART_BET1AND2:
		strTemp.Format("标尺间") ;
		break;
	case MEAPART_LOOPBY1:
		strTemp.Format("标尺1循环") ;
		break;
	case MEAPART_LOOPBY2:
		strTemp.Format("标尺2循环") ;
		break;
	}
	
	return strTemp;
}

void CMeasureDlg::MeasureDelete()
{
	OnMeaDelmeasure();
}







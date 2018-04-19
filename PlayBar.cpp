// PlayBar.cpp : implementation file
//

#include "stdafx.h"
#include "PlayBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
//#include "RecorderView.h"
#include <math.h>
#include "MainFrm.h"
#include "RecorderDoc.h"
//class CRecorderView;
//////////////////////////////////////////////////////////////////////////
extern CEvent g_TransAndPlayEvent;
extern CEvent g_PlayPauseEvent;
//////////////////////////////////////////////////////////////////////////
// CPlayBar

CPlayBar::CPlayBar()
{
	
	m_nPlayPos = 0;
	m_nPlayWndWidth = 1;
	m_nCachePos = 0;

	m_nMinPos = 0;
	m_nMaxPos = 0;
	m_nRange = 0;

	m_bPressed = false;

	m_nWndLeftToClickPoint = 0;





}

CPlayBar::~CPlayBar()
{
}


IMPLEMENT_DYNCREATE(CPlayBar, CWnd)  
BEGIN_MESSAGE_MAP(CPlayBar, CWnd)
	//{{AFX_MSG_MAP(CPlayBar)
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CPlayBar message handlers


BOOL CPlayBar::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	lpszClassName=AfxRegisterWndClass( CS_HREDRAW | CS_VREDRAW , AfxGetApp()->LoadStandardCursor(IDC_ARROW),   
		(HBRUSH)GetStockObject(WHITE_BRUSH), NULL) ; 

	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

void CPlayBar::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CRecorderDoc *pDoc = (CRecorderDoc*)(pFrame->GetActiveDocument());
	m_pProduct = &(pDoc->m_Product);	
/*
	CPen blackPen(PS_SOLID, 0, RGB(0,0,0)); // Black
	dc.SelectObject(blackPen);
	dc.MoveTo(0,0);
	dc.LineTo(500,500);
*/
	CDC dcMem;
	CBitmap bm;
	CRect rc;
	
	GetClientRect(&rc);
	int nWidth = rc.Width();
	int nHeight = rc.Height();
	dcMem.CreateCompatibleDC(&dc);
	bm.CreateCompatibleBitmap(&dc,rc.Width(),rc.Height());
	dcMem.SelectObject(&bm); 

	CBrush* pGrayBrush = CBrush::FromHandle((HBRUSH)GetStockObject(LTGRAY_BRUSH));
	dcMem.SelectObject(pGrayBrush);
	dcMem.FillSolidRect(rc,dc.GetBkColor());
/////////////////////////进度条数字部分/////////////////////////////////////////////////
	CRect rcProgressNum;
	rcProgressNum.SetRect(rc.right-PRONUM_WIDTH , 0 , rc.right , nHeight);
	dcMem.FillSolidRect(rcProgressNum,RGB(0,0,0));

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
	pOldFont = dcMem.SelectObject(&itemFont);
	int nOldMode = dcMem.SetBkMode(TRANSPARENT);
	dcMem.SetTextColor(RGB(255,255,255));

	CString strTextOut;
	strTextOut.Format("%d/%d",m_nCachePos,m_pProduct->m_nSegmentNum);
	dcMem.TextOut(nWidth-PRONUM_WIDTH , 3 , strTextOut);

	dcMem.SelectObject(pOldFont);
	dcMem.SetBkMode(nOldMode);

/////////////////////////缓冲进度部分/////////////////////////////////////////////////
	CRect rcProgressCache;
	float fRatio = (float)m_nCachePos / (float)m_pProduct->m_nSegmentNum;
	rcProgressCache.SetRect(rc.left , 0 , (nWidth-PRONUM_WIDTH)*fRatio , nHeight+1);
	CPen *pNullPen=CPen::FromHandle((HPEN)GetStockObject(NULL_PEN));
    CPen *pOldPen=dcMem.SelectObject(pNullPen);
	dcMem.Rectangle(rcProgressCache);

	dcMem.SelectObject(pOldPen);
/////////////////////////播放进度部分/////////////////////////////////////////////////
/*
	CRect rcProgressPlay;
	fRatio = (float)m_nPlayPos / (float)m_nRange;
	rcProgressPlay.SetRect(rc.left , 0 , (nWidth-PRONUM_WIDTH)*fRatio , nHeight);
	dcMem.FillSolidRect(rcProgressPlay,RGB(0,0,200));
*/
	CRect rcProgressPlayWnd;
//	fRatio = (float)m_nPlayPos / (float)m_nRange;

	fRatio = ((float)m_pProduct->m_nPlayStartIndex) / ((float)m_nRange );
//	TRACE("----fRatio : %f------\n",fRatio);
//	TRACE("----m_pProduct->m_nPlayStartIndex : %d------\n",m_pProduct->m_nPlayStartIndex);
//	TRACE("----m_nRange : %d------\n",m_nRange);

	int nDisplayWndWidth = ceil(((float)m_nPlayWndWidth/(float)m_nRange)*(nWidth-PRONUM_WIDTH));
	rcProgressPlayWnd.SetRect((nWidth-PRONUM_WIDTH)*fRatio , 0 , (nWidth-PRONUM_WIDTH)*fRatio+nDisplayWndWidth , nHeight);
	CPen yellowPen(PS_SOLID,3,RGB(255,242,0));
	CBrush* pNullBrush = CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
	pOldPen = dcMem.SelectObject(&yellowPen);
	CBrush*	pOldBrush = dcMem.SelectObject(pNullBrush);
//TRACE("OnPaint----------m_nCachePos  : %d\n",m_nCachePos);
	dcMem.Rectangle(rcProgressPlayWnd);

	dcMem.SelectObject(pOldPen);
	dcMem.SelectObject(pOldBrush);
/////////////////////////拖动光标/////////////////////////////////////////////////
	

	
/////////////////////////双缓冲/////////////////////////////////////////////////

	dc.BitBlt(0,0,rc.Width(),rc.Height(),&dcMem,0,0,SRCCOPY);
	dcMem.DeleteDC();
	bm.DeleteObject();


	// Do not call CWnd::OnPaint() for painting messages
}

void CPlayBar::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here

	
	
}

void CPlayBar::SetCachePos(int nCachePos)
{
	m_nCachePos = nCachePos;
//	TRACE("CPlayBar::SetCachePos(int nCachePos) : %d\n",m_nCachePos);
	Invalidate();
}

void CPlayBar::SetPlayPos(int nPlayPos)
{
	m_nPlayPos = nPlayPos;
	Invalidate();
}

void CPlayBar::SetRange(int nMin, int nMax)
{
	m_nMinPos = nMin;
	m_nMaxPos = nMax;
	m_nRange = nMax - nMin;
	Invalidate();
}

BOOL CPlayBar::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	return true;
//	return CWnd::OnEraseBkgnd(pDC);
}


BOOL CPlayBar::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// TODO: Add your message handler code here and/or call default

	CPoint pos;  
	::GetCursorPos(&pos); 
	ScreenToClient(&pos);

	CRect rcClient;
	GetClientRect(rcClient);
	int nWidth = rcClient.Width();
	int nHeight = rcClient.Height();
	CRect rcChange;
	rcChange.SetRect(	(nWidth-PRONUM_WIDTH)*((float)m_nPlayPos/(float)m_nRange)-3,
						0,
						(nWidth-PRONUM_WIDTH)*((float)(m_nPlayPos+m_nPlayWndWidth)/(float)m_nRange)+3,
						nHeight
					);
	if (rcChange.PtInRect(pos))
	{
		::SetCursor(LoadCursor(NULL,IDC_SIZEWE));
	} 
	else
	{
		::SetCursor(LoadCursor(NULL,IDC_ARROW));
	}

	return TRUE;
//	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}

void CPlayBar::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	g_TransAndPlayEvent.PulseEvent();

	m_bPressed = false;
	ReleaseCapture();



	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
/*	CRecorderView* pView = (CRecorderView*)(pFrame->GetActiveView());
	pView->WndManageRefresh();
*/	
	pFrame->ViewWndManageRefresh();
	
	CWnd::OnLButtonUp(nFlags, point);
}

void CPlayBar::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	
	CRect rcClient;
	GetClientRect(rcClient);
	int nWidth = rcClient.Width();
	int nHeight = rcClient.Height();
	CRect rcChange;
	rcChange.SetRect(	(nWidth-PRONUM_WIDTH)*((float)m_nPlayPos/(float)m_nRange)-3,
						0,
						(nWidth-PRONUM_WIDTH)*((float)(m_nPlayPos+m_nPlayWndWidth)/(float)m_nRange)+3,
						nHeight
						);
	if (rcChange.PtInRect(point))
	{
		m_nWndLeftToClickPoint = point.x - (nWidth-PRONUM_WIDTH)*((float)m_nPlayPos/(float)m_nRange);
		m_bPressed = true;
		SetCapture();
	} 


	CWnd::OnLButtonDown(nFlags, point);
}


void CPlayBar::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CRecorderDoc *pDoc = (CRecorderDoc*)(pFrame->GetActiveDocument());
	m_pProduct = &(pDoc->m_Product);

	int nPlayPos;
	CRect rcClient;
	GetClientRect(rcClient);
	int nWidth = rcClient.Width();

	int nWndLeftStartPos = point.x - m_nWndLeftToClickPoint;

	if (m_bPressed)
	{
		nPlayPos = (int)((float)nWndLeftStartPos/(float)(nWidth-PRONUM_WIDTH)*m_nRange);
		if (nPlayPos<=0){
			nPlayPos = 0;
		}
		else{
			if (	m_nCachePos*DATASTORAGE_SEGMENT < m_pProduct->m_AcqInfo.samplingLength	)	{
				if (	nPlayPos+m_nPlayWndWidth > m_nCachePos*DATASTORAGE_SEGMENT	){
					nPlayPos = m_nCachePos*DATASTORAGE_SEGMENT - m_nPlayWndWidth;
				}
			} 
			else	{
				if (	nPlayPos+m_nPlayWndWidth > m_pProduct->m_AcqInfo.samplingLength	){
					nPlayPos = m_pProduct->m_AcqInfo.samplingLength - m_nPlayWndWidth;
				}
			}
		}

/*
/ *
	else if (nPlayPos+m_nPlayWndWidth > m_pProduct->m_AcqInfo.samplingLength/ *m_nCachePos*DATASTORAGE_SEGMENT* /){
		nPlayPos = m_pProduct->m_AcqInfo.samplingLength - m_nPlayWndWidth;
		TRACE("---------nPlayPos : %d\n",nPlayPos);
		TRACE("---------m_nPlayWndWidth : %d\n",m_nPlayWndWidth);			
	}
* /

	else if (nPlayPos+m_nPlayWndWidth > m_nCachePos*DATASTORAGE_SEGMENT)
	{
		if (m_nCachePos*DATASTORAGE_SEGMENT < m_pProduct->m_AcqInfo.samplingLength){
			nPlayPos = m_nCachePos*DATASTORAGE_SEGMENT - m_nPlayWndWidth;
		}
		else{
			nPlayPos = m_pProduct->m_AcqInfo.samplingLength - m_nPlayWndWidth;
		}
		
	}
	else if (	nPlayPos+m_nPlayWndWidth > m_pProduct->m_AcqInfo.samplingLength	)
	{
		nPlayPos = m_pProduct->m_AcqInfo.samplingLength - m_nPlayWndWidth;
	}
*/


		m_pProduct->m_nPlayStartIndex = nPlayPos;
		SetPlayPos(nPlayPos);
		
	}

	
	CWnd::OnMouseMove(nFlags, point);
}

void CPlayBar::SetPlayWndWidth(int nWidth)
{
//	m_nPlayWndWidth= nWidth;

	if (0 == m_nCachePos)
	{
		//属于初始化阶段
		m_nPlayWndWidth = nWidth;
		return;
	}

	if (nWidth >= m_nCachePos*DATASTORAGE_SEGMENT)
	{
		m_nPlayWndWidth = m_nCachePos*DATASTORAGE_SEGMENT;
		m_nPlayPos = 0;
	}
	else{
		//nWidth < m_nCachePos
		m_nPlayWndWidth= nWidth;
		if (m_nPlayPos + nWidth >= m_nCachePos*DATASTORAGE_SEGMENT)
		{
			m_nPlayPos = m_nCachePos*DATASTORAGE_SEGMENT - nWidth;
		} 
		else
		{
		}
	}
	Invalidate();

}

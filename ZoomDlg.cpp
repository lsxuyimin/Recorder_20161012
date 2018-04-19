// ZoomDlg.cpp : implementation file
//

#include "stdafx.h"
#include "recorder.h"
#include "ZoomDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
#include "WatchDlg.h"
#include "WaveData.h"
#include "Product.h"


//////////////////////////////////////////////////////////////////////////
// CZoomDlg dialog


CZoomDlg::CZoomDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CZoomDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CZoomDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pOwnedDlg = (CWatchDlg*)pParent ;

	m_fRatioStartX =0 ;
	m_fRatioStartY =0 ;
	m_fRatioEndX =0 ;
	m_fRatioEndY =0 ;
	
	m_bIsPick = false;
	m_bMinFlag = false ;

	m_nZoomDlgHeight = Zoom_DEFAULT_HEIGHT;
	m_nZoomDlgWidth = Zoom_DEFAULT_WIDTH;

}


void CZoomDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CZoomDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CZoomDlg, CDialog)
	//{{AFX_MSG_MAP(CZoomDlg)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_NCHITTEST()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CZoomDlg message handlers

void CZoomDlg::DrawWave(CDC *pDC)
{
	
	
	int i;
	CPen* pOldPen;
	CPen mypen(PS_SOLID,1,RGB(0,0,255));
	pOldPen = pDC->SelectObject (&mypen);
// 	CRect rcCurDlg;
// 	GetClientRect(&rcCurDlg);
//	CRecorderDoc* pDoc = (CRecorderDoc*)(((CMainFrame *)AfxGetMainWnd())->GetActiveDocument());

	//Ҫ����ȫ������
	
	CTypedPtrList<CObList , CWaveData*>* pWaveDataList = &(((CWatchDlg*)GetParent())->m_WaveDataList);
	CWaveData* pWaveData = pWaveDataList->GetAt(pWaveDataList->FindIndex(0));
	
	int dlgWidth = m_rcDisplayFrame.Width();
	int originX, originY;
	originX = m_rcDisplayFrame.left;
	originY = m_rcDisplayFrame.top + m_rcDisplayFrame.Height()/2;
	int halfHeight =  m_rcDisplayFrame.Height()/2;
	int interval = (int)(((pWaveData->m_WaveData).GetSize()+0.0) / dlgWidth);          //���interval������dlgwidthΪ׼  ��rcReal.widthΪ׼�Ŷ�
	int XcorrdStart = 0;
	int drawPixelCount = m_rcDisplayFrame.Width();

	for ( i=XcorrdStart ; i<XcorrdStart+drawPixelCount ; i++)
	{

		pDC->MoveTo(originX+ i ,		 originY+ (pWaveData->m_WaveData).GetAt((int)(interval * i)) *halfHeight /5000 );
		pDC->LineTo(originX+ (i+1) ,	 originY+ (pWaveData->m_WaveData).GetAt((int)(interval * (i+1))) *halfHeight /5000   );
		
//		TRACE("DATA:%d\n",(pWaveData->m_WaveData).GetAt((int)(interval * i)));
	}


	
	pDC->SelectObject(pOldPen);

}

void CZoomDlg::DrawCurBorder(CDC *pDC)
{	
	CPen blackPen(PS_SOLID,1,RGB(0,0,0));
	CPen* pOldPen = pDC->SelectObject(&blackPen);
	CBrush* transBrush = CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
	CBrush *pOldBrush = pDC->SelectObject(transBrush);
	
	pDC->Rectangle(m_rcBorder);
	
	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);

}

void CZoomDlg::DrawTitle(CDC *pDC)
{
	//�����ⱳ��TitleBar
	CRect rcZoomDlg;
	GetClientRect(&rcZoomDlg);
	int nZoomDlgWidth = rcZoomDlg.Width();
	int nZoomDlgHeight = rcZoomDlg.Height();

	CBrush grayBrush(RGB(100,100,100));
	CBrush* pOldBrush = pDC->SelectObject(&grayBrush);	
	CPen transPen(PS_NULL, 0 , RGB(0,0,0));
	CPen* pOldPen = pDC->SelectObject(&transPen);
	
	pDC->Rectangle(0,0,nZoomDlgWidth,Zoom_TITLE_HEIGHT);
		
	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);

	//������Title
	CFont itemFont;	
	itemFont.CreateFont( 12, // nHeight 
		5, // nWidth 
		0, // nEscapement�ı�����ʱ����ת�Ƕ� 
		0, // nOrientation����Ƕ�
		FW_BOLD, // nWeight�����ϸ�̶� 
		FALSE, // bItalic 
		FALSE, // bUnderline 
		0, // cStrikeOut ɾ����
		ANSI_CHARSET, // nCharSet 
		OUT_DEFAULT_PRECIS, // nOutPrecision 
		CLIP_DEFAULT_PRECIS, // nClipPrecision 
		DEFAULT_QUALITY, // nQuality 
		DEFAULT_PITCH | FF_SWISS, // nPitchAndFamily 
		_T("����")); // lpszFac		pDC->SelectStockObject(SYSTEM_FIXED_FONT);		
	CFont *pOldFont;
	pOldFont = pDC->SelectObject(&itemFont);
	
	int nOldMode = pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(RGB(255,255,255));
	
/*
	if (m_bMinFlag)
	{
		//��С��ֻҪд������
*/
		pDC->TextOut(5 , 3 , "Zoom Overview");
		pDC->SelectObject(pOldFont);
		pDC->SetBkMode(nOldMode);
/*
			return;
		}
*/
	
}

void CZoomDlg::DrawBtn(CDC *pDC)
{
	CRect rcClient;
	GetClientRect(&rcClient);
	int nWidth = rcClient.Width();
		
	////////////////////����m_rcBtnClose,m_rcBtnMinOrRe//////////////////////////////////////////////////////
	CRect rcTemp;
	rcTemp.right = nWidth;
	rcTemp.left = rcTemp.right - 19;
	rcTemp.top = 0;
	rcTemp.bottom = Zoom_TITLE_HEIGHT;
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

void CZoomDlg::DrawGrid(CDC *pDC)
{
	//��ʼ��m_rcDisplayFrame
	CRect rcClient;
	GetClientRect(&rcClient);
	m_rcDisplayFrame.left = rcClient.left+3;
	m_rcDisplayFrame.top = rcClient.top+3+Zoom_TITLE_HEIGHT;
	m_rcDisplayFrame.right = rcClient.right-4;
	m_rcDisplayFrame.bottom = rcClient.bottom-3;
	//������
	CPen* pOldPen;
	CPen mypen(PS_DOT ,1,RGB(173,216,230));
	pOldPen = pDC->SelectObject (&mypen);
	int i,j;
	CString str;
	for (i=0 ; i<10 ; i++)
	{	//����
		pDC->MoveTo(m_rcDisplayFrame.left + i*m_rcDisplayFrame.Width()/10 , m_rcDisplayFrame.top );
		pDC->LineTo(m_rcDisplayFrame.left + i*m_rcDisplayFrame.Width()/10 , m_rcDisplayFrame.bottom );
	}
	pDC->MoveTo(m_rcDisplayFrame.right , m_rcDisplayFrame.top );
	pDC->LineTo(m_rcDisplayFrame.right , m_rcDisplayFrame.bottom );
	for (j=0 ; j<10 ; j++)
	{	//����
		pDC->MoveTo(m_rcDisplayFrame.left , m_rcDisplayFrame.top + j*m_rcDisplayFrame.Height()/10 );
		pDC->LineTo(m_rcDisplayFrame.right , m_rcDisplayFrame.top +  j*m_rcDisplayFrame.Height()/10 );
	}
	pDC->MoveTo(m_rcDisplayFrame.left , m_rcDisplayFrame.bottom);
	pDC->LineTo(m_rcDisplayFrame.right , m_rcDisplayFrame.bottom);

	pDC->SelectObject(pOldPen);

}

void CZoomDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	//����ҲҪ��˫����
	DrawZoomDlg(&dc);

	
	// Do not call CDialog::OnPaint() for painting messages
}

void CZoomDlg::DrawZoomDlg(CDC *pDC)
{
	//���еĻ��ƶ������������  ��Ҫ����˫����
//	CDC* pWinDC = GetWindowDC();
/*
			if (pWinDC)
				DrawTitleBar(pWinDC);
*/
/*
	CRect rc;
	GetClientRect(&rc);
	rc.left -= 5;
	rc.right += 3;
	
	CBrush whiteBrush(RGB(255,255,255));
	CBrush* pOldBrush = pWinDC->SelectObject(&whiteBrush);
	
	pWinDC->Rectangle(rc);
*/		
	CDC dcMem;							//˫����CDC
	CBitmap bm;
	CRect rc;
	GetClientRect(&rc);	
	dcMem.CreateCompatibleDC(pDC);
	bm.CreateCompatibleBitmap(pDC,rc.Width(),rc.Height());
	dcMem.SelectObject(&bm); 

	//���������ݻ�����dcmem��

	//����pMemDC��ɫ
	CRect rcZoomDlg;
	GetClientRect(&rcZoomDlg);
	int nZoomDlgWidth = rcZoomDlg.Width();
	int nZoomDlgHeight = rcZoomDlg.Height();
	CPen transPen(PS_NULL, 0 , RGB(0,0,0));
	CPen* pOldPen = dcMem.SelectObject(&transPen);
	CBrush BgWhiteBrush(RGB(255,255,255));
	CBrush* pOldBrush = dcMem.SelectObject(&BgWhiteBrush);
	dcMem.Rectangle(0,0,nZoomDlgWidth+2,nZoomDlgHeight+2);
	dcMem.SelectObject(pOldBrush);
	dcMem.SelectObject(pOldPen);

	
	DrawTitle(&dcMem);
	DrawBtn(&dcMem);

	if (!m_bMinFlag)
	{		
		DrawGrid(&dcMem);
		if (((CWatchDlg*)GetParent())->m_pProduct->m_bExistData)
		{
			DrawWave(&dcMem);
		}
		DrawCurBorder(&dcMem);
	}
	
	
	//�ѻ��ƺõ�ͼ�Ρ�����������Ļ��:BitBlt()
	pDC->BitBlt(0,0,rc.Width(),rc.Height(),&dcMem,0,0,SRCCOPY);
	dcMem.DeleteDC();
	bm.DeleteObject();



	
//	ReleaseDC(pWinDC);
}

BOOL CZoomDlg::OnInitDialog() 
{
	//��ģ̬�Ի��򲻵���OnInitDialog
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
/*
	//��ʼ��m_rcDisplayFrame
	CRect rcClient;
	GetClientRect(&rcClient);
	m_rcDisplayFrame.left = rcClient.left+3;
	m_rcDisplayFrame.top = rcClient.top+3+Zoom_TITLE_HEIGHT;
	m_rcDisplayFrame.right = rcClient.right-3;
	m_rcDisplayFrame.bottom = rcClient.bottom-3;
*/

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CZoomDlg::ModifyZoomRectRatio(	float fRatioStartX, float fRatioStartY, float fRatioEndX, float fRatioEndY)
{
	//��ʼ��m_rcDisplayFrame
	CRect rcClient;
	GetClientRect(&rcClient);
	m_rcDisplayFrame.left = rcClient.left+3;
	m_rcDisplayFrame.top = rcClient.top+3+Zoom_TITLE_HEIGHT;
	m_rcDisplayFrame.right = rcClient.right-4;
	m_rcDisplayFrame.bottom = rcClient.bottom-3;

	
	if (fRatioStartX != -1.0)	{	m_fRatioStartX = fRatioStartX;	}
	if (fRatioStartY != -1.0)	{	m_fRatioStartY = fRatioStartY;	}
	if (fRatioEndX != -1.0)		{	m_fRatioEndX = fRatioEndX;	}
	if (fRatioEndY != -1.0)		{	m_fRatioEndY = fRatioEndY;	}

	int nDisplayWidth = m_rcDisplayFrame.Width();
	int nDisplayHeight = m_rcDisplayFrame.Height();
	m_rcBorder.left = m_rcDisplayFrame.left + m_fRatioStartX * nDisplayWidth;
	m_rcBorder.top = m_rcDisplayFrame.top + fRatioStartY * nDisplayHeight;
	m_rcBorder.right = m_rcDisplayFrame.left + m_fRatioEndX * nDisplayWidth;
	m_rcBorder.bottom = m_rcDisplayFrame.top + fRatioEndY * nDisplayHeight;
	
}

void CZoomDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	TRACE("====into CZoomDlg::OnMouseMove==========================\n");

	CRect rcClient;
	CRect rcMarkDlg;
	GetClientRect(&rcClient);
	int nWidth = rcClient.Width();
	int nHeight = rcClient.Height();
	if (m_bIsPick)
	{
		//��걻���� �����϶�״̬ �����϶�
		return;
	}


	//���δ������ ��������Ƿ񻮹���ť�Ĳ���
	if (!(m_rcBtnMinOrRe.PtInRect(point) || m_rcBtnClose.PtInRect(point)))
	{
		//û���ƶ���2����ť��
		return;
	}

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

UINT CZoomDlg::OnNcHitTest(CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	CRect rcClient;
	GetClientRect(&rcClient);
	CRect rcTitleBar;
	rcTitleBar.left = 0;
	rcTitleBar.top = 0;
	rcTitleBar.right = m_rcBtnMinOrRe.left-1;
	rcTitleBar.bottom = MARK_TITLE_HEIGHT;
	
	//	TRACE("====CMarkDlg::OnNcHitTest ScreenToClient before: %d, %d==========================\n",point.x,point.y);
	ScreenToClient(&point);
	//	TRACE("====CMarkDlg::OnNcHitTest ScreenToClient  after: %d, %d==========================\n",point.x,point.y);
	
	CRect rc;
	
	CWatchDlg* pParentDlg;
	pParentDlg = (CWatchDlg*)GetParent();
	GetClientRect(&rc);
	ClientToScreen(&rc);
	pParentDlg->ScreenToClient(&rc);
	m_rcZoomDlgInWatchDlg = rc;
	
	if(rcTitleBar.PtInRect(point))
	{
		return HTCAPTION;
	}
	else
	{
		return CDialog::OnNcHitTest(point);
	}


	
	return CDialog::OnNcHitTest(point);
}

void CZoomDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnLButtonDown(nFlags, point);
}

void CZoomDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default


	if (m_rcBtnMinOrRe.PtInRect(point))
	{
		//�������BtnMinOrRe��ť

		m_bMinFlag = !m_bMinFlag;
		if (m_bMinFlag)
		{
			//������С��
			m_nZoomDlgHeight = Zoom_TITLE_HEIGHT;
			m_nZoomDlgWidth = Zoom_MIN_WIDTH;
			m_rcZoomDlgInWatchDlg.left = m_rcZoomDlgInWatchDlg.right - m_nZoomDlgWidth;
			m_rcZoomDlgInWatchDlg.bottom = m_rcZoomDlgInWatchDlg.top + m_nZoomDlgHeight;
			//�����+1��-1��Ŀ����΢���Ի����ƶ���λ�ã���Ȼ�Ի����һֱ�����·��ƶ���ԭ���꣬��ͬ
			MoveWindow(m_rcZoomDlgInWatchDlg.left+1,m_rcZoomDlgInWatchDlg.top-1,m_nZoomDlgWidth,m_nZoomDlgHeight);
			Invalidate();
		}
		else{
			//��ԭ
			m_nZoomDlgHeight = Zoom_DEFAULT_HEIGHT;
			m_nZoomDlgWidth = Zoom_DEFAULT_WIDTH;
			m_rcZoomDlgInWatchDlg.left = m_rcZoomDlgInWatchDlg.right - m_nZoomDlgWidth;
			m_rcZoomDlgInWatchDlg.bottom = m_rcZoomDlgInWatchDlg.top + m_nZoomDlgHeight;
			MoveWindow(m_rcZoomDlgInWatchDlg.left+1,m_rcZoomDlgInWatchDlg.top-1,m_nZoomDlgWidth,m_nZoomDlgHeight);
			Invalidate();
		}
				

		
		
	}
	else if (m_rcBtnClose.PtInRect(point))
	{
		//�������BtnClose��ť
		ShowWindow(SW_HIDE);
		m_pOwnedDlg->Invalidate();
		
	}
	else
	{

	
	}
	
	CDialog::OnLButtonUp(nFlags, point);
}

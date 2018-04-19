// RecorderView.cpp : implementation of the CRecorderView class
//

#include "stdafx.h"
#include "Recorder.h"

#include "RecorderDoc.h"
#include "RecorderView.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRecorderView

IMPLEMENT_DYNCREATE(CRecorderView, CFormView)

BEGIN_MESSAGE_MAP(CRecorderView, CFormView)
	//{{AFX_MSG_MAP(CRecorderView)
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CFormView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRecorderView construction/destruction

CRecorderView::CRecorderView()
	: CFormView(CRecorderView::IDD)
{
	//{{AFX_DATA_INIT(CRecorderView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// TODO: add construction code here

}

CRecorderView::~CRecorderView()
{
}

void CRecorderView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRecorderView)
	DDX_Control(pDX, IDC_STATIC_InitialMark, m_bInitialMark);
	//}}AFX_DATA_MAP
}

BOOL CRecorderView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFormView::PreCreateWindow(cs);
}

void CRecorderView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

}

/////////////////////////////////////////////////////////////////////////////
// CRecorderView printing

BOOL CRecorderView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CRecorderView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CRecorderView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CRecorderView::OnPrint(CDC* pDC, CPrintInfo* /*pInfo*/)
{
	// TODO: add customized printing code here
}

/////////////////////////////////////////////////////////////////////////////
// CRecorderView diagnostics

#ifdef _DEBUG
void CRecorderView::AssertValid() const
{
	CFormView::AssertValid();
}

void CRecorderView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CRecorderDoc* CRecorderView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CRecorderDoc)));
	return (CRecorderDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CRecorderView message handlers

void CRecorderView::OnSize(UINT nType, int cx, int cy) 
{
	CFormView::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	CRect rc;
	GetClientRect(&rc);
/*
	CString str;
	str.Format("in OnSize of View: %d,%d,%d,%d\n",rc.bottom,rc.left,rc.top,rc.right);
	TRACE(str);
*/
	int btestbutton;
	btestbutton = (m_bInitialMark.GetSafeHwnd() != NULL);
	//	MessageBox(str);
	if (btestbutton)
	{
		//调整watchdlg 位置
		m_WndManage.m_rcFrameWnd = rc;
		m_WndManage.Postion();
		Invalidate();

		
		//消除出现的滚动条
		CSize size;
		size.cx = rc.right - rc.left;
		size.cy = rc.bottom - rc.top;
		SetScrollSizes(MM_HIMETRIC, size); // 将CScrollView的大小设置为当前客户区大小
	} 
	else
	{
	}
	

	m_WndManage.Display(m_WndManage.m_nCurMode);
}

void CRecorderView::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	WndManageRefresh();
		
		
	CFormView::OnTimer(nIDEvent);
}

int CRecorderView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here

	//给DOC中的Product赋值
	GetDocument()->m_Product.m_pRecView = this;

	//建立管理窗口类
	CRect rc;
	GetClientRect(&rc);
	m_WndManage.Create(MAX_CHANNUM, rc , this);


	return 0;
}

void CRecorderView::WndManageRefresh()
{
/*
	int i = m_WndManage.m_nFirst;
	for ( ; i <= m_WndManage.m_nLast ; i++)
	{
		m_WndManage.m_pWatchDlg[i].Invalidate();
	}
*/
	m_WndManage.AllWndRefresh();
}

void CRecorderView::ViewInitialize(int mode)
{
	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
	//1.改工具栏
	pMain->ChangeToolBar(mode);
	//2.初始化View客户区
/*
		switch (mode)
		{
		case 0:
			TRACE("Initialize MODE PHYCARD");
			break;
		case 1:
			TRACE("Initialize MODE DEMO");
			break;
		case 2:
			TRACE("Initialize MODE PLAY");
			break;
		default:
			TRACE("Initialize MODE NODEFINE");
			break;
		}
*/
	
}

void CRecorderView::WndManageRefresh_Complete()
{
	m_WndManage.AllWndRefresh_Complete();
}

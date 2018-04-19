// WatchDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Recorder.h"
#include "WatchDlg.h"

#include "MainFrm.h"
#include "RecorderDoc.h"
#include "RecorderView.h"
#include "WaveData.h"
#include "MeasureDlg.h"
#include "BgDlg.h"
#include "Product.h"
#include "WndManage.h"
#include "MeasureAddDlg.h"
#include "MeasureEditDlg.h"
#include "MarkDlg.h"
#include "ZoomDlg.h"
#include "DlgBarEx.h"
#include "DlgBarExBottomPlay.h"
#include "PlayBar.h"
#include "PopupPropertyDlg.h"
#include "ChSelDlg.h"
#include "DlgBarExTopChnSel.h"
#include "FftComplex.h"

#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWatchDlg dialog


CWatchDlg::CWatchDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWatchDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWatchDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	int nCount = 0;
	m_AcqInfo.range=10;											//初始量程 10v	
	m_AcqInfo.samplingLength = 1048576;							//初始采集长度 
	m_AcqInfo.samplingRate = 1000000;							//初始采样频率1M

	m_WDW.fCurRatio = 1.0;
	m_WDW.fCurRatioX = 1.0;
	m_WDW.fCurRatioY = 1.0;
	m_WDW.fNewRatio = 1.0;
	m_WDW.ptDisplayInReal.x = 0;
	m_WDW.ptDisplayInReal.y = 0;
	

	m_nWaveDataNum = 0;

	m_pMeasureDlg = NULL;

	m_pRecorderDoc = (CRecorderDoc*)(((CMainFrame *)AfxGetMainWnd())->GetActiveDocument());
	m_pProduct = &(m_pRecorderDoc->m_Product);

	CRecorderView* pView= (CRecorderView*)(((CMainFrame *)AfxGetMainWnd())->GetActiveView());
	m_pWndManage = &(pView->m_WndManage);

	m_pOwnedBgDlg = NULL;

	//所有通道关闭
	m_nWaveDataRange[0] = 0;
	m_nWaveDataRange[1] = 0;
	m_nWaveDataRange[2] = 0;
	m_nWaveDataRange[3] = 0;

//////m_XMF,m_YMF结构体赋值///////////////////////////////////////////////////////////////////
	m_XMF.rcXAxisMarkFrame = CRect(0,0,0,0);
	m_XMF.rcPosMarkX_1 = CRect(0,0,0,0);
	m_XMF.rcPosMarkX_2 = CRect(0,0,0,0);
	m_XMF.rcPosScale_1 = CRect(0,0,0,0);
	m_XMF.rcPosScale_2 = CRect(0,0,0,0);
	m_XMF.fRatioMarkX_1 = 0.0;
	m_XMF.fRatioMarkX_2 = 0.0;
	m_XMF.fRatioScale_1 = 1.0;
	m_XMF.fRatioScale_2 = 1.0;

	for ( nCount=0 ; nCount<DRAW_CHNUM_MAX ; nCount++)
	{
		m_YMF[nCount].rcYAxisMarkFrame = CRect(0,0,0,0);
		m_YMF[nCount].rcPosMarkY_1 = CRect(0,0,0,0);
		m_YMF[nCount].rcPosMarkY_2 = CRect(0,0,0,0);
		m_YMF[nCount].fRatioMarkY_1 = 0.0;
		m_YMF[nCount].fRatioMarkY_2 = 0.0;
	}
	

	m_nMarkMode = 0;
	m_bMarkIsPick = false;

	m_bmpMarkX.LoadBitmap(IDB_BMP_MARK_X);
	m_bmpMarkY.LoadBitmap(IDB_BMP_MARK_Y);
	m_bmpScale.LoadBitmap(IDB_BMP_SCALE);
	
	m_pMarkDlg = NULL;

	m_pZoomDlg = NULL;
	m_bZoomDragIsPick = false;
}


void CWatchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWatchDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWatchDlg, CDialog)
	//{{AFX_MSG_MAP(CWatchDlg)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_DESTROY()
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_MEA_ADDMEASURE, OnMeaAddmeasure)
	ON_COMMAND(ID_MEA_DELMEASURE, OnMeaDelmeasure)
	ON_WM_LBUTTONUP()
	ON_WM_SETCURSOR()
	ON_COMMAND(ID_LAYOUT_Mode1, OnLAYOUTMode1)
	ON_COMMAND(ID_LAYOUT_Mode2, OnLAYOUTMode2)
	ON_COMMAND(ID_LAYOUT_Mode3, OnLAYOUTMode3)
	ON_COMMAND(ID_LAYOUT_Mode4, OnLAYOUTMode4)
	ON_COMMAND(ID_WATCH_Property, OnWATCHProperty)
	ON_COMMAND(ID_WATCH_AddOsc, OnWATCHAddOsc)
	ON_COMMAND(ID_WATCH_AddFreDomain, OnWATCHAddFreDomain)
	ON_COMMAND(ID_WATCH_ChSel, OnWATCHChSel)
	ON_COMMAND(ID_WATCH_CloseWatchDlg, OnWATCHCloseWatchDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWatchDlg message handlers




void CWatchDlg::SetIndex(int index)
{
	m_nIndexInWaveList = index;
}

BOOL CWatchDlg::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	
//	return CDialog::OnEraseBkgnd(pDC);
	return TRUE;
}

void CWatchDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CDC dcMem;
	CBitmap bm;
	CRect rc;

	GetClientRect(&rc);
	// Step 1:为屏幕DC创建兼容的内存DC :CreateCompatibleDC()

//	TRACE("========into paint: rect %d  %d  %d  %d  ==================\n",rc.top ,rc.bottom ,rc.left , rc.right);

	dcMem.CreateCompatibleDC(&dc);
	// Step 2:创建位图：CreateCompatibleBitmap()
	bm.CreateCompatibleBitmap(&dc,rc.Width(),rc.Height());
	// Step 3:把位图选入设备环境：SelectObject()，可以理解为选择画布
	dcMem.SelectObject(&bm); 
	// dcMem.FillSolidRect(0,0,rc.Width(),rc.Height(),RGB(255,255,255));
	//dcMem.FillSolidRect(rc,dc.GetBkColor());
	dcMem.FillSolidRect(rc,RGB(0,0,0));
	//	dcMem.SetBkMode(TRANSPARENT);//设置透明背景色
	//	dcMem.TextOut(-1,-3,"888888888888");
	//绘制顶部文字 “示波器%”
//	DrawTitle(&dcMem);
	//绘制光标读数
//	DrawCursorRead(&dcMem);
	//绘制bg
	DrawBG(&dcMem);

	//测试文字
// 	CString str;
// 	str.Format("window %d",m_nIndexInWaveList);
// 	dcMem.TextOut(10,10,str);

	//绘制波形
	//TRACE("m_pProduct->m_bExistData : %d\n" , m_pProduct->m_bExistData);

	if (m_pProduct->m_bExistData)				//这里也要改  判断是否DrawWave 应该看product里面是否有数据  而不是看m_bAcqing
	{
		DrawWave(&dcMem);
	}

	//绘制选定的方框
//	DrawSelectedBorder(&dcMem);

	//绘制Mark Sacle 以及由它们引出的虚线
	DrawMark(&dcMem);

	//绘制由ZoomDrag产生的矩形框
	if (m_bZoomDragIsPick)
	{
		DrawZoomDragBorder(&dcMem);
	}

/*
	//test矩形区域
	CPen pen(PS_SOLID,1,RGB(200,200,200));
	dcMem.SelectObject(&pen);
	CBrush *pBrush=CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
	dcMem.SelectObject(pBrush);
	dcMem.Rectangle(m_YF[0].rcFrame);
*/

	
	// Step 4:把绘制好的图形“拷贝”到屏幕上:BitBlt()
	dc.BitBlt(0,0,rc.Width(),rc.Height(),&dcMem,0,0,SRCCOPY);
	dcMem.DeleteDC();
	bm.DeleteObject();
	//闪屏实验
	//	dc.Ellipse(10,10,50+m_nCount*5,50+m_nCount*5);
	// Do not call CDialog::OnPaint() for painting messages
}

void CWatchDlg::DrawWave(CDC* pDC)
{
	
	AcqInfo* pAcqInfo = &(m_pProduct->m_AcqInfo);
//////////////////////////////////////////////////////////////////////////
	int i;
	CWaveData* pWaveData = NULL;
	CPen* pOldPen;
	int nDrawPixelWidth = 0;		//画的m_WDW的像素宽
	int originX = 0;			//源点坐标x值
	int originY = 0;			//源点坐标y值
	int nHeightPixel = 0;
	int nHalfHeight = 0;			//m_WDW一半的像素高
	int nCoun = 0;	
	int nFirstY = 0;			//前一个点的Y值
	int nSecondY = 0;			//后一个点的Y值
	int nRange = 0;				//当前通道的量程
	int nCount = 0;


	nDrawPixelWidth = m_WDW.rcDisplay.Width();
	//short* pDataBuffer = (short*)malloc(nDrawPixelWidth * sizeof(short));
	short* pDataBuffer = NULL;
	
	//要画的全部数据
	//画多个数据时 这里应当进行循环

	if ( m_nMode == 0)
	{
		//时域	
		pDataBuffer = (short*)malloc(nDrawPixelWidth * sizeof(short));		//缓存要显示的数据

		int nWaveIndex=0;
		for ( nWaveIndex=0 ; nWaveIndex<DRAW_CHNUM_MAX ; nWaveIndex++)
		{
			if ( m_nWaveDataNum >= nWaveIndex+1 )
			{
				/*start 画一个Wave*/
				pWaveData = m_WaveDataList.GetAt(m_WaveDataList.FindIndex(nWaveIndex));
				
				
				CPen mypen(PS_SOLID,1,pWaveData->m_LineColor);
				pOldPen = pDC->SelectObject (&mypen);
				
				//nDrawPixelWidth = m_WDW.rcDisplay.Width();
				//short* pDataBuffer = (short*)malloc(drawPixelCount * sizeof(short));
				m_pProduct->GetDataFromHDD(	pWaveData->m_nChnIndex,			//这里要改  现在固定取0通道数据
					m_pProduct->m_nPlayStartIndex,
					m_pProduct->m_nPlayWndWidth/nDrawPixelWidth,
					pDataBuffer,
					nDrawPixelWidth);
				/*更新MeasureDlg上的数据*/
				if (NULL != m_pMeasureDlg){
					m_pMeasureDlg->ComputeItemTime(pWaveData->m_nChnIndex , pDataBuffer , nDrawPixelWidth);
				}
				
				originX = m_WDW.rcDisplay.left;
				originY = m_WDW.rcDisplay.top + m_WDW.rcDisplay.Height()/2;
				nHalfHeight = m_WDW.rcDisplay.Height()/2;
				nCount=0;
				nRange = m_nWaveDataRange[nWaveIndex];

				
				for (nCount=0 ; nCount<nDrawPixelWidth-1 ; nCount++)
				{
					nFirstY = originY+ (*(pDataBuffer+nCount)) *nHalfHeight / (nRange*1000);
					nSecondY = originY+ (*(pDataBuffer+nCount+1)) *nHalfHeight / (nRange*1000);
					
					if ( (nFirstY<m_WDW.rcDisplay.bottom && nSecondY<m_WDW.rcDisplay.bottom) &&
						(nFirstY>m_WDW.rcDisplay.top && nSecondY>m_WDW.rcDisplay.top)  )
					{
						pDC->MoveTo(originX+nCount ,	 nFirstY );
						pDC->LineTo(originX+nCount+1 ,	 nSecondY );
					}
					else if ( nFirstY<=m_WDW.rcDisplay.bottom && nSecondY>=m_WDW.rcDisplay.bottom )
					{
						pDC->MoveTo(originX+nCount ,	 nFirstY );
						pDC->LineTo(originX+nCount+1 ,	 m_WDW.rcDisplay.bottom );
					}
					else if ( nFirstY>=m_WDW.rcDisplay.bottom && nSecondY<=m_WDW.rcDisplay.bottom )
					{
						pDC->MoveTo(originX+nCount ,	 m_WDW.rcDisplay.bottom );
						pDC->LineTo(originX+nCount+1 ,	 nSecondY );
					}
					else if ( nFirstY>=m_WDW.rcDisplay.top && nSecondY<=m_WDW.rcDisplay.top )
					{
						pDC->MoveTo(originX+nCount ,	 nFirstY );
						pDC->LineTo(originX+nCount+1 ,	 m_WDW.rcDisplay.top );
					}
					else if ( nFirstY<=m_WDW.rcDisplay.top && nSecondY>=m_WDW.rcDisplay.top )
					{
						pDC->MoveTo(originX+nCount ,	 m_WDW.rcDisplay.top );
						pDC->LineTo(originX+nCount+1 ,	 nSecondY );
					}
					
						
				}
				/*end 画一个wave*/
			}

		}

	}
	else{
		//频域
		pDataBuffer = (short*)malloc(m_pProduct->m_nPlayWndWidth * sizeof(short));		//缓存要做FFT的数据

		int nWaveIndex=0;
		for ( nWaveIndex=0 ; nWaveIndex<DRAW_CHNUM_MAX ; nWaveIndex++)
		{
			if ( m_nWaveDataNum >= nWaveIndex+1 )
			{
				/*start 画一个Wave*/
				pWaveData = m_WaveDataList.GetAt(m_WaveDataList.FindIndex(nWaveIndex));
				
				
				CPen mypen(PS_SOLID,1,pWaveData->m_LineColor);
				pOldPen = pDC->SelectObject (&mypen);
				
				/*
				m_pProduct->GetDataFromHDD(	pWaveData->m_nChnIndex,			
					m_pProduct->m_nPlayStartIndex,
					m_pProduct->m_nPlayWndWidth/nDrawPixelWidth,
					pDataBuffer,
					nDrawPixelWidth);
				*/
				m_pProduct->GetDataFromHDD(	pWaveData->m_nChnIndex,			
					m_pProduct->m_nPlayStartIndex,
					1,
					pDataBuffer,
					m_pProduct->m_nPlayWndWidth);

				double* pDoubleDataBuffer = (double*)malloc(m_pProduct->m_nPlayWndWidth * sizeof(double));
				int nXAxisLen = 0;
				int nYAxisLen = 0;
				int nFftResultLen = m_pProduct->m_FftConf.nFftBins;
				double* pXAxisArray = (double*)malloc( nFftResultLen * sizeof(double));
				
				for (nCount=0 ; nCount<m_pProduct->m_nPlayWndWidth ; nCount++)
				{
					pDoubleDataBuffer[nCount] = (double)(pDataBuffer[nCount] / 1000);
				}
				/*FFT运算*/
				m_pProduct->m_pFftModule->FftConf(m_pProduct->m_FftConf.nWindowMode , 1000000/*这个值好像没有毛用*/ , m_pProduct->m_FftConf.nFftBins , m_pProduct->m_FftConf.nOutputMode);
				m_pProduct->m_pFftModule->FftRun(pDoubleDataBuffer, 
												m_pProduct->m_nPlayWndWidth, 
												pXAxisArray, 
												&nXAxisLen,   
												m_pProduct->m_aFftResult[pWaveData->m_nChnIndex] , 
												&nYAxisLen);//fft

				free(pXAxisArray );
				free(pDoubleDataBuffer);

				/*更新MeasureDlg上的数据 这里应该放m_pProduct->m_aFftResult[pWaveData->m_nChnIndex]的数据*/
				if (NULL != m_pMeasureDlg){
					m_pMeasureDlg->ComputeItemFreq(pWaveData->m_nChnIndex , m_pProduct->m_aFftResult[pWaveData->m_nChnIndex] , nXAxisLen);
				}

				originX = m_WDW.rcDisplay.left;
				originY = m_WDW.rcDisplay.bottom;
				nHalfHeight = m_WDW.rcDisplay.Height()/2;
				nHeightPixel = m_WDW.rcDisplay.Height();
				nCount=0;

				nRange = 1;		//这里要改

				float fYAxisLen = 0.0;
				switch ( m_nWaveDataRange[nWaveIndex] )
				{
				case RANGE_FREQ_V_0_500:
					fYAxisLen = 0.5;
					break;
				case RANGE_FREQ_V_0_1000:
					fYAxisLen = 1.0;
					break;
				case RANGE_FREQ_DBV_n100_0:
					fYAxisLen = 100.0;
					break;
				case RANGE_FREQ_DBV_n100_100:
					fYAxisLen = 200.0;
					break;
				case RANGE_FREQ_DBu_0_100:
					fYAxisLen = 100.0;
					break;
				case RANGE_FREQ_DBu_0_200:
					fYAxisLen = 200.0;
					break;
				case RANGE_FREQ_DBm_n100_0:
					fYAxisLen = 100.0;
					break;
				case RANGE_FREQ_DBm_n100_100:
					fYAxisLen = 200.0;
					break;
				}


				int nDrawPixelCount = 0;
				nDrawPixelCount = min(nDrawPixelWidth , nXAxisLen);							
				
				for (nCount=0 ; nCount<nDrawPixelCount-1 ; nCount++)
				{
					switch ( m_nWaveDataRange[nWaveIndex] )
					{
					case RANGE_FREQ_V_0_500:
						nFirstY	= originY - (*(m_pProduct->m_aFftResult[pWaveData->m_nChnIndex]+(int)((nCount+0.0)*nXAxisLen/nDrawPixelCount))) *nHeightPixel / fYAxisLen;
						nSecondY= originY - (*(m_pProduct->m_aFftResult[pWaveData->m_nChnIndex]+(int)((nCount+1.0)*nXAxisLen/nDrawPixelCount))) *nHeightPixel / fYAxisLen;
						break;
					case RANGE_FREQ_V_0_1000:
						nFirstY	= originY - (*(m_pProduct->m_aFftResult[pWaveData->m_nChnIndex]+(int)((nCount+0.0)*nXAxisLen/nDrawPixelCount))) *nHeightPixel / fYAxisLen;
						nSecondY= originY - (*(m_pProduct->m_aFftResult[pWaveData->m_nChnIndex]+(int)((nCount+1.0)*nXAxisLen/nDrawPixelCount))) *nHeightPixel / fYAxisLen;
						break;
					case RANGE_FREQ_DBV_n100_0:
						nFirstY = originY - ((*(m_pProduct->m_aFftResult[pWaveData->m_nChnIndex]+(int)((nCount+0.0)*nXAxisLen/nDrawPixelCount)))+100.0) *nHeightPixel / fYAxisLen;
						nSecondY= originY - ((*(m_pProduct->m_aFftResult[pWaveData->m_nChnIndex]+(int)((nCount+1.0)*nXAxisLen/nDrawPixelCount)))+100.0) *nHeightPixel / fYAxisLen;
						break;
					case RANGE_FREQ_DBV_n100_100:
						nFirstY = originY - ((*(m_pProduct->m_aFftResult[pWaveData->m_nChnIndex]+(int)((nCount+0.0)*nXAxisLen/nDrawPixelCount)))+100.0) *nHeightPixel / fYAxisLen;
						nSecondY= originY - ((*(m_pProduct->m_aFftResult[pWaveData->m_nChnIndex]+(int)((nCount+1.0)*nXAxisLen/nDrawPixelCount)))+100.0) *nHeightPixel / fYAxisLen;
						break;
					case RANGE_FREQ_DBu_0_100:
						nFirstY	= originY - (*(m_pProduct->m_aFftResult[pWaveData->m_nChnIndex]+(int)((nCount+0.0)*nXAxisLen/nDrawPixelCount))) *nHeightPixel / fYAxisLen;
						nSecondY= originY - (*(m_pProduct->m_aFftResult[pWaveData->m_nChnIndex]+(int)((nCount+1.0)*nXAxisLen/nDrawPixelCount))) *nHeightPixel / fYAxisLen;
						break;
					case RANGE_FREQ_DBu_0_200:
						nFirstY	= originY - (*(m_pProduct->m_aFftResult[pWaveData->m_nChnIndex]+(int)((nCount+0.0)*nXAxisLen/nDrawPixelCount))) *nHeightPixel / fYAxisLen;
						nSecondY= originY - (*(m_pProduct->m_aFftResult[pWaveData->m_nChnIndex]+(int)((nCount+1.0)*nXAxisLen/nDrawPixelCount))) *nHeightPixel / fYAxisLen;
						break;
					case RANGE_FREQ_DBm_n100_0:
						nFirstY = originY - ((*(m_pProduct->m_aFftResult[pWaveData->m_nChnIndex]+(int)((nCount+0.0)*nXAxisLen/nDrawPixelCount)))+100.0) *nHeightPixel / fYAxisLen;
						nSecondY= originY - ((*(m_pProduct->m_aFftResult[pWaveData->m_nChnIndex]+(int)((nCount+1.0)*nXAxisLen/nDrawPixelCount)))+100.0) *nHeightPixel / fYAxisLen;
						break;
					case RANGE_FREQ_DBm_n100_100:
						nFirstY = originY - ((*(m_pProduct->m_aFftResult[pWaveData->m_nChnIndex]+(int)((nCount+0.0)*nXAxisLen/nDrawPixelCount)))+100.0) *nHeightPixel / fYAxisLen;
						nSecondY= originY - ((*(m_pProduct->m_aFftResult[pWaveData->m_nChnIndex]+(int)((nCount+1.0)*nXAxisLen/nDrawPixelCount)))+100.0) *nHeightPixel / fYAxisLen;
						break;
					}
				/*
					nFirstY = originY - (*(m_pProduct->m_aFftResult[pWaveData->m_nChnIndex]+nCount)) *nHeightPixel / fYAxisLen;
					nSecondY = originY - (*(m_pProduct->m_aFftResult[pWaveData->m_nChnIndex]+nCount+1)) *nHeightPixel / fYAxisLen;
				*/
					
					if ( (nFirstY<m_WDW.rcDisplay.bottom && nSecondY<m_WDW.rcDisplay.bottom) &&
						(nFirstY>m_WDW.rcDisplay.top && nSecondY>m_WDW.rcDisplay.top)  )
					{
						pDC->MoveTo(originX+(int)((nCount+0.0)*nDrawPixelWidth/nDrawPixelCount) ,	 nFirstY );
						pDC->LineTo(originX+(int)((nCount+1.0)*nDrawPixelWidth/nDrawPixelCount) ,	 nSecondY );
					}
					else if ( nFirstY<=m_WDW.rcDisplay.bottom && nSecondY>=m_WDW.rcDisplay.bottom )
					{
						pDC->MoveTo(originX+(int)((nCount+0.0)*nDrawPixelWidth/nDrawPixelCount) ,	 nFirstY );
						pDC->LineTo(originX+(int)((nCount+1.0)*nDrawPixelWidth/nDrawPixelCount) ,	 m_WDW.rcDisplay.bottom );
					}
					else if ( nFirstY>=m_WDW.rcDisplay.bottom && nSecondY<=m_WDW.rcDisplay.bottom )
					{
						pDC->MoveTo(originX+(int)((nCount+0.0)*nDrawPixelWidth/nDrawPixelCount) ,	 m_WDW.rcDisplay.bottom );
						pDC->LineTo(originX+(int)((nCount+1.0)*nDrawPixelWidth/nDrawPixelCount) ,	 nSecondY );
					}
					else if ( nFirstY>=m_WDW.rcDisplay.top && nSecondY<=m_WDW.rcDisplay.top )
					{
						pDC->MoveTo(originX+(int)((nCount+0.0)*nDrawPixelWidth/nDrawPixelCount) ,	 nFirstY );
						pDC->LineTo(originX+(int)((nCount+1.0)*nDrawPixelWidth/nDrawPixelCount) ,	 m_WDW.rcDisplay.top );
					}
					else if ( nFirstY<=m_WDW.rcDisplay.top && nSecondY>=m_WDW.rcDisplay.top )
					{
						pDC->MoveTo(originX+(int)((nCount+0.0)*nDrawPixelWidth/nDrawPixelCount) ,	 m_WDW.rcDisplay.top );
						pDC->LineTo(originX+(int)((nCount+1.0)*nDrawPixelWidth/nDrawPixelCount) ,	 nSecondY );
					}
					
					
				}
				/*end 画一个wave*/
			}
			
		}
	}
	
	if ( pDataBuffer != NULL )
	{
		free(pDataBuffer);
	}
	
//	pDataBuffer = NULL;

	
	pDC->SelectObject(pOldPen);
}	

void CWatchDlg::SetWatchDlgFrame(CRect rc)
{
	//复制外围框架矩形大小
	m_rcWatchDlgFrame = rc;
	//计算内部波形显示框架大小WatchDlgFrame
	int WatchDlgWidth = rc.Width();
	int WatchDlgHeight = rc.Height();
	m_WDW.rcDisplay.left = WatchDlgWidth * 1 / 10;
	m_WDW.rcDisplay.top = WatchDlgHeight * 2 / 10;
	m_WDW.rcDisplay.right = WatchDlgWidth * 19 / 20;
	m_WDW.rcDisplay.bottom = WatchDlgHeight * 9 / 10;

	m_WDW.rcReal = m_WDW.rcDisplay;
	m_WDW.fCurRatio = 1.0;
	m_WDW.fCurRatioX = 1.0;
	m_WDW.fCurRatioY = 1.0;
}

void CWatchDlg::DrawBG(CDC *pCdc)
{
	CString strTest;
//////////////////////////////////////////////////////////////////////////
	/*画网格*/
	CPen* pOldPen;
	//CPen mypen(PS_DOT ,1,RGB(173,216,230));
	CPen mypen(PS_DOT ,1,RGB(50,50,50));
	pOldPen = pCdc->SelectObject (&mypen);
	int i,j;
	CString str;
	for (i=0 ; i<10 ; i++)
	{	//竖线
		pCdc->MoveTo(m_WDW.rcDisplay.left + i*m_WDW.rcDisplay.Width()/10 , m_WDW.rcDisplay.top );
		pCdc->LineTo(m_WDW.rcDisplay.left + i*m_WDW.rcDisplay.Width()/10 , m_WDW.rcDisplay.bottom );
	}
	pCdc->MoveTo(m_WDW.rcDisplay.right , m_WDW.rcDisplay.top );
	pCdc->LineTo(m_WDW.rcDisplay.right , m_WDW.rcDisplay.bottom );
	for (j=0 ; j<10 ; j++)
	{	//横线
		pCdc->MoveTo(m_WDW.rcDisplay.left , m_WDW.rcDisplay.top + j*m_WDW.rcDisplay.Height()/10 );
		pCdc->LineTo(m_WDW.rcDisplay.right , m_WDW.rcDisplay.top +  j*m_WDW.rcDisplay.Height()/10 );
	}
	pCdc->MoveTo(m_WDW.rcDisplay.left , m_WDW.rcDisplay.bottom);
	pCdc->LineTo(m_WDW.rcDisplay.right , m_WDW.rcDisplay.bottom);
	pCdc->SelectObject(pOldPen);
//////////////////////////////////////////////////////////////////////////
	/*画坐标*/

	/*纵坐标*/
	if ( m_nMode == DISPLAY_MODE_TIME)
	{
		//画时域的Y轴坐标
		DrawYCoordinateTime(pCdc);
	}
	else{
		//画频域的Y轴坐标
		DrawYCoordinateFreq(pCdc);
	}

	/*横坐标*/
	if ( m_nMode == DISPLAY_MODE_TIME)
	{
		//画时域的Y轴坐标
		DrawXCoordinateTime(pCdc);
	}
	else{
		//画频域的Y轴坐标
		DrawXCoordinateFreq(pCdc);
	}
		
	
	

}

void CWatchDlg::DrawTitle(CDC *pDC)
{
	CBrush mybrush(RGB(239,235,224));
//	CBrush* pOldBrush  = pDC->SelectObject(&mybrush);
	pDC->FillRect(CRect(0,0,m_rcWatchDlgFrame.Width(),20),&mybrush);
	CString str;
	str.Format("示波器 %d",1);
	//pDC->TextOut(0,0,str);
	GetDlgItem(IDC_STATIC_Title)->MoveWindow(CRect(2,2,m_rcWatchDlgFrame.Width()-4,22));
	GetDlgItem(IDC_STATIC_Title)->SetWindowText(str);

}

HBRUSH CWatchDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	if( GetDlgItem(IDC_STATIC_Title)->m_hWnd == pWnd->m_hWnd ) //IDC_STATICM为你要改变颜色的Static控件资源ID
	{
		pDC->SetTextColor(RGB(0, 0, 0));
		pDC->SetBkColor(RGB(239,235,224));
		//return m_hBlueBrush;
	}
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

BOOL CWatchDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message == WM_KEYDOWN) 
	{  
		switch(pMsg->wParam) 
		{  
			case VK_RETURN: //回车  
				return true;  
			case VK_ESCAPE: //ESC  
				return true;  	
		}  
	}  
	return CDialog::PreTranslateMessage(pMsg);
}

void CWatchDlg::OnOk()//啥也不做就OK了  
{  
	
}  

void CWatchDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	//首先将该WatchDlg所属的BgDlg变为选中
	SetOwnedBgDlgSeleted();

//	TRACE("======click at: %d , %d ================================================\n",point.x,point.y);
//	TRACE("======m_YMF.rcPosMarkY_1 at: %d , %d , %d , %d =========================\n",m_YMF.rcPosMarkY_1.left,m_YMF.rcPosMarkY_1.top,m_YMF.rcPosMarkY_1.right,m_YMF.rcPosMarkY_1.bottom);

//	m_bMarkIsPick = true;
	m_ptMouseMove = point;
	//====================点中某个MARK=============================================================
	if (m_YMF[0].rcPosMarkY_1.PtInRect(point))
	{
		m_bMarkIsPick = true;
		m_nMarkMode = MARK_MODE_LEFTONE_1;
		m_YMF[0].nDisPickToTopMarkY_1 = point.y - m_YMF[0].rcPosMarkY_1.top;
		SetCapture();
	} 
	else if (m_YMF[0].rcPosMarkY_2.PtInRect(point))
	{
		m_bMarkIsPick = true;
		m_nMarkMode = MARK_MODE_LEFTONE_2;
		m_YMF[0].nDisPickToTopMarkY_2 = point.y - m_YMF[0].rcPosMarkY_2.top;
		SetCapture();
	} 
	else if (m_YMF[1].rcPosMarkY_1.PtInRect(point))
	{
		m_bMarkIsPick = true;
		m_nMarkMode = MARK_MODE_RIGHTONE_1;
		m_YMF[1].nDisPickToTopMarkY_1 = point.y - m_YMF[1].rcPosMarkY_1.top;
		SetCapture();
	} 
	else if (m_YMF[1].rcPosMarkY_2.PtInRect(point))
	{
		m_bMarkIsPick = true;
		m_nMarkMode = MARK_MODE_RIGHTONE_2;
		m_YMF[1].nDisPickToTopMarkY_2 = point.y - m_YMF[1].rcPosMarkY_2.top;
		SetCapture();
	} 
	else if (m_YMF[2].rcPosMarkY_1.PtInRect(point))
	{
		m_bMarkIsPick = true;
		m_nMarkMode = MARK_MODE_LEFTTWO_1;
		m_YMF[2].nDisPickToTopMarkY_1 = point.y - m_YMF[2].rcPosMarkY_1.top;
		SetCapture();
	} 
	else if (m_YMF[2].rcPosMarkY_2.PtInRect(point))
	{
		m_bMarkIsPick = true;
		m_nMarkMode = MARK_MODE_LEFTTWO_2;
		m_YMF[2].nDisPickToTopMarkY_2 = point.y - m_YMF[2].rcPosMarkY_2.top;
		SetCapture();
	} 
	else if (m_YMF[3].rcPosMarkY_1.PtInRect(point))
	{
		m_bMarkIsPick = true;
		m_nMarkMode = MARK_MODE_RIGHTTWO_1;
		m_YMF[3].nDisPickToTopMarkY_1 = point.y - m_YMF[3].rcPosMarkY_1.top;
		SetCapture();
	} 
	else if (m_YMF[3].rcPosMarkY_2.PtInRect(point))
	{
		m_bMarkIsPick = true;
		m_nMarkMode = MARK_MODE_RIGHTTWO_2;
		m_YMF[3].nDisPickToTopMarkY_2 = point.y - m_YMF[3].rcPosMarkY_2.top;
		SetCapture();
	} 
	else if (m_XMF.rcPosMarkX_1.PtInRect(point))
	{
		m_bMarkIsPick = true;
		m_nMarkMode = MARK_MODE_MARKX_1;		
		m_XMF.nDisPickToLeftMarkX_1 = point.x - m_XMF.rcPosMarkX_1.left;
		SetCapture();
	} 
	else if (m_XMF.rcPosMarkX_2.PtInRect(point))
	{
		m_bMarkIsPick = true;
		m_nMarkMode = MARK_MODE_MARKX_2;
		m_XMF.nDisPickToLeftMarkX_2 = point.x - m_XMF.rcPosMarkX_2.left;
		SetCapture();
	} 
	else if (m_XMF.rcPosScale_1.PtInRect(point))
	{
		m_bMarkIsPick = true;
		m_nMarkMode = MARK_MODE_SCALE_1;
		m_XMF.nDisPickToLeftScale_1 = point.x - m_XMF.rcPosScale_1.left;
		SetCapture();
	} 
	else if (m_XMF.rcPosScale_2.PtInRect(point))
	{
		m_bMarkIsPick = true;
		m_nMarkMode = MARK_MODE_SCALE_2;
		m_XMF.nDisPickToLeftScale_2 = point.x - m_XMF.rcPosScale_2.left;
		SetCapture();
	} 
	else if (m_WDW.rcDisplay.PtInRect(point))
	{

			
		//====================处理放大缩小功能=============================================================
			int MouseMode =((CMainFrame*)AfxGetMainWnd())->m_nMouseMode;
/*
			if (((CRecorderView*)GetParent())->m_WndManage.m_nCurSelect == m_nIndexInWaveList)
			{
*/
			/*return;*/
			//根据当前的鼠标选择进行操作
			if (MouseMode == MOUSEMODE_ZOOMIN)
			{
				//放大 功能
				CString str;
				str.Format("点击当前位置为%d,%d\n",point.x,point.y);
				TRACE(str);
				
				ZoomInModifyEveryWndRc(&point);
				//((CRecorderView*)GetParent())->WndManageRefresh();
			}
			else if (MouseMode == MOUSEMODE_ZOOMOUT)
			{
/*
				if (m_WDW.fCurRatioX == 1.0 && m_WDW.fCurRatioY == 1.0  )
				{
					return;
				}
*/
				ZoomOutModifyEveryWndRc(&point);
				//((CRecorderView*)GetParent())->WndManageRefresh();
			}
			else if (MouseMode == MOUSEMODE_ZOOMDRAG)
			{
				m_bZoomDragIsPick = true;
				m_ptZoomDragStart = point;
				SetCapture();
			}
			else if (MouseMode == MOUSEMODE_RESTORE)
			{
				ZoomRestoreModifyEveryWndRc(&point);
			}
			else if (MouseMode == MOUSEMODE_NONE)
			{
			}

	}

	CDialog::OnLButtonDown(nFlags, point);
}

BOOL CWatchDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
//	CView *pView= (CView *)GetParent();
	
	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CWatchDlg::DrawSelectedBorder(CDC *pCdc)		//这个函数已不再调用  绘制选中边框的代码放在了CBgDlg的OnPaint代码中
{
	CBrush *pBrush = CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
	CBrush *pOldBrushp = pCdc->SelectObject(pBrush);
	if (((CRecorderView*)GetParent())->m_WndManage.m_nCurSelect == m_nIndexInWaveList)
	{
		CRect rc;
		GetClientRect(&rc);
		CBrush mybrush(RGB(239,235,224));
		//	CBrush* pOldBrush  = pDC->SelectObject(&mybrush);
		/*rc.top+=23;*/
		pCdc->Rectangle(&rc);
	}
	pCdc->SelectObject(pOldBrushp);
}

void CWatchDlg::RefreshParaDlgBar()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();

	CRecorderDoc *pDoc = (CRecorderDoc*)(pFrame->GetActiveDocument());
	CProduct* pProduct = &(pDoc->m_Product);
	//采样率
	switch(pProduct->m_AcqInfo.samplingRate){
	case  100000000:
		pFrame->m_pDlgBarEx->m_ComboRate.SetCurSel(0);
		break;
	case  50000000:
		pFrame->m_pDlgBarEx->m_ComboRate.SetCurSel(1);
		break;
	case  10000000:
		pFrame->m_pDlgBarEx->m_ComboRate.SetCurSel(2);
		break;
	case  1000000:
		pFrame->m_pDlgBarEx->m_ComboRate.SetCurSel(3);
		break;
	case  100000:
		pFrame->m_pDlgBarEx->m_ComboRate.SetCurSel(4);
		break;
	case  10000:
		pFrame->m_pDlgBarEx->m_ComboRate.SetCurSel(5);
		break;
	default:
		break;
	}
	//采样长度
	switch(pProduct->m_AcqInfo.samplingLength){
	case  67108864:
		pFrame->m_pDlgBarEx->m_ComboLength.SetCurSel(0);
		break;
	case  33554432:
		pFrame->m_pDlgBarEx->m_ComboLength.SetCurSel(1);
		break;
	case  16777216:
		pFrame->m_pDlgBarEx->m_ComboLength.SetCurSel(2);
		break;
	case  4194304:
		pFrame->m_pDlgBarEx->m_ComboLength.SetCurSel(3);
		break;
	case  1048576:
		pFrame->m_pDlgBarEx->m_ComboLength.SetCurSel(4);
		break;
	case  262144:
		pFrame->m_pDlgBarEx->m_ComboLength.SetCurSel(5);
		break;
	case  65536:
		pFrame->m_pDlgBarEx->m_ComboLength.SetCurSel(6);
		break;
	case  16384:
		pFrame->m_pDlgBarEx->m_ComboLength.SetCurSel(7);
		break;
	default:
		break;
	}
	//量程
	switch(pProduct->m_AcqInfo.range){
	case  20:
		pFrame->m_pDlgBarEx->m_ComboRange.SetCurSel(0);
		break;
	case  10:
		pFrame->m_pDlgBarEx->m_ComboRange.SetCurSel(1);
		break;
	case  5:
		pFrame->m_pDlgBarEx->m_ComboRange.SetCurSel(2);
		break;
	case  1:
		pFrame->m_pDlgBarEx->m_ComboRange.SetCurSel(3);
		break;
	default:
		break;
	}



}


void CWatchDlg::ZoomInModifyEveryWndRc(CPoint *point)
{
	CPoint ptTemp;
	CPoint* pt = &ptTemp;
	pt->x = point->x;
	pt->y = point->y;

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CRecorderView* pView= (CRecorderView*)(pFrame->GetActiveView());
	CRecorderDoc *pDoc = (CRecorderDoc*)(pFrame->GetActiveDocument());
	CProduct* pProduct = &(pDoc->m_Product);
	CWndManage* pWndMange = &(pView->m_WndManage);	
	WavePlayCtrl* pWPC = &(pWndMange->m_WPCUnified);

	if (pWPC->fRatioX == 1.0)
	{
		//放大倍数已经最大
		return;
	}


	//step1 进行点击的点的坐标修正
	if (pt->x<m_WDW.rcDisplay.left || pt->x>m_WDW.rcDisplay.right || pt->y<m_WDW.rcDisplay.top || pt->y>m_WDW.rcDisplay.bottom )
	{	//点击在rcDisplay之外
		return;
	} 
	else 
	{	//点击在rcDisplay之中
		m_WDW.ptClickCorrd = *pt;
		if (pt->x < m_WDW.rcDisplay.left + m_WDW.rcDisplay.Width()/4)
		{
		//	m_WDW.ptClickCorrd.x = m_WDW.rcDisplay.left + m_WDW.rcDisplay.Width()/4;
			pt->x =  m_WDW.rcDisplay.left + m_WDW.rcDisplay.Width()/4;
		}
		if (pt->x > m_WDW.rcDisplay.left + m_WDW.rcDisplay.Width()/4*3)
		{
		//	m_WDW.ptClickCorrd.x = m_WDW.rcDisplay.left + m_WDW.rcDisplay.Width()/4*3;
			pt->x = m_WDW.rcDisplay.left + m_WDW.rcDisplay.Width()/4*3;
		}
	}
	

	pWPC->fRatioX = pWPC->fRatioX / 2;
	int nOldPlayWndWidth = pProduct->m_nPlayWndWidth;					//记录旧的m_nPlayWndWidth 

	if ( pWPC->fRatioX < 1.0){
		pProduct->m_nPlayWndWidth = m_WDW.rcDisplay.Width();			//新的m_nPlayWndWidth 就是一个显示屏幕长度
		
		if ( (pt->x - m_WDW.rcDisplay.left)* pWPC->fRatioX * 2 < m_WDW.rcDisplay.Width() /2 )
		{
			//pProduct->m_nPlayStartIndex 不变
		}
		else if ( (m_WDW.rcDisplay.right- pt->x)* pWPC->fRatioX * 2 < m_WDW.rcDisplay.Width() /2 )
		{
			pProduct->m_nPlayStartIndex = pProduct->m_nPlayStartIndex + nOldPlayWndWidth - m_WDW.rcDisplay.Width();
		}
		else{
			pProduct->m_nPlayStartIndex = pProduct->m_nPlayStartIndex + (pt->x - m_WDW.rcDisplay.left)* pWPC->fRatioX * 2 - m_WDW.rcDisplay.Width() /2;
		}
		pWPC->fRatioX = 1.0;
		
	}
	else{
		pProduct->m_nPlayStartIndex = pProduct->m_nPlayStartIndex + (pt->x - m_WDW.rcDisplay.Width()/4 - m_WDW.rcDisplay.left)* nOldPlayWndWidth / m_WDW.rcDisplay.Width();
		pProduct->m_nPlayWndWidth = pProduct->m_nPlayWndWidth / 2;
	}

/*
	if ( pWPC->fRatioX < 1.0){
		pProduct->m_nPlayWndWidth = m_WDW.rcDisplay.Width();			//新的m_nPlayWndWidth 就是一个显示屏幕长度

		if ( pt->x - (pProduct->m_nPlayWndWidth / 4) < m_WDW.rcDisplay.left )
		{
			//不变
		//	pProduct->m_nPlayStartIndex = pProduct->m_nPlayStartIndex ;		
		}
		else if ( pt->x + (pProduct->m_nPlayWndWidth / 4) > m_WDW.rcDisplay.right )
		{
			pProduct->m_nPlayStartIndex = pProduct->m_nPlayStartIndex + nOldPlayWndWidth - pProduct->m_nPlayWndWidth ;	
		}
		else{
			pProduct->m_nPlayStartIndex = pProduct->m_nPlayStartIndex + ( ( pt->x - m_WDW.rcDisplay.left )* nOldPlayWndWidth / m_WDW.rcDisplay.Width() - pProduct->m_nPlayWndWidth / 2 ) ;	
		}

		pWPC->fRatioX = 1.0;
		
	}
	else{
		pProduct->m_nPlayStartIndex = pProduct->m_nPlayStartIndex + (pt->x - m_WDW.rcDisplay.Width()/4 - m_WDW.rcDisplay.left)* nOldPlayWndWidth / m_WDW.rcDisplay.Width();
		pProduct->m_nPlayWndWidth = pProduct->m_nPlayWndWidth / 2;
	}
*/

	pFrame->m_pDlgBarExBottomPlay->m_pPlayBar->SetPlayPos(pProduct->m_nPlayStartIndex);
	pFrame->m_pDlgBarExBottomPlay->m_pPlayBar->SetPlayWndWidth(pProduct->m_nPlayWndWidth);

	pView->m_WndManage.AllWndRefresh_Complete();

}

void CWatchDlg::ZoomOutModifyEveryWndRc(CPoint *pt)
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CRecorderView* pView= (CRecorderView*)(pFrame->GetActiveView());
	CRecorderDoc *pDoc = (CRecorderDoc*)(pFrame->GetActiveDocument());
	CProduct* pProduct = &(pDoc->m_Product);
	CWndManage* pWndMange = &(pView->m_WndManage);	
	WavePlayCtrl* pWPC = &(pWndMange->m_WPCUnified);

	pWPC->fRatioX = pWPC->fRatioX * 2;
	int nOldPlayWndWidth = pProduct->m_nPlayWndWidth;
	pProduct->m_nPlayWndWidth = pProduct->m_nPlayWndWidth * 2;

	if ( pProduct->m_nPlayWndWidth >= pProduct->m_AcqInfo.samplingLength )
	{
		//缩小后的PlayWnd过大
		pProduct->m_nPlayStartIndex = 0;
		pProduct->m_nPlayWndWidth = pProduct->m_AcqInfo.samplingLength;
		pWPC->fRatioX = pProduct->m_nPlayWndWidth / m_WDW.rcDisplay.Width();

	}
	else if (	pProduct->m_nPlayWndWidth >= pProduct->m_nSegmentValidIndex * DATASTORAGE_SEGMENT	)
	{
		//缩小后的PlayWnd过大
		pProduct->m_nPlayStartIndex = 0;
		pProduct->m_nPlayWndWidth = pProduct->m_nSegmentValidIndex * DATASTORAGE_SEGMENT;
		pWPC->fRatioX = pProduct->m_nPlayWndWidth / m_WDW.rcDisplay.Width();
	}
	else{
		if ( pProduct->m_nPlayStartIndex + pProduct->m_nPlayWndWidth >= pProduct->m_AcqInfo.samplingLength )
		{
			//缩小后的PlayWnd过大
			pProduct->m_nPlayStartIndex = pProduct->m_AcqInfo.samplingLength - pProduct->m_nPlayWndWidth;
			//pProduct->m_nPlayWndWidth = pProduct->m_AcqInfo.samplingLength;
			//pWPC->fRatioX = pProduct->m_nPlayWndWidth / m_WDW.rcDisplay.Width();
		}
		else if ( pProduct->m_nPlayStartIndex + pProduct->m_nPlayWndWidth > pProduct->m_nSegmentValidIndex * DATASTORAGE_SEGMENT)
		{
			
			pProduct->m_nPlayStartIndex = pProduct->m_nSegmentValidIndex * DATASTORAGE_SEGMENT - pProduct->m_nPlayWndWidth;
			//pProduct->m_nPlayWndWidth = pProduct->m_nSegmentValidIndex * DATASTORAGE_SEGMENT;
			//pWPC->fRatioX = pProduct->m_nPlayWndWidth / m_WDW.rcDisplay.Width();
		}
		else{
			int nPointClickToLeft = (pt->x - m_WDW.rcDisplay.left) * nOldPlayWndWidth / m_WDW.rcDisplay.Width();
			if ( pProduct->m_nPlayStartIndex+ nPointClickToLeft - nOldPlayWndWidth <= 0)
			{
				pProduct->m_nPlayStartIndex = 0;			
			}
/*
			else if ( pProduct->m_nPlayStartIndex+ nPointClickToLeft - nOldPlayWndWidth > pProduct->m_nSegmentValidIndex * DATASTORAGE_SEGMENT)
			{
				pProduct->m_nPlayStartIndex = pProduct->m_nSegmentValidIndex * DATASTORAGE_SEGMENT - pProduct->m_nPlayWndWidth;
			}
*/
			else{
				pProduct->m_nPlayStartIndex = pProduct->m_nPlayStartIndex + nPointClickToLeft - nOldPlayWndWidth;
			}

		}
	}




	
	pFrame->m_pDlgBarExBottomPlay->m_pPlayBar->SetPlayPos(pProduct->m_nPlayStartIndex);
	pFrame->m_pDlgBarExBottomPlay->m_pPlayBar->SetPlayWndWidth(pProduct->m_nPlayWndWidth);
	
	pView->m_WndManage.AllWndRefresh_Complete();


/*
	//step1 先给个rcReal一个初值
//	m_WDW.fCurRatio = m_WDW.fCurRatio / 2;
	m_WDW.fCurRatioX = m_WDW.fCurRatioX / 2;
	m_WDW.fCurRatioY = m_WDW.fCurRatioY / 2;
	if (m_WDW.fCurRatioX < 1.0)
	{
		m_WDW.fCurRatioX = 1.0;
	}
	if (m_WDW.fCurRatioY < 1.0)
	{
		m_WDW.fCurRatioY = 1.0;
	}

	int rcReal_left = m_WDW.rcReal.left;
	int rcReal_top = m_WDW.rcReal.top;
	m_WDW.rcReal.left = pt->x - (pt->x - rcReal_left)/2 ;
	m_WDW.rcReal.right = m_WDW.rcReal.left + m_WDW.rcDisplay.Width()*m_WDW.fCurRatioX ;
	m_WDW.rcReal.top = pt->y - (pt->y - rcReal_top)/2 ;
	m_WDW.rcReal.bottom = m_WDW.rcReal.top + m_WDW.rcDisplay.Height()*m_WDW.fCurRatioY ;
	
	//step2 判断rcReal是否越界 越界进行修改
	if (m_WDW.rcReal.left > m_WDW.rcDisplay.left)
	{
		m_WDW.rcReal.left = m_WDW.rcDisplay.left;
		m_WDW.rcReal.right = m_WDW.rcReal.left + m_WDW.rcDisplay.Width()*m_WDW.fCurRatioX ;
	}
	if (m_WDW.rcReal.right < m_WDW.rcDisplay.right)
	{
		m_WDW.rcReal.right = m_WDW.rcDisplay.right;
		m_WDW.rcReal.left = m_WDW.rcReal.right - m_WDW.rcDisplay.Width()*m_WDW.fCurRatioX ;
	}
	if (m_WDW.rcReal.top > m_WDW.rcDisplay.top)
	{
		m_WDW.rcReal.top = m_WDW.rcDisplay.top;
		m_WDW.rcReal.bottom = m_WDW.rcReal.top + m_WDW.rcDisplay.Height()*m_WDW.fCurRatioY ;
	}
	if (m_WDW.rcReal.bottom < m_WDW.rcDisplay.bottom)
	{
		m_WDW.rcReal.bottom = m_WDW.rcDisplay.bottom;
		m_WDW.rcReal.top = m_WDW.rcReal.bottom - m_WDW.rcDisplay.Height()*m_WDW.fCurRatioY ;
	}


	//呼出ZoomDlg
	if (m_pZoomDlg == NULL)
	{
		m_pZoomDlg = new CZoomDlg(this);
		m_pZoomDlg->Create(IDD_DIALOG_Zoom,this);
		m_pZoomDlg->MoveWindow(0,0,Zoom_DEFAULT_WIDTH,Zoom_DEFAULT_HEIGHT);
		//	m_pZoomDlg->ShowWindow(SW_SHOW);
	}

	float fRatioStartX	= (float)(m_WDW.rcDisplay.left - m_WDW.rcReal.left) / (float)(m_WDW.rcReal.Width());
	float fRatioStartY	= (float)(m_WDW.rcDisplay.top - m_WDW.rcReal.top) / (float)(m_WDW.rcReal.Height());
	float fRatioEndX	= (float)(m_WDW.rcDisplay.right - m_WDW.rcReal.left) / (float)(m_WDW.rcReal.Width());
	float fRatioEndY	= (float)(m_WDW.rcDisplay.bottom - m_WDW.rcReal.top) / (float)(m_WDW.rcReal.Height());

	m_pZoomDlg->ModifyZoomRectRatio( fRatioStartX, fRatioStartY, fRatioEndX, fRatioEndY);
	if (m_WDW.fCurRatioX == 1.0 && m_WDW.fCurRatioX == 1.0)
	{
		m_pZoomDlg->ShowWindow(SW_HIDE);
	} 
	else
	{
		m_pZoomDlg->ShowWindow(SW_SHOW);
	}
*/


}


void CWatchDlg::ZoomInModifyEveryWndRc2(CPoint *pt)
{
	//step1 进行点击的点的坐标修正
	if (pt->x<m_WDW.rcDisplay.left || pt->x>m_WDW.rcDisplay.right || pt->y<m_WDW.rcDisplay.top || pt->y>m_WDW.rcDisplay.bottom )
	{	//点击在rcDisplay之外
		return;
	} 
	else 
	{	//点击在rcDisplay之中
		m_WDW.ptClickCorrd = *pt;
		if (pt->x < m_WDW.rcDisplay.left + m_WDW.rcDisplay.Width()/4)
		{
			m_WDW.ptClickCorrd.x = m_WDW.rcDisplay.left + m_WDW.rcDisplay.Width()/4;
		}
		if (pt->x > m_WDW.rcDisplay.left + m_WDW.rcDisplay.Width()/4*3)
		{
			m_WDW.ptClickCorrd.x = m_WDW.rcDisplay.left + m_WDW.rcDisplay.Width()/4*3;
		}
		if (pt->y < m_WDW.rcDisplay.top + m_WDW.rcDisplay.Height()/4)
		{
			m_WDW.ptClickCorrd.y = m_WDW.rcDisplay.top + m_WDW.rcDisplay.Height()/4;
		}
		if (pt->y > m_WDW.rcDisplay.top + m_WDW.rcDisplay.Height()/4*3)
		{
			m_WDW.ptClickCorrd.y = m_WDW.rcDisplay.top + m_WDW.rcDisplay.Height()/4*3;
		}
	}
	
	//step2 更新rcReal
//	TRACE("1=====rcReal: %d %d %d %d =============\n",m_WDW.rcReal.left,m_WDW.rcReal.top,m_WDW.rcReal.right,m_WDW.rcReal.bottom);
//	TRACE("1=====rcDisplay: %d %d %d %d =============\n",m_WDW.rcDisplay.left,m_WDW.rcDisplay.top,m_WDW.rcDisplay.right,m_WDW.rcDisplay.bottom);
	int rcReal_left = m_WDW.rcReal.left;
	int rcReal_top = m_WDW.rcReal.top;
	int rcPhy_width = m_pProduct->m_AcqInfo.samplingLength;
	int rcPhy_height = m_pProduct->m_AcqInfo.range * 2 * 1000;
	//(m_WDW.ptClickCorrd.x - rcReal_left)		计算出点击的点相对于外围大矩形rcReal的坐标
	//(m_WDW.ptClickCorrd.x - rcReal_left)*2	将放大的点放在正中间
	m_WDW.rcReal.left =		-((m_WDW.ptClickCorrd.x - rcReal_left)*2 - m_WDW.rcDisplay.Width()/2 - m_WDW.rcDisplay.left);
	m_WDW.rcReal.top =	-((m_WDW.ptClickCorrd.y - rcReal_top)*2 - m_WDW.rcDisplay.Height()/2 - m_WDW.rcDisplay.top);
//	m_WDW.fCurRatio = m_WDW.fCurRatio * 2;
	m_WDW.fCurRatioX = m_WDW.fCurRatioX * 2;
	m_WDW.fCurRatioY = m_WDW.fCurRatioY * 2;
	if (m_WDW.rcDisplay.Width()*m_WDW.fCurRatioX > rcPhy_width)
	{
		m_WDW.fCurRatioX = rcPhy_width / m_WDW.rcDisplay.Width();
	}
	if (m_WDW.rcDisplay.Height()*m_WDW.fCurRatioY > rcPhy_height)
	{
		m_WDW.fCurRatioY = rcPhy_height / m_WDW.rcDisplay.Height();
	}
	m_WDW.rcReal.right = m_WDW.rcReal.left + m_WDW.rcDisplay.Width()*m_WDW.fCurRatioX;
	m_WDW.rcReal.bottom = m_WDW.rcReal.top + m_WDW.rcDisplay.Height()*m_WDW.fCurRatioY;
//	TRACE("2=====rcReal: %d %d %d %d =============\n",m_WDW.rcReal.left,m_WDW.rcReal.top,m_WDW.rcReal.right,m_WDW.rcReal.bottom);
//	TRACE("2=====rcDisplay: %d %d %d %d =============\n",m_WDW.rcDisplay.left,m_WDW.rcDisplay.top,m_WDW.rcDisplay.right,m_WDW.rcDisplay.bottom);
	
	//呼出ZoomDlg
	if (m_pZoomDlg == NULL)
	{
		m_pZoomDlg = new CZoomDlg(this);
		m_pZoomDlg->Create(IDD_DIALOG_Zoom,this);
		m_pZoomDlg->MoveWindow(0,0,Zoom_DEFAULT_WIDTH,Zoom_DEFAULT_HEIGHT);
		//	m_pZoomDlg->ShowWindow(SW_SHOW);
	}
//	m_bZoomDragIsPick = false;	
/*
	float fRatioStartX	= (float)(m_ptZoomDragStart.x - m_WDW.rcDisplay.left) / (float)(m_WDW.rcDisplay.Width());
	float fRatioStartY	= (float)(m_ptZoomDragStart.y - m_WDW.rcDisplay.top) / (float)(m_WDW.rcDisplay.Height());
	float fRatioEndX	= (float)(m_ptZoomDragEnd.x - m_WDW.rcDisplay.left) / (float)(m_WDW.rcDisplay.Width());
	float fRatioEndY	= (float)(m_ptZoomDragEnd.y - m_WDW.rcDisplay.top) / (float)(m_WDW.rcDisplay.Height());
*/	
	float fRatioStartX	= (float)(m_WDW.rcDisplay.left - m_WDW.rcReal.left) / (float)(m_WDW.rcReal.Width());
	float fRatioStartY	= (float)(m_WDW.rcDisplay.top - m_WDW.rcReal.top) / (float)(m_WDW.rcReal.Height());
	float fRatioEndX	= (float)(m_WDW.rcDisplay.right - m_WDW.rcReal.left) / (float)(m_WDW.rcReal.Width());
	float fRatioEndY	= (float)(m_WDW.rcDisplay.bottom - m_WDW.rcReal.top) / (float)(m_WDW.rcReal.Height());

	m_pZoomDlg->ModifyZoomRectRatio( fRatioStartX, fRatioStartY, fRatioEndX, fRatioEndY);
	m_pZoomDlg->ShowWindow(SW_SHOW);
}

void CWatchDlg::ZoomOutModifyEveryWndRc2(CPoint *pt)
{
	//step1 先给个rcReal一个初值
//	m_WDW.fCurRatio = m_WDW.fCurRatio / 2;
	m_WDW.fCurRatioX = m_WDW.fCurRatioX / 2;
	m_WDW.fCurRatioY = m_WDW.fCurRatioY / 2;
	if (m_WDW.fCurRatioX < 1.0)
	{
		m_WDW.fCurRatioX = 1.0;
	}
	if (m_WDW.fCurRatioY < 1.0)
	{
		m_WDW.fCurRatioY = 1.0;
	}

	int rcReal_left = m_WDW.rcReal.left;
	int rcReal_top = m_WDW.rcReal.top;
	m_WDW.rcReal.left = pt->x - (pt->x - rcReal_left)/2 ;
	m_WDW.rcReal.right = m_WDW.rcReal.left + m_WDW.rcDisplay.Width()*m_WDW.fCurRatioX ;
	m_WDW.rcReal.top = pt->y - (pt->y - rcReal_top)/2 ;
	m_WDW.rcReal.bottom = m_WDW.rcReal.top + m_WDW.rcDisplay.Height()*m_WDW.fCurRatioY ;
	
	//step2 判断rcReal是否越界 越界进行修改
	if (m_WDW.rcReal.left > m_WDW.rcDisplay.left)
	{
		m_WDW.rcReal.left = m_WDW.rcDisplay.left;
		m_WDW.rcReal.right = m_WDW.rcReal.left + m_WDW.rcDisplay.Width()*m_WDW.fCurRatioX ;
	}
	if (m_WDW.rcReal.right < m_WDW.rcDisplay.right)
	{
		m_WDW.rcReal.right = m_WDW.rcDisplay.right;
		m_WDW.rcReal.left = m_WDW.rcReal.right - m_WDW.rcDisplay.Width()*m_WDW.fCurRatioX ;
	}
	if (m_WDW.rcReal.top > m_WDW.rcDisplay.top)
	{
		m_WDW.rcReal.top = m_WDW.rcDisplay.top;
		m_WDW.rcReal.bottom = m_WDW.rcReal.top + m_WDW.rcDisplay.Height()*m_WDW.fCurRatioY ;
	}
	if (m_WDW.rcReal.bottom < m_WDW.rcDisplay.bottom)
	{
		m_WDW.rcReal.bottom = m_WDW.rcDisplay.bottom;
		m_WDW.rcReal.top = m_WDW.rcReal.bottom - m_WDW.rcDisplay.Height()*m_WDW.fCurRatioY ;
	}


	//呼出ZoomDlg
	if (m_pZoomDlg == NULL)
	{
		m_pZoomDlg = new CZoomDlg(this);
		m_pZoomDlg->Create(IDD_DIALOG_Zoom,this);
		m_pZoomDlg->MoveWindow(0,0,Zoom_DEFAULT_WIDTH,Zoom_DEFAULT_HEIGHT);
		//	m_pZoomDlg->ShowWindow(SW_SHOW);
	}

	float fRatioStartX	= (float)(m_WDW.rcDisplay.left - m_WDW.rcReal.left) / (float)(m_WDW.rcReal.Width());
	float fRatioStartY	= (float)(m_WDW.rcDisplay.top - m_WDW.rcReal.top) / (float)(m_WDW.rcReal.Height());
	float fRatioEndX	= (float)(m_WDW.rcDisplay.right - m_WDW.rcReal.left) / (float)(m_WDW.rcReal.Width());
	float fRatioEndY	= (float)(m_WDW.rcDisplay.bottom - m_WDW.rcReal.top) / (float)(m_WDW.rcReal.Height());

	m_pZoomDlg->ModifyZoomRectRatio( fRatioStartX, fRatioStartY, fRatioEndX, fRatioEndY);
	if (m_WDW.fCurRatioX == 1.0 && m_WDW.fCurRatioX == 1.0)
	{
		m_pZoomDlg->ShowWindow(SW_HIDE);
	} 
	else
	{
		m_pZoomDlg->ShowWindow(SW_SHOW);
	}


}



void CWatchDlg::DrawCursorRead(CDC *pDC)
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();

	CRecorderDoc *pDoc = (CRecorderDoc*)(pFrame->GetActiveDocument());
	CProduct* pProduct = &(pDoc->m_Product);

	if (pFrame->m_nMouseMode == MOUSEMODE_NONE)
	{
		//是光标读数模式下
		float xRead,yRead;
		xRead = (m_ptMouseMove.x +0.0 - m_WDW.rcReal.left)/m_WDW.rcReal.Width()*pProduct->m_AcqInfo.samplingTime;
		yRead = pProduct->m_AcqInfo.range - (m_ptMouseMove.y +0.0 - m_WDW.rcReal.top)/m_WDW.rcReal.Height()*pProduct->m_AcqInfo.range*2;
		CString str;
		str.Format("%.3f,%.3f",xRead,yRead);
		//pDC->TextOut(m_rcWatchDlgFrame.Width()/2,2,str);
		GetDlgItem(IDC_STATIC_CursorRead)->MoveWindow(m_rcWatchDlgFrame.Width()/2,22,m_rcWatchDlgFrame.Width()/2-4,20);
		GetDlgItem(IDC_STATIC_CursorRead)->SetWindowText(str);
	}

}

void CWatchDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	if ( m_bMarkIsPick)
	{
		//移动Mark滑块
		ModifyMarkPos(point);
		
	}

	if ( m_bZoomDragIsPick)
	{
		m_ptZoomDragEnd = point;
		if ( m_ptZoomDragEnd.x < m_WDW.rcDisplay.left)	{	m_ptZoomDragEnd.x = m_WDW.rcDisplay.left;	}
		if ( m_ptZoomDragEnd.x > m_WDW.rcDisplay.right)	{	m_ptZoomDragEnd.x = m_WDW.rcDisplay.right;	}
		if ( m_ptZoomDragEnd.y < m_WDW.rcDisplay.top)	{	m_ptZoomDragEnd.y = m_WDW.rcDisplay.top;	}
		if ( m_ptZoomDragEnd.y > m_WDW.rcDisplay.bottom){	m_ptZoomDragEnd.y = m_WDW.rcDisplay.bottom;	}

		InvalidateRect(m_WDW.rcDisplay);
	}
	m_ptMouseMove = point;

	ChangeCursor(point);

	if (point.x<m_WDW.rcDisplay.left || point.x>m_WDW.rcDisplay.right || point.y<m_WDW.rcDisplay.top || point.y>m_WDW.rcDisplay.bottom )
	{	//点击在rcDisplay之外
		return;
	} 

	CDialog::OnMouseMove(nFlags, point);
}

void CWatchDlg::AddWaveDataToWatchDlg(CWaveData *pWaveData)
{
	//首先检查该WatchDlg有无要添加的pWaveData
	bool existFlag = false;
	int count;
	for (count=0 ; count<m_nWaveDataNum ; count++)
	{
		if (pWaveData == (CWaveData*)m_WaveDataList.GetAt(m_WaveDataList.FindIndex(count)))
		{
			//存在重复WaveData
			existFlag = true;
			TRACE("WaveData数据重复");
			return;
		}
	}

	if (existFlag == false)
	{
		//不存在重复WaveData  将pWaveData加入m_WaveDataList
		m_WaveDataList.AddTail(pWaveData);
		m_nWaveDataRange[m_nWaveDataNum] = 10;							//新建的CWaveData量程设置为10V	m_nWaveDataNum基于0的，所以先设置，再++
		m_nWaveDataNum++;
	}

	
}

void CWatchDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	//对话框析构时发送WM_Destroy 因此DLG不要去添加析构函数 改destroy即可
}

void CWatchDlg::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
//	delete this;				//已经进行了在父窗口BgDlg中进行显式delete 这里就不delete了
	if (m_pMarkDlg != NULL)
	{
		delete m_pMarkDlg;
		m_pMarkDlg = NULL;
	}

	if (m_pZoomDlg != NULL)
	{
		delete m_pZoomDlg;
		m_pZoomDlg = NULL;
	}

	CDialog::PostNcDestroy();
}

BOOL CWatchDlg::ShowWindow(int nCmdShow)
{
	if ( NULL == m_pMeasureDlg){
	}
	else
	{
		m_pMeasureDlg->ShowWindow(nCmdShow);
	}

	return CDialog::ShowWindow(nCmdShow);
}

void CWatchDlg::ComputeFrame()
{
	//重新计算	m_WDW	 m_XF	 m_YFL	 m_YFR	 /*m_MF*/	 m_rcWatchDlgFrame		 m_rcMFinTab;
	//1. 先计算出WatchDlg和MeasureDlg的CRect: 
	//					m_rcWatchDlgFrame和m_rcMFinTab
	//2. 再在ComputeDisplayFrame()计算m_WDW	 m_XF	 m_YFL	 m_YFR

	if (NULL == m_pMeasureDlg)
	{
		//当前的CWatchDlg中没有MeasureDlg
		CRect rect;
		((CBgDlg*)GetParent())->GetClientRect(&rect);

		rect.top += 25;
		rect.bottom -= 2;
		rect.left += 1;
		rect.right -= 2;

		m_rcWatchDlgFrame = rect;

		//计算m_WDW	 m_XF	 m_YFL	 m_YFR
		ComputeDisplayFrame();


		rect.SetRect(0,0,0,0);
		m_rcMFinTab = rect;
	} 
	else
	{
		//当前的CWatchDlg包含了MeasureDlg
		//先计算MeasureDlg位置	m_rcMFinTab
		CRect rect;
		CRect rcMeasure;
//		((CBgDlg*)GetParent())->m_TabBgDlg.GetClientRect(&rect);
		GetParent()->GetClientRect(&rect);

		rcMeasure.bottom = rect.bottom-5;
		rcMeasure.left = rect.left+1;
		rcMeasure.right = rect.right-2;

		if (m_pMeasureDlg->m_bMinFlag == false)						//这里一行数据看做20px 标题栏也为20px
		{
			//并非最小化
			if (rcMeasure.right-rcMeasure.left < m_pMeasureDlg->m_nColWidthSum)
			{
				//超出宽度 需要HScroll
				rcMeasure.top = rcMeasure.bottom - MEA_TITLE_HEIGHT - MEA_ITEM_HEIGHT*m_pMeasureDlg->m_nMeasureNum - MEA_HSCROLL_HEIGHT;
				m_pMeasureDlg->AddHScroll(0, m_pMeasureDlg->m_nColWidthSum-(rcMeasure.right-rcMeasure.left) , &rcMeasure);
			//	m_pMeasureDlg->m_pHScroll->ShowWindow(SW_SHOW);
			} 
			else
			{
				//不需要HScroll
				rcMeasure.top = rcMeasure.bottom - MEA_TITLE_HEIGHT - MEA_ITEM_HEIGHT*m_pMeasureDlg->m_nMeasureNum;
				m_pMeasureDlg->DeleteHScroll();
			}
			
		}
		else{
			//处于最小化
			rcMeasure.top = rcMeasure.bottom - MEA_TITLE_HEIGHT;
			m_pMeasureDlg->DeleteHScroll();
		}
		m_rcMFinTab = rcMeasure;


		rect.top += 25;
		rect.bottom = rcMeasure.top-1;
		rect.left += 1;
		rect.right -= 2;
		
		m_rcWatchDlgFrame = rect;
		//计算m_WDW	 m_XF	 m_YFL	 m_YFR
		ComputeDisplayFrame();

	}

}

void CWatchDlg::ComputeDisplayFrame()
{
	CRect rcDisplay;
	rcDisplay.SetRect(0,0,m_rcWatchDlgFrame.Width(),m_rcWatchDlgFrame.Height());

	//根据该CWatchDlg中有几个CWaveData 来计算m_YFLeftOne，m_YFLeftTwo，m_YFRightOne，m_YFRightTwo，m_WDW的left，right
	switch (m_nWaveDataNum)
	{
	case 1:
		m_YF[0].rcFrame.left	= INTERVAL_Frame;
		m_YF[0].rcFrame.right	= INTERVAL_Frame + Y_COORDINATE_WIDTH ;
		m_YF[2].rcFrame.left	= 0 ;
		m_YF[2].rcFrame.right	= 0 ;
		m_YF[1].rcFrame.left	= 0 ;
		m_YF[1].rcFrame.right	= 0 ;
		m_YF[3].rcFrame.left	= 0 ;
		m_YF[3].rcFrame.right	= 0 ;
		m_WDW.rcDisplay.left	= INTERVAL_Frame + Y_COORDINATE_WIDTH ;
		m_WDW.rcDisplay.right	= rcDisplay.right - INTERVAL_Frame ;
		break;
	case 2:
		m_YF[0].rcFrame.left	= INTERVAL_Frame;
		m_YF[0].rcFrame.right	= INTERVAL_Frame + Y_COORDINATE_WIDTH ;
		m_YF[2].rcFrame.left	= 0 ;
		m_YF[2].rcFrame.right	= 0 ;
		m_YF[1].rcFrame.left	= rcDisplay.right - Y_COORDINATE_WIDTH - INTERVAL_Frame ;
		m_YF[1].rcFrame.right	= rcDisplay.right - INTERVAL_Frame ;
		m_YF[3].rcFrame.left	= 0 ;
		m_YF[3].rcFrame.right	= 0 ;		
		m_WDW.rcDisplay.left	= INTERVAL_Frame + Y_COORDINATE_WIDTH ;
		m_WDW.rcDisplay.right	= rcDisplay.right - Y_COORDINATE_WIDTH - INTERVAL_Frame ;
		break;
	case 3:
		m_YF[0].rcFrame.left	= INTERVAL_Frame;
		m_YF[0].rcFrame.right	= INTERVAL_Frame + Y_COORDINATE_WIDTH ;
		m_YF[2].rcFrame.left	= INTERVAL_Frame + Y_COORDINATE_WIDTH ;
		m_YF[2].rcFrame.right	= INTERVAL_Frame + Y_COORDINATE_WIDTH * 2 ;
		m_YF[1].rcFrame.left	= rcDisplay.right - Y_COORDINATE_WIDTH - INTERVAL_Frame ;
		m_YF[1].rcFrame.right	= rcDisplay.right - INTERVAL_Frame ;
		m_YF[3].rcFrame.left	= 0 ;
		m_YF[3].rcFrame.right	= 0 ;
		m_WDW.rcDisplay.left	= INTERVAL_Frame + Y_COORDINATE_WIDTH * 2 ;
		m_WDW.rcDisplay.right	= rcDisplay.right - Y_COORDINATE_WIDTH - INTERVAL_Frame ;
		break;
	case 4:
		m_YF[0].rcFrame.left	= INTERVAL_Frame;
		m_YF[0].rcFrame.right	= INTERVAL_Frame + Y_COORDINATE_WIDTH ;
		m_YF[2].rcFrame.left	= INTERVAL_Frame + Y_COORDINATE_WIDTH ;
		m_YF[2].rcFrame.right	= INTERVAL_Frame + Y_COORDINATE_WIDTH * 2 ;
		m_YF[1].rcFrame.left	= rcDisplay.right - Y_COORDINATE_WIDTH - INTERVAL_Frame ;
		m_YF[1].rcFrame.right	= rcDisplay.right - INTERVAL_Frame ;
		m_YF[3].rcFrame.left	= rcDisplay.right - Y_COORDINATE_WIDTH * 2 - INTERVAL_Frame ;
		m_YF[3].rcFrame.right	= rcDisplay.right - Y_COORDINATE_WIDTH - INTERVAL_Frame ;
		m_WDW.rcDisplay.left	= INTERVAL_Frame + Y_COORDINATE_WIDTH * 2 ;
		m_WDW.rcDisplay.right	= rcDisplay.right - Y_COORDINATE_WIDTH * 2 - INTERVAL_Frame ;
		break;
	default:
		break;
	}
	//计算m_YFLeftOne，m_YFLeftTwo，m_YFRightOne，m_YFRightTwo，m_WDW的top，bottom
	m_YF[0].rcFrame.top = INTERVAL_Frame;
	m_YF[0].rcFrame.bottom = rcDisplay.bottom - X_COORDINATE_HEIGHT;
	m_YF[2].rcFrame.top = INTERVAL_Frame;
	m_YF[2].rcFrame.bottom = rcDisplay.bottom - X_COORDINATE_HEIGHT;
	m_YF[1].rcFrame.top = INTERVAL_Frame;
	m_YF[1].rcFrame.bottom = rcDisplay.bottom - X_COORDINATE_HEIGHT;
	m_YF[3].rcFrame.top = INTERVAL_Frame;
	m_YF[3].rcFrame.bottom = rcDisplay.bottom - X_COORDINATE_HEIGHT;
	m_WDW.rcDisplay.top = INTERVAL_Frame;
	m_WDW.rcDisplay.bottom = rcDisplay.bottom - X_COORDINATE_HEIGHT;

	//计算m_XF
	m_XF.rcFrame.left = INTERVAL_Frame;
	m_XF.rcFrame.right = rcDisplay.right - INTERVAL_Frame;
	m_XF.rcFrame.top = rcDisplay.bottom - Y_COORDINATE_WIDTH;		//X_COORDINATE_HEIGHT = 30 有点多了
	m_XF.rcFrame.bottom = rcDisplay.bottom;

	m_WDW.rcReal = m_WDW.rcDisplay;

	//计算 m_XMF.rcXAxisMarkFrame	m_YMF.rcYAxisMarkFrame;
	ComputeMarkFrame();


}

void CWatchDlg::SetOwnedBgDlg(CBgDlg* pBgDlg)
{
	m_pOwnedBgDlg = pBgDlg;
}

void CWatchDlg::SetOwnedBgDlgSeleted()
{
	int nOldSeleted;
	nOldSeleted = m_pWndManage->m_nCurSelect;
	if (m_pOwnedBgDlg->m_nIndex == m_pWndManage->m_nCurSelect)
	{
		//不需要改变 WndManage已经选中当前的BgDlg
	} 
	else
	{
		m_pWndManage->m_nCurSelect = m_pOwnedBgDlg->m_nIndex;
		m_pOwnedBgDlg->Invalidate();
		m_pOwnedBgDlg->UpdateWindow();
		m_pWndManage->m_pBgDlg[nOldSeleted].Invalidate();
		m_pWndManage->m_pBgDlg[nOldSeleted].UpdateWindow();
		
	//	RefreshParaDlgBar();
		
		//更新显示量程的DlgBar和显示FFT/TIME的工具栏
		ChangeAllToolBars();

		

/*
		if (m_pMarkDlg != NULL)
		{
			m_pMarkDlg->Invalidate();
		}
*/
	}
}

void CWatchDlg::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default


	//首先将该WatchDlg所属的BgDlg变为选中
	SetOwnedBgDlgSeleted();
//==========================实现右键菜单==========================================================================
	CMenu menu,*pSubMenu;//定义下面要用到的cmenu对象  
    menu.LoadMenu(IDR_MENU_WatchDlgPopup);//装载自定义的右键菜单  
    pSubMenu=menu.GetSubMenu(0);//获取第一个弹出菜单，所以第一个菜单必须有子菜单  
    CPoint oPoint;//定义一个用于确定光标位置的位置  
    GetCursorPos(&oPoint);//获取当前光标的位置，以便使得菜单可以跟随光标  
    pSubMenu->TrackPopupMenu(TPM_LEFTALIGN,oPoint.x,oPoint.y,this);   //在指定位置显示弹出菜单


	
	CDialog::OnRButtonDown(nFlags, point);
}

void CWatchDlg::MeasureAdd()
{
	CMeasureAddDlg measureAddDlg(this);
//	measureAddDlg.DoModal();
	if (IDOK == measureAddDlg.DoModal())
	{
		//添加 measureItem
	}
	
}

void CWatchDlg::OnMeaAddmeasure() 
{
	// TODO: Add your command handler code here
	MeasureAdd();
}

void CWatchDlg::AddToMeasureDlg(CWaveData* pWaveData,int nMeaType ,int nMeaPart)
{
	if (m_pMeasureDlg == NULL)
	{
		//该CWatchDlg还没有配套的MeasureDlg
		m_pMeasureDlg = new CMeasureDlg;
		m_pMeasureDlg->Create(IDD_DIALOG_Measure , &(m_pOwnedBgDlg->m_TabBgDlg));
		m_pMeasureDlg->SetMatchWatchDlg(this);
		m_pMeasureDlg->AddMeasureItem(pWaveData, nMeaType , nMeaPart);
		ComputeFrame();

	} 
	else
	{
		//该CWatchDlg已经有配套的MeasureDlg 将MeasureItem加入
		m_pMeasureDlg->AddMeasureItem(pWaveData, nMeaType , nMeaPart);
		ComputeFrame();
	}
/*
	MoveWindow(m_rcWatchDlgFrame);
	if (m_pMeasureDlg != NULL)
	{
		m_pMeasureDlg->MoveWindow(m_rcMFinTab);
	}
*/
	ModifyPos();
	
}

void CWatchDlg::ModifyPos()
{
	MoveWindow(m_rcWatchDlgFrame);
	if (m_pMeasureDlg != NULL)
	{
		m_pMeasureDlg->MoveWindow(m_rcMFinTab);
	}
//	m_pOwnedBgDlg->Invalidate();
	Invalidate();
}

void CWatchDlg::MeasureDelete()					//该函数存在delete问题 不再调用
{
	if (m_pMeasureDlg == NULL)
	{
		//没有MeasureDlg
//		TRACE("===============warning:m_pMeasureDlg == NULL=================");
		MessageBeep(MB_ICONHAND);
		Sleep(500);
		MessageBeep(MB_ICONEXCLAMATION);
		Sleep(500);
		MessageBeep(-1);

		return;
	}

	if (m_pMeasureDlg->m_nMeasureNum <= 1)
	{
		//该MeasureDlg只有最后一个Item了 直接关闭
		m_pMeasureDlg->SendMessage(WM_CLOSE);
		m_pMeasureDlg = NULL;

	} 
	else
	{
		//删除当前Item
		CMeasureItem* pMeasureItem = NULL;


		CTypedPtrList<CObList , CMeasureItem*>* pMeasureItemList;
		pMeasureItemList = &(m_pMeasureDlg->m_MeasureItemList);
		pMeasureItem = (CMeasureItem*)pMeasureItemList->GetAt(pMeasureItemList->FindIndex(m_pMeasureDlg->m_nSelItem));
		
		pMeasureItemList->RemoveAt(pMeasureItemList->FindIndex(m_pMeasureDlg->m_nSelItem));
/*
		pMeasureItem = (CMeasureItem*)m_pMeasureDlg->m_MeasureItemList.GetAt(m_pMeasureDlg->m_MeasureItemList.FindIndex(m_pMeasureDlg->m_nSelItem));
		m_pMeasureDlg->m_MeasureItemList.RemoveAt(m_pMeasureDlg->m_MeasureItemList.FindIndex(m_pMeasureDlg->m_nSelItem));
*/
		delete pMeasureItem;				//这里有问题  老是有delete报错 phead->nblockuse == nblockuse  不过该函数写到CMeasureDlg中就没事

		m_pMeasureDlg->m_nMeasureNum--;
		if (m_pMeasureDlg->m_nMeasureNum == m_pMeasureDlg->m_nSelItem)
		{
			//m_nSelItem选择的是最后一个Item 需要修改
			m_pMeasureDlg->m_nSelItem--;
		}
	}
	
//	m_pMeasureDlg = NULL;

	ComputeFrame();
	ModifyPos();

}

void CWatchDlg::OnMeaDelmeasure() 
{
	// TODO: Add your command handler code here
//	TRACE("=============CWatchDlg::OnMeaDelmeasure() =================================\n");
//	MeasureDelete();
}

void CWatchDlg::MeasureEdit()
{
	CMeasureEditDlg measureEditDlg(this);
	//	measureAddDlg.DoModal();
	if (IDOK == measureEditDlg.DoModal())
	{
		//添加 measureItem
	}
}

void CWatchDlg::ComputeMarkFrame()
{
	//计算 m_XMF.rcXAxisMarkFrame	m_YMF.rcYAxisMarkFrame;
	m_YMF[0].rcYAxisMarkFrame.left		= m_YF[0].rcFrame.left - BMP_MARK_SIZE/2;
	m_YMF[0].rcYAxisMarkFrame.top		= m_YF[0].rcFrame.top - BMP_MARK_SIZE/2;
	m_YMF[0].rcYAxisMarkFrame.right		= m_YF[0].rcFrame.left + BMP_MARK_SIZE/2;
	m_YMF[0].rcYAxisMarkFrame.bottom	= m_YF[0].rcFrame.bottom + BMP_MARK_SIZE/2;
	
	m_YMF[2].rcYAxisMarkFrame.left		= m_YF[2].rcFrame.left - BMP_MARK_SIZE/2;
	m_YMF[2].rcYAxisMarkFrame.top		= m_YF[2].rcFrame.top - BMP_MARK_SIZE/2;
	m_YMF[2].rcYAxisMarkFrame.right		= m_YF[2].rcFrame.left + BMP_MARK_SIZE/2;
	m_YMF[2].rcYAxisMarkFrame.bottom	= m_YF[2].rcFrame.bottom + BMP_MARK_SIZE/2;
	
	m_YMF[1].rcYAxisMarkFrame.left		= m_YF[1].rcFrame.right - BMP_MARK_SIZE/2;
	m_YMF[1].rcYAxisMarkFrame.top		= m_YF[1].rcFrame.top - BMP_MARK_SIZE/2;
	m_YMF[1].rcYAxisMarkFrame.right		= m_YF[1].rcFrame.right + BMP_MARK_SIZE/2;
	m_YMF[1].rcYAxisMarkFrame.bottom	= m_YF[1].rcFrame.bottom + BMP_MARK_SIZE/2;
	
	m_YMF[3].rcYAxisMarkFrame.left		= m_YF[3].rcFrame.right - BMP_MARK_SIZE/2;
	m_YMF[3].rcYAxisMarkFrame.top		= m_YF[3].rcFrame.top - BMP_MARK_SIZE/2;
	m_YMF[3].rcYAxisMarkFrame.right		= m_YF[3].rcFrame.right + BMP_MARK_SIZE/2;
	m_YMF[3].rcYAxisMarkFrame.bottom	= m_YF[3].rcFrame.bottom + BMP_MARK_SIZE/2;
	
	m_XMF.rcXAxisMarkFrame.left		= m_WDW.rcDisplay.left - BMP_MARK_SIZE/2;
	m_XMF.rcXAxisMarkFrame.top		= m_WDW.rcDisplay.bottom - BMP_MARK_SIZE/2;
	m_XMF.rcXAxisMarkFrame.right	= m_WDW.rcDisplay.right + BMP_MARK_SIZE/2;
	m_XMF.rcXAxisMarkFrame.bottom	= m_WDW.rcDisplay.bottom + BMP_MARK_SIZE/2;

	//根据各个Ratio 计算不同的rcBmp位置
	m_YMF[0].rcPosMarkY_1.top		= m_YMF[0].rcYAxisMarkFrame.top + m_YMF[0].fRatioMarkY_1 * m_WDW.rcDisplay.Height();
	m_YMF[0].rcPosMarkY_1.bottom	= m_YMF[0].rcPosMarkY_1.top + BMP_MARK_SIZE;
	m_YMF[0].rcPosMarkY_1.left		= m_YMF[0].rcYAxisMarkFrame.left;
	m_YMF[0].rcPosMarkY_1.right		= m_YMF[0].rcYAxisMarkFrame.left + BMP_MARK_SIZE;	
	m_YMF[0].rcPosMarkY_2.top		= m_YMF[0].rcYAxisMarkFrame.top + m_YMF[0].fRatioMarkY_2 * m_WDW.rcDisplay.Height();
	m_YMF[0].rcPosMarkY_2.bottom	= m_YMF[0].rcPosMarkY_2.top + BMP_MARK_SIZE;
	m_YMF[0].rcPosMarkY_2.left		= m_YMF[0].rcYAxisMarkFrame.left;
	m_YMF[0].rcPosMarkY_2.right		= m_YMF[0].rcYAxisMarkFrame.left + BMP_MARK_SIZE;
	
	m_YMF[2].rcPosMarkY_1.top		= m_YMF[2].rcYAxisMarkFrame.top + m_YMF[2].fRatioMarkY_1 * m_WDW.rcDisplay.Height();
	m_YMF[2].rcPosMarkY_1.bottom	= m_YMF[2].rcPosMarkY_1.top + BMP_MARK_SIZE;
	m_YMF[2].rcPosMarkY_1.left		= m_YMF[2].rcYAxisMarkFrame.left;
	m_YMF[2].rcPosMarkY_1.right		= m_YMF[2].rcYAxisMarkFrame.left + BMP_MARK_SIZE;	
	m_YMF[2].rcPosMarkY_2.top		= m_YMF[2].rcYAxisMarkFrame.top + m_YMF[2].fRatioMarkY_2 * m_WDW.rcDisplay.Height();
	m_YMF[2].rcPosMarkY_2.bottom	= m_YMF[2].rcPosMarkY_2.top + BMP_MARK_SIZE;
	m_YMF[2].rcPosMarkY_2.left		= m_YMF[2].rcYAxisMarkFrame.left;
	m_YMF[2].rcPosMarkY_2.right		= m_YMF[2].rcYAxisMarkFrame.left + BMP_MARK_SIZE;
	
	m_YMF[1].rcPosMarkY_1.top		= m_YMF[1].rcYAxisMarkFrame.top + m_YMF[1].fRatioMarkY_1 * m_WDW.rcDisplay.Height();
	m_YMF[1].rcPosMarkY_1.bottom	= m_YMF[1].rcPosMarkY_1.top + BMP_MARK_SIZE;
	m_YMF[1].rcPosMarkY_1.left		= m_YMF[1].rcYAxisMarkFrame.left;
	m_YMF[1].rcPosMarkY_1.right		= m_YMF[1].rcYAxisMarkFrame.left + BMP_MARK_SIZE;	
	m_YMF[1].rcPosMarkY_2.top		= m_YMF[1].rcYAxisMarkFrame.top + m_YMF[1].fRatioMarkY_2 * m_WDW.rcDisplay.Height();
	m_YMF[1].rcPosMarkY_2.bottom	= m_YMF[1].rcPosMarkY_2.top + BMP_MARK_SIZE;
	m_YMF[1].rcPosMarkY_2.left		= m_YMF[1].rcYAxisMarkFrame.left;
	m_YMF[1].rcPosMarkY_2.right		= m_YMF[1].rcYAxisMarkFrame.left + BMP_MARK_SIZE;
	
	m_YMF[3].rcPosMarkY_1.top		= m_YMF[3].rcYAxisMarkFrame.top + m_YMF[3].fRatioMarkY_1 * m_WDW.rcDisplay.Height();
	m_YMF[3].rcPosMarkY_1.bottom	= m_YMF[3].rcPosMarkY_1.top + BMP_MARK_SIZE;
	m_YMF[3].rcPosMarkY_1.left		= m_YMF[3].rcYAxisMarkFrame.left;
	m_YMF[3].rcPosMarkY_1.right		= m_YMF[3].rcYAxisMarkFrame.left + BMP_MARK_SIZE;	
	m_YMF[3].rcPosMarkY_2.top		= m_YMF[3].rcYAxisMarkFrame.top + m_YMF[3].fRatioMarkY_2 * m_WDW.rcDisplay.Height();
	m_YMF[3].rcPosMarkY_2.bottom	= m_YMF[3].rcPosMarkY_2.top + BMP_MARK_SIZE;
	m_YMF[3].rcPosMarkY_2.left		= m_YMF[3].rcYAxisMarkFrame.left;
	m_YMF[3].rcPosMarkY_2.right		= m_YMF[3].rcYAxisMarkFrame.left + BMP_MARK_SIZE;
	
	m_XMF.rcPosMarkX_1.left		= m_XMF.rcXAxisMarkFrame.left + m_XMF.fRatioMarkX_1 * m_WDW.rcDisplay.Width();
	m_XMF.rcPosMarkX_1.right	= m_XMF.rcPosMarkX_1.left + BMP_MARK_SIZE;
	m_XMF.rcPosMarkX_1.top		= m_XMF.rcXAxisMarkFrame.top;
	m_XMF.rcPosMarkX_1.bottom	= m_XMF.rcXAxisMarkFrame.top + BMP_MARK_SIZE;	
	m_XMF.rcPosMarkX_2.left		= m_XMF.rcXAxisMarkFrame.left + m_XMF.fRatioMarkX_2 * m_WDW.rcDisplay.Width();
	m_XMF.rcPosMarkX_2.right	= m_XMF.rcPosMarkX_2.left + BMP_MARK_SIZE;
	m_XMF.rcPosMarkX_2.top		= m_XMF.rcXAxisMarkFrame.top;
	m_XMF.rcPosMarkX_2.bottom	= m_XMF.rcXAxisMarkFrame.top + BMP_MARK_SIZE;
	
	m_XMF.rcPosScale_1.left		= m_XMF.rcXAxisMarkFrame.left + m_XMF.fRatioScale_1 * m_WDW.rcDisplay.Width();
	m_XMF.rcPosScale_1.right	= m_XMF.rcPosScale_1.left + BMP_MARK_SIZE;
	m_XMF.rcPosScale_1.top		= m_XMF.rcXAxisMarkFrame.top;
	m_XMF.rcPosScale_1.bottom	= m_XMF.rcXAxisMarkFrame.top + BMP_MARK_SIZE;
	m_XMF.rcPosScale_2.left		= m_XMF.rcXAxisMarkFrame.left + m_XMF.fRatioScale_2 * m_WDW.rcDisplay.Width();
	m_XMF.rcPosScale_2.right	= m_XMF.rcPosScale_2.left + BMP_MARK_SIZE;
	m_XMF.rcPosScale_2.top		= m_XMF.rcXAxisMarkFrame.top;
	m_XMF.rcPosScale_2.bottom	= m_XMF.rcXAxisMarkFrame.top + BMP_MARK_SIZE;
}

void CWatchDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if ( TRUE == m_bMarkIsPick)
	{
		m_bMarkIsPick = FALSE;
		InvalidateRect(m_WDW.rcDisplay);
	}

//	TRACE("before ReleaseCapture\n");
	ReleaseCapture();
//	TRACE("after ReleaseCapture\n");
	m_nMarkMode = 0;

	

	if (m_bZoomDragIsPick)
	{
		//进行了ZoomDrag操作  需要呼出ZoomDlg
		if (m_pZoomDlg == NULL)
		{
			m_pZoomDlg = new CZoomDlg(this);
			m_pZoomDlg->Create(IDD_DIALOG_Zoom,this);
			m_pZoomDlg->MoveWindow(0,0,Zoom_DEFAULT_WIDTH,Zoom_DEFAULT_HEIGHT);
			//	m_pZoomDlg->ShowWindow(SW_SHOW);
		}
		m_bZoomDragIsPick = false;

		float fRatioStartX	= (float)(m_ptZoomDragStart.x - m_WDW.rcDisplay.left) / (float)(m_WDW.rcDisplay.Width());
		float fRatioStartY	= (float)(m_ptZoomDragStart.y - m_WDW.rcDisplay.top) / (float)(m_WDW.rcDisplay.Height());
		float fRatioEndX	= (float)(m_ptZoomDragEnd.x - m_WDW.rcDisplay.left) / (float)(m_WDW.rcDisplay.Width());
		float fRatioEndY	= (float)(m_ptZoomDragEnd.y - m_WDW.rcDisplay.top) / (float)(m_WDW.rcDisplay.Height());

		m_pZoomDlg->ModifyZoomRectRatio( fRatioStartX, fRatioStartY, fRatioEndX, fRatioEndY);
		m_pZoomDlg->ShowWindow(SW_SHOW);
	}

	CDialog::OnLButtonUp(nFlags, point);
}

void CWatchDlg::ModifyMarkPos(CPoint point)
{	
	float fChangeValue;
	CRect rcMarkAndDisplay;
	int nWaveIndex = 0 ;
	int nMarkIndex = 0 ;
	rcMarkAndDisplay.left	= m_YMF[0].rcYAxisMarkFrame.left;
	rcMarkAndDisplay.top	= m_YMF[0].rcYAxisMarkFrame.top;
	rcMarkAndDisplay.right	= m_YMF[1].rcYAxisMarkFrame.right;
	rcMarkAndDisplay.bottom	= m_XMF.rcXAxisMarkFrame.bottom;
	//TRACE("======step into : ModifyMarkPos====mode: %d ===============================\n",m_nMarkMode);
	if ( m_nMarkMode>=MARK_MODE_LEFTONE_1 && m_nMarkMode<=MARK_MODE_RIGHTTWO_2 )
	{
		//处理在Y轴上的MARK移动
		nWaveIndex = (int)((m_nMarkMode-1)/2) ;
		nMarkIndex = (int)((m_nMarkMode-1)%2) ;
		if ( 0 == nMarkIndex )
		{
			if ( point.y-m_YMF[nWaveIndex].nDisPickToTopMarkY_1 > m_YMF[nWaveIndex].rcYAxisMarkFrame.top && point.y+BMP_MARK_SIZE-m_YMF[nWaveIndex].nDisPickToTopMarkY_1 < m_YMF[nWaveIndex].rcYAxisMarkFrame.bottom )
			{			
				m_YMF[nWaveIndex].rcPosMarkY_1.top += point.y - m_ptMouseMove.y; 
				m_YMF[nWaveIndex].rcPosMarkY_1.bottom = m_YMF[nWaveIndex].rcPosMarkY_1.top + BMP_MARK_SIZE;
				m_YMF[nWaveIndex].fRatioMarkY_1 = (((float)m_YMF[nWaveIndex].rcPosMarkY_1.top-(float)m_YMF[nWaveIndex].rcYAxisMarkFrame.top) / (float)m_WDW.rcDisplay.Height());
				//TRACE("======step into : ModifyMarkPos====fRatioMarkY_1: %f ===============================\n",m_YMF.fRatioMarkY_1 );
				m_ptMouseMove = point;
			}
			else if (point.y-m_YMF[nWaveIndex].nDisPickToTopMarkY_1 <= m_YMF[nWaveIndex].rcYAxisMarkFrame.top)
			{
			//	TRACE("======step into here 1: ModifyMarkPos====mode: %d ===============================\n",m_nMarkMode);
				m_YMF[nWaveIndex].rcPosMarkY_1.top =  m_YMF[nWaveIndex].rcYAxisMarkFrame.top;
				m_YMF[nWaveIndex].rcPosMarkY_1.bottom = m_YMF[nWaveIndex].rcPosMarkY_1.top + BMP_MARK_SIZE;
				m_YMF[nWaveIndex].fRatioMarkY_1 = 0.0;
				
				m_ptMouseMove.y = m_YMF[nWaveIndex].rcYAxisMarkFrame.top + m_YMF[nWaveIndex].nDisPickToTopMarkY_1;
			} 
			else
			{
				m_YMF[nWaveIndex].rcPosMarkY_1.top =  m_YMF[nWaveIndex].rcYAxisMarkFrame.bottom - BMP_MARK_SIZE;
				m_YMF[nWaveIndex].rcPosMarkY_1.bottom = m_YMF[nWaveIndex].rcPosMarkY_1.top + BMP_MARK_SIZE;
				m_YMF[nWaveIndex].fRatioMarkY_1 = 1.0;

				m_ptMouseMove.y = m_YMF[nWaveIndex].rcYAxisMarkFrame.bottom - BMP_MARK_SIZE + m_YMF[nWaveIndex].nDisPickToTopMarkY_1;
			}
			fChangeValue = 1.0 - m_YMF[nWaveIndex].fRatioMarkY_1;
			InvalidateRect(rcMarkAndDisplay);
		//	InvalidateRect(m_YMF[nWaveIndex].rcYAxisMarkFrame);
		//	UpdateWindow();
		}
		else{
			if ( point.y-m_YMF[nWaveIndex].nDisPickToTopMarkY_2 > m_YMF[nWaveIndex].rcYAxisMarkFrame.top && point.y+BMP_MARK_SIZE-m_YMF[nWaveIndex].nDisPickToTopMarkY_2 < m_YMF[nWaveIndex].rcYAxisMarkFrame.bottom )
			{
				m_YMF[nWaveIndex].rcPosMarkY_2.top += point.y - m_ptMouseMove.y; 
				m_YMF[nWaveIndex].rcPosMarkY_2.bottom = m_YMF[nWaveIndex].rcPosMarkY_2.top + BMP_MARK_SIZE;
				m_YMF[nWaveIndex].fRatioMarkY_2 = (((float)m_YMF[nWaveIndex].rcPosMarkY_2.top-(float)m_YMF[nWaveIndex].rcYAxisMarkFrame.top) / (float)m_WDW.rcDisplay.Height());
				
				m_ptMouseMove = point;
			}
			else if (point.y-m_YMF[nWaveIndex].nDisPickToTopMarkY_2 <= m_YMF[nWaveIndex].rcYAxisMarkFrame.top)
			{
				m_YMF[nWaveIndex].rcPosMarkY_2.top =  m_YMF[nWaveIndex].rcYAxisMarkFrame.top;
				m_YMF[nWaveIndex].rcPosMarkY_2.bottom = m_YMF[nWaveIndex].rcPosMarkY_2.top + BMP_MARK_SIZE;
				m_YMF[nWaveIndex].fRatioMarkY_2 = 0.0;
				
				m_ptMouseMove.y = m_YMF[nWaveIndex].rcYAxisMarkFrame.top + m_YMF[nWaveIndex].nDisPickToTopMarkY_1;
			} 
			else
			{
				m_YMF[nWaveIndex].rcPosMarkY_2.top =  m_YMF[nWaveIndex].rcYAxisMarkFrame.bottom - BMP_MARK_SIZE;
				m_YMF[nWaveIndex].rcPosMarkY_2.bottom = m_YMF[nWaveIndex].rcPosMarkY_2.top + BMP_MARK_SIZE;
				m_YMF[nWaveIndex].fRatioMarkY_2 = 1.0;
				
				m_ptMouseMove.y = m_YMF[nWaveIndex].rcYAxisMarkFrame.bottom - BMP_MARK_SIZE + m_YMF[nWaveIndex].nDisPickToTopMarkY_1;
			}
			fChangeValue = 1.0 - m_YMF[nWaveIndex].fRatioMarkY_2;
			InvalidateRect(rcMarkAndDisplay);
		//	InvalidateRect(m_YMF[nWaveIndex].rcYAxisMarkFrame);
		}
	}
	else if ( m_nMarkMode>=MARK_MODE_MARKX_1 && m_nMarkMode<=MARK_MODE_SCALE_2 )
	{
		//处理在X轴上的MARK移动
		switch ( m_nMarkMode )
		{
		
		case MARK_MODE_MARKX_1:
			if ( point.x-m_XMF.nDisPickToLeftMarkX_1 > m_XMF.rcXAxisMarkFrame.left && point.x+BMP_MARK_SIZE-m_XMF.nDisPickToLeftMarkX_1 < m_XMF.rcXAxisMarkFrame.right )
			{
				m_XMF.rcPosMarkX_1.left += point.x - m_ptMouseMove.x;
				m_XMF.rcPosMarkX_1.right = m_XMF.rcPosMarkX_1.left + BMP_MARK_SIZE;
				m_XMF.fRatioMarkX_1 = (((float)m_XMF.rcPosMarkX_1.left-(float)m_XMF.rcXAxisMarkFrame.left) / (float)m_WDW.rcDisplay.Width());
				
				m_ptMouseMove = point;
			}
			else if ( point.x-m_XMF.nDisPickToLeftMarkX_1 <= m_XMF.rcXAxisMarkFrame.left )
			{
				m_XMF.rcPosMarkX_1.left =  m_XMF.rcXAxisMarkFrame.left;
				m_XMF.rcPosMarkX_1.right = m_XMF.rcPosMarkX_1.left + BMP_MARK_SIZE;
				m_XMF.fRatioMarkX_1 = 0.0;
				
				m_ptMouseMove.x = m_XMF.rcXAxisMarkFrame.left + m_XMF.nDisPickToLeftMarkX_1;
			} 
			else
			{
				m_XMF.rcPosMarkX_1.left =  m_XMF.rcXAxisMarkFrame.right - BMP_MARK_SIZE;
				m_XMF.rcPosMarkX_1.right = m_XMF.rcPosMarkX_1.left + BMP_MARK_SIZE;
				m_XMF.fRatioMarkX_1 = 1.0;
				
				m_ptMouseMove.x = m_XMF.rcXAxisMarkFrame.right - BMP_MARK_SIZE + m_XMF.nDisPickToLeftMarkX_1;
			}
			fChangeValue = m_XMF.fRatioMarkX_1 ;
			InvalidateRect(rcMarkAndDisplay);
			break;
		case MARK_MODE_MARKX_2:
			if ( point.x-m_XMF.nDisPickToLeftMarkX_2 > m_XMF.rcXAxisMarkFrame.left && point.x+BMP_MARK_SIZE-m_XMF.nDisPickToLeftMarkX_2 < m_XMF.rcXAxisMarkFrame.right )
			{
				m_XMF.rcPosMarkX_2.left += point.x - m_ptMouseMove.x;
				m_XMF.rcPosMarkX_2.right = m_XMF.rcPosMarkX_2.left + BMP_MARK_SIZE;
				m_XMF.fRatioMarkX_2 = (((float)m_XMF.rcPosMarkX_2.left-(float)m_XMF.rcXAxisMarkFrame.left) / (float)m_WDW.rcDisplay.Width());
				
				m_ptMouseMove = point;
			}
			else if ( point.x-m_XMF.nDisPickToLeftMarkX_2 <= m_XMF.rcXAxisMarkFrame.left )
			{
				m_XMF.rcPosMarkX_2.left =  m_XMF.rcXAxisMarkFrame.left;
				m_XMF.rcPosMarkX_2.right = m_XMF.rcPosMarkX_2.left + BMP_MARK_SIZE;
				m_XMF.fRatioMarkX_2 = 0.0;
				
				m_ptMouseMove.x = m_XMF.rcXAxisMarkFrame.left + m_XMF.nDisPickToLeftMarkX_2;
			} 
			else
			{
				m_XMF.rcPosMarkX_2.left =  m_XMF.rcXAxisMarkFrame.right - BMP_MARK_SIZE;
				m_XMF.rcPosMarkX_2.right = m_XMF.rcPosMarkX_2.left + BMP_MARK_SIZE;
				m_XMF.fRatioMarkX_2 = 1.0;
				
				m_ptMouseMove.x = m_XMF.rcXAxisMarkFrame.right - BMP_MARK_SIZE + m_XMF.nDisPickToLeftMarkX_2;
			}
			fChangeValue = m_XMF.fRatioMarkX_2 ;
			InvalidateRect(rcMarkAndDisplay);
			break;
		case MARK_MODE_SCALE_1:
			if ( point.x-m_XMF.nDisPickToLeftScale_1 > m_XMF.rcXAxisMarkFrame.left && point.x+BMP_MARK_SIZE-m_XMF.nDisPickToLeftScale_1 < m_XMF.rcXAxisMarkFrame.right )
			{
				m_XMF.rcPosScale_1.left += point.x - m_ptMouseMove.x;
				m_XMF.rcPosScale_1.right = m_XMF.rcPosScale_1.left + BMP_MARK_SIZE;
				m_XMF.fRatioScale_1 = (((float)m_XMF.rcPosScale_1.left-(float)m_XMF.rcXAxisMarkFrame.left) / (float)m_WDW.rcDisplay.Width());
				
				m_ptMouseMove = point;
			}
			else if ( point.x-m_XMF.nDisPickToLeftScale_1 <= m_XMF.rcXAxisMarkFrame.left )
			{
				m_XMF.rcPosScale_1.left =  m_XMF.rcXAxisMarkFrame.left;
				m_XMF.rcPosScale_1.right = m_XMF.rcPosScale_1.left + BMP_MARK_SIZE;
				m_XMF.fRatioScale_1 = 0.0;
				
				m_ptMouseMove.x = m_XMF.rcXAxisMarkFrame.left + m_XMF.nDisPickToLeftScale_1;
			} 
			else
			{
				m_XMF.rcPosScale_1.left =  m_XMF.rcXAxisMarkFrame.right - BMP_MARK_SIZE;
				m_XMF.rcPosScale_1.right = m_XMF.rcPosScale_1.left + BMP_MARK_SIZE;
				m_XMF.fRatioScale_1 = 1.0;
				
				m_ptMouseMove.x = m_XMF.rcXAxisMarkFrame.right - BMP_MARK_SIZE + m_XMF.nDisPickToLeftScale_1;
			}
			fChangeValue = m_XMF.fRatioScale_1 ;
			InvalidateRect(rcMarkAndDisplay);
			break;
		case MARK_MODE_SCALE_2:
			if ( point.x-m_XMF.nDisPickToLeftScale_2 > m_XMF.rcXAxisMarkFrame.left && point.x+BMP_MARK_SIZE-m_XMF.nDisPickToLeftScale_2 < m_XMF.rcXAxisMarkFrame.right )
			{
				m_XMF.rcPosScale_2.left += point.x - m_ptMouseMove.x;
				m_XMF.rcPosScale_2.right = m_XMF.rcPosScale_2.left + BMP_MARK_SIZE;
				m_XMF.fRatioScale_2 = (((float)m_XMF.rcPosScale_2.left-(float)m_XMF.rcXAxisMarkFrame.left) / (float)m_WDW.rcDisplay.Width());
				
				m_ptMouseMove = point;
			}
			else if ( point.x-m_XMF.nDisPickToLeftScale_2 <= m_XMF.rcXAxisMarkFrame.left )
			{
				m_XMF.rcPosScale_2.left =  m_XMF.rcXAxisMarkFrame.left;
				m_XMF.rcPosScale_2.right = m_XMF.rcPosScale_2.left + BMP_MARK_SIZE;
				m_XMF.fRatioScale_2 = 0.0;
				
				m_ptMouseMove.x = m_XMF.rcXAxisMarkFrame.left + m_XMF.nDisPickToLeftScale_2;
			} 
			else
			{
				m_XMF.rcPosScale_2.left =  m_XMF.rcXAxisMarkFrame.right - BMP_MARK_SIZE;
				m_XMF.rcPosScale_2.right = m_XMF.rcPosScale_2.left + BMP_MARK_SIZE;
				m_XMF.fRatioScale_2 = 1.0;
				
				m_ptMouseMove.x = m_XMF.rcXAxisMarkFrame.right - BMP_MARK_SIZE + m_XMF.nDisPickToLeftScale_2;
			}
			fChangeValue = m_XMF.fRatioScale_2 ;
			InvalidateRect(rcMarkAndDisplay);
			break;

		}
		//InvalidateRect(m_XMF.rcXAxisMarkFrame);
	}
	else{
		TRACE("Bunny Tips : CWatchDlg::ModifyMarkPos(CPoint point) error!!!\n");
	}


//	m_ptMouseMove = point;
//	TRACE("===========CWatchDlg::ModifyMarkPos : fChangeValue : %.3f=================================\n",fChangeValue);
//	TRACE("===========CWatchDlg::ModifyMarkPos : m_nMarkMode : %d=================================\n",m_nMarkMode);
	ModifyValueMarkDlg(m_nMarkMode , fChangeValue);
	Invalidate();
	UpdateWindow();
}

void CWatchDlg::ModifyValueMarkDlg(int nMarkMode , float fChangeValue)
{

	if (m_pMarkDlg == NULL)
	{
		m_pMarkDlg = new CMarkDlg(this);
		m_pMarkDlg->Create(IDD_DIALOG_Mark,this);
	//	m_pMarkDlg->MoveWindow(0,0,100,100);
	//	m_pMarkDlg->ShowWindow(SW_SHOW);
	}
	m_pMarkDlg->ModifyValue(nMarkMode , fChangeValue);

	m_pMarkDlg->Invalidate();
	m_pMarkDlg->UpdateWindow();
}

void CWatchDlg::DrawMark(CDC *pDC)
{
	CString strTest;

	CDC	dcMem;	
	dcMem.CreateCompatibleDC(pDC);

	CWaveData* pWaveData = NULL;
	//画Mark的小块
	int nWaveIndex = 0;

	for ( nWaveIndex=0 ; nWaveIndex<m_nWaveDataNum ; nWaveIndex++ )
	{
		pWaveData = (CWaveData*)m_WaveDataList.GetAt(m_WaveDataList.FindIndex(nWaveIndex));
		pDC->FillSolidRect( &(m_YMF[nWaveIndex].rcPosMarkY_1) , pWaveData->m_LineColor );
		pDC->FillSolidRect( &(m_YMF[nWaveIndex].rcPosMarkY_2) , pWaveData->m_LineColor );

		strTest.Format("Bunny Tips : , nWaveIndex is %d ,CWatchDlg index is ", nWaveIndex);
		strTest = strTest + m_strWatchDlgName + "\n";
		TRACE(strTest);
		strTest.Format("Bunny Tips : top bottom left right of m_YMF[nWaveIndex].rcPosMarkY_1 is %d,%d,%d,%d . \n" , m_YMF[nWaveIndex].rcPosMarkY_1.top,m_YMF[nWaveIndex].rcPosMarkY_1.bottom,m_YMF[nWaveIndex].rcPosMarkY_1.left,m_YMF[nWaveIndex].rcPosMarkY_1.right);
		TRACE(strTest);
	}



/*
	dcMem.SelectObject(&m_bmpMarkY);
	pDC->BitBlt(m_YMF.rcPosMarkY_1.left, m_YMF.rcPosMarkY_1.top, BMP_MARK_SIZE, BMP_MARK_SIZE, &dcMem, 0, 0, SRCCOPY);
	
	dcMem.SelectObject(&m_bmpMarkY);
	pDC->BitBlt(m_YMF.rcPosMarkY_2.left, m_YMF.rcPosMarkY_2.top, BMP_MARK_SIZE, BMP_MARK_SIZE, &dcMem, 0, 0, SRCCOPY);
*/
	dcMem.SelectObject(&m_bmpMarkX);
	pDC->BitBlt(m_XMF.rcPosMarkX_1.left, m_XMF.rcPosMarkX_1.top, BMP_MARK_SIZE, BMP_MARK_SIZE, &dcMem, 0, 0, SRCCOPY);

	dcMem.SelectObject(&m_bmpMarkX);
	pDC->BitBlt(m_XMF.rcPosMarkX_2.left, m_XMF.rcPosMarkX_2.top, BMP_MARK_SIZE, BMP_MARK_SIZE, &dcMem, 0, 0, SRCCOPY);


	dcMem.SelectObject(&m_bmpScale);
	pDC->BitBlt(m_XMF.rcPosScale_1.left, m_XMF.rcPosScale_1.top, BMP_MARK_SIZE, BMP_MARK_SIZE, &dcMem, 0, 0, SRCCOPY);

	dcMem.SelectObject(&m_bmpScale);
	pDC->BitBlt(m_XMF.rcPosScale_2.left, m_XMF.rcPosScale_2.top, BMP_MARK_SIZE, BMP_MARK_SIZE, &dcMem, 0, 0, SRCCOPY);

	
	//画配套虚线
	//这里想虚线要注意，起点终点不一样，不能用循环
	CPen blackPen(PS_DASH,1,RGB(200,200,200));
	CPen* pOldPen = pDC->SelectObject(&blackPen);
	//m_YMF[0]
	if (m_YMF[0].fRatioMarkY_1!=0.0 && m_YMF[0].fRatioMarkY_1!=1.0)
	{
		//TRACE("======step into : DrawMark====fRatioMarkY_1: %f ===============================\n",m_YMF.fRatioMarkY_1 );
		pDC->MoveTo(m_YMF[0].rcYAxisMarkFrame.right , m_YMF[0].rcPosMarkY_1.top+BMP_MARK_SIZE/2);
		pDC->LineTo(m_WDW.rcDisplay.right , m_YMF[0].rcPosMarkY_1.top+BMP_MARK_SIZE/2);
	}
	if (m_YMF[0].fRatioMarkY_2!=0.0 && m_YMF[0].fRatioMarkY_2!=1.0)
	{
		pDC->MoveTo(m_YMF[0].rcYAxisMarkFrame.right , m_YMF[0].rcPosMarkY_2.top+BMP_MARK_SIZE/2);
		pDC->LineTo(m_WDW.rcDisplay.right , m_YMF[0].rcPosMarkY_2.top+BMP_MARK_SIZE/2);
	}
	//m_YMF[1]
	if (m_YMF[1].fRatioMarkY_1!=0.0 && m_YMF[1].fRatioMarkY_1!=1.0)
	{
		pDC->MoveTo(m_YMF[1].rcYAxisMarkFrame.left , m_YMF[1].rcPosMarkY_1.top+BMP_MARK_SIZE/2);
		pDC->LineTo(m_WDW.rcDisplay.left , m_YMF[1].rcPosMarkY_1.top+BMP_MARK_SIZE/2);
	}
	if (m_YMF[1].fRatioMarkY_2!=0.0 && m_YMF[1].fRatioMarkY_2!=1.0)
	{
		pDC->MoveTo(m_YMF[1].rcYAxisMarkFrame.left , m_YMF[1].rcPosMarkY_2.top+BMP_MARK_SIZE/2);
		pDC->LineTo(m_WDW.rcDisplay.left , m_YMF[1].rcPosMarkY_2.top+BMP_MARK_SIZE/2);
	}
	//m_YMF[2]
	if (m_YMF[2].fRatioMarkY_1!=0.0 && m_YMF[2].fRatioMarkY_1!=1.0)
	{
		//TRACE("======step into : DrawMark====fRatioMarkY_1: %f ===============================\n",m_YMF.fRatioMarkY_1 );
		pDC->MoveTo(m_YMF[2].rcYAxisMarkFrame.right , m_YMF[2].rcPosMarkY_1.top+BMP_MARK_SIZE/2);
		pDC->LineTo(m_WDW.rcDisplay.right , m_YMF[2].rcPosMarkY_1.top+BMP_MARK_SIZE/2);
	}
	if (m_YMF[2].fRatioMarkY_2!=0.0 && m_YMF[2].fRatioMarkY_2!=1.0)
	{
		pDC->MoveTo(m_YMF[2].rcYAxisMarkFrame.right , m_YMF[2].rcPosMarkY_2.top+BMP_MARK_SIZE/2);
		pDC->LineTo(m_WDW.rcDisplay.right , m_YMF[2].rcPosMarkY_2.top+BMP_MARK_SIZE/2);
	}
	//m_YMF[3]
	if (m_YMF[3].fRatioMarkY_1!=0.0 && m_YMF[3].fRatioMarkY_1!=1.0)
	{
		pDC->MoveTo(m_YMF[3].rcYAxisMarkFrame.left , m_YMF[3].rcPosMarkY_1.top+BMP_MARK_SIZE/2);
		pDC->LineTo(m_WDW.rcDisplay.left , m_YMF[3].rcPosMarkY_1.top+BMP_MARK_SIZE/2);
	}
	if (m_YMF[3].fRatioMarkY_2!=0.0 && m_YMF[3].fRatioMarkY_2!=1.0)
	{
		pDC->MoveTo(m_YMF[3].rcYAxisMarkFrame.left , m_YMF[3].rcPosMarkY_2.top+BMP_MARK_SIZE/2);
		pDC->LineTo(m_WDW.rcDisplay.left , m_YMF[3].rcPosMarkY_2.top+BMP_MARK_SIZE/2);
	}
	//m_XMF	MarkX
	if (m_XMF.fRatioMarkX_1!=0.0 && m_XMF.fRatioMarkX_1!=1.0)
	{		
		pDC->MoveTo(m_XMF.rcPosMarkX_1.left+BMP_MARK_SIZE/2 , m_XMF.rcPosMarkX_1.top);
		pDC->LineTo(m_XMF.rcPosMarkX_1.left+BMP_MARK_SIZE/2 , m_WDW.rcDisplay.top);
	}
	if (m_XMF.fRatioMarkX_2!=0.0 && m_XMF.fRatioMarkX_2!=1.0)
	{
		pDC->MoveTo(m_XMF.rcPosMarkX_2.left+BMP_MARK_SIZE/2 , m_XMF.rcPosMarkX_2.top);
		pDC->LineTo(m_XMF.rcPosMarkX_2.left+BMP_MARK_SIZE/2 , m_WDW.rcDisplay.top);
	}
	//m_XMF	Scale
	if (m_XMF.fRatioScale_1!=0.0 && m_XMF.fRatioScale_1!=1.0)
	{
		pDC->MoveTo(m_XMF.rcPosScale_1.left+BMP_MARK_SIZE/2 , m_XMF.rcPosScale_1.top);
		pDC->LineTo(m_XMF.rcPosScale_1.left+BMP_MARK_SIZE/2 , m_WDW.rcDisplay.top);
	}
	if (m_XMF.fRatioScale_2!=0.0 && m_XMF.fRatioScale_2!=1.0)
	{
		pDC->MoveTo(m_XMF.rcPosScale_2.left+BMP_MARK_SIZE/2 , m_XMF.rcPosScale_2.top);
		pDC->LineTo(m_XMF.rcPosScale_2.left+BMP_MARK_SIZE/2 , m_WDW.rcDisplay.top);
	}


	pDC->SelectObject(pOldPen);

}

void CWatchDlg::SetMarkByDefault()
{
	int nCount = 0;
	//===================m_YMF==================================================
	for ( nCount=0 ; nCount<DRAW_CHNUM_MAX ; nCount++ )
	{
		m_YMF[nCount].rcPosMarkY_1.top = m_YMF[nCount].rcYAxisMarkFrame.top;
		m_YMF[nCount].rcPosMarkY_1.bottom = m_YMF[nCount].rcPosMarkY_1.top + BMP_MARK_SIZE;
		
		m_YMF[nCount].rcPosMarkY_2.top = m_YMF[nCount].rcYAxisMarkFrame.top;
		m_YMF[nCount].rcPosMarkY_2.bottom = m_YMF[nCount].rcPosMarkY_2.top + BMP_MARK_SIZE;
		
		m_YMF[nCount].fRatioMarkY_1 = 0.0;
		m_YMF[nCount].fRatioMarkY_2 = 0.0;
		m_YMF[nCount].nDisPickToTopMarkY_1 = 0;
		m_YMF[nCount].nDisPickToTopMarkY_2 = 0;
	}
	
	
	//===================m_XMF==================================================
	m_XMF.rcPosMarkX_1.left = m_XMF.rcXAxisMarkFrame.left;
	m_XMF.rcPosMarkX_1.right = m_XMF.rcPosMarkX_1.left + BMP_MARK_SIZE;

	m_XMF.rcPosMarkX_2.left = m_XMF.rcXAxisMarkFrame.left;		
	m_XMF.rcPosMarkX_2.right = m_XMF.rcPosMarkX_2.left + BMP_MARK_SIZE;

	
	m_XMF.rcPosScale_1.left = m_XMF.rcXAxisMarkFrame.right - BMP_MARK_SIZE;		
	m_XMF.rcPosScale_1.right = m_XMF.rcPosScale_1.left + BMP_MARK_SIZE;

	m_XMF.rcPosScale_2.left = m_XMF.rcXAxisMarkFrame.right - BMP_MARK_SIZE;		
	m_XMF.rcPosScale_2.right = m_XMF.rcPosScale_2.left + BMP_MARK_SIZE;

	m_XMF.fRatioMarkX_1 = 0.0;
	m_XMF.fRatioMarkX_2 = 0.0;
	m_XMF.fRatioScale_1 = 1.0;
	m_XMF.fRatioScale_2 = 1.0;
	m_XMF.nDisPickToLeftMarkX_1 = 0;
	m_XMF.nDisPickToLeftMarkX_2 = 0;
	m_XMF.nDisPickToLeftScale_1 = 0;
	m_XMF.nDisPickToLeftScale_2 = 0;
}

BOOL CWatchDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// TODO: Add your message handler code here and/or call default

	::SetCursor(m_hCurCursor);
	return TRUE;
	
	return CDialog::OnSetCursor(pWnd, nHitTest, message);
}

void CWatchDlg::DrawZoomDragBorder(CDC *pDC)
{

	CPen blackPen(PS_SOLID,1,RGB(0,0,0));
	CPen* pOldPen = pDC->SelectObject(&blackPen);
	CBrush* transBrush = CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
	CBrush *pOldbrush = pDC->SelectObject(transBrush);

	pDC->Rectangle(m_ptZoomDragStart.x ,m_ptZoomDragStart.y ,m_ptZoomDragEnd.x ,m_ptZoomDragEnd.y );

	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldbrush);
}

void CWatchDlg::ChangeCursor(CPoint point)
{
	if (m_YMF[0].rcPosMarkY_1.PtInRect(point))
	{
		m_hCurCursor = LoadCursor(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDC_CURSOR_MarkDragY));
	} 
	else if (m_YMF[0].rcPosMarkY_2.PtInRect(point))
	{
		m_hCurCursor = LoadCursor(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDC_CURSOR_MarkDragY));
	} 
	else if (m_YMF[1].rcPosMarkY_1.PtInRect(point))
	{
		m_hCurCursor = LoadCursor(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDC_CURSOR_MarkDragY));
	} 
	else if (m_YMF[1].rcPosMarkY_2.PtInRect(point))
	{
		m_hCurCursor = LoadCursor(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDC_CURSOR_MarkDragY));
	} 
	else if (m_YMF[2].rcPosMarkY_1.PtInRect(point))
	{
		m_hCurCursor = LoadCursor(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDC_CURSOR_MarkDragY));
	} 
	else if (m_YMF[2].rcPosMarkY_2.PtInRect(point))
	{
		m_hCurCursor = LoadCursor(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDC_CURSOR_MarkDragY));
	} 
	else if (m_YMF[3].rcPosMarkY_1.PtInRect(point))
	{
		m_hCurCursor = LoadCursor(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDC_CURSOR_MarkDragY));
	} 
	else if (m_YMF[3].rcPosMarkY_2.PtInRect(point))
	{
		m_hCurCursor = LoadCursor(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDC_CURSOR_MarkDragY));
	} 
	else if (m_XMF.rcPosMarkX_1.PtInRect(point))
	{
		m_hCurCursor = LoadCursor(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDC_CURSOR_MarkDragX));
	} 
	else if (m_XMF.rcPosMarkX_2.PtInRect(point))
	{
		m_hCurCursor = LoadCursor(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDC_CURSOR_MarkDragX));
	} 
	else if (m_XMF.rcPosScale_1.PtInRect(point))
	{
		m_hCurCursor = LoadCursor(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDC_CURSOR_MarkDragX));
	} 
	else if (m_XMF.rcPosScale_2.PtInRect(point))
	{
		m_hCurCursor = LoadCursor(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDC_CURSOR_MarkDragX));
	} 
	else if (m_WDW.rcDisplay.PtInRect(point))
	{
		int MouseMode =((CMainFrame*)AfxGetMainWnd())->m_nMouseMode;
		if (MouseMode == MOUSEMODE_ZOOMIN)
		{
			m_hCurCursor = LoadCursor(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDC_CURSOR_ZoomIn));
		}
		else if (MouseMode == MOUSEMODE_ZOOMOUT)
		{
			m_hCurCursor = LoadCursor(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDC_CURSOR_ZoomOut));
		}
		else if (MouseMode == MOUSEMODE_ZOOMDRAG)
		{
			m_hCurCursor = LoadCursor(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDC_CURSOR_ZoomDrag));
		}
		else if (MouseMode == MOUSEMODE_NONE)
		{
			m_hCurCursor = LoadCursor(NULL,IDC_ARROW);
		}
		else if (MouseMode == MOUSEMODE_RESTORE)
		{
			m_hCurCursor = LoadCursor(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDC_CURSOR_ZoomRestore));
		}
	}
	else{
		//默认情况还是用原来的箭头
		m_hCurCursor = LoadCursor(NULL,IDC_ARROW);
	}
}

void CWatchDlg::ZoomRestoreModifyEveryWndRc(CPoint *point)
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CRecorderView* pView= (CRecorderView*)(pFrame->GetActiveView());
	CRecorderDoc *pDoc = (CRecorderDoc*)(pFrame->GetActiveDocument());
	CProduct* pProduct = &(pDoc->m_Product);
	CWndManage* pWndMange = &(pView->m_WndManage);	
	WavePlayCtrl* pWPC = &(pWndMange->m_WPCUnified);


	int nOldPlayWndWidth = pProduct->m_nPlayWndWidth;
	pProduct->m_nPlayWndWidth = DATASTORAGE_SEGMENT;


	int nPointClickToLeft = (point->x - m_WDW.rcDisplay.left) * nOldPlayWndWidth / m_WDW.rcDisplay.Width();
	if ( pProduct->m_nPlayStartIndex+ nPointClickToLeft - DATASTORAGE_SEGMENT/2 <= 0)
	{
		pProduct->m_nPlayStartIndex = 0;			
	}
	else if( pProduct->m_nPlayStartIndex+ nPointClickToLeft + DATASTORAGE_SEGMENT/2 > pProduct->m_AcqInfo.samplingLength)
	{
		pProduct->m_nPlayStartIndex = pProduct->m_AcqInfo.samplingLength - pProduct->m_nPlayWndWidth;;
	}
	else{
		pProduct->m_nPlayStartIndex = pProduct->m_nPlayStartIndex+ nPointClickToLeft - DATASTORAGE_SEGMENT/2 ;
	}
	pWPC->fRatioX = DATASTORAGE_SEGMENT/m_WDW.rcDisplay.Width();

	pFrame->m_pDlgBarExBottomPlay->m_pPlayBar->SetPlayPos(pProduct->m_nPlayStartIndex);
	pFrame->m_pDlgBarExBottomPlay->m_pPlayBar->SetPlayWndWidth(pProduct->m_nPlayWndWidth);
	
	pView->m_WndManage.AllWndRefresh_Complete();
}

CString CWatchDlg::ComputeHorUnitTime(double *pfResult, double fInput)
{
	CString strResult;
	if (fInput <0.001)
	{
		*pfResult = fInput * 1000000;
		strResult.Format("  ns"); 
	}
	else if (fInput <1)
	{
		*pfResult = fInput * 1000;
		strResult.Format("  ms"); 
	}
	else if (fInput <1000)
	{
		*pfResult = fInput;
		strResult.Format("  s"); 
	}
	else 
	{
		*pfResult = fInput / 1000;
		strResult.Format("  ks"); 
	}
	return strResult;
}

void CWatchDlg::OnLAYOUTMode1() 
{
	// TODO: Add your command handler code here
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CRecorderView* pView = (CRecorderView*)(pFrame->GetActiveView());
	pView->m_WndManage.Display(1);
}

void CWatchDlg::OnLAYOUTMode2() 
{
	// TODO: Add your command handler code here
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CRecorderView* pView = (CRecorderView*)(pFrame->GetActiveView());
	pView->m_WndManage.Display(2);
}

void CWatchDlg::OnLAYOUTMode3() 
{
	// TODO: Add your command handler code here
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CRecorderView* pView = (CRecorderView*)(pFrame->GetActiveView());
	pView->m_WndManage.Display(3);
}

void CWatchDlg::OnLAYOUTMode4() 
{
	// TODO: Add your command handler code here
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CRecorderView* pView = (CRecorderView*)(pFrame->GetActiveView());
	pView->m_WndManage.Display(4);
}

void CWatchDlg::OnWATCHProperty() 
{
	// TODO: Add your command handler code here
	CPopupPropertyDlg* pDlgPopupPro = new CPopupPropertyDlg;
	pDlgPopupPro->DoModal();
	delete pDlgPopupPro;
	pDlgPopupPro = NULL;
}

void CWatchDlg::OnWATCHAddOsc() 
{
	// TODO: Add your command handler code here
	//调用BgDlg中的AddWaveDataToBgDlg  -1新建 pWaveData随前一个WatchDlg 0正常显示 (1 FFT)
	m_pOwnedBgDlg->AddWaveDataToBgDlg(-1 , (CWatchDlg*)NULL , 0);
}

void CWatchDlg::OnWATCHAddFreDomain() 
{
	// TODO: Add your command handler code here
	//调用BgDlg中的AddWaveDataToBgDlg  -1新建 pWaveData随前一个WatchDlg 1FFT
	m_pOwnedBgDlg->AddWaveDataToBgDlg(-1 , (CWatchDlg*)NULL , 1);
}

void CWatchDlg::ChannelSelect()
{
	CChSelDlg dlg(this);
	dlg.DoModal();
}	

void CWatchDlg::OnWATCHChSel() 
{
	// TODO: Add your command handler code here
	ChannelSelect();
}

void CWatchDlg::UpdateWaveDataList(int nWaveIndexArray[], int nWaveNum)
{
	int nCount = 0 ;
	for ( nCount=0 ; nCount<nWaveNum ; nCount++)
	{
		
	}
}

void CWatchDlg::OnWATCHCloseWatchDlg() 
{
	// TODO: Add your command handler code here
	//让父窗口的BgDlg关闭该WatchDlg
	m_pOwnedBgDlg->CloseCurSelWatchDlg();
}

void CWatchDlg::AddWaveDataToWatchDlg(CWatchDlg *pAddWatchDlg)
{
	//加入一个CWatchDlg的全部CWaveData只会在新建一个CWatchDlg的时候发生，所以就不用进行重复CWaveData的检测
	CWaveData* pWaveData;
	int nWaveDataNum = pAddWatchDlg->m_nWaveDataNum;
	int nCount = 0;
	for (nCount=0 ; nCount<nWaveDataNum ; nCount++)
	{
		pWaveData = pAddWatchDlg->m_WaveDataList.GetAt(pAddWatchDlg->m_WaveDataList.FindIndex(nCount));
		AddWaveDataToWatchDlg(pWaveData);
	}

	/*根据当前CWatchDlg的m_nMode来初始化当前的量程m_nWaveDataRange*/
	if ( m_nMode == DISPLAY_MODE_TIME )
	{
		//时域 加入的全部CWaveData（通道）的初始量程都设置为10V
		for (nCount=0 ; nCount<DRAW_CHNUM_MAX ; nCount++)
		{
			if (nCount<nWaveDataNum)
			{
				m_nWaveDataRange[nCount] = 10;
			}
			else{
				m_nWaveDataRange[nCount] = 0;
			}
		}
	}
	else{
		//频域 要分4种不同情况
		CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
		CRecorderDoc *pDoc = (CRecorderDoc*)(pFrame->GetActiveDocument());
		CProduct* pProduct = &(pDoc->m_Product);
		switch ( pProduct->m_FftConf.nOutputMode )
		{
		case YAXIS_UNIT_V:
			for (nCount=0 ; nCount<DRAW_CHNUM_MAX ; nCount++)
			{
				if (nCount<nWaveDataNum)
				{
					m_nWaveDataRange[nCount] = RANGE_FREQ_V_0_500;
				}
				else{
					m_nWaveDataRange[nCount] = RANGE_CLOSED;
				}
			}
			break;
		case YAXIS_UNIT_DBV:
			for (nCount=0 ; nCount<DRAW_CHNUM_MAX ; nCount++)
			{
				if (nCount<nWaveDataNum)
				{
					m_nWaveDataRange[nCount] = RANGE_FREQ_DBV_n100_0;
				}
				else{
					m_nWaveDataRange[nCount] = RANGE_CLOSED;
				}
			}
			break;
		case YAXIS_UNIT_DBu:
			for (nCount=0 ; nCount<DRAW_CHNUM_MAX ; nCount++)
			{
				if (nCount<nWaveDataNum)
				{
					m_nWaveDataRange[nCount] = RANGE_FREQ_DBu_0_100;
				}
				else{
					m_nWaveDataRange[nCount] = RANGE_CLOSED;
				}
			}
			break;
		case YAXIS_UNIT_DBm:
			for (nCount=0 ; nCount<DRAW_CHNUM_MAX ; nCount++)
			{
				if (nCount<nWaveDataNum)
				{
					m_nWaveDataRange[nCount] = RANGE_FREQ_DBm_n100_0;
				}
				else{
					m_nWaveDataRange[nCount] = RANGE_CLOSED;
				}
			}
			break;
		default:
			break;
		}
	}
	
	
}

int CWatchDlg::SynToolBar()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CWaveData* pWaveData = NULL;

	int nWaveDataNum = m_WaveDataList.GetCount();
	int nCount = 0;
	for ( nCount=0 ; nCount<DRAW_CHNUM_MAX ; nCount++)
	{
		if ( nCount<nWaveDataNum )
		{
			pWaveData = (CWaveData*)m_WaveDataList.GetAt(m_WaveDataList.FindIndex(nCount));
			pFrame->m_pDlgBarExTopChnSel->m_pStaticChnSel[nCount]->SetWindowText(pWaveData->m_strChanName);
		}
		else{
			pFrame->m_pDlgBarExTopChnSel->m_pStaticChnSel[nCount]->SetWindowText("N/A");
		}
	}


	return 1;
}

int CWatchDlg::ChangeDisplayChn(const int *pChnSelected , int nChnSelectedNum)
{

	int nCount=0;
	CWaveData* pWaveData;

	m_nWaveDataNum = nChnSelectedNum;

	m_WaveDataList.RemoveAll();
	for ( nCount=0 ; nCount<DRAW_CHNUM_MAX ; nCount++)
	{
		if ( -1 != pChnSelected[nCount] )
		{
			pWaveData = (CWaveData*)m_pProduct->m_WaveList.GetAt(m_pProduct->m_WaveList.FindIndex(pChnSelected[nCount]));
			m_WaveDataList.AddTail(pWaveData);


			/*设置该WavaData在CWatchDlg中的Range，分时域和频域来处理*/
			if ( m_nMode == DISPLAY_MODE_TIME)
			{
				/*时域*/
				m_nWaveDataRange[nCount] = RANGE_TIME_pn10V;
			}
			else{
				/*频域 分4中情况来考虑*/
				switch ( m_pProduct->m_FftConf.nWindowMode )
				{
				case YAXIS_UNIT_V:
					m_nWaveDataRange[nCount] = RANGE_FREQ_V_0_500;
					break;
				case YAXIS_UNIT_DBV:
					m_nWaveDataRange[nCount] = RANGE_FREQ_DBV_n100_0;
					break;
				case YAXIS_UNIT_DBu:
					m_nWaveDataRange[nCount] = RANGE_FREQ_DBu_0_100;
					break;
				case YAXIS_UNIT_DBm:
					m_nWaveDataRange[nCount] = RANGE_FREQ_DBm_n100_0;
					break;
				default :
					TRACE("!!!ERROR : CWatchDlg::ChangeDisplayChn");
					break;
				}
			}
		}
		else{
			/*pChnSelected[nCount]==-1 该通道是关闭的*/
			m_nWaveDataRange[nCount] = RANGE_CLOSED;
		}
	}



	return 0;
}

int CWatchDlg::AdjustAllToolBar()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();

	/*调整显示4个通道的量程的ToolBar*/
	pFrame->m_pDlgBarExTopChnSel->ChangeToolBarDisplayChn(this);

	/*调整显示频域分析还是时域分析的ToolBar*/
	if ( m_nMode == 0 )
	{
		/*时域*/
		pFrame->m_aWatchMode[0] = 1;
		pFrame->m_aWatchMode[1] = 0;
	}
	else{
		/*频域*/
		pFrame->m_aWatchMode[0] = 0;
		pFrame->m_aWatchMode[1] = 1;
	}


	return 1;
}

int CWatchDlg::SwitchTimeAndFreq(int nDirection)
{
	/*nDirection代表方向：0 Freq->Time	1 Time->Freq	2 Freq一种单位->Freq另外一种单位*/

	int nCount = 0;

	if ( nDirection == 0 )
	{
		/*Freq->Time*/
		m_nMode = 0;
		/*改变CWatchDlg中的Range数组*/
		for ( nCount=0 ; nCount<DRAW_CHNUM_MAX ; nCount++)
		{
			if ( nCount < m_nWaveDataNum )
			{
				m_nWaveDataRange[nCount] = RANGE_TIME_pn10V;
			}
			else{
				m_nWaveDataRange[nCount] = RANGE_CLOSED;
			}
		}
	}//end Freq
	else if( nDirection == 1 )
	{
		/*Time->Freq*/
		m_nMode = 1;
		/*切换到频域分析的时候要分4中情况*/
		for ( nCount=0 ; nCount<DRAW_CHNUM_MAX ; nCount++)
		{
			if ( nCount < m_nWaveDataNum )
			{
				switch ( m_pProduct->m_FftConf.nOutputMode )
				{
				case YAXIS_UNIT_V:
					m_nWaveDataRange[nCount] = RANGE_FREQ_V_0_500;
					break;
				case YAXIS_UNIT_DBV:
					m_nWaveDataRange[nCount] = RANGE_FREQ_DBV_n100_0;
					break;
				case YAXIS_UNIT_DBu:
					m_nWaveDataRange[nCount] = RANGE_FREQ_DBu_0_100;
					break;
				case YAXIS_UNIT_DBm:
					m_nWaveDataRange[nCount] = RANGE_FREQ_DBm_n100_0;
					break;
				}
				
			}
			else{
				m_nWaveDataRange[nCount] = RANGE_CLOSED;
			}
		}
	
	}
	else{
		/*Freq一种单位->Freq另外一种单位*/
		m_nMode = 1;
		/*切换到频域分析的时候要分4中情况*/
		for ( nCount=0 ; nCount<DRAW_CHNUM_MAX ; nCount++)
		{
			if ( nCount < m_nWaveDataNum )
			{
				switch ( m_pProduct->m_FftConf.nOutputMode )
				{
				case YAXIS_UNIT_V:
					m_nWaveDataRange[nCount] = RANGE_FREQ_V_0_500;
					break;
				case YAXIS_UNIT_DBV:
					m_nWaveDataRange[nCount] = RANGE_FREQ_DBV_n100_0;
					break;
				case YAXIS_UNIT_DBu:
					m_nWaveDataRange[nCount] = RANGE_FREQ_DBu_0_100;
					break;
				case YAXIS_UNIT_DBm:
					m_nWaveDataRange[nCount] = RANGE_FREQ_DBm_n100_0;
					break;
				}
				
			}
			else{
				m_nWaveDataRange[nCount] = RANGE_CLOSED;
			}
		}
	}

	return 0;

}

int CWatchDlg::DrawYCoordinateTime(CDC *pCdc)
{
	CString str;
	CString strTest;

	int i;
	int charH = 13;
	int charW = 5;
	double ratio;		//判断纵横坐标可容纳多少个坐标字段
	CFont* pFont = NULL;
	CWaveData* pWaveData;

	strTest.Format("Bunny Tips : m_nWaveDataNum : %d . \n" , m_nWaveDataNum);
	TRACE(strTest);

	/*纵坐标*/
	if ( m_nWaveDataNum >= 1 )
	{
		//画m_YFLeftOne
		pCdc->SetTextAlign(TA_RIGHT);
		if (pFont != NULL)
		{
			delete pFont;
			pFont = NULL;
		}
		pFont = new CFont;
		pFont->CreateFont(charH, charW, 0, 0, FW_BOLD, false, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_SWISS,_T("Times New Roman"));
		CFont* pOldFont = pCdc->SelectObject(pFont);
		pWaveData = m_WaveDataList.GetAt(m_WaveDataList.FindIndex(0));			//用和pWaveData一样的颜色
		pCdc->SetTextColor(pWaveData->m_LineColor);
		pCdc->SetBkMode(TRANSPARENT);
			
		ratio = (double)(charH) / (((double)(m_WDW.rcDisplay.Height()))/10);    //字符高 除以 十分之一的显示框架高
		float yStart;
		float yLength;
		yStart = m_nWaveDataRange[0];											//用针对pWaveData的量程
		yLength = m_nWaveDataRange[0] * 2;

		strTest.Format("Bunny Tips : m_nWaveDataRange[0] : %d . \n",m_nWaveDataRange[0]);
		TRACE(strTest);
	
		

		if (ratio < 1.0)
		{	//可以显示全部的纵坐标
			
			for (i=0 ; i<11 ; i++)
			{
				str.Format("%.1f", yStart-i*yLength*0.1);
				pCdc->TextOut(m_YF[0].rcFrame.left+Y_COORDINATE_ALIGNRIGHT_TO_Y_FRAMERIGHT, m_YF[0].rcFrame.top+(i*m_YF[0].rcFrame.Height()/10) - (m_YF[0].rcFrame.Height()/60) , str);
			}
		} 
		else if(ratio < 2.0)
		{	//可以显示全部的纵坐标
			for (i=0 ; i<11 ; i++)
			{
				str.Format("%.1f", yStart-i*yLength*0.1);
				pCdc->TextOut(m_YF[0].rcFrame.left+Y_COORDINATE_ALIGNRIGHT_TO_Y_FRAMERIGHT, m_YF[0].rcFrame.top+(i*m_YF[0].rcFrame.Height()/10) - (m_YF[0].rcFrame.Height()/60) , str);
			}
		}
		else if(ratio < 4.0)
		{	//每两个个显示一个
			for (i=0 ; i<5 ; i++)
			{
				str.Format("%.1f", yStart-yLength*0.1-i*yLength*0.2);
				pCdc->TextOut(m_YF[0].rcFrame.left+Y_COORDINATE_ALIGNRIGHT_TO_Y_FRAMERIGHT, m_YF[0].rcFrame.top+(i*m_YF[0].rcFrame.Height()/7) - (m_YF[0].rcFrame.Height()/60) + m_YF[0].rcFrame.Height()/7 , str);
			}
		}
		else
		{	//每四个显示一个
			for (i=0 ; i<3 ; i++)
			{
				str.Format("%.1f", yStart-yLength*0.1-i*yLength*0.4);
				pCdc->TextOut(m_YF[0].rcFrame.left+Y_COORDINATE_ALIGNRIGHT_TO_Y_FRAMERIGHT, m_YF[0].rcFrame.top+(i*m_YF[0].rcFrame.Height()/5) - (m_YF[0].rcFrame.Height()/60) + m_YF[0].rcFrame.Height()/5 , str);
			}
		}
		str.Format("V");//纵坐标标题电压
		pCdc->TextOut(m_YF[0].rcFrame.left+15, m_YF[0].rcFrame.top+m_YF[0].rcFrame.Height()/2,str);
	}

	if ( m_nWaveDataNum >= 2 )
	{
		//画m_YFRightOne
		pCdc->SetTextAlign(TA_LEFT);
		if (pFont != NULL)
		{
			delete pFont;
			pFont = NULL;
		}
		pFont = new CFont;
		pFont->CreateFont(charH, charW, 0, 0, FW_BOLD, false, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_SWISS,_T("Times New Roman"));
		CFont* pOldFont = pCdc->SelectObject(pFont);
		pWaveData = m_WaveDataList.GetAt(m_WaveDataList.FindIndex(1));			//用和pWaveData一样的颜色
		pCdc->SetTextColor(pWaveData->m_LineColor);
		pCdc->SetBkMode(TRANSPARENT);
		
		ratio = (double)(charH) / (((double)(m_WDW.rcDisplay.Height()))/10);    //字符高 除以 十分之一的显示框架高
		float yStart;
		float yLength;
		yStart = m_nWaveDataRange[1];											//用针对pWaveData的量程
		yLength = m_nWaveDataRange[1] * 2;
		if (ratio < 1.0)
		{	//可以显示全部的纵坐标
			
			for (i=0 ; i<11 ; i++)
			{
				str.Format("%.1f", yStart-i*yLength*0.1);
				pCdc->TextOut(m_YF[1].rcFrame.right-Y_COORDINATE_ALIGNLEFT_TO_Y_FRAMELEFT, m_YF[1].rcFrame.top+(i*m_YF[1].rcFrame.Height()/10) - (m_YF[1].rcFrame.Height()/60) , str);
			}
		} 
		else if(ratio < 2.0)
		{	//可以显示全部的纵坐标
			for (i=0 ; i<11 ; i++)
			{
				str.Format("%.1f", yStart-i*yLength*0.1);
				pCdc->TextOut(m_YF[1].rcFrame.right-Y_COORDINATE_ALIGNLEFT_TO_Y_FRAMELEFT, m_YF[1].rcFrame.top+(i*m_YF[1].rcFrame.Height()/10) - (m_YF[1].rcFrame.Height()/60) , str);
			}
		}
		else if(ratio < 4.0)
		{	//每两个个显示一个
			for (i=0 ; i<5 ; i++)
			{
				str.Format("%.1f", yStart-yLength*0.1-i*yLength*0.2);
				pCdc->TextOut(m_YF[1].rcFrame.right-Y_COORDINATE_ALIGNLEFT_TO_Y_FRAMELEFT, m_YF[1].rcFrame.top+(i*m_YF[1].rcFrame.Height()/7) - (m_YF[1].rcFrame.Height()/60) + m_YF[1].rcFrame.Height()/7 , str);
			}
		}
		else
		{	//每四个显示一个
			for (i=0 ; i<3 ; i++)
			{
				str.Format("%.1f", yStart-yLength*0.1-i*yLength*0.4);
				pCdc->TextOut(m_YF[1].rcFrame.right-Y_COORDINATE_ALIGNLEFT_TO_Y_FRAMELEFT, m_YF[1].rcFrame.top+(i*m_YF[1].rcFrame.Height()/5) - (m_YF[1].rcFrame.Height()/60) + m_YF[1].rcFrame.Height()/5 , str);
			}
		}
		str.Format("V");//纵坐标标题电压
		pCdc->TextOut(m_YF[1].rcFrame.right-15, m_YF[1].rcFrame.top+m_YF[1].rcFrame.Height()/2,str);		
	}
	if ( m_nWaveDataNum >= 3 )
	{
		//画m_YFLeftTwo
		pCdc->SetTextAlign(TA_RIGHT);
		if (pFont != NULL)
		{
			delete pFont;
			pFont = NULL;
		}
		pFont = new CFont;
		pFont->CreateFont(charH, charW, 0, 0, FW_BOLD, false, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_SWISS,_T("Times New Roman"));
		CFont* pOldFont = pCdc->SelectObject(pFont);
		pWaveData = m_WaveDataList.GetAt(m_WaveDataList.FindIndex(2));			//用和pWaveData一样的颜色
		pCdc->SetTextColor(pWaveData->m_LineColor);
		pCdc->SetBkMode(TRANSPARENT);
		
		ratio = (double)(charH) / (((double)(m_WDW.rcDisplay.Height()))/10);    //字符高 除以 十分之一的显示框架高
		float yStart;
		float yLength;
		yStart = m_nWaveDataRange[2];											//用针对pWaveData的量程
		yLength = m_nWaveDataRange[2] * 2;
		if (ratio < 1.0)
		{	//可以显示全部的纵坐标
			
			for (i=0 ; i<11 ; i++)
			{
				str.Format("%.1f", yStart-i*yLength*0.1);
				pCdc->TextOut(m_YF[2].rcFrame.left+Y_COORDINATE_ALIGNRIGHT_TO_Y_FRAMERIGHT, m_YF[2].rcFrame.top+(i*m_YF[2].rcFrame.Height()/10) - (m_YF[2].rcFrame.Height()/60) , str);
			}
		} 
		else if(ratio < 2.0)
		{	//可以显示全部的纵坐标
			for (i=0 ; i<11 ; i++)
			{
				str.Format("%.1f", yStart-i*yLength*0.1);
				pCdc->TextOut(m_YF[2].rcFrame.left+Y_COORDINATE_ALIGNRIGHT_TO_Y_FRAMERIGHT, m_YF[2].rcFrame.top+(i*m_YF[2].rcFrame.Height()/10) - (m_YF[2].rcFrame.Height()/60) , str);
			}
		}
		else if(ratio < 4.0)
		{	//每两个个显示一个
			for (i=0 ; i<5 ; i++)
			{
				str.Format("%.1f", yStart-yLength*0.1-i*yLength*0.2);
				pCdc->TextOut(m_YF[2].rcFrame.left+Y_COORDINATE_ALIGNRIGHT_TO_Y_FRAMERIGHT, m_YF[2].rcFrame.top+(i*m_YF[2].rcFrame.Height()/7) - (m_YF[2].rcFrame.Height()/60) + m_YF[2].rcFrame.Height()/7 , str);
			}
		}
		else
		{	//每四个显示一个
			for (i=0 ; i<3 ; i++)
			{
				str.Format("%.1f", yStart-yLength*0.1-i*yLength*0.4);
				pCdc->TextOut(m_YF[2].rcFrame.left+Y_COORDINATE_ALIGNRIGHT_TO_Y_FRAMERIGHT, m_YF[2].rcFrame.top+(i*m_YF[2].rcFrame.Height()/5) - (m_YF[2].rcFrame.Height()/60) + m_YF[2].rcFrame.Height()/5 , str);
			}
		}
		str.Format("V");//纵坐标标题电压
		pCdc->TextOut(m_YF[2].rcFrame.left+15, m_YF[2].rcFrame.top+m_YF[2].rcFrame.Height()/2,str);		
		
	}
	if ( m_nWaveDataNum >= 4 )
	{
		//画m_YFLeftOne
		pCdc->SetTextAlign(TA_LEFT);
		if (pFont != NULL)
		{
			delete pFont;
			pFont = NULL;
		}
		pFont = new CFont;
		pFont->CreateFont(charH, charW, 0, 0, FW_BOLD, false, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_SWISS,_T("Times New Roman"));
		CFont* pOldFont = pCdc->SelectObject(pFont);
		pWaveData = m_WaveDataList.GetAt(m_WaveDataList.FindIndex(3));			//用和pWaveData一样的颜色
		pCdc->SetTextColor(pWaveData->m_LineColor);
		pCdc->SetBkMode(TRANSPARENT);
		
		ratio = (double)(charH) / (((double)(m_WDW.rcDisplay.Height()))/10);    //字符高 除以 十分之一的显示框架高
		float yStart;
		float yLength;
		yStart = m_nWaveDataRange[3];											//用针对pWaveData的量程
		yLength = m_nWaveDataRange[3] * 2;
		if (ratio < 1.0)
		{	//可以显示全部的纵坐标
			
			for (i=0 ; i<11 ; i++)
			{
				str.Format("%.1f", yStart-i*yLength*0.1);
				pCdc->TextOut(m_YF[3].rcFrame.right-Y_COORDINATE_ALIGNLEFT_TO_Y_FRAMELEFT, m_YF[3].rcFrame.top+(i*m_YF[3].rcFrame.Height()/10) - (m_YF[3].rcFrame.Height()/60) , str);
			}
		} 
		else if(ratio < 2.0)
		{	//可以显示全部的纵坐标
			for (i=0 ; i<11 ; i++)
			{
				str.Format("%.1f", yStart-i*yLength*0.1);
				pCdc->TextOut(m_YF[3].rcFrame.right-Y_COORDINATE_ALIGNLEFT_TO_Y_FRAMELEFT, m_YF[3].rcFrame.top+(i*m_YF[3].rcFrame.Height()/10) - (m_YF[3].rcFrame.Height()/60) , str);
			}
		}
		else if(ratio < 4.0)
		{	//每两个个显示一个
			for (i=0 ; i<5 ; i++)
			{
				str.Format("%.1f", yStart-yLength*0.1-i*yLength*0.2);
				pCdc->TextOut(m_YF[3].rcFrame.right-Y_COORDINATE_ALIGNLEFT_TO_Y_FRAMELEFT, m_YF[3].rcFrame.top+(i*m_YF[3].rcFrame.Height()/7) - (m_YF[3].rcFrame.Height()/60) + m_YF[3].rcFrame.Height()/7 , str);
			}
		}
		else
		{	//每四个显示一个
			for (i=0 ; i<3 ; i++)
			{
				str.Format("%.1f", yStart-yLength*0.1-i*yLength*0.4);
				pCdc->TextOut(m_YF[3].rcFrame.right-Y_COORDINATE_ALIGNLEFT_TO_Y_FRAMELEFT, m_YF[3].rcFrame.top+(i*m_YF[3].rcFrame.Height()/5) - (m_YF[3].rcFrame.Height()/60) + m_YF[3].rcFrame.Height()/5 , str);
			}
		}
		str.Format("V");//纵坐标标题电压
		pCdc->TextOut(m_YF[3].rcFrame.right-15, m_YF[3].rcFrame.top+m_YF[3].rcFrame.Height()/2,str);		
		
	}

	delete pFont;
	pFont = NULL;


	return 0;
}

int CWatchDlg::DrawYCoordinateFreq(CDC *pCdc)
{
	CString str;
	CString strTest;

	int i;
	int charH = 13;
	int charW = 5;
	double ratio;		//判断纵横坐标可容纳多少个坐标字段
	CFont* pFont = NULL;
	CWaveData* pWaveData;

	strTest.Format("Bunny Tips : m_nWaveDataNum : %d . \n" , m_nWaveDataNum);
	TRACE(strTest);

	/*纵坐标*/
	if ( m_nWaveDataNum >= 1 )
	{
		//画m_YFLeftOne
		pCdc->SetTextAlign(TA_RIGHT);
		if (pFont != NULL)
		{
			delete pFont;
			pFont = NULL;
		}
		pFont = new CFont;
		pFont->CreateFont(charH, charW, 0, 0, FW_BOLD, false, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_SWISS,_T("Times New Roman"));
		CFont* pOldFont = pCdc->SelectObject(pFont);
		pWaveData = m_WaveDataList.GetAt(m_WaveDataList.FindIndex(0));			//用和pWaveData一样的颜色
		pCdc->SetTextColor(pWaveData->m_LineColor);
		pCdc->SetBkMode(TRANSPARENT);
			
		ratio = (double)(charH) / (((double)(m_WDW.rcDisplay.Height()))/10);    //字符高 除以 十分之一的显示框架高
		float yStart;
		float yLength;

		//用针对pWaveData的量程
		switch ( m_nWaveDataRange[0] )
		{
		case RANGE_FREQ_V_0_500:
			yStart	= 500;				//这里yStart实际上是最大值，因为是递减的							
			yLength	= 500;
			break;
		case RANGE_FREQ_V_0_1000:
			yStart	= 1000;								
			yLength	= 1000;
			break;
		case RANGE_FREQ_DBV_n100_0:
			yStart	= 0;								
			yLength	= 100;
			break;
		case RANGE_FREQ_DBV_n100_100:
			yStart	= 100;								
			yLength	= 200;
			break;
		case RANGE_FREQ_DBu_0_100:
			yStart	= 100;								
			yLength	= 100;
			break;
		case RANGE_FREQ_DBu_0_200:
			yStart	= 200;								
			yLength	= 200;
			break;
		case RANGE_FREQ_DBm_n100_0:
			yStart	= 0;								
			yLength	= 100;
			break;
		case RANGE_FREQ_DBm_n100_100:
			yStart	= 100;								
			yLength	= 200;
			break;
		}


		strTest.Format("Bunny Tips : m_nWaveDataRange[0] : %d . \n",m_nWaveDataRange[0]);
		TRACE(strTest);
	
		

		if (ratio < 1.0)
		{	//可以显示全部的纵坐标
			
			for (i=0 ; i<11 ; i++)
			{
				str.Format("%.1f", yStart-i*yLength*0.1);
				pCdc->TextOut(m_YF[0].rcFrame.left+Y_COORDINATE_ALIGNRIGHT_TO_Y_FRAMERIGHT, m_YF[0].rcFrame.top+(i*m_YF[0].rcFrame.Height()/10) - (m_YF[0].rcFrame.Height()/60) , str);
			}
		} 
		else if(ratio < 2.0)
		{	//可以显示全部的纵坐标
			for (i=0 ; i<11 ; i++)
			{
				str.Format("%.1f", yStart-i*yLength*0.1);
				pCdc->TextOut(m_YF[0].rcFrame.left+Y_COORDINATE_ALIGNRIGHT_TO_Y_FRAMERIGHT, m_YF[0].rcFrame.top+(i*m_YF[0].rcFrame.Height()/10) - (m_YF[0].rcFrame.Height()/60) , str);
			}
		}
		else if(ratio < 4.0)
		{	//每两个个显示一个
			for (i=0 ; i<5 ; i++)
			{
				str.Format("%.1f", yStart-yLength*0.1-i*yLength*0.2);
				pCdc->TextOut(m_YF[0].rcFrame.left+Y_COORDINATE_ALIGNRIGHT_TO_Y_FRAMERIGHT, m_YF[0].rcFrame.top+(i*m_YF[0].rcFrame.Height()/7) - (m_YF[0].rcFrame.Height()/60) + m_YF[0].rcFrame.Height()/7 , str);
			}
		}
		else
		{	//每四个显示一个
			for (i=0 ; i<3 ; i++)
			{
				str.Format("%.1f", yStart-yLength*0.1-i*yLength*0.4);
				pCdc->TextOut(m_YF[0].rcFrame.left+Y_COORDINATE_ALIGNRIGHT_TO_Y_FRAMERIGHT, m_YF[0].rcFrame.top+(i*m_YF[0].rcFrame.Height()/5) - (m_YF[0].rcFrame.Height()/60) + m_YF[0].rcFrame.Height()/5 , str);
			}
		}
		if ( m_pProduct->m_FftConf.nOutputMode == YAXIS_UNIT_V )
		{
			/*在频域分析中，只有在线性分析中才会有单位mV*/
			str.Format("mV");//纵坐标标题电压
			pCdc->TextOut(m_YF[0].rcFrame.left+5, m_YF[0].rcFrame.top+m_YF[0].rcFrame.Height()/2+m_YF[0].rcFrame.Height()/40,str);
		}
	}

	if ( m_nWaveDataNum >= 2 )
	{
		//画m_YFRightOne
		pCdc->SetTextAlign(TA_LEFT);
		if (pFont != NULL)
		{
			delete pFont;
			pFont = NULL;
		}
		pFont = new CFont;
		pFont->CreateFont(charH, charW, 0, 0, FW_BOLD, false, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_SWISS,_T("Times New Roman"));
		CFont* pOldFont = pCdc->SelectObject(pFont);
		pWaveData = m_WaveDataList.GetAt(m_WaveDataList.FindIndex(1));			//用和pWaveData一样的颜色
		pCdc->SetTextColor(pWaveData->m_LineColor);
		pCdc->SetBkMode(TRANSPARENT);
		
		ratio = (double)(charH) / (((double)(m_WDW.rcDisplay.Height()))/10);    //字符高 除以 十分之一的显示框架高
		float yStart;
		float yLength;
	
		//用针对pWaveData的量程
		switch ( m_nWaveDataRange[1] )
		{
		case RANGE_FREQ_V_0_500:
			yStart	= 500;				//这里yStart实际上是最大值，因为是递减的							
			yLength	= 500;
			break;
		case RANGE_FREQ_V_0_1000:
			yStart	= 1000;								
			yLength	= 1000;
			break;
		case RANGE_FREQ_DBV_n100_0:
			yStart	= 0;								
			yLength	= 100;
			break;
		case RANGE_FREQ_DBV_n100_100:
			yStart	= 100;								
			yLength	= 200;
			break;
		case RANGE_FREQ_DBu_0_100:
			yStart	= 100;								
			yLength	= 100;
			break;
		case RANGE_FREQ_DBu_0_200:
			yStart	= 200;								
			yLength	= 200;
			break;
		case RANGE_FREQ_DBm_n100_0:
			yStart	= 0;								
			yLength	= 100;
			break;
		case RANGE_FREQ_DBm_n100_100:
			yStart	= 100;								
			yLength	= 200;
			break;
		}

		if (ratio < 1.0)
		{	//可以显示全部的纵坐标
			
			for (i=0 ; i<11 ; i++)
			{
				str.Format("%.1f", yStart-i*yLength*0.1);
				pCdc->TextOut(m_YF[1].rcFrame.right-Y_COORDINATE_ALIGNLEFT_TO_Y_FRAMELEFT, m_YF[1].rcFrame.top+(i*m_YF[1].rcFrame.Height()/10) - (m_YF[1].rcFrame.Height()/60) , str);
			}
		} 
		else if(ratio < 2.0)
		{	//可以显示全部的纵坐标
			for (i=0 ; i<11 ; i++)
			{
				str.Format("%.1f", yStart-i*yLength*0.1);
				pCdc->TextOut(m_YF[1].rcFrame.right-Y_COORDINATE_ALIGNLEFT_TO_Y_FRAMELEFT, m_YF[1].rcFrame.top+(i*m_YF[1].rcFrame.Height()/10) - (m_YF[1].rcFrame.Height()/60) , str);
			}
		}
		else if(ratio < 4.0)
		{	//每两个个显示一个
			for (i=0 ; i<5 ; i++)
			{
				str.Format("%.1f", yStart-yLength*0.1-i*yLength*0.2);
				pCdc->TextOut(m_YF[1].rcFrame.right-Y_COORDINATE_ALIGNLEFT_TO_Y_FRAMELEFT, m_YF[1].rcFrame.top+(i*m_YF[1].rcFrame.Height()/7) - (m_YF[1].rcFrame.Height()/60) + m_YF[1].rcFrame.Height()/7 , str);
			}
		}
		else
		{	//每四个显示一个
			for (i=0 ; i<3 ; i++)
			{
				str.Format("%.1f", yStart-yLength*0.1-i*yLength*0.4);
				pCdc->TextOut(m_YF[1].rcFrame.right-Y_COORDINATE_ALIGNLEFT_TO_Y_FRAMELEFT, m_YF[1].rcFrame.top+(i*m_YF[1].rcFrame.Height()/5) - (m_YF[1].rcFrame.Height()/60) + m_YF[1].rcFrame.Height()/5 , str);
			}
		}
		if ( m_pProduct->m_FftConf.nOutputMode == YAXIS_UNIT_V )
		{
			/*在频域分析中，只有在线性分析中才会有单位mV*/
			str.Format("mV");//纵坐标标题电压
			pCdc->TextOut(m_YF[1].rcFrame.right-5, m_YF[1].rcFrame.top+m_YF[1].rcFrame.Height()/2+m_YF[1].rcFrame.Height()/40,str);	
		}	
	}
	if ( m_nWaveDataNum >= 3 )
	{
		//画m_YFLeftTwo
		pCdc->SetTextAlign(TA_RIGHT);
		if (pFont != NULL)
		{
			delete pFont;
			pFont = NULL;
		}
		pFont = new CFont;
		pFont->CreateFont(charH, charW, 0, 0, FW_BOLD, false, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_SWISS,_T("Times New Roman"));
		CFont* pOldFont = pCdc->SelectObject(pFont);
		pWaveData = m_WaveDataList.GetAt(m_WaveDataList.FindIndex(2));			//用和pWaveData一样的颜色
		pCdc->SetTextColor(pWaveData->m_LineColor);
		pCdc->SetBkMode(TRANSPARENT);
		
		ratio = (double)(charH) / (((double)(m_WDW.rcDisplay.Height()))/10);    //字符高 除以 十分之一的显示框架高
		float yStart;
		float yLength;

		//用针对pWaveData的量程
		switch ( m_nWaveDataRange[2] )
		{
		case RANGE_FREQ_V_0_500:
			yStart	= 500;				//这里yStart实际上是最大值，因为是递减的							
			yLength	= 500;
			break;
		case RANGE_FREQ_V_0_1000:
			yStart	= 1000;								
			yLength	= 1000;
			break;
		case RANGE_FREQ_DBV_n100_0:
			yStart	= 0;								
			yLength	= 100;
			break;
		case RANGE_FREQ_DBV_n100_100:
			yStart	= 100;								
			yLength	= 200;
			break;
		case RANGE_FREQ_DBu_0_100:
			yStart	= 100;								
			yLength	= 100;
			break;
		case RANGE_FREQ_DBu_0_200:
			yStart	= 200;								
			yLength	= 200;
			break;
		case RANGE_FREQ_DBm_n100_0:
			yStart	= 0;								
			yLength	= 100;
			break;
		case RANGE_FREQ_DBm_n100_100:
			yStart	= 100;								
			yLength	= 200;
			break;
		}

		if (ratio < 1.0)
		{	//可以显示全部的纵坐标
			
			for (i=0 ; i<11 ; i++)
			{
				str.Format("%.1f", yStart-i*yLength*0.1);
				pCdc->TextOut(m_YF[2].rcFrame.left+Y_COORDINATE_ALIGNRIGHT_TO_Y_FRAMERIGHT, m_YF[2].rcFrame.top+(i*m_YF[2].rcFrame.Height()/10) - (m_YF[2].rcFrame.Height()/60) , str);
			}
		} 
		else if(ratio < 2.0)
		{	//可以显示全部的纵坐标
			for (i=0 ; i<11 ; i++)
			{
				str.Format("%.1f", yStart-i*yLength*0.1);
				pCdc->TextOut(m_YF[2].rcFrame.left+Y_COORDINATE_ALIGNRIGHT_TO_Y_FRAMERIGHT, m_YF[2].rcFrame.top+(i*m_YF[2].rcFrame.Height()/10) - (m_YF[2].rcFrame.Height()/60) , str);
			}
		}
		else if(ratio < 4.0)
		{	//每两个个显示一个
			for (i=0 ; i<5 ; i++)
			{
				str.Format("%.1f", yStart-yLength*0.1-i*yLength*0.2);
				pCdc->TextOut(m_YF[2].rcFrame.left+Y_COORDINATE_ALIGNRIGHT_TO_Y_FRAMERIGHT, m_YF[2].rcFrame.top+(i*m_YF[2].rcFrame.Height()/7) - (m_YF[2].rcFrame.Height()/60) + m_YF[2].rcFrame.Height()/7 , str);
			}
		}
		else
		{	//每四个显示一个
			for (i=0 ; i<3 ; i++)
			{
				str.Format("%.1f", yStart-yLength*0.1-i*yLength*0.4);
				pCdc->TextOut(m_YF[2].rcFrame.left+Y_COORDINATE_ALIGNRIGHT_TO_Y_FRAMERIGHT, m_YF[2].rcFrame.top+(i*m_YF[2].rcFrame.Height()/5) - (m_YF[2].rcFrame.Height()/60) + m_YF[2].rcFrame.Height()/5 , str);
			}
		}
		if ( m_pProduct->m_FftConf.nOutputMode == YAXIS_UNIT_V )
		{
			/*在频域分析中，只有在线性分析中才会有单位mV*/
			str.Format("mV");//纵坐标标题电压
			pCdc->TextOut(m_YF[2].rcFrame.left+5, m_YF[2].rcFrame.top+m_YF[2].rcFrame.Height()/2+m_YF[2].rcFrame.Height()/40,str);	
		}	
		
	}
	if ( m_nWaveDataNum >= 4 )
	{
		//画m_YFLeftOne
		pCdc->SetTextAlign(TA_LEFT);
		if (pFont != NULL)
		{
			delete pFont;
			pFont = NULL;
		}
		pFont = new CFont;
		pFont->CreateFont(charH, charW, 0, 0, FW_BOLD, false, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_SWISS,_T("Times New Roman"));
		CFont* pOldFont = pCdc->SelectObject(pFont);
		pWaveData = m_WaveDataList.GetAt(m_WaveDataList.FindIndex(3));			//用和pWaveData一样的颜色
		pCdc->SetTextColor(pWaveData->m_LineColor);
		pCdc->SetBkMode(TRANSPARENT);
		
		ratio = (double)(charH) / (((double)(m_WDW.rcDisplay.Height()))/10);    //字符高 除以 十分之一的显示框架高
		float yStart;
		float yLength;
	
		//用针对pWaveData的量程
		switch ( m_nWaveDataRange[3] )
		{
		case RANGE_FREQ_V_0_500:
			yStart	= 500;				//这里yStart实际上是最大值，因为是递减的							
			yLength	= 500;
			break;
		case RANGE_FREQ_V_0_1000:
			yStart	= 1000;								
			yLength	= 1000;
			break;
		case RANGE_FREQ_DBV_n100_0:
			yStart	= 0;								
			yLength	= 100;
			break;
		case RANGE_FREQ_DBV_n100_100:
			yStart	= 100;								
			yLength	= 200;
			break;
		case RANGE_FREQ_DBu_0_100:
			yStart	= 100;								
			yLength	= 100;
			break;
		case RANGE_FREQ_DBu_0_200:
			yStart	= 200;								
			yLength	= 200;
			break;
		case RANGE_FREQ_DBm_n100_0:
			yStart	= 0;								
			yLength	= 100;
			break;
		case RANGE_FREQ_DBm_n100_100:
			yStart	= 100;								
			yLength	= 200;
			break;
		}

		if (ratio < 1.0)
		{	//可以显示全部的纵坐标
			
			for (i=0 ; i<11 ; i++)
			{
				str.Format("%.1f", yStart-i*yLength*0.1);
				pCdc->TextOut(m_YF[3].rcFrame.right-Y_COORDINATE_ALIGNLEFT_TO_Y_FRAMELEFT, m_YF[3].rcFrame.top+(i*m_YF[3].rcFrame.Height()/10) - (m_YF[3].rcFrame.Height()/60) , str);
			}
		} 
		else if(ratio < 2.0)
		{	//可以显示全部的纵坐标
			for (i=0 ; i<11 ; i++)
			{
				str.Format("%.1f", yStart-i*yLength*0.1);
				pCdc->TextOut(m_YF[3].rcFrame.right-Y_COORDINATE_ALIGNLEFT_TO_Y_FRAMELEFT, m_YF[3].rcFrame.top+(i*m_YF[3].rcFrame.Height()/10) - (m_YF[3].rcFrame.Height()/60) , str);
			}
		}
		else if(ratio < 4.0)
		{	//每两个个显示一个
			for (i=0 ; i<5 ; i++)
			{
				str.Format("%.1f", yStart-yLength*0.1-i*yLength*0.2);
				pCdc->TextOut(m_YF[3].rcFrame.right-Y_COORDINATE_ALIGNLEFT_TO_Y_FRAMELEFT, m_YF[3].rcFrame.top+(i*m_YF[3].rcFrame.Height()/7) - (m_YF[3].rcFrame.Height()/60) + m_YF[3].rcFrame.Height()/7 , str);
			}
		}
		else
		{	//每四个显示一个
			for (i=0 ; i<3 ; i++)
			{
				str.Format("%.1f", yStart-yLength*0.1-i*yLength*0.4);
				pCdc->TextOut(m_YF[3].rcFrame.right-Y_COORDINATE_ALIGNLEFT_TO_Y_FRAMELEFT, m_YF[3].rcFrame.top+(i*m_YF[3].rcFrame.Height()/5) - (m_YF[3].rcFrame.Height()/60) + m_YF[3].rcFrame.Height()/5 , str);
			}
		}
		
		if ( m_pProduct->m_FftConf.nOutputMode == YAXIS_UNIT_V )
		{
			/*在频域分析中，只有在线性分析中才会有单位mV*/
			str.Format("mV");//纵坐标标题电压
			pCdc->TextOut(m_YF[3].rcFrame.right-5, m_YF[3].rcFrame.top+m_YF[3].rcFrame.Height()/2+m_YF[3].rcFrame.Height()/40,str);	
		}	
		
	}

	delete pFont;
	pFont = NULL;

	return 0;
}

int CWatchDlg::DrawXCoordinateTime(CDC *pCdc)
{
	/*横坐标*/
	AcqInfo* pAcqInfo = &(m_pProduct->m_AcqInfo);
	CString str;
	int i = 0;
	//	CString strTest;
	
	int charH = 13;
	int charW = 5;
	double ratio;		//判断纵横坐标可容纳多少个坐标字段

	CWaveData* pWaveData;
	
	
	pAcqInfo->samplingTime = (double)(pAcqInfo->samplingLength)/(double)(pAcqInfo->samplingRate);
	ratio = (double)(charW) / (((double)(m_WDW.rcDisplay.Width()))/10);    //字符宽 除以 十分之一的显示框架宽
	int xStart;
	double xLength;
	xStart = pAcqInfo->samplingTime *(m_WDW.rcDisplay.left+0.0-m_WDW.rcReal.left)/m_WDW.rcReal.Width();
	//	xLength = (m_WDW.rcDisplay.Width()+0.0)/m_WDW.rcReal.Width()*pAcqInfo->samplingTime;
	ComputeHorUnitTime(&xLength , (double)(m_pProduct->m_nPlayWndWidth)/(double)(pAcqInfo->samplingRate));
	//	TRACE("-***-pProduct->m_nPlayStartIndex : %d------\n",pProduct->m_nPlayStartIndex);
	TRACE("-***-m_pProduct->m_nPlayWndWidth : %d------\n",m_pProduct->m_nPlayWndWidth);
	//	TRACE("-***-pProduct->m_AcqInfo.samplingLength : %d------\n",pProduct->m_AcqInfo.samplingLength);
	
	pCdc->SetTextAlign(TA_LEFT);
	pCdc->SetTextColor(RGB(255,255,255));
	if (ratio < 0.2)
	{	//可以显示全部的横坐标
		// 		for (i=0 ; i<21 ; i++)
		// 		{
		// 			str.Format("%.1f", pAcqInfo->range *(1-0.1*i));
		// 			pCdc->TextOut(m_WDW.rcDisplay.left-m_rcWatchDlgFrame.Width()/20, m_WDW.rcDisplay.top+(i*m_WDW.rcDisplay.Height()/20) - (m_WDW.rcDisplay.Height()/30),str);
		// 		}
		for (i=0 ; i<11 ; i++)
		{
			str.Format("%.3f", xStart + xLength*i*0.1);
			pCdc->TextOut(m_WDW.rcDisplay.left-m_rcWatchDlgFrame.Width()/50 + i*m_WDW.rcDisplay.Width()/10, m_WDW.rcDisplay.bottom + (m_WDW.rcDisplay.Height()/60),str);
		}
	} 
	else if(ratio < 0.25)
	{	//可以显示全部的横坐标
		for (i=0 ; i<11 ; i++)
		{
			str.Format("%.3f", xStart + xLength*i*0.1);
			pCdc->TextOut(m_WDW.rcDisplay.left-m_rcWatchDlgFrame.Width()/50 + i*m_WDW.rcDisplay.Width()/10, m_WDW.rcDisplay.bottom + (m_WDW.rcDisplay.Height()/60),str);
		}
	}
	else if(ratio < 0.4)
	{	//每两个个显示一个
		for (i=0 ; i<5 ; i++)
		{
			str.Format("%.3f", xStart + xLength*i*0.2);
			pCdc->TextOut(m_WDW.rcDisplay.left-m_rcWatchDlgFrame.Width()/50 + i*m_WDW.rcDisplay.Width()/5, m_WDW.rcDisplay.bottom + (m_WDW.rcDisplay.Height()/60),str);
		}
	}
	else
	{	//每四个显示一个
		for (i=0 ; i<3 ; i++)
		{
			str.Format("%.3f", xStart + xLength*i*0.4);
			pCdc->TextOut(m_WDW.rcDisplay.left-m_rcWatchDlgFrame.Width()/50 + i*m_WDW.rcDisplay.Width()*4/10, m_WDW.rcDisplay.bottom + (m_WDW.rcDisplay.Height()/60),str);
		}
	}
	//	CFont *pVFont;
	//	pVFont = new CFont;					//竖着显示的字体
	//	pVFont->CreateFont(charH, charW, 900, 0, FW_BOLD, TRUE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_SWISS,_T("Times New Roman"));
	//	pCdc->SelectObject(pVFont);
	//	str.Format("  ms");//横坐标标题时间
	str = ComputeHorUnitTime(&xLength , (double)(m_pProduct->m_nPlayWndWidth)/(double)(pAcqInfo->samplingRate));
	pCdc->TextOut(m_WDW.rcDisplay.right - m_WDW.rcDisplay.Width()/15, m_WDW.rcDisplay.bottom + 13,str);
	
	//	pCdc->SelectObject(pOldFont);

	return 0;
}

int CWatchDlg::DrawXCoordinateFreq(CDC *pCdc)
{

	/*横坐标*/
	AcqInfo* pAcqInfo = &(m_pProduct->m_AcqInfo);
	int nSampleRate = pAcqInfo->samplingRate;
	int nAvaliableSampleRate = nSampleRate / 2;					//有效采样率为真实采样率的一半，奈奎斯特采样定律

	CString strUnit;
	int i = 0;
	CString str;
	
	int charH = 13;
	int charW = 5;
	double ratio;		//判断纵横坐标可容纳多少个坐标字段
	
	CWaveData* pWaveData;
	
	
	pAcqInfo->samplingTime = (double)(pAcqInfo->samplingLength)/(double)(pAcqInfo->samplingRate);
	ratio = (double)(charW) / (((double)(m_WDW.rcDisplay.Width()))/10);    //字符宽 除以 十分之一的显示框架宽
	int xStart;
	double xLength;
	xStart = pAcqInfo->samplingTime *(m_WDW.rcDisplay.left+0.0-m_WDW.rcReal.left)/m_WDW.rcReal.Width();
	//	xLength = (m_WDW.rcDisplay.Width()+0.0)/m_WDW.rcReal.Width()*pAcqInfo->samplingTime;
	strUnit = ComputeHorUnitFreq( &xLength , (double)nAvaliableSampleRate );
	//	TRACE("-***-pProduct->m_nPlayStartIndex : %d------\n",pProduct->m_nPlayStartIndex);
	TRACE("-***-m_pProduct->m_nPlayWndWidth : %d------\n",m_pProduct->m_nPlayWndWidth);
	//	TRACE("-***-pProduct->m_AcqInfo.samplingLength : %d------\n",pProduct->m_AcqInfo.samplingLength);
	
	pCdc->SetTextAlign(TA_LEFT);
	pCdc->SetTextColor(RGB(255,255,255));
	if (ratio < 0.2)
	{	//可以显示全部的横坐标
		// 		for (i=0 ; i<21 ; i++)
		// 		{
		// 			str.Format("%.1f", pAcqInfo->range *(1-0.1*i));
		// 			pCdc->TextOut(m_WDW.rcDisplay.left-m_rcWatchDlgFrame.Width()/20, m_WDW.rcDisplay.top+(i*m_WDW.rcDisplay.Height()/20) - (m_WDW.rcDisplay.Height()/30),str);
		// 		}
		for (i=0 ; i<11 ; i++)
		{
			str.Format("%.1f", xStart + xLength*i*0.1);
			pCdc->TextOut(m_WDW.rcDisplay.left-m_rcWatchDlgFrame.Width()/50 + i*m_WDW.rcDisplay.Width()/10, m_WDW.rcDisplay.bottom + (m_WDW.rcDisplay.Height()/60),str);
		}
	} 
	else if(ratio < 0.25)
	{	//可以显示全部的横坐标
		for (i=0 ; i<11 ; i++)
		{
			str.Format("%.1f", xStart + xLength*i*0.1);
			pCdc->TextOut(m_WDW.rcDisplay.left-m_rcWatchDlgFrame.Width()/50 + i*m_WDW.rcDisplay.Width()/10, m_WDW.rcDisplay.bottom + (m_WDW.rcDisplay.Height()/60),str);
		}
	}
	else if(ratio < 0.4)
	{	//每两个个显示一个
		for (i=0 ; i<5 ; i++)
		{
			str.Format("%.1f", xStart + xLength*i*0.2);
			pCdc->TextOut(m_WDW.rcDisplay.left-m_rcWatchDlgFrame.Width()/50 + i*m_WDW.rcDisplay.Width()/5, m_WDW.rcDisplay.bottom + (m_WDW.rcDisplay.Height()/60),str);
		}
	}
	else
	{	//每四个显示一个
		for (i=0 ; i<3 ; i++)
		{
			str.Format("%.1f", xStart + xLength*i*0.4);
			pCdc->TextOut(m_WDW.rcDisplay.left-m_rcWatchDlgFrame.Width()/50 + i*m_WDW.rcDisplay.Width()*4/10, m_WDW.rcDisplay.bottom + (m_WDW.rcDisplay.Height()/60),str);
		}
	}
	//	CFont *pVFont;
	//	pVFont = new CFont;					//竖着显示的字体
	//	pVFont->CreateFont(charH, charW, 900, 0, FW_BOLD, TRUE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_SWISS,_T("Times New Roman"));
	//	pCdc->SelectObject(pVFont);
	//	str.Format("  ms");//横坐标标题时间
	//str = ComputeHorUnitTime(&xLength , (double)(m_pProduct->m_nPlayWndWidth)/(double)(pAcqInfo->samplingRate));
	pCdc->TextOut(m_WDW.rcDisplay.right - m_WDW.rcDisplay.Width()/15, m_WDW.rcDisplay.bottom + 13,strUnit);

	return 0;
}

CString CWatchDlg::ComputeHorUnitFreq(double *pfResult, double fInput)
{
	CString strResult;
	if ( fInput/1000000 >= 1.0 )
	{
		*pfResult = fInput/1000000;
		strResult.Format("  MHz"); 
	}
	else if ( fInput/1000 >= 1.0 )
	{
		*pfResult = fInput/1000;
		strResult.Format("  kHz"); 
	}
	else{
		*pfResult = fInput;
		strResult.Format("  Hz"); 
	}

	return strResult;
}

int CWatchDlg::ChangeAllToolBars()
{
	
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();

	/*改变显示FFT/TIME按钮的工具栏*/
	if ( m_nMode == DISPLAY_MODE_TIME )
	{
		pFrame->m_aWatchMode[0] = 1;
		pFrame->m_aWatchMode[1] = 0;
	}
	else{		
		pFrame->m_aWatchMode[0] = 0;
		pFrame->m_aWatchMode[1] = 1;
	}
	

	/*改变显示通道的工具栏*/
	pFrame->m_pDlgBarExTopChnSel->ChangeToolBarDisplayChn(this);

	

	return 1;
}

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
	m_AcqInfo.range=10;											//��ʼ���� 10v	
	m_AcqInfo.samplingLength = 1048576;							//��ʼ�ɼ����� 
	m_AcqInfo.samplingRate = 1000000;							//��ʼ����Ƶ��1M

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

	//����ͨ���ر�
	m_nWaveDataRange[0] = 0;
	m_nWaveDataRange[1] = 0;
	m_nWaveDataRange[2] = 0;
	m_nWaveDataRange[3] = 0;

//////m_XMF,m_YMF�ṹ�帳ֵ///////////////////////////////////////////////////////////////////
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
	// Step 1:Ϊ��ĻDC�������ݵ��ڴ�DC :CreateCompatibleDC()

//	TRACE("========into paint: rect %d  %d  %d  %d  ==================\n",rc.top ,rc.bottom ,rc.left , rc.right);

	dcMem.CreateCompatibleDC(&dc);
	// Step 2:����λͼ��CreateCompatibleBitmap()
	bm.CreateCompatibleBitmap(&dc,rc.Width(),rc.Height());
	// Step 3:��λͼѡ���豸������SelectObject()���������Ϊѡ�񻭲�
	dcMem.SelectObject(&bm); 
	// dcMem.FillSolidRect(0,0,rc.Width(),rc.Height(),RGB(255,255,255));
	//dcMem.FillSolidRect(rc,dc.GetBkColor());
	dcMem.FillSolidRect(rc,RGB(0,0,0));
	//	dcMem.SetBkMode(TRANSPARENT);//����͸������ɫ
	//	dcMem.TextOut(-1,-3,"888888888888");
	//���ƶ������� ��ʾ����%��
//	DrawTitle(&dcMem);
	//���ƹ�����
//	DrawCursorRead(&dcMem);
	//����bg
	DrawBG(&dcMem);

	//��������
// 	CString str;
// 	str.Format("window %d",m_nIndexInWaveList);
// 	dcMem.TextOut(10,10,str);

	//���Ʋ���
	//TRACE("m_pProduct->m_bExistData : %d\n" , m_pProduct->m_bExistData);

	if (m_pProduct->m_bExistData)				//����ҲҪ��  �ж��Ƿ�DrawWave Ӧ�ÿ�product�����Ƿ�������  �����ǿ�m_bAcqing
	{
		DrawWave(&dcMem);
	}

	//����ѡ���ķ���
//	DrawSelectedBorder(&dcMem);

	//����Mark Sacle �Լ�����������������
	DrawMark(&dcMem);

	//������ZoomDrag�����ľ��ο�
	if (m_bZoomDragIsPick)
	{
		DrawZoomDragBorder(&dcMem);
	}

/*
	//test��������
	CPen pen(PS_SOLID,1,RGB(200,200,200));
	dcMem.SelectObject(&pen);
	CBrush *pBrush=CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
	dcMem.SelectObject(pBrush);
	dcMem.Rectangle(m_YF[0].rcFrame);
*/

	
	// Step 4:�ѻ��ƺõ�ͼ�Ρ�����������Ļ��:BitBlt()
	dc.BitBlt(0,0,rc.Width(),rc.Height(),&dcMem,0,0,SRCCOPY);
	dcMem.DeleteDC();
	bm.DeleteObject();
	//����ʵ��
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
	int nDrawPixelWidth = 0;		//����m_WDW�����ؿ�
	int originX = 0;			//Դ������xֵ
	int originY = 0;			//Դ������yֵ
	int nHeightPixel = 0;
	int nHalfHeight = 0;			//m_WDWһ������ظ�
	int nCoun = 0;	
	int nFirstY = 0;			//ǰһ�����Yֵ
	int nSecondY = 0;			//��һ�����Yֵ
	int nRange = 0;				//��ǰͨ��������
	int nCount = 0;


	nDrawPixelWidth = m_WDW.rcDisplay.Width();
	//short* pDataBuffer = (short*)malloc(nDrawPixelWidth * sizeof(short));
	short* pDataBuffer = NULL;
	
	//Ҫ����ȫ������
	//���������ʱ ����Ӧ������ѭ��

	if ( m_nMode == 0)
	{
		//ʱ��	
		pDataBuffer = (short*)malloc(nDrawPixelWidth * sizeof(short));		//����Ҫ��ʾ������

		int nWaveIndex=0;
		for ( nWaveIndex=0 ; nWaveIndex<DRAW_CHNUM_MAX ; nWaveIndex++)
		{
			if ( m_nWaveDataNum >= nWaveIndex+1 )
			{
				/*start ��һ��Wave*/
				pWaveData = m_WaveDataList.GetAt(m_WaveDataList.FindIndex(nWaveIndex));
				
				
				CPen mypen(PS_SOLID,1,pWaveData->m_LineColor);
				pOldPen = pDC->SelectObject (&mypen);
				
				//nDrawPixelWidth = m_WDW.rcDisplay.Width();
				//short* pDataBuffer = (short*)malloc(drawPixelCount * sizeof(short));
				m_pProduct->GetDataFromHDD(	pWaveData->m_nChnIndex,			//����Ҫ��  ���ڹ̶�ȡ0ͨ������
					m_pProduct->m_nPlayStartIndex,
					m_pProduct->m_nPlayWndWidth/nDrawPixelWidth,
					pDataBuffer,
					nDrawPixelWidth);
				/*����MeasureDlg�ϵ�����*/
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
				/*end ��һ��wave*/
			}

		}

	}
	else{
		//Ƶ��
		pDataBuffer = (short*)malloc(m_pProduct->m_nPlayWndWidth * sizeof(short));		//����Ҫ��FFT������

		int nWaveIndex=0;
		for ( nWaveIndex=0 ; nWaveIndex<DRAW_CHNUM_MAX ; nWaveIndex++)
		{
			if ( m_nWaveDataNum >= nWaveIndex+1 )
			{
				/*start ��һ��Wave*/
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
				/*FFT����*/
				m_pProduct->m_pFftModule->FftConf(m_pProduct->m_FftConf.nWindowMode , 1000000/*���ֵ����û��ë��*/ , m_pProduct->m_FftConf.nFftBins , m_pProduct->m_FftConf.nOutputMode);
				m_pProduct->m_pFftModule->FftRun(pDoubleDataBuffer, 
												m_pProduct->m_nPlayWndWidth, 
												pXAxisArray, 
												&nXAxisLen,   
												m_pProduct->m_aFftResult[pWaveData->m_nChnIndex] , 
												&nYAxisLen);//fft

				free(pXAxisArray );
				free(pDoubleDataBuffer);

				/*����MeasureDlg�ϵ����� ����Ӧ�÷�m_pProduct->m_aFftResult[pWaveData->m_nChnIndex]������*/
				if (NULL != m_pMeasureDlg){
					m_pMeasureDlg->ComputeItemFreq(pWaveData->m_nChnIndex , m_pProduct->m_aFftResult[pWaveData->m_nChnIndex] , nXAxisLen);
				}

				originX = m_WDW.rcDisplay.left;
				originY = m_WDW.rcDisplay.bottom;
				nHalfHeight = m_WDW.rcDisplay.Height()/2;
				nHeightPixel = m_WDW.rcDisplay.Height();
				nCount=0;

				nRange = 1;		//����Ҫ��

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
				/*end ��һ��wave*/
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
	//������Χ��ܾ��δ�С
	m_rcWatchDlgFrame = rc;
	//�����ڲ�������ʾ��ܴ�СWatchDlgFrame
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
	/*������*/
	CPen* pOldPen;
	//CPen mypen(PS_DOT ,1,RGB(173,216,230));
	CPen mypen(PS_DOT ,1,RGB(50,50,50));
	pOldPen = pCdc->SelectObject (&mypen);
	int i,j;
	CString str;
	for (i=0 ; i<10 ; i++)
	{	//����
		pCdc->MoveTo(m_WDW.rcDisplay.left + i*m_WDW.rcDisplay.Width()/10 , m_WDW.rcDisplay.top );
		pCdc->LineTo(m_WDW.rcDisplay.left + i*m_WDW.rcDisplay.Width()/10 , m_WDW.rcDisplay.bottom );
	}
	pCdc->MoveTo(m_WDW.rcDisplay.right , m_WDW.rcDisplay.top );
	pCdc->LineTo(m_WDW.rcDisplay.right , m_WDW.rcDisplay.bottom );
	for (j=0 ; j<10 ; j++)
	{	//����
		pCdc->MoveTo(m_WDW.rcDisplay.left , m_WDW.rcDisplay.top + j*m_WDW.rcDisplay.Height()/10 );
		pCdc->LineTo(m_WDW.rcDisplay.right , m_WDW.rcDisplay.top +  j*m_WDW.rcDisplay.Height()/10 );
	}
	pCdc->MoveTo(m_WDW.rcDisplay.left , m_WDW.rcDisplay.bottom);
	pCdc->LineTo(m_WDW.rcDisplay.right , m_WDW.rcDisplay.bottom);
	pCdc->SelectObject(pOldPen);
//////////////////////////////////////////////////////////////////////////
	/*������*/

	/*������*/
	if ( m_nMode == DISPLAY_MODE_TIME)
	{
		//��ʱ���Y������
		DrawYCoordinateTime(pCdc);
	}
	else{
		//��Ƶ���Y������
		DrawYCoordinateFreq(pCdc);
	}

	/*������*/
	if ( m_nMode == DISPLAY_MODE_TIME)
	{
		//��ʱ���Y������
		DrawXCoordinateTime(pCdc);
	}
	else{
		//��Ƶ���Y������
		DrawXCoordinateFreq(pCdc);
	}
		
	
	

}

void CWatchDlg::DrawTitle(CDC *pDC)
{
	CBrush mybrush(RGB(239,235,224));
//	CBrush* pOldBrush  = pDC->SelectObject(&mybrush);
	pDC->FillRect(CRect(0,0,m_rcWatchDlgFrame.Width(),20),&mybrush);
	CString str;
	str.Format("ʾ���� %d",1);
	//pDC->TextOut(0,0,str);
	GetDlgItem(IDC_STATIC_Title)->MoveWindow(CRect(2,2,m_rcWatchDlgFrame.Width()-4,22));
	GetDlgItem(IDC_STATIC_Title)->SetWindowText(str);

}

HBRUSH CWatchDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	if( GetDlgItem(IDC_STATIC_Title)->m_hWnd == pWnd->m_hWnd ) //IDC_STATICMΪ��Ҫ�ı���ɫ��Static�ؼ���ԴID
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
			case VK_RETURN: //�س�  
				return true;  
			case VK_ESCAPE: //ESC  
				return true;  	
		}  
	}  
	return CDialog::PreTranslateMessage(pMsg);
}

void CWatchDlg::OnOk()//ɶҲ������OK��  
{  
	
}  

void CWatchDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	//���Ƚ���WatchDlg������BgDlg��Ϊѡ��
	SetOwnedBgDlgSeleted();

//	TRACE("======click at: %d , %d ================================================\n",point.x,point.y);
//	TRACE("======m_YMF.rcPosMarkY_1 at: %d , %d , %d , %d =========================\n",m_YMF.rcPosMarkY_1.left,m_YMF.rcPosMarkY_1.top,m_YMF.rcPosMarkY_1.right,m_YMF.rcPosMarkY_1.bottom);

//	m_bMarkIsPick = true;
	m_ptMouseMove = point;
	//====================����ĳ��MARK=============================================================
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

			
		//====================����Ŵ���С����=============================================================
			int MouseMode =((CMainFrame*)AfxGetMainWnd())->m_nMouseMode;
/*
			if (((CRecorderView*)GetParent())->m_WndManage.m_nCurSelect == m_nIndexInWaveList)
			{
*/
			/*return;*/
			//���ݵ�ǰ�����ѡ����в���
			if (MouseMode == MOUSEMODE_ZOOMIN)
			{
				//�Ŵ� ����
				CString str;
				str.Format("�����ǰλ��Ϊ%d,%d\n",point.x,point.y);
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

void CWatchDlg::DrawSelectedBorder(CDC *pCdc)		//��������Ѳ��ٵ���  ����ѡ�б߿�Ĵ��������CBgDlg��OnPaint������
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
	//������
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
	//��������
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
	//����
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
		//�Ŵ����Ѿ����
		return;
	}


	//step1 ���е���ĵ����������
	if (pt->x<m_WDW.rcDisplay.left || pt->x>m_WDW.rcDisplay.right || pt->y<m_WDW.rcDisplay.top || pt->y>m_WDW.rcDisplay.bottom )
	{	//�����rcDisplay֮��
		return;
	} 
	else 
	{	//�����rcDisplay֮��
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
	int nOldPlayWndWidth = pProduct->m_nPlayWndWidth;					//��¼�ɵ�m_nPlayWndWidth 

	if ( pWPC->fRatioX < 1.0){
		pProduct->m_nPlayWndWidth = m_WDW.rcDisplay.Width();			//�µ�m_nPlayWndWidth ����һ����ʾ��Ļ����
		
		if ( (pt->x - m_WDW.rcDisplay.left)* pWPC->fRatioX * 2 < m_WDW.rcDisplay.Width() /2 )
		{
			//pProduct->m_nPlayStartIndex ����
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
		pProduct->m_nPlayWndWidth = m_WDW.rcDisplay.Width();			//�µ�m_nPlayWndWidth ����һ����ʾ��Ļ����

		if ( pt->x - (pProduct->m_nPlayWndWidth / 4) < m_WDW.rcDisplay.left )
		{
			//����
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
		//��С���PlayWnd����
		pProduct->m_nPlayStartIndex = 0;
		pProduct->m_nPlayWndWidth = pProduct->m_AcqInfo.samplingLength;
		pWPC->fRatioX = pProduct->m_nPlayWndWidth / m_WDW.rcDisplay.Width();

	}
	else if (	pProduct->m_nPlayWndWidth >= pProduct->m_nSegmentValidIndex * DATASTORAGE_SEGMENT	)
	{
		//��С���PlayWnd����
		pProduct->m_nPlayStartIndex = 0;
		pProduct->m_nPlayWndWidth = pProduct->m_nSegmentValidIndex * DATASTORAGE_SEGMENT;
		pWPC->fRatioX = pProduct->m_nPlayWndWidth / m_WDW.rcDisplay.Width();
	}
	else{
		if ( pProduct->m_nPlayStartIndex + pProduct->m_nPlayWndWidth >= pProduct->m_AcqInfo.samplingLength )
		{
			//��С���PlayWnd����
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
	//step1 �ȸ���rcRealһ����ֵ
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
	
	//step2 �ж�rcReal�Ƿ�Խ�� Խ������޸�
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


	//����ZoomDlg
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
	//step1 ���е���ĵ����������
	if (pt->x<m_WDW.rcDisplay.left || pt->x>m_WDW.rcDisplay.right || pt->y<m_WDW.rcDisplay.top || pt->y>m_WDW.rcDisplay.bottom )
	{	//�����rcDisplay֮��
		return;
	} 
	else 
	{	//�����rcDisplay֮��
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
	
	//step2 ����rcReal
//	TRACE("1=====rcReal: %d %d %d %d =============\n",m_WDW.rcReal.left,m_WDW.rcReal.top,m_WDW.rcReal.right,m_WDW.rcReal.bottom);
//	TRACE("1=====rcDisplay: %d %d %d %d =============\n",m_WDW.rcDisplay.left,m_WDW.rcDisplay.top,m_WDW.rcDisplay.right,m_WDW.rcDisplay.bottom);
	int rcReal_left = m_WDW.rcReal.left;
	int rcReal_top = m_WDW.rcReal.top;
	int rcPhy_width = m_pProduct->m_AcqInfo.samplingLength;
	int rcPhy_height = m_pProduct->m_AcqInfo.range * 2 * 1000;
	//(m_WDW.ptClickCorrd.x - rcReal_left)		���������ĵ��������Χ�����rcReal������
	//(m_WDW.ptClickCorrd.x - rcReal_left)*2	���Ŵ�ĵ�������м�
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
	
	//����ZoomDlg
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
	//step1 �ȸ���rcRealһ����ֵ
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
	
	//step2 �ж�rcReal�Ƿ�Խ�� Խ������޸�
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


	//����ZoomDlg
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
		//�ǹ�����ģʽ��
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
		//�ƶ�Mark����
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
	{	//�����rcDisplay֮��
		return;
	} 

	CDialog::OnMouseMove(nFlags, point);
}

void CWatchDlg::AddWaveDataToWatchDlg(CWaveData *pWaveData)
{
	//���ȼ���WatchDlg����Ҫ��ӵ�pWaveData
	bool existFlag = false;
	int count;
	for (count=0 ; count<m_nWaveDataNum ; count++)
	{
		if (pWaveData == (CWaveData*)m_WaveDataList.GetAt(m_WaveDataList.FindIndex(count)))
		{
			//�����ظ�WaveData
			existFlag = true;
			TRACE("WaveData�����ظ�");
			return;
		}
	}

	if (existFlag == false)
	{
		//�������ظ�WaveData  ��pWaveData����m_WaveDataList
		m_WaveDataList.AddTail(pWaveData);
		m_nWaveDataRange[m_nWaveDataNum] = 10;							//�½���CWaveData��������Ϊ10V	m_nWaveDataNum����0�ģ����������ã���++
		m_nWaveDataNum++;
	}

	
}

void CWatchDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	//�Ի�������ʱ����WM_Destroy ���DLG��Ҫȥ����������� ��destroy����
}

void CWatchDlg::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
//	delete this;				//�Ѿ��������ڸ�����BgDlg�н�����ʽdelete ����Ͳ�delete��
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
	//���¼���	m_WDW	 m_XF	 m_YFL	 m_YFR	 /*m_MF*/	 m_rcWatchDlgFrame		 m_rcMFinTab;
	//1. �ȼ����WatchDlg��MeasureDlg��CRect: 
	//					m_rcWatchDlgFrame��m_rcMFinTab
	//2. ����ComputeDisplayFrame()����m_WDW	 m_XF	 m_YFL	 m_YFR

	if (NULL == m_pMeasureDlg)
	{
		//��ǰ��CWatchDlg��û��MeasureDlg
		CRect rect;
		((CBgDlg*)GetParent())->GetClientRect(&rect);

		rect.top += 25;
		rect.bottom -= 2;
		rect.left += 1;
		rect.right -= 2;

		m_rcWatchDlgFrame = rect;

		//����m_WDW	 m_XF	 m_YFL	 m_YFR
		ComputeDisplayFrame();


		rect.SetRect(0,0,0,0);
		m_rcMFinTab = rect;
	} 
	else
	{
		//��ǰ��CWatchDlg������MeasureDlg
		//�ȼ���MeasureDlgλ��	m_rcMFinTab
		CRect rect;
		CRect rcMeasure;
//		((CBgDlg*)GetParent())->m_TabBgDlg.GetClientRect(&rect);
		GetParent()->GetClientRect(&rect);

		rcMeasure.bottom = rect.bottom-5;
		rcMeasure.left = rect.left+1;
		rcMeasure.right = rect.right-2;

		if (m_pMeasureDlg->m_bMinFlag == false)						//����һ�����ݿ���20px ������ҲΪ20px
		{
			//������С��
			if (rcMeasure.right-rcMeasure.left < m_pMeasureDlg->m_nColWidthSum)
			{
				//������� ��ҪHScroll
				rcMeasure.top = rcMeasure.bottom - MEA_TITLE_HEIGHT - MEA_ITEM_HEIGHT*m_pMeasureDlg->m_nMeasureNum - MEA_HSCROLL_HEIGHT;
				m_pMeasureDlg->AddHScroll(0, m_pMeasureDlg->m_nColWidthSum-(rcMeasure.right-rcMeasure.left) , &rcMeasure);
			//	m_pMeasureDlg->m_pHScroll->ShowWindow(SW_SHOW);
			} 
			else
			{
				//����ҪHScroll
				rcMeasure.top = rcMeasure.bottom - MEA_TITLE_HEIGHT - MEA_ITEM_HEIGHT*m_pMeasureDlg->m_nMeasureNum;
				m_pMeasureDlg->DeleteHScroll();
			}
			
		}
		else{
			//������С��
			rcMeasure.top = rcMeasure.bottom - MEA_TITLE_HEIGHT;
			m_pMeasureDlg->DeleteHScroll();
		}
		m_rcMFinTab = rcMeasure;


		rect.top += 25;
		rect.bottom = rcMeasure.top-1;
		rect.left += 1;
		rect.right -= 2;
		
		m_rcWatchDlgFrame = rect;
		//����m_WDW	 m_XF	 m_YFL	 m_YFR
		ComputeDisplayFrame();

	}

}

void CWatchDlg::ComputeDisplayFrame()
{
	CRect rcDisplay;
	rcDisplay.SetRect(0,0,m_rcWatchDlgFrame.Width(),m_rcWatchDlgFrame.Height());

	//���ݸ�CWatchDlg���м���CWaveData ������m_YFLeftOne��m_YFLeftTwo��m_YFRightOne��m_YFRightTwo��m_WDW��left��right
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
	//����m_YFLeftOne��m_YFLeftTwo��m_YFRightOne��m_YFRightTwo��m_WDW��top��bottom
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

	//����m_XF
	m_XF.rcFrame.left = INTERVAL_Frame;
	m_XF.rcFrame.right = rcDisplay.right - INTERVAL_Frame;
	m_XF.rcFrame.top = rcDisplay.bottom - Y_COORDINATE_WIDTH;		//X_COORDINATE_HEIGHT = 30 �е����
	m_XF.rcFrame.bottom = rcDisplay.bottom;

	m_WDW.rcReal = m_WDW.rcDisplay;

	//���� m_XMF.rcXAxisMarkFrame	m_YMF.rcYAxisMarkFrame;
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
		//����Ҫ�ı� WndManage�Ѿ�ѡ�е�ǰ��BgDlg
	} 
	else
	{
		m_pWndManage->m_nCurSelect = m_pOwnedBgDlg->m_nIndex;
		m_pOwnedBgDlg->Invalidate();
		m_pOwnedBgDlg->UpdateWindow();
		m_pWndManage->m_pBgDlg[nOldSeleted].Invalidate();
		m_pWndManage->m_pBgDlg[nOldSeleted].UpdateWindow();
		
	//	RefreshParaDlgBar();
		
		//������ʾ���̵�DlgBar����ʾFFT/TIME�Ĺ�����
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


	//���Ƚ���WatchDlg������BgDlg��Ϊѡ��
	SetOwnedBgDlgSeleted();
//==========================ʵ���Ҽ��˵�==========================================================================
	CMenu menu,*pSubMenu;//��������Ҫ�õ���cmenu����  
    menu.LoadMenu(IDR_MENU_WatchDlgPopup);//װ���Զ�����Ҽ��˵�  
    pSubMenu=menu.GetSubMenu(0);//��ȡ��һ�������˵������Ե�һ���˵��������Ӳ˵�  
    CPoint oPoint;//����һ������ȷ�����λ�õ�λ��  
    GetCursorPos(&oPoint);//��ȡ��ǰ����λ�ã��Ա�ʹ�ò˵����Ը�����  
    pSubMenu->TrackPopupMenu(TPM_LEFTALIGN,oPoint.x,oPoint.y,this);   //��ָ��λ����ʾ�����˵�


	
	CDialog::OnRButtonDown(nFlags, point);
}

void CWatchDlg::MeasureAdd()
{
	CMeasureAddDlg measureAddDlg(this);
//	measureAddDlg.DoModal();
	if (IDOK == measureAddDlg.DoModal())
	{
		//��� measureItem
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
		//��CWatchDlg��û�����׵�MeasureDlg
		m_pMeasureDlg = new CMeasureDlg;
		m_pMeasureDlg->Create(IDD_DIALOG_Measure , &(m_pOwnedBgDlg->m_TabBgDlg));
		m_pMeasureDlg->SetMatchWatchDlg(this);
		m_pMeasureDlg->AddMeasureItem(pWaveData, nMeaType , nMeaPart);
		ComputeFrame();

	} 
	else
	{
		//��CWatchDlg�Ѿ������׵�MeasureDlg ��MeasureItem����
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

void CWatchDlg::MeasureDelete()					//�ú�������delete���� ���ٵ���
{
	if (m_pMeasureDlg == NULL)
	{
		//û��MeasureDlg
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
		//��MeasureDlgֻ�����һ��Item�� ֱ�ӹر�
		m_pMeasureDlg->SendMessage(WM_CLOSE);
		m_pMeasureDlg = NULL;

	} 
	else
	{
		//ɾ����ǰItem
		CMeasureItem* pMeasureItem = NULL;


		CTypedPtrList<CObList , CMeasureItem*>* pMeasureItemList;
		pMeasureItemList = &(m_pMeasureDlg->m_MeasureItemList);
		pMeasureItem = (CMeasureItem*)pMeasureItemList->GetAt(pMeasureItemList->FindIndex(m_pMeasureDlg->m_nSelItem));
		
		pMeasureItemList->RemoveAt(pMeasureItemList->FindIndex(m_pMeasureDlg->m_nSelItem));
/*
		pMeasureItem = (CMeasureItem*)m_pMeasureDlg->m_MeasureItemList.GetAt(m_pMeasureDlg->m_MeasureItemList.FindIndex(m_pMeasureDlg->m_nSelItem));
		m_pMeasureDlg->m_MeasureItemList.RemoveAt(m_pMeasureDlg->m_MeasureItemList.FindIndex(m_pMeasureDlg->m_nSelItem));
*/
		delete pMeasureItem;				//����������  ������delete���� phead->nblockuse == nblockuse  �����ú���д��CMeasureDlg�о�û��

		m_pMeasureDlg->m_nMeasureNum--;
		if (m_pMeasureDlg->m_nMeasureNum == m_pMeasureDlg->m_nSelItem)
		{
			//m_nSelItemѡ��������һ��Item ��Ҫ�޸�
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
		//��� measureItem
	}
}

void CWatchDlg::ComputeMarkFrame()
{
	//���� m_XMF.rcXAxisMarkFrame	m_YMF.rcYAxisMarkFrame;
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

	//���ݸ���Ratio ���㲻ͬ��rcBmpλ��
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
		//������ZoomDrag����  ��Ҫ����ZoomDlg
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
		//������Y���ϵ�MARK�ƶ�
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
		//������X���ϵ�MARK�ƶ�
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
	//��Mark��С��
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

	
	//����������
	//����������Ҫע�⣬����յ㲻һ����������ѭ��
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
		//Ĭ�����������ԭ���ļ�ͷ
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
	//����BgDlg�е�AddWaveDataToBgDlg  -1�½� pWaveData��ǰһ��WatchDlg 0������ʾ (1 FFT)
	m_pOwnedBgDlg->AddWaveDataToBgDlg(-1 , (CWatchDlg*)NULL , 0);
}

void CWatchDlg::OnWATCHAddFreDomain() 
{
	// TODO: Add your command handler code here
	//����BgDlg�е�AddWaveDataToBgDlg  -1�½� pWaveData��ǰһ��WatchDlg 1FFT
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
	//�ø����ڵ�BgDlg�رո�WatchDlg
	m_pOwnedBgDlg->CloseCurSelWatchDlg();
}

void CWatchDlg::AddWaveDataToWatchDlg(CWatchDlg *pAddWatchDlg)
{
	//����һ��CWatchDlg��ȫ��CWaveDataֻ�����½�һ��CWatchDlg��ʱ���������ԾͲ��ý����ظ�CWaveData�ļ��
	CWaveData* pWaveData;
	int nWaveDataNum = pAddWatchDlg->m_nWaveDataNum;
	int nCount = 0;
	for (nCount=0 ; nCount<nWaveDataNum ; nCount++)
	{
		pWaveData = pAddWatchDlg->m_WaveDataList.GetAt(pAddWatchDlg->m_WaveDataList.FindIndex(nCount));
		AddWaveDataToWatchDlg(pWaveData);
	}

	/*���ݵ�ǰCWatchDlg��m_nMode����ʼ����ǰ������m_nWaveDataRange*/
	if ( m_nMode == DISPLAY_MODE_TIME )
	{
		//ʱ�� �����ȫ��CWaveData��ͨ�����ĳ�ʼ���̶�����Ϊ10V
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
		//Ƶ�� Ҫ��4�ֲ�ͬ���
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


			/*���ø�WavaData��CWatchDlg�е�Range����ʱ���Ƶ��������*/
			if ( m_nMode == DISPLAY_MODE_TIME)
			{
				/*ʱ��*/
				m_nWaveDataRange[nCount] = RANGE_TIME_pn10V;
			}
			else{
				/*Ƶ�� ��4�����������*/
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
			/*pChnSelected[nCount]==-1 ��ͨ���ǹرյ�*/
			m_nWaveDataRange[nCount] = RANGE_CLOSED;
		}
	}



	return 0;
}

int CWatchDlg::AdjustAllToolBar()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();

	/*������ʾ4��ͨ�������̵�ToolBar*/
	pFrame->m_pDlgBarExTopChnSel->ChangeToolBarDisplayChn(this);

	/*������ʾƵ���������ʱ�������ToolBar*/
	if ( m_nMode == 0 )
	{
		/*ʱ��*/
		pFrame->m_aWatchMode[0] = 1;
		pFrame->m_aWatchMode[1] = 0;
	}
	else{
		/*Ƶ��*/
		pFrame->m_aWatchMode[0] = 0;
		pFrame->m_aWatchMode[1] = 1;
	}


	return 1;
}

int CWatchDlg::SwitchTimeAndFreq(int nDirection)
{
	/*nDirection������0 Freq->Time	1 Time->Freq	2 Freqһ�ֵ�λ->Freq����һ�ֵ�λ*/

	int nCount = 0;

	if ( nDirection == 0 )
	{
		/*Freq->Time*/
		m_nMode = 0;
		/*�ı�CWatchDlg�е�Range����*/
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
		/*�л���Ƶ�������ʱ��Ҫ��4�����*/
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
		/*Freqһ�ֵ�λ->Freq����һ�ֵ�λ*/
		m_nMode = 1;
		/*�л���Ƶ�������ʱ��Ҫ��4�����*/
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
	double ratio;		//�ж��ݺ���������ɶ��ٸ������ֶ�
	CFont* pFont = NULL;
	CWaveData* pWaveData;

	strTest.Format("Bunny Tips : m_nWaveDataNum : %d . \n" , m_nWaveDataNum);
	TRACE(strTest);

	/*������*/
	if ( m_nWaveDataNum >= 1 )
	{
		//��m_YFLeftOne
		pCdc->SetTextAlign(TA_RIGHT);
		if (pFont != NULL)
		{
			delete pFont;
			pFont = NULL;
		}
		pFont = new CFont;
		pFont->CreateFont(charH, charW, 0, 0, FW_BOLD, false, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_SWISS,_T("Times New Roman"));
		CFont* pOldFont = pCdc->SelectObject(pFont);
		pWaveData = m_WaveDataList.GetAt(m_WaveDataList.FindIndex(0));			//�ú�pWaveDataһ������ɫ
		pCdc->SetTextColor(pWaveData->m_LineColor);
		pCdc->SetBkMode(TRANSPARENT);
			
		ratio = (double)(charH) / (((double)(m_WDW.rcDisplay.Height()))/10);    //�ַ��� ���� ʮ��֮һ����ʾ��ܸ�
		float yStart;
		float yLength;
		yStart = m_nWaveDataRange[0];											//�����pWaveData������
		yLength = m_nWaveDataRange[0] * 2;

		strTest.Format("Bunny Tips : m_nWaveDataRange[0] : %d . \n",m_nWaveDataRange[0]);
		TRACE(strTest);
	
		

		if (ratio < 1.0)
		{	//������ʾȫ����������
			
			for (i=0 ; i<11 ; i++)
			{
				str.Format("%.1f", yStart-i*yLength*0.1);
				pCdc->TextOut(m_YF[0].rcFrame.left+Y_COORDINATE_ALIGNRIGHT_TO_Y_FRAMERIGHT, m_YF[0].rcFrame.top+(i*m_YF[0].rcFrame.Height()/10) - (m_YF[0].rcFrame.Height()/60) , str);
			}
		} 
		else if(ratio < 2.0)
		{	//������ʾȫ����������
			for (i=0 ; i<11 ; i++)
			{
				str.Format("%.1f", yStart-i*yLength*0.1);
				pCdc->TextOut(m_YF[0].rcFrame.left+Y_COORDINATE_ALIGNRIGHT_TO_Y_FRAMERIGHT, m_YF[0].rcFrame.top+(i*m_YF[0].rcFrame.Height()/10) - (m_YF[0].rcFrame.Height()/60) , str);
			}
		}
		else if(ratio < 4.0)
		{	//ÿ��������ʾһ��
			for (i=0 ; i<5 ; i++)
			{
				str.Format("%.1f", yStart-yLength*0.1-i*yLength*0.2);
				pCdc->TextOut(m_YF[0].rcFrame.left+Y_COORDINATE_ALIGNRIGHT_TO_Y_FRAMERIGHT, m_YF[0].rcFrame.top+(i*m_YF[0].rcFrame.Height()/7) - (m_YF[0].rcFrame.Height()/60) + m_YF[0].rcFrame.Height()/7 , str);
			}
		}
		else
		{	//ÿ�ĸ���ʾһ��
			for (i=0 ; i<3 ; i++)
			{
				str.Format("%.1f", yStart-yLength*0.1-i*yLength*0.4);
				pCdc->TextOut(m_YF[0].rcFrame.left+Y_COORDINATE_ALIGNRIGHT_TO_Y_FRAMERIGHT, m_YF[0].rcFrame.top+(i*m_YF[0].rcFrame.Height()/5) - (m_YF[0].rcFrame.Height()/60) + m_YF[0].rcFrame.Height()/5 , str);
			}
		}
		str.Format("V");//����������ѹ
		pCdc->TextOut(m_YF[0].rcFrame.left+15, m_YF[0].rcFrame.top+m_YF[0].rcFrame.Height()/2,str);
	}

	if ( m_nWaveDataNum >= 2 )
	{
		//��m_YFRightOne
		pCdc->SetTextAlign(TA_LEFT);
		if (pFont != NULL)
		{
			delete pFont;
			pFont = NULL;
		}
		pFont = new CFont;
		pFont->CreateFont(charH, charW, 0, 0, FW_BOLD, false, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_SWISS,_T("Times New Roman"));
		CFont* pOldFont = pCdc->SelectObject(pFont);
		pWaveData = m_WaveDataList.GetAt(m_WaveDataList.FindIndex(1));			//�ú�pWaveDataһ������ɫ
		pCdc->SetTextColor(pWaveData->m_LineColor);
		pCdc->SetBkMode(TRANSPARENT);
		
		ratio = (double)(charH) / (((double)(m_WDW.rcDisplay.Height()))/10);    //�ַ��� ���� ʮ��֮һ����ʾ��ܸ�
		float yStart;
		float yLength;
		yStart = m_nWaveDataRange[1];											//�����pWaveData������
		yLength = m_nWaveDataRange[1] * 2;
		if (ratio < 1.0)
		{	//������ʾȫ����������
			
			for (i=0 ; i<11 ; i++)
			{
				str.Format("%.1f", yStart-i*yLength*0.1);
				pCdc->TextOut(m_YF[1].rcFrame.right-Y_COORDINATE_ALIGNLEFT_TO_Y_FRAMELEFT, m_YF[1].rcFrame.top+(i*m_YF[1].rcFrame.Height()/10) - (m_YF[1].rcFrame.Height()/60) , str);
			}
		} 
		else if(ratio < 2.0)
		{	//������ʾȫ����������
			for (i=0 ; i<11 ; i++)
			{
				str.Format("%.1f", yStart-i*yLength*0.1);
				pCdc->TextOut(m_YF[1].rcFrame.right-Y_COORDINATE_ALIGNLEFT_TO_Y_FRAMELEFT, m_YF[1].rcFrame.top+(i*m_YF[1].rcFrame.Height()/10) - (m_YF[1].rcFrame.Height()/60) , str);
			}
		}
		else if(ratio < 4.0)
		{	//ÿ��������ʾһ��
			for (i=0 ; i<5 ; i++)
			{
				str.Format("%.1f", yStart-yLength*0.1-i*yLength*0.2);
				pCdc->TextOut(m_YF[1].rcFrame.right-Y_COORDINATE_ALIGNLEFT_TO_Y_FRAMELEFT, m_YF[1].rcFrame.top+(i*m_YF[1].rcFrame.Height()/7) - (m_YF[1].rcFrame.Height()/60) + m_YF[1].rcFrame.Height()/7 , str);
			}
		}
		else
		{	//ÿ�ĸ���ʾһ��
			for (i=0 ; i<3 ; i++)
			{
				str.Format("%.1f", yStart-yLength*0.1-i*yLength*0.4);
				pCdc->TextOut(m_YF[1].rcFrame.right-Y_COORDINATE_ALIGNLEFT_TO_Y_FRAMELEFT, m_YF[1].rcFrame.top+(i*m_YF[1].rcFrame.Height()/5) - (m_YF[1].rcFrame.Height()/60) + m_YF[1].rcFrame.Height()/5 , str);
			}
		}
		str.Format("V");//����������ѹ
		pCdc->TextOut(m_YF[1].rcFrame.right-15, m_YF[1].rcFrame.top+m_YF[1].rcFrame.Height()/2,str);		
	}
	if ( m_nWaveDataNum >= 3 )
	{
		//��m_YFLeftTwo
		pCdc->SetTextAlign(TA_RIGHT);
		if (pFont != NULL)
		{
			delete pFont;
			pFont = NULL;
		}
		pFont = new CFont;
		pFont->CreateFont(charH, charW, 0, 0, FW_BOLD, false, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_SWISS,_T("Times New Roman"));
		CFont* pOldFont = pCdc->SelectObject(pFont);
		pWaveData = m_WaveDataList.GetAt(m_WaveDataList.FindIndex(2));			//�ú�pWaveDataһ������ɫ
		pCdc->SetTextColor(pWaveData->m_LineColor);
		pCdc->SetBkMode(TRANSPARENT);
		
		ratio = (double)(charH) / (((double)(m_WDW.rcDisplay.Height()))/10);    //�ַ��� ���� ʮ��֮һ����ʾ��ܸ�
		float yStart;
		float yLength;
		yStart = m_nWaveDataRange[2];											//�����pWaveData������
		yLength = m_nWaveDataRange[2] * 2;
		if (ratio < 1.0)
		{	//������ʾȫ����������
			
			for (i=0 ; i<11 ; i++)
			{
				str.Format("%.1f", yStart-i*yLength*0.1);
				pCdc->TextOut(m_YF[2].rcFrame.left+Y_COORDINATE_ALIGNRIGHT_TO_Y_FRAMERIGHT, m_YF[2].rcFrame.top+(i*m_YF[2].rcFrame.Height()/10) - (m_YF[2].rcFrame.Height()/60) , str);
			}
		} 
		else if(ratio < 2.0)
		{	//������ʾȫ����������
			for (i=0 ; i<11 ; i++)
			{
				str.Format("%.1f", yStart-i*yLength*0.1);
				pCdc->TextOut(m_YF[2].rcFrame.left+Y_COORDINATE_ALIGNRIGHT_TO_Y_FRAMERIGHT, m_YF[2].rcFrame.top+(i*m_YF[2].rcFrame.Height()/10) - (m_YF[2].rcFrame.Height()/60) , str);
			}
		}
		else if(ratio < 4.0)
		{	//ÿ��������ʾһ��
			for (i=0 ; i<5 ; i++)
			{
				str.Format("%.1f", yStart-yLength*0.1-i*yLength*0.2);
				pCdc->TextOut(m_YF[2].rcFrame.left+Y_COORDINATE_ALIGNRIGHT_TO_Y_FRAMERIGHT, m_YF[2].rcFrame.top+(i*m_YF[2].rcFrame.Height()/7) - (m_YF[2].rcFrame.Height()/60) + m_YF[2].rcFrame.Height()/7 , str);
			}
		}
		else
		{	//ÿ�ĸ���ʾһ��
			for (i=0 ; i<3 ; i++)
			{
				str.Format("%.1f", yStart-yLength*0.1-i*yLength*0.4);
				pCdc->TextOut(m_YF[2].rcFrame.left+Y_COORDINATE_ALIGNRIGHT_TO_Y_FRAMERIGHT, m_YF[2].rcFrame.top+(i*m_YF[2].rcFrame.Height()/5) - (m_YF[2].rcFrame.Height()/60) + m_YF[2].rcFrame.Height()/5 , str);
			}
		}
		str.Format("V");//����������ѹ
		pCdc->TextOut(m_YF[2].rcFrame.left+15, m_YF[2].rcFrame.top+m_YF[2].rcFrame.Height()/2,str);		
		
	}
	if ( m_nWaveDataNum >= 4 )
	{
		//��m_YFLeftOne
		pCdc->SetTextAlign(TA_LEFT);
		if (pFont != NULL)
		{
			delete pFont;
			pFont = NULL;
		}
		pFont = new CFont;
		pFont->CreateFont(charH, charW, 0, 0, FW_BOLD, false, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_SWISS,_T("Times New Roman"));
		CFont* pOldFont = pCdc->SelectObject(pFont);
		pWaveData = m_WaveDataList.GetAt(m_WaveDataList.FindIndex(3));			//�ú�pWaveDataһ������ɫ
		pCdc->SetTextColor(pWaveData->m_LineColor);
		pCdc->SetBkMode(TRANSPARENT);
		
		ratio = (double)(charH) / (((double)(m_WDW.rcDisplay.Height()))/10);    //�ַ��� ���� ʮ��֮һ����ʾ��ܸ�
		float yStart;
		float yLength;
		yStart = m_nWaveDataRange[3];											//�����pWaveData������
		yLength = m_nWaveDataRange[3] * 2;
		if (ratio < 1.0)
		{	//������ʾȫ����������
			
			for (i=0 ; i<11 ; i++)
			{
				str.Format("%.1f", yStart-i*yLength*0.1);
				pCdc->TextOut(m_YF[3].rcFrame.right-Y_COORDINATE_ALIGNLEFT_TO_Y_FRAMELEFT, m_YF[3].rcFrame.top+(i*m_YF[3].rcFrame.Height()/10) - (m_YF[3].rcFrame.Height()/60) , str);
			}
		} 
		else if(ratio < 2.0)
		{	//������ʾȫ����������
			for (i=0 ; i<11 ; i++)
			{
				str.Format("%.1f", yStart-i*yLength*0.1);
				pCdc->TextOut(m_YF[3].rcFrame.right-Y_COORDINATE_ALIGNLEFT_TO_Y_FRAMELEFT, m_YF[3].rcFrame.top+(i*m_YF[3].rcFrame.Height()/10) - (m_YF[3].rcFrame.Height()/60) , str);
			}
		}
		else if(ratio < 4.0)
		{	//ÿ��������ʾһ��
			for (i=0 ; i<5 ; i++)
			{
				str.Format("%.1f", yStart-yLength*0.1-i*yLength*0.2);
				pCdc->TextOut(m_YF[3].rcFrame.right-Y_COORDINATE_ALIGNLEFT_TO_Y_FRAMELEFT, m_YF[3].rcFrame.top+(i*m_YF[3].rcFrame.Height()/7) - (m_YF[3].rcFrame.Height()/60) + m_YF[3].rcFrame.Height()/7 , str);
			}
		}
		else
		{	//ÿ�ĸ���ʾһ��
			for (i=0 ; i<3 ; i++)
			{
				str.Format("%.1f", yStart-yLength*0.1-i*yLength*0.4);
				pCdc->TextOut(m_YF[3].rcFrame.right-Y_COORDINATE_ALIGNLEFT_TO_Y_FRAMELEFT, m_YF[3].rcFrame.top+(i*m_YF[3].rcFrame.Height()/5) - (m_YF[3].rcFrame.Height()/60) + m_YF[3].rcFrame.Height()/5 , str);
			}
		}
		str.Format("V");//����������ѹ
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
	double ratio;		//�ж��ݺ���������ɶ��ٸ������ֶ�
	CFont* pFont = NULL;
	CWaveData* pWaveData;

	strTest.Format("Bunny Tips : m_nWaveDataNum : %d . \n" , m_nWaveDataNum);
	TRACE(strTest);

	/*������*/
	if ( m_nWaveDataNum >= 1 )
	{
		//��m_YFLeftOne
		pCdc->SetTextAlign(TA_RIGHT);
		if (pFont != NULL)
		{
			delete pFont;
			pFont = NULL;
		}
		pFont = new CFont;
		pFont->CreateFont(charH, charW, 0, 0, FW_BOLD, false, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_SWISS,_T("Times New Roman"));
		CFont* pOldFont = pCdc->SelectObject(pFont);
		pWaveData = m_WaveDataList.GetAt(m_WaveDataList.FindIndex(0));			//�ú�pWaveDataһ������ɫ
		pCdc->SetTextColor(pWaveData->m_LineColor);
		pCdc->SetBkMode(TRANSPARENT);
			
		ratio = (double)(charH) / (((double)(m_WDW.rcDisplay.Height()))/10);    //�ַ��� ���� ʮ��֮һ����ʾ��ܸ�
		float yStart;
		float yLength;

		//�����pWaveData������
		switch ( m_nWaveDataRange[0] )
		{
		case RANGE_FREQ_V_0_500:
			yStart	= 500;				//����yStartʵ���������ֵ����Ϊ�ǵݼ���							
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
		{	//������ʾȫ����������
			
			for (i=0 ; i<11 ; i++)
			{
				str.Format("%.1f", yStart-i*yLength*0.1);
				pCdc->TextOut(m_YF[0].rcFrame.left+Y_COORDINATE_ALIGNRIGHT_TO_Y_FRAMERIGHT, m_YF[0].rcFrame.top+(i*m_YF[0].rcFrame.Height()/10) - (m_YF[0].rcFrame.Height()/60) , str);
			}
		} 
		else if(ratio < 2.0)
		{	//������ʾȫ����������
			for (i=0 ; i<11 ; i++)
			{
				str.Format("%.1f", yStart-i*yLength*0.1);
				pCdc->TextOut(m_YF[0].rcFrame.left+Y_COORDINATE_ALIGNRIGHT_TO_Y_FRAMERIGHT, m_YF[0].rcFrame.top+(i*m_YF[0].rcFrame.Height()/10) - (m_YF[0].rcFrame.Height()/60) , str);
			}
		}
		else if(ratio < 4.0)
		{	//ÿ��������ʾһ��
			for (i=0 ; i<5 ; i++)
			{
				str.Format("%.1f", yStart-yLength*0.1-i*yLength*0.2);
				pCdc->TextOut(m_YF[0].rcFrame.left+Y_COORDINATE_ALIGNRIGHT_TO_Y_FRAMERIGHT, m_YF[0].rcFrame.top+(i*m_YF[0].rcFrame.Height()/7) - (m_YF[0].rcFrame.Height()/60) + m_YF[0].rcFrame.Height()/7 , str);
			}
		}
		else
		{	//ÿ�ĸ���ʾһ��
			for (i=0 ; i<3 ; i++)
			{
				str.Format("%.1f", yStart-yLength*0.1-i*yLength*0.4);
				pCdc->TextOut(m_YF[0].rcFrame.left+Y_COORDINATE_ALIGNRIGHT_TO_Y_FRAMERIGHT, m_YF[0].rcFrame.top+(i*m_YF[0].rcFrame.Height()/5) - (m_YF[0].rcFrame.Height()/60) + m_YF[0].rcFrame.Height()/5 , str);
			}
		}
		if ( m_pProduct->m_FftConf.nOutputMode == YAXIS_UNIT_V )
		{
			/*��Ƶ������У�ֻ�������Է����вŻ��е�λmV*/
			str.Format("mV");//����������ѹ
			pCdc->TextOut(m_YF[0].rcFrame.left+5, m_YF[0].rcFrame.top+m_YF[0].rcFrame.Height()/2+m_YF[0].rcFrame.Height()/40,str);
		}
	}

	if ( m_nWaveDataNum >= 2 )
	{
		//��m_YFRightOne
		pCdc->SetTextAlign(TA_LEFT);
		if (pFont != NULL)
		{
			delete pFont;
			pFont = NULL;
		}
		pFont = new CFont;
		pFont->CreateFont(charH, charW, 0, 0, FW_BOLD, false, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_SWISS,_T("Times New Roman"));
		CFont* pOldFont = pCdc->SelectObject(pFont);
		pWaveData = m_WaveDataList.GetAt(m_WaveDataList.FindIndex(1));			//�ú�pWaveDataһ������ɫ
		pCdc->SetTextColor(pWaveData->m_LineColor);
		pCdc->SetBkMode(TRANSPARENT);
		
		ratio = (double)(charH) / (((double)(m_WDW.rcDisplay.Height()))/10);    //�ַ��� ���� ʮ��֮һ����ʾ��ܸ�
		float yStart;
		float yLength;
	
		//�����pWaveData������
		switch ( m_nWaveDataRange[1] )
		{
		case RANGE_FREQ_V_0_500:
			yStart	= 500;				//����yStartʵ���������ֵ����Ϊ�ǵݼ���							
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
		{	//������ʾȫ����������
			
			for (i=0 ; i<11 ; i++)
			{
				str.Format("%.1f", yStart-i*yLength*0.1);
				pCdc->TextOut(m_YF[1].rcFrame.right-Y_COORDINATE_ALIGNLEFT_TO_Y_FRAMELEFT, m_YF[1].rcFrame.top+(i*m_YF[1].rcFrame.Height()/10) - (m_YF[1].rcFrame.Height()/60) , str);
			}
		} 
		else if(ratio < 2.0)
		{	//������ʾȫ����������
			for (i=0 ; i<11 ; i++)
			{
				str.Format("%.1f", yStart-i*yLength*0.1);
				pCdc->TextOut(m_YF[1].rcFrame.right-Y_COORDINATE_ALIGNLEFT_TO_Y_FRAMELEFT, m_YF[1].rcFrame.top+(i*m_YF[1].rcFrame.Height()/10) - (m_YF[1].rcFrame.Height()/60) , str);
			}
		}
		else if(ratio < 4.0)
		{	//ÿ��������ʾһ��
			for (i=0 ; i<5 ; i++)
			{
				str.Format("%.1f", yStart-yLength*0.1-i*yLength*0.2);
				pCdc->TextOut(m_YF[1].rcFrame.right-Y_COORDINATE_ALIGNLEFT_TO_Y_FRAMELEFT, m_YF[1].rcFrame.top+(i*m_YF[1].rcFrame.Height()/7) - (m_YF[1].rcFrame.Height()/60) + m_YF[1].rcFrame.Height()/7 , str);
			}
		}
		else
		{	//ÿ�ĸ���ʾһ��
			for (i=0 ; i<3 ; i++)
			{
				str.Format("%.1f", yStart-yLength*0.1-i*yLength*0.4);
				pCdc->TextOut(m_YF[1].rcFrame.right-Y_COORDINATE_ALIGNLEFT_TO_Y_FRAMELEFT, m_YF[1].rcFrame.top+(i*m_YF[1].rcFrame.Height()/5) - (m_YF[1].rcFrame.Height()/60) + m_YF[1].rcFrame.Height()/5 , str);
			}
		}
		if ( m_pProduct->m_FftConf.nOutputMode == YAXIS_UNIT_V )
		{
			/*��Ƶ������У�ֻ�������Է����вŻ��е�λmV*/
			str.Format("mV");//����������ѹ
			pCdc->TextOut(m_YF[1].rcFrame.right-5, m_YF[1].rcFrame.top+m_YF[1].rcFrame.Height()/2+m_YF[1].rcFrame.Height()/40,str);	
		}	
	}
	if ( m_nWaveDataNum >= 3 )
	{
		//��m_YFLeftTwo
		pCdc->SetTextAlign(TA_RIGHT);
		if (pFont != NULL)
		{
			delete pFont;
			pFont = NULL;
		}
		pFont = new CFont;
		pFont->CreateFont(charH, charW, 0, 0, FW_BOLD, false, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_SWISS,_T("Times New Roman"));
		CFont* pOldFont = pCdc->SelectObject(pFont);
		pWaveData = m_WaveDataList.GetAt(m_WaveDataList.FindIndex(2));			//�ú�pWaveDataһ������ɫ
		pCdc->SetTextColor(pWaveData->m_LineColor);
		pCdc->SetBkMode(TRANSPARENT);
		
		ratio = (double)(charH) / (((double)(m_WDW.rcDisplay.Height()))/10);    //�ַ��� ���� ʮ��֮һ����ʾ��ܸ�
		float yStart;
		float yLength;

		//�����pWaveData������
		switch ( m_nWaveDataRange[2] )
		{
		case RANGE_FREQ_V_0_500:
			yStart	= 500;				//����yStartʵ���������ֵ����Ϊ�ǵݼ���							
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
		{	//������ʾȫ����������
			
			for (i=0 ; i<11 ; i++)
			{
				str.Format("%.1f", yStart-i*yLength*0.1);
				pCdc->TextOut(m_YF[2].rcFrame.left+Y_COORDINATE_ALIGNRIGHT_TO_Y_FRAMERIGHT, m_YF[2].rcFrame.top+(i*m_YF[2].rcFrame.Height()/10) - (m_YF[2].rcFrame.Height()/60) , str);
			}
		} 
		else if(ratio < 2.0)
		{	//������ʾȫ����������
			for (i=0 ; i<11 ; i++)
			{
				str.Format("%.1f", yStart-i*yLength*0.1);
				pCdc->TextOut(m_YF[2].rcFrame.left+Y_COORDINATE_ALIGNRIGHT_TO_Y_FRAMERIGHT, m_YF[2].rcFrame.top+(i*m_YF[2].rcFrame.Height()/10) - (m_YF[2].rcFrame.Height()/60) , str);
			}
		}
		else if(ratio < 4.0)
		{	//ÿ��������ʾһ��
			for (i=0 ; i<5 ; i++)
			{
				str.Format("%.1f", yStart-yLength*0.1-i*yLength*0.2);
				pCdc->TextOut(m_YF[2].rcFrame.left+Y_COORDINATE_ALIGNRIGHT_TO_Y_FRAMERIGHT, m_YF[2].rcFrame.top+(i*m_YF[2].rcFrame.Height()/7) - (m_YF[2].rcFrame.Height()/60) + m_YF[2].rcFrame.Height()/7 , str);
			}
		}
		else
		{	//ÿ�ĸ���ʾһ��
			for (i=0 ; i<3 ; i++)
			{
				str.Format("%.1f", yStart-yLength*0.1-i*yLength*0.4);
				pCdc->TextOut(m_YF[2].rcFrame.left+Y_COORDINATE_ALIGNRIGHT_TO_Y_FRAMERIGHT, m_YF[2].rcFrame.top+(i*m_YF[2].rcFrame.Height()/5) - (m_YF[2].rcFrame.Height()/60) + m_YF[2].rcFrame.Height()/5 , str);
			}
		}
		if ( m_pProduct->m_FftConf.nOutputMode == YAXIS_UNIT_V )
		{
			/*��Ƶ������У�ֻ�������Է����вŻ��е�λmV*/
			str.Format("mV");//����������ѹ
			pCdc->TextOut(m_YF[2].rcFrame.left+5, m_YF[2].rcFrame.top+m_YF[2].rcFrame.Height()/2+m_YF[2].rcFrame.Height()/40,str);	
		}	
		
	}
	if ( m_nWaveDataNum >= 4 )
	{
		//��m_YFLeftOne
		pCdc->SetTextAlign(TA_LEFT);
		if (pFont != NULL)
		{
			delete pFont;
			pFont = NULL;
		}
		pFont = new CFont;
		pFont->CreateFont(charH, charW, 0, 0, FW_BOLD, false, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_SWISS,_T("Times New Roman"));
		CFont* pOldFont = pCdc->SelectObject(pFont);
		pWaveData = m_WaveDataList.GetAt(m_WaveDataList.FindIndex(3));			//�ú�pWaveDataһ������ɫ
		pCdc->SetTextColor(pWaveData->m_LineColor);
		pCdc->SetBkMode(TRANSPARENT);
		
		ratio = (double)(charH) / (((double)(m_WDW.rcDisplay.Height()))/10);    //�ַ��� ���� ʮ��֮һ����ʾ��ܸ�
		float yStart;
		float yLength;
	
		//�����pWaveData������
		switch ( m_nWaveDataRange[3] )
		{
		case RANGE_FREQ_V_0_500:
			yStart	= 500;				//����yStartʵ���������ֵ����Ϊ�ǵݼ���							
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
		{	//������ʾȫ����������
			
			for (i=0 ; i<11 ; i++)
			{
				str.Format("%.1f", yStart-i*yLength*0.1);
				pCdc->TextOut(m_YF[3].rcFrame.right-Y_COORDINATE_ALIGNLEFT_TO_Y_FRAMELEFT, m_YF[3].rcFrame.top+(i*m_YF[3].rcFrame.Height()/10) - (m_YF[3].rcFrame.Height()/60) , str);
			}
		} 
		else if(ratio < 2.0)
		{	//������ʾȫ����������
			for (i=0 ; i<11 ; i++)
			{
				str.Format("%.1f", yStart-i*yLength*0.1);
				pCdc->TextOut(m_YF[3].rcFrame.right-Y_COORDINATE_ALIGNLEFT_TO_Y_FRAMELEFT, m_YF[3].rcFrame.top+(i*m_YF[3].rcFrame.Height()/10) - (m_YF[3].rcFrame.Height()/60) , str);
			}
		}
		else if(ratio < 4.0)
		{	//ÿ��������ʾһ��
			for (i=0 ; i<5 ; i++)
			{
				str.Format("%.1f", yStart-yLength*0.1-i*yLength*0.2);
				pCdc->TextOut(m_YF[3].rcFrame.right-Y_COORDINATE_ALIGNLEFT_TO_Y_FRAMELEFT, m_YF[3].rcFrame.top+(i*m_YF[3].rcFrame.Height()/7) - (m_YF[3].rcFrame.Height()/60) + m_YF[3].rcFrame.Height()/7 , str);
			}
		}
		else
		{	//ÿ�ĸ���ʾһ��
			for (i=0 ; i<3 ; i++)
			{
				str.Format("%.1f", yStart-yLength*0.1-i*yLength*0.4);
				pCdc->TextOut(m_YF[3].rcFrame.right-Y_COORDINATE_ALIGNLEFT_TO_Y_FRAMELEFT, m_YF[3].rcFrame.top+(i*m_YF[3].rcFrame.Height()/5) - (m_YF[3].rcFrame.Height()/60) + m_YF[3].rcFrame.Height()/5 , str);
			}
		}
		
		if ( m_pProduct->m_FftConf.nOutputMode == YAXIS_UNIT_V )
		{
			/*��Ƶ������У�ֻ�������Է����вŻ��е�λmV*/
			str.Format("mV");//����������ѹ
			pCdc->TextOut(m_YF[3].rcFrame.right-5, m_YF[3].rcFrame.top+m_YF[3].rcFrame.Height()/2+m_YF[3].rcFrame.Height()/40,str);	
		}	
		
	}

	delete pFont;
	pFont = NULL;

	return 0;
}

int CWatchDlg::DrawXCoordinateTime(CDC *pCdc)
{
	/*������*/
	AcqInfo* pAcqInfo = &(m_pProduct->m_AcqInfo);
	CString str;
	int i = 0;
	//	CString strTest;
	
	int charH = 13;
	int charW = 5;
	double ratio;		//�ж��ݺ���������ɶ��ٸ������ֶ�

	CWaveData* pWaveData;
	
	
	pAcqInfo->samplingTime = (double)(pAcqInfo->samplingLength)/(double)(pAcqInfo->samplingRate);
	ratio = (double)(charW) / (((double)(m_WDW.rcDisplay.Width()))/10);    //�ַ��� ���� ʮ��֮һ����ʾ��ܿ�
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
	{	//������ʾȫ���ĺ�����
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
	{	//������ʾȫ���ĺ�����
		for (i=0 ; i<11 ; i++)
		{
			str.Format("%.3f", xStart + xLength*i*0.1);
			pCdc->TextOut(m_WDW.rcDisplay.left-m_rcWatchDlgFrame.Width()/50 + i*m_WDW.rcDisplay.Width()/10, m_WDW.rcDisplay.bottom + (m_WDW.rcDisplay.Height()/60),str);
		}
	}
	else if(ratio < 0.4)
	{	//ÿ��������ʾһ��
		for (i=0 ; i<5 ; i++)
		{
			str.Format("%.3f", xStart + xLength*i*0.2);
			pCdc->TextOut(m_WDW.rcDisplay.left-m_rcWatchDlgFrame.Width()/50 + i*m_WDW.rcDisplay.Width()/5, m_WDW.rcDisplay.bottom + (m_WDW.rcDisplay.Height()/60),str);
		}
	}
	else
	{	//ÿ�ĸ���ʾһ��
		for (i=0 ; i<3 ; i++)
		{
			str.Format("%.3f", xStart + xLength*i*0.4);
			pCdc->TextOut(m_WDW.rcDisplay.left-m_rcWatchDlgFrame.Width()/50 + i*m_WDW.rcDisplay.Width()*4/10, m_WDW.rcDisplay.bottom + (m_WDW.rcDisplay.Height()/60),str);
		}
	}
	//	CFont *pVFont;
	//	pVFont = new CFont;					//������ʾ������
	//	pVFont->CreateFont(charH, charW, 900, 0, FW_BOLD, TRUE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_SWISS,_T("Times New Roman"));
	//	pCdc->SelectObject(pVFont);
	//	str.Format("  ms");//���������ʱ��
	str = ComputeHorUnitTime(&xLength , (double)(m_pProduct->m_nPlayWndWidth)/(double)(pAcqInfo->samplingRate));
	pCdc->TextOut(m_WDW.rcDisplay.right - m_WDW.rcDisplay.Width()/15, m_WDW.rcDisplay.bottom + 13,str);
	
	//	pCdc->SelectObject(pOldFont);

	return 0;
}

int CWatchDlg::DrawXCoordinateFreq(CDC *pCdc)
{

	/*������*/
	AcqInfo* pAcqInfo = &(m_pProduct->m_AcqInfo);
	int nSampleRate = pAcqInfo->samplingRate;
	int nAvaliableSampleRate = nSampleRate / 2;					//��Ч������Ϊ��ʵ�����ʵ�һ�룬�ο�˹�ز�������

	CString strUnit;
	int i = 0;
	CString str;
	
	int charH = 13;
	int charW = 5;
	double ratio;		//�ж��ݺ���������ɶ��ٸ������ֶ�
	
	CWaveData* pWaveData;
	
	
	pAcqInfo->samplingTime = (double)(pAcqInfo->samplingLength)/(double)(pAcqInfo->samplingRate);
	ratio = (double)(charW) / (((double)(m_WDW.rcDisplay.Width()))/10);    //�ַ��� ���� ʮ��֮һ����ʾ��ܿ�
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
	{	//������ʾȫ���ĺ�����
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
	{	//������ʾȫ���ĺ�����
		for (i=0 ; i<11 ; i++)
		{
			str.Format("%.1f", xStart + xLength*i*0.1);
			pCdc->TextOut(m_WDW.rcDisplay.left-m_rcWatchDlgFrame.Width()/50 + i*m_WDW.rcDisplay.Width()/10, m_WDW.rcDisplay.bottom + (m_WDW.rcDisplay.Height()/60),str);
		}
	}
	else if(ratio < 0.4)
	{	//ÿ��������ʾһ��
		for (i=0 ; i<5 ; i++)
		{
			str.Format("%.1f", xStart + xLength*i*0.2);
			pCdc->TextOut(m_WDW.rcDisplay.left-m_rcWatchDlgFrame.Width()/50 + i*m_WDW.rcDisplay.Width()/5, m_WDW.rcDisplay.bottom + (m_WDW.rcDisplay.Height()/60),str);
		}
	}
	else
	{	//ÿ�ĸ���ʾһ��
		for (i=0 ; i<3 ; i++)
		{
			str.Format("%.1f", xStart + xLength*i*0.4);
			pCdc->TextOut(m_WDW.rcDisplay.left-m_rcWatchDlgFrame.Width()/50 + i*m_WDW.rcDisplay.Width()*4/10, m_WDW.rcDisplay.bottom + (m_WDW.rcDisplay.Height()/60),str);
		}
	}
	//	CFont *pVFont;
	//	pVFont = new CFont;					//������ʾ������
	//	pVFont->CreateFont(charH, charW, 900, 0, FW_BOLD, TRUE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_SWISS,_T("Times New Roman"));
	//	pCdc->SelectObject(pVFont);
	//	str.Format("  ms");//���������ʱ��
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

	/*�ı���ʾFFT/TIME��ť�Ĺ�����*/
	if ( m_nMode == DISPLAY_MODE_TIME )
	{
		pFrame->m_aWatchMode[0] = 1;
		pFrame->m_aWatchMode[1] = 0;
	}
	else{		
		pFrame->m_aWatchMode[0] = 0;
		pFrame->m_aWatchMode[1] = 1;
	}
	

	/*�ı���ʾͨ���Ĺ�����*/
	pFrame->m_pDlgBarExTopChnSel->ChangeToolBarDisplayChn(this);

	

	return 1;
}

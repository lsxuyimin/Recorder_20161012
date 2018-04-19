// MarkDlg.cpp : implementation file
//

#include "stdafx.h"
#include "recorder.h"
#include "MarkDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//////////////////////////////////////////////////////////////////////////
#include "WatchDlg.h"
#include "MainFrm.h"
#include "RecorderDoc.h"
#include "Product.h"
#include "WaveData.h"
#include "FftComplex.h"
//////////////////////////////////////////////////////////////////////////
int g_aMarkColWidth[MARK_COL_NUM]={10,60,60,80};
CString g_aMarkColName[MARK_COL_NUM]={" ","1","2","Δ"};


/////////////////////////////////////////////////////////////////////////////
// CMarkDlg dialog


CMarkDlg::CMarkDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMarkDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMarkDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pOwnedDlg = (CWatchDlg*)pParent;
	m_aValue[0] = 1.0;
	m_aValue[1] = 1.0;
	m_aValue[2] = 1.0;
	m_aValue[3] = 1.0;
	m_aValue[4] = 1.0;
	m_aValue[5] = 1.0;
	m_aValue[6] = 1.0;
	m_aValue[7] = 1.0;
	
	m_aValue[8] = 0.0;
	m_aValue[9] = 0.0;
	m_aValue[10] = 1.0;
	m_aValue[11] = 1.0;

	m_nRowNum = 0;

	int nCount;
	m_nMarkDlgWidthNormal = 0;
	for (nCount=0 ; nCount<MARK_COL_NUM ; nCount++)
	{
		m_nMarkDlgWidthNormal += g_aMarkColWidth[nCount];
	}
	m_nMarkDlgWidth = m_nMarkDlgWidthNormal;

	m_bmpMarkX.LoadBitmap(IDB_BMP_MARK_X);
	m_bmpMarkY.LoadBitmap(IDB_BMP_MARK_Y);
	m_bmpScale.LoadBitmap(IDB_BMP_SCALE_BgGray);

	m_bMinFlag = false;

	m_rcMarkDlgInWatchDlg = CRect(100,10,100+m_nMarkDlgWidthNormal,MARK_TITLE_HEIGHT);

	m_bIsPick = false;

	m_nClickPointToLeft = 0;
	m_nClickPointToTop = 0;
	m_nClickPointToRight = 0;
	m_nClickPointToBottom = 0;


}


void CMarkDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMarkDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMarkDlg, CDialog)
	//{{AFX_MSG_MAP(CMarkDlg)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_NCHITTEST()
	ON_WM_LBUTTONDOWN()
	ON_WM_ERASEBKGND()
	ON_WM_NCLBUTTONUP()
	ON_WM_NCMOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMarkDlg message handlers

void CMarkDlg::ModifyValue(int nMarkMode, float fChangeValue)
{
	int nCount = 0;

//	TRACE("===========CMarkDlg::ModifyValue : fChangeValue : %.3f=================================\n",fChangeValue);
//	TRACE("===========CMarkDlg::ModifyValue : m_nMarkMode : %d=================================\n",nMarkMode);
	
	m_aValue[nMarkMode-1] = fChangeValue;
	//重新计算行数
	m_nRowNum = 0;
	for ( nCount=0 ; nCount<6 ; nCount++ )
	{
		if ( m_aValue[nCount*2]!=0.0 && m_aValue[nCount*2]!=1.0 || m_aValue[nCount*2+1]!=0.0 && m_aValue[nCount*2+1]!=1.0 )
		{
			m_nRowNum++;
		}
	}
/*
	if ( m_aValue[0]!=0.0 && m_aValue[0]!=1.0 || m_aValue[1]!=0.0 && m_aValue[1]!=1.0 )
	{
		m_nRowNum++;
	}
	if ( m_aValue[2]!=0.0 && m_aValue[2]!=1.0 || m_aValue[3]!=0.0 && m_aValue[3]!=1.0 )
	{
		m_nRowNum++;
	}
	if ( m_aValue[4]!=0.0 && m_aValue[4]!=1.0 || m_aValue[5]!=0.0 && m_aValue[5]!=1.0 )
	{
		m_nRowNum++;
	}
*/
	
	//计算新的CMarkDlg大小
	if (m_bMinFlag)
	{
		//m_nRowNum = 0;
	} 
	else
	{
		m_nMarkDlgHeight = MARK_TITLE_HEIGHT + MARK_ITEM_HEIGHT * m_nRowNum;
		
		//更新窗口
		MoveWindow(m_rcMarkDlgInWatchDlg.left,m_rcMarkDlgInWatchDlg.top, m_nMarkDlgWidth,m_nMarkDlgHeight);
	}
	
	if (m_bMinFlag)
	{
		//在最小化状态下
		ShowWindow(SW_SHOW);
	} 
	else
	{
		if (m_nRowNum > 0)
		{
			ShowWindow(SW_SHOW);
		}
		else{
			ShowWindow(SW_HIDE);
		}
	}
	
}

void CMarkDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here

	//建pDisplayMemDC
	CRect rcTitleAndItem;
	GetClientRect(&rcTitleAndItem);
	

	CDC* pMemDC=new CDC;
	pMemDC->CreateCompatibleDC(&dc);
	CBitmap* pBitmap = new CBitmap;
//	pBitmap->CreateCompatibleBitmap(&dc ,m_nBmpWidth ,m_nBmpHeight );
	pBitmap->CreateCompatibleBitmap(&dc ,m_nMarkDlgWidth ,m_nMarkDlgHeight );
	pMemDC->SelectObject(pBitmap);

	//绘制pMemDC底色
	CBrush BgWhiteBrush(RGB(255,255,255));
	CBrush* pOldBrush = pMemDC->SelectObject(&BgWhiteBrush);
	pMemDC->Rectangle(0,0,m_nMarkDlgWidth,m_nMarkDlgHeight);
	pMemDC->SelectObject(pOldBrush);
	
	//黑底色标题栏
	DrawTitleBar(pMemDC);
	
	//绘制Item
	DrawItem(pMemDC);
	
	//绘制网格
	DrawGrid(pMemDC);
	
	//BitBlt: pMemDC to paintDC
	dc.BitBlt(0, 0, m_nMarkDlgWidth, m_nMarkDlgHeight, pMemDC, 0, 0, SRCCOPY);

	
	//标题栏按钮
	DrawBtn(&dc);

	
	delete pMemDC;
	delete pBitmap;



	
	// Do not call CDialog::OnPaint() for painting messages
}

void CMarkDlg::DrawTitleBar(CDC *pDC)
{
	CBrush grayBrush(RGB(100,100,100));
	CBrush* pOldBrush = pDC->SelectObject(&grayBrush);
	
	CPen transPen(PS_NULL, 0 , RGB(0,0,0));
	CPen* pOldPen = pDC->SelectObject(&transPen);
	
	pDC->Rectangle(0,0,m_nMarkDlgWidth,MARK_TITLE_HEIGHT);
	pDC->Rectangle(0,MARK_TITLE_HEIGHT,17,m_nMarkDlgHeight);
	
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

	if (m_bMinFlag)
	{
		//最小化只要写个标题
		pDC->TextOut(10 , 3 , "Rulers");
		pDC->SelectObject(pOldFont);
		pDC->SetBkMode(nOldMode);
		return;
	}


	//写Item
	int nDrawX=10;
	int nColCount = 0;
	for (nColCount=0 ;nColCount<MARK_COL_NUM ;nColCount++ )
	{
		pDC->TextOut(nDrawX , 3 , g_aMarkColName[nColCount] );
		nDrawX += g_aMarkColWidth[nColCount];
	}
	
	
	pDC->SelectObject(pOldFont);
	pDC->SetBkMode(nOldMode);
}

void CMarkDlg::DrawItem(CDC *pDC)
{
	if (m_bMinFlag)
	{
		//最小化不用绘Item
		return;
	}

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CRecorderDoc *pDoc = (CRecorderDoc*)(pFrame->GetActiveDocument());
	CProduct* pProduct = &(pDoc->m_Product);
	CWaveData* pWaveData = NULL;
	
	CRect rcMarkInItem;
	double fRange =0.0;

	double fSamplingTime = 0;	
	CString strHorUnitTime;
	strHorUnitTime = ComputeHorUnitTime(&fSamplingTime , (double)(pProduct->m_nPlayWndWidth)/(double)(pProduct->m_AcqInfo.samplingRate));
	double fFreqRange = 0.0;
	CString strHorUnitFreq;
	double fAvailSampleRate = (double)pProduct->m_AcqInfo.samplingRate / 2;
	strHorUnitFreq = ComputeHorUnitFreq(&fFreqRange , fAvailSampleRate );

	CDC BmpDC;
	BmpDC.CreateCompatibleDC(pDC);
	CBitmap bmpMark;
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

	CString strTemp;
	int nRowCount = 0;
	int nColCount = 0;
	int nTextOutRowCount = 0;						//该值是当前输出行 请和nRowCount区别
	int nDrawX=0;
	float fValue_1 = 0.0;
	float fValue_2 = 0.0;
//	float fValueTemp = 0.0;

	pDC->SetTextColor(RGB(0,0,0));

	CString strUnit;

	if ( m_pOwnedDlg->m_nMode == DISPLAY_MODE_TIME )
	{
		/*时域下DrawItem*/
		for (nRowCount=0 ; nRowCount<6 ; nRowCount++)
		{

			if (0<=nRowCount && nRowCount<=3){

				if ( nRowCount < m_pOwnedDlg->m_nWaveDataNum )
				{
					strUnit = " V";
					
					pWaveData = (CWaveData*)m_pOwnedDlg->m_WaveDataList.GetAt(m_pOwnedDlg->m_WaveDataList.FindIndex(nRowCount));
					fRange = m_pOwnedDlg->m_nWaveDataRange[nRowCount];
				}
				else{
					continue;
				}
				
			}
			else{
				strUnit = strHorUnitTime;
			}

			nDrawX = 0;
			fValue_1 = m_aValue[ nRowCount*2 ];
			fValue_2 = m_aValue[ nRowCount*2 + 1 ];
			
			//读出CWatchDlg里面的该通道的量程
		

	//		TRACE("nRowCount:%d ====fValue_1: %.3f============fValue_2: %.3f=========================\n",nRowCount,fValue_1,fValue_2);
			if ( (fValue_1!=0.0 &&  fValue_1!=1.0) || (fValue_2!=0.0 &&  fValue_2!=1.0) ) 
			{
				//存在一个有效值 需要绘制该行
				nTextOutRowCount++;
			}
			else{
				//无有效值
				continue;
			}
			//写Item
			for ( nColCount=0 ; nColCount<MARK_COL_NUM ; nColCount++)
			{
				if (0 == nColCount)
				{
					//第一列特殊处理 因为第一列是色块	
					//这里应该读出pWaveData里面的颜色<---------
					switch (nRowCount)
					{
					case 0:
					case 1:
					case 2:
					case 3:					
						rcMarkInItem.left	= 3 ;
						rcMarkInItem.top	= MARK_TITLE_HEIGHT+(nTextOutRowCount-1)*MARK_ITEM_HEIGHT+3 ;
						rcMarkInItem.right	= rcMarkInItem.left + BMP_MARK_SIZE ; 
						rcMarkInItem.bottom	= rcMarkInItem.top + BMP_MARK_SIZE ; 
						pDC->FillSolidRect(rcMarkInItem, pWaveData->m_LineColor);
						break;
					case 4:
						BmpDC.SelectObject(&m_bmpMarkX);
						pDC->BitBlt(3, MARK_TITLE_HEIGHT+(nTextOutRowCount-1)*MARK_ITEM_HEIGHT+3 , BMP_MARK_SIZE, BMP_MARK_SIZE , &BmpDC, 0, 0, SRCCOPY);
						break;
					case 5:
						BmpDC.SelectObject(&m_bmpScale);
						pDC->BitBlt(3, MARK_TITLE_HEIGHT+(nTextOutRowCount-1)*MARK_ITEM_HEIGHT+3 , BMP_MARK_SIZE, BMP_MARK_SIZE , &BmpDC, 0, 0, SRCCOPY);
						break;
					}
					
					nDrawX = nDrawX + g_aMarkColWidth[nColCount] + 10 ;
					continue;
				}
				
				switch (nColCount)
				{
				case 1:		
					if ( fValue_1!=0.0 && fValue_1!=1.0 ){
						if (0<=nRowCount && nRowCount<=3){
							strTemp.Format("%.3f",(fValue_1-0.5)*2*fRange);
						} 
						else{
							strTemp.Format("%.3f",fValue_1*fSamplingTime);
						}
						strTemp = strTemp + strUnit;					
					}
					else{
						strTemp.Format("- -");					
					}
					pDC->TextOut( nDrawX, MARK_TITLE_HEIGHT + (nTextOutRowCount-1)*MARK_ITEM_HEIGHT+2, strTemp);
					nDrawX = nDrawX + g_aMarkColWidth[nColCount];
					break;

				case 2:
					if ( fValue_2!=0.0 && fValue_2!=1.0 ){
						if (0<=nRowCount && nRowCount<=3){
							strTemp.Format("%.3f",(fValue_2-0.5)*2*fRange);
						} 
						else{
							strTemp.Format("%.3f",fValue_2*fSamplingTime);
						}
						strTemp = strTemp + strUnit;
					}
					else{
						strTemp.Format("- -");
					}
					pDC->TextOut( nDrawX, MARK_TITLE_HEIGHT + (nTextOutRowCount-1)*MARK_ITEM_HEIGHT+2, strTemp);
					nDrawX = nDrawX + g_aMarkColWidth[nColCount];
					break;

				case 3:
					if ( fValue_1!=0.0 && fValue_1!=1.0 && fValue_2!=0.0 && fValue_2!=1.0){
						if (0<=nRowCount && nRowCount<=3){
							strTemp.Format("%.3f",(fValue_1 - fValue_2)*2*fRange);
						} 
						else{
							strTemp.Format("%.3f",(fValue_1 - fValue_2)*fSamplingTime);
						}
						strTemp = strTemp + strUnit;
					}
					else{
						strTemp.Format("- -");
					}
					pDC->TextOut( nDrawX, MARK_TITLE_HEIGHT + (nTextOutRowCount-1)*MARK_ITEM_HEIGHT+2, strTemp);
					nDrawX = nDrawX + g_aMarkColWidth[nColCount];
					break;

				}
				

			}
		}
		/*end 时域下绘制DrawItem*/
	} 
	else
	{
		/*频域下DrawItem*/
		double fYAxisFreqLen = 0.0;		//Y轴全长
		double fYAxisFreqZero = 0.0;	//Y轴零点
		

		for (nRowCount=0 ; nRowCount<6 ; nRowCount++)
		{

			if (0<=nRowCount && nRowCount<=3){
				/*前4行的表格是4个通道的8个mark块*/
				if ( nRowCount < m_pOwnedDlg->m_nWaveDataNum )
				{
					/*这里要分4种情况处理*/
					switch ( pProduct->m_FftConf.nOutputMode )
					{
					case YAXIS_UNIT_V:
						strUnit = " mV";
						break;
					case YAXIS_UNIT_DBV:
					case YAXIS_UNIT_DBu:
					case YAXIS_UNIT_DBm:
						strUnit = " ";
						break;
					}
					
					
					pWaveData = (CWaveData*)m_pOwnedDlg->m_WaveDataList.GetAt(m_pOwnedDlg->m_WaveDataList.FindIndex(nRowCount));
					fRange = m_pOwnedDlg->m_nWaveDataRange[nRowCount];
				}
				else{
					continue;
				}
				
			}
			else{
				/*最后2排的表格是X轴上的mark块 针对频域就是Hz*/
				strUnit = strHorUnitFreq;
			}

			switch ( (int)fRange )
			{
			case RANGE_FREQ_V_0_500:
				fYAxisFreqLen = 500.0;		//Y轴全长
				fYAxisFreqZero = 0.0;		//Y轴零点
				break;
			case RANGE_FREQ_V_0_1000:
				fYAxisFreqLen = 1000.0;		
				fYAxisFreqZero = 0.0;	
				break;
			case RANGE_FREQ_DBV_n100_0:
				fYAxisFreqLen = 100.0;		
				fYAxisFreqZero = -100.0;		
				break;
			case RANGE_FREQ_DBV_n100_100:
				fYAxisFreqLen = 200.0;		
				fYAxisFreqZero = -100.0;	
				break;
			case RANGE_FREQ_DBu_0_100:
				fYAxisFreqLen = 100.0;		
				fYAxisFreqZero = 0.0;	
				break;
			case RANGE_FREQ_DBu_0_200:
				fYAxisFreqLen = 200.0;		
				fYAxisFreqZero = 0.0;	
				break;
			case RANGE_FREQ_DBm_n100_0:
				fYAxisFreqLen = 100.0;		
				fYAxisFreqZero = -100.0;
				break;
			case RANGE_FREQ_DBm_n100_100:
				fYAxisFreqLen = 200.0;		
				fYAxisFreqZero = -100.0;
				break;
			}

			nDrawX = 0;
			fValue_1 = m_aValue[ nRowCount*2 ];
			fValue_2 = m_aValue[ nRowCount*2 + 1 ];
			
			//读出CWatchDlg里面的该通道的量程
		

	//		TRACE("nRowCount:%d ====fValue_1: %.3f============fValue_2: %.3f=========================\n",nRowCount,fValue_1,fValue_2);
			if ( (fValue_1!=0.0 &&  fValue_1!=1.0) || (fValue_2!=0.0 &&  fValue_2!=1.0) ) 
			{
				//存在一个有效值 需要绘制该行
				nTextOutRowCount++;
			}
			else{
				//无有效值
				continue;
			}

			/*写第nRowCount行的Item，一共有MARK_COL_NUM(4)列*/
			for ( nColCount=0 ; nColCount<MARK_COL_NUM ; nColCount++)
			{
				//第一列特殊处理 因为第一列是色块	
				if (0 == nColCount)
				{					
					//这里应该读出pWaveData里面的颜色<---------
					switch (nRowCount)
					{
					case 0:
					case 1:
					case 2:
					case 3:					
						rcMarkInItem.left	= 3 ;
						rcMarkInItem.top	= MARK_TITLE_HEIGHT+(nTextOutRowCount-1)*MARK_ITEM_HEIGHT+3 ;
						rcMarkInItem.right	= rcMarkInItem.left + BMP_MARK_SIZE ; 
						rcMarkInItem.bottom	= rcMarkInItem.top + BMP_MARK_SIZE ; 
						pDC->FillSolidRect(rcMarkInItem, pWaveData->m_LineColor);
						break;
					case 4:
						BmpDC.SelectObject(&m_bmpMarkX);
						pDC->BitBlt(3, MARK_TITLE_HEIGHT+(nTextOutRowCount-1)*MARK_ITEM_HEIGHT+3 , BMP_MARK_SIZE, BMP_MARK_SIZE , &BmpDC, 0, 0, SRCCOPY);
						break;
					case 5:
						BmpDC.SelectObject(&m_bmpScale);
						pDC->BitBlt(3, MARK_TITLE_HEIGHT+(nTextOutRowCount-1)*MARK_ITEM_HEIGHT+3 , BMP_MARK_SIZE, BMP_MARK_SIZE , &BmpDC, 0, 0, SRCCOPY);
						break;
					}
					
					nDrawX = nDrawX + g_aMarkColWidth[nColCount] + 10 ;
					continue;
				}
				
				//每行的后面3列
				switch (nColCount)
				{
				case 1:		
					if ( fValue_1!=0.0 && fValue_1!=1.0 ){
						
						if (0<=nRowCount && nRowCount<=3){
							//前4行是Y轴的mark块
							strTemp.Format("%.3f", fValue_1*fYAxisFreqLen+fYAxisFreqZero );
						} 
						else{
							//后2行是X轴的mark块
							strTemp.Format("%.3f", fValue_1*fFreqRange);
						}
						strTemp = strTemp + strUnit;					
					}
					else{
						strTemp.Format("- -");					
					}
					pDC->TextOut( nDrawX, MARK_TITLE_HEIGHT + (nTextOutRowCount-1)*MARK_ITEM_HEIGHT+2, strTemp);
					nDrawX = nDrawX + g_aMarkColWidth[nColCount];
					break;

				case 2:
					if ( fValue_2!=0.0 && fValue_2!=1.0 ){
						if (0<=nRowCount && nRowCount<=3){
							strTemp.Format("%.3f", fValue_2*fYAxisFreqLen+fYAxisFreqZero);
						} 
						else{
							strTemp.Format("%.3f", fValue_2*fFreqRange);
						}
						strTemp = strTemp + strUnit;
					}
					else{
						strTemp.Format("- -");
					}
					pDC->TextOut( nDrawX, MARK_TITLE_HEIGHT + (nTextOutRowCount-1)*MARK_ITEM_HEIGHT+2, strTemp);
					nDrawX = nDrawX + g_aMarkColWidth[nColCount];
					break;

				case 3:
					if ( fValue_1!=0.0 && fValue_1!=1.0 && fValue_2!=0.0 && fValue_2!=1.0){
						if (0<=nRowCount && nRowCount<=3){
							strTemp.Format("%.3f",(fValue_1 - fValue_2)*fYAxisFreqLen);
						} 
						else{
							strTemp.Format("%.3f",(fValue_1 - fValue_2)*fFreqRange);
						}
						strTemp = strTemp + strUnit;
					}
					else{
						strTemp.Format("- -");
					}
					pDC->TextOut( nDrawX, MARK_TITLE_HEIGHT + (nTextOutRowCount-1)*MARK_ITEM_HEIGHT+2, strTemp);
					nDrawX = nDrawX + g_aMarkColWidth[nColCount];
					break;

				}
				

			}
		}
		/*end 频域下绘制DrawItem*/
	}

	
//	CRect rcTest(0,0,20,20);
//	BmpDC.FillSolidRect(rcTest, RGB(255,0,0));


/*

	CString strTemp;
	for(nCount=0 ; nCount<m_nRowNum ; nCount++)
	{

		pDC->SetTextColor(RGB(0,0,0));
		//写Item
		int nDrawX=10;
		int nColCount = 0;
		for (nColCount=0 ;nColCount<MARK_COL_NUM ;nColCount++ )
		{
			nDrawX = 10;
			//pDC->TextOut(nDrawX , (nCount+1)*MEA_ITEM_HEIGHT+4,"9999" );
			switch (nColCount)
			{
			case 0:		
				if ( ( m_aValue[0]!=0.0 && m_aValue[0]!=1.0 ) || ( m_aValue[1]!=0.0 && m_aValue[1]!=1.0 ) )
				{
					//改行有效
					if ( m_aValue[0]!=0.0 && m_aValue[0]!=1.0 )	{
						//该值有效
						strTemp.Format("%.3f",m_aValue[0]);						
					}
					else{
						//该值无效
						strTemp.Format( "- -" ); 
					}
					pDC->TextOut(nDrawX , (nCount+1)*MEA_ITEM_HEIGHT+4, strTemp);

					nDrawX += g_aMarkColWidth[0];
					if ( m_aValue[1]!=0.0 && m_aValue[1]!=1.0 )
					{
						//该值有效
						strTemp.Format("%.3f",m_aValue[1]);
					}
					else{
						strTemp.Format( "- -" ); 
					}
					pDC->TextOut(nDrawX , (nCount+1)*MEA_ITEM_HEIGHT+4, strTemp);
					
					nDrawX += g_aMarkColWidth[1];
					if (( m_aValue[0]!=0.0 && m_aValue[0]!=1.0 ) && ( m_aValue[1]!=0.0 && m_aValue[1]!=1.0 )){
						strTemp.Format("%.3f",m_aValue[0]-m_aValue[1]);	
					} 
					else{
						strTemp.Format( "- -" ); 
					}
					pDC->TextOut(nDrawX , (nCount+1)*MEA_ITEM_HEIGHT+4, strTemp);
				}
				
				break;
			case 1:
				//strTemp.Format("%d",pMeasureItem->nMeaType);
				//pDC->TextOut(nDrawX , (nCount+1)*MEA_ITEM_HEIGHT+4,TransMeaType(pMeasureItem->nMeaType) );
				break;
			case 2:
				//pDC->TextOut(nDrawX , (nCount+1)*MEA_ITEM_HEIGHT+4,"9999" );
				break;
		
			default :
				break;
			}
			//nDrawX += g_aColWidth[nCount];
		}

	//pDC->TextOut(0 , (nCount+1)*MEA_ITEM_HEIGHT+4,"哈哈哈哈哈哈哈哈哈哈" );
		

	}
	*/	

	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldFont);
	pDC->SetBkMode(nOldMode);



}

void CMarkDlg::DrawGrid(CDC *pDC)
{
	if (m_bMinFlag)
	{
		//最小化不用绘网格
		return;
	}


	int nCount;
	CPen grayPen(PS_SOLID, 0 , RGB(212,208,200));
	CPen whitePen(PS_SOLID, 0 , RGB(255,255,255));
	//表格横线
	CPen* pOldPen = pDC->SelectObject(&grayPen);
	for(nCount=0 ; nCount<m_nRowNum ; nCount++)
	{
		
		pDC->MoveTo(0 , MARK_TITLE_HEIGHT+(nCount)*MARK_ITEM_HEIGHT);
		pDC->LineTo(m_nMarkDlgWidth , MARK_TITLE_HEIGHT+(nCount)*MARK_ITEM_HEIGHT);	
		//pDC->SelectObject(pOldPen);
	}
	
	//表格竖线
	int nDrawX = 5;					
	for(nCount=0 ; nCount<MARK_COL_NUM ; nCount++)
	{
		if (nCount == 0)
		{
			//第一道线不画  为了美观
			nDrawX += g_aMarkColWidth[nCount];
			continue;
		}
		pDC->SelectObject(&whitePen);
		pDC->MoveTo(nDrawX , 0);
		pDC->LineTo(nDrawX , MARK_TITLE_HEIGHT);	
		pDC->SelectObject(pOldPen);
		
		pDC->SelectObject(&grayPen);
		pDC->MoveTo(nDrawX , MARK_TITLE_HEIGHT);
		pDC->LineTo(nDrawX , m_nMarkDlgHeight);	
		pDC->SelectObject(pOldPen);
		
		nDrawX += g_aMarkColWidth[nCount];
	}



			CRect rcClient;
			GetClientRect(&rcClient);
			CPen blackPen(PS_SOLID, 1 , RGB(0,0,0));
			
			CBrush *pBrush = CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
			CBrush *pOldBrush = pDC->SelectObject(pBrush);
			
			pDC->Rectangle(&rcClient);
		
			pDC->SelectObject(pOldPen);
			pDC->SelectObject(pOldBrush);

	

}

void CMarkDlg::DrawBtn(CDC *pDC)
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
	rcTemp.bottom = MARK_TITLE_HEIGHT;
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

void CMarkDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	TRACE("====into CMarkDlg::OnMouseMove==========================\n");

	CRect rcClient;
	CRect rcMarkDlg;
	GetClientRect(&rcClient);
	int nWidth = rcClient.Width();
	int nHeight = rcClient.Height();
	if (m_bIsPick)
	{
		//鼠标被按下 进入拖动状态
		
/*
		if (point.x <= m_nClickPointToLeft)
		{
			m_ptMoveMouse.x = m_nClickPointToLeft;
			
		} 
		else if (point.x > nWidth - m_nClickPointToRight)
		{
			m_ptMoveMouse.x = nWidth - m_nClickPointToRight;
		}

		if (point.y <= m_nClickPointToTop)
		{
			m_ptMoveMouse.y = m_nClickPointToTop;
		} 
		else if (point.y > nHeight - m_nClickPointToBottom)
		{
			m_ptMoveMouse.y = nHeight - m_nClickPointToBottom;
		}


		m_rcMarkDlgInWatchDlg.left += point.x - m_ptMoveMouse.x;
		m_rcMarkDlgInWatchDlg.right += point.x - m_ptMoveMouse.x;
		m_rcMarkDlgInWatchDlg.top += point.y - m_ptMoveMouse.y;
		m_rcMarkDlgInWatchDlg.bottom += point.x - m_ptMoveMouse.y;
		m_ptMoveMouse = point;
//		MoveWindow(m_rcMarkDlgInWatchDlg);
*/
		return;


	}

	
	if (!(m_rcBtnMinOrRe.PtInRect(point) || m_rcBtnClose.PtInRect(point)))
	{
		//没有移动到2个按钮上
		return;
	}


	//鼠标未被按下 进行鼠标是否划过按钮的测试
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

void CMarkDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	TRACE("====into CMarkDlg::OnLButtonUp==========================\n");

	if (m_bIsPick)
	{
		//正在拖动标题栏
		m_bIsPick	= FALSE;
		
//		TRACE("CMarkDlg: before ReleaseCapture\n");
		ReleaseCapture();
//		TRACE("CMarkDlg: after ReleaseCapture\n");

		//调整MarkDlg位置
		CRect rc;
		CRect rcParentClient;
		CWatchDlg* pParentDlg;
		pParentDlg = (CWatchDlg*)GetParent();
		pParentDlg->GetClientRect(&rcParentClient);
		GetClientRect(&rc);
		ClientToScreen(&rc);
		pParentDlg->ScreenToClient(&rc);

		if (rc.left < 0)
		{
			rc.left = 0;
			rc.right = m_nMarkDlgWidth;
		}
		if (rc.top < 0)
		{
			rc.top = 0;
			rc.bottom = m_nMarkDlgHeight;
		}
		if (rc.right > rcParentClient.Width())
		{
			rc.right = rcParentClient.Width();
			rc.left = rc.right - m_nMarkDlgWidth;			
		}
		if (rc.bottom > rcParentClient.Height())
		{
			rc.bottom = rcParentClient.Height();
			rc.top = rc.bottom - m_nMarkDlgHeight;			
		}
		MoveWindow(rc);

		return;
	}


	if (m_rcBtnMinOrRe.PtInRect(point))
	{
		//鼠标点击了BtnMinOrRe按钮

		m_bMinFlag = !m_bMinFlag;
		if (m_bMinFlag)
		{
			//进入最小化
			m_nMarkDlgHeight = MARK_TITLE_HEIGHT;
			m_nMarkDlgWidth = 100;
			m_rcMarkDlgInWatchDlg.left = m_rcMarkDlgInWatchDlg.right - m_nMarkDlgWidth;
			m_rcMarkDlgInWatchDlg.bottom = m_rcMarkDlgInWatchDlg.top - MARK_TITLE_HEIGHT;
			MoveWindow(m_rcMarkDlgInWatchDlg.left,m_rcMarkDlgInWatchDlg.top,m_nMarkDlgWidth,m_nMarkDlgHeight);
			Invalidate();
		}
		else{
			//还原
			m_nMarkDlgHeight = MARK_TITLE_HEIGHT + MARK_ITEM_HEIGHT * m_nRowNum;
			m_nMarkDlgWidth = m_nMarkDlgWidthNormal;
			m_rcMarkDlgInWatchDlg.left = m_rcMarkDlgInWatchDlg.right - m_nMarkDlgWidth;
			m_nMarkDlgHeight = MARK_TITLE_HEIGHT + MARK_ITEM_HEIGHT * m_nRowNum;
			m_rcMarkDlgInWatchDlg.bottom = m_rcMarkDlgInWatchDlg.top - m_nMarkDlgHeight;
			MoveWindow(m_rcMarkDlgInWatchDlg.left,m_rcMarkDlgInWatchDlg.top,m_nMarkDlgWidth,m_nMarkDlgHeight);
			Invalidate();
		}
				
/*				
		m_pMatchWatchDlg->ComputeFrame();
		m_pMatchWatchDlg->ModifyPos();
*/
		
	}
	else if (m_rcBtnClose.PtInRect(point))
	{
		//鼠标点击了BtnClose按钮
		ClearMark();		
		ShowWindow(SW_HIDE);
		m_pOwnedDlg->Invalidate();
		
	}
	else
	{
		//点击了标题栏  准备移动框体
	
	}

	m_bIsPick	= FALSE;
	
//	TRACE("CMarkDlg: before ReleaseCapture\n");
	ReleaseCapture();
//	TRACE("CMarkDlg: after ReleaseCapture\n");


	
	CDialog::OnLButtonUp(nFlags, point);
}

void CMarkDlg::ClearMark()
{
	m_aValue[0] = 1.0;
	m_aValue[1] = 1.0;
	m_aValue[2] = 1.0;
	m_aValue[3] = 1.0;
	m_aValue[4] = 1.0;
	m_aValue[5] = 1.0;
	m_aValue[6] = 1.0;
	m_aValue[7] = 1.0;

	m_aValue[8] = 0.0;
	m_aValue[9] = 0.0;
	m_aValue[10] = 1.0;
	m_aValue[11] = 1.0;

	m_pOwnedDlg->SetMarkByDefault();
}

UINT CMarkDlg::OnNcHitTest(CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

//	TRACE("====into CMarkDlg::OnNcHitTest==========================\n");
/*	*/

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
	m_rcMarkDlgInWatchDlg = rc;
	
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

void CMarkDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	TRACE("====into CMarkDlg::OnLButtonDown==========================\n");

	CRect rcClient;
	GetClientRect(&rcClient);
	CRect rcTitleBar;
	rcTitleBar.left = 0;
	rcTitleBar.top = 0;
	rcTitleBar.right = m_rcBtnMinOrRe.left-1;
	rcTitleBar.bottom = MARK_TITLE_HEIGHT;

	if(rcTitleBar.PtInRect(point))
	{
		//鼠标点击在了标题栏处
		m_bIsPick = TRUE;
		m_ptMoveMouse= point;

		//计算点击位置
		m_nClickPointToLeft = point.x;
		m_nClickPointToTop = point.y;
		m_nClickPointToRight = rcClient.Width()-point.x;
		m_nClickPointToBottom = rcClient.Height()-point.y;
		
//		TRACE("CMarkDlg::OnLButtonDown : position : %d , %d , %d , %d \n",m_nClickPointToLeft,m_nClickPointToTop,m_nClickPointToRight,m_nClickPointToBottom);
		//TRACE("before SetCapture\n");
		SetCapture();
		//TRACE("after SetCapture\n");
	}


	CDialog::OnLButtonDown(nFlags, point);
}

BOOL CMarkDlg::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
//	return TRUE;
	return CDialog::OnEraseBkgnd(pDC);
}

void CMarkDlg::OnNcLButtonUp(UINT nHitTest, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	TRACE("===into CMarkDlg::OnNcLButtonUp============\n");
	//调整MarkDlg位置
	CRect rc;
	CRect rcParentClient;
	CWatchDlg* pParentDlg;
	pParentDlg = (CWatchDlg*)GetParent();
	pParentDlg->GetClientRect(&rcParentClient);
	GetClientRect(&rc);
	ClientToScreen(&rc);
	pParentDlg->ScreenToClient(&rc);
	
	if (rc.left < 0)
	{
		rc.left = 0;
		rc.right = m_nMarkDlgWidth;
	}
	if (rc.top < 0)
	{
		rc.top = 0;
		rc.bottom = m_nMarkDlgHeight;
	}
	if (rc.right > rcParentClient.Width())
	{
		rc.right = rcParentClient.Width();
		rc.left = rc.right - m_nMarkDlgWidth;			
	}
	if (rc.bottom > rcParentClient.Height())
	{
		rc.bottom = rcParentClient.Height();
		rc.top = rc.bottom - m_nMarkDlgHeight;			
	}
	MoveWindow(rc);

	CDialog::OnNcLButtonUp(nHitTest, point);
}

void CMarkDlg::OnNcMouseMove(UINT nHitTest, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	CDialog::OnNcMouseMove(nHitTest, point);
}

CString CMarkDlg::ComputeHorUnitTime(double *pfResult, double fInput)
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

CString CMarkDlg::ComputeHorUnitFreq(double *pfResult, double fInput)
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

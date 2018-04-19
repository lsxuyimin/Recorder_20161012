// RecorderDoc.cpp : implementation of the CRecorderDoc class
//

#include "stdafx.h"
#include "Recorder.h"


#include "RecorderDoc.h"
#include <math.h>


#include "WaveData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRecorderDoc

IMPLEMENT_DYNCREATE(CRecorderDoc, CDocument)

BEGIN_MESSAGE_MAP(CRecorderDoc, CDocument)
	//{{AFX_MSG_MAP(CRecorderDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRecorderDoc construction/destruction
int g_nScreenWidth; 
int g_nScreenHeight; 


CRecorderDoc::CRecorderDoc()
{
	// TODO: add one-time construction code here
	m_bAcqing = false;
	
	
//	g_nScreenWidth = GetSystemMetrics ( SM_CXSCREEN ); 
//	g_nScreenHeight= GetSystemMetrics ( SM_CYSCREEN ); 
	
	
/*
	for (int i=0 ; i<MAX_CHANNUM ; i++)
	{
		CWaveData* pWaveData = new CWaveData();
		m_WaveList.AddTail(pWaveData);
	}
*/
}


CRecorderDoc::~CRecorderDoc()
{
}


BOOL CRecorderDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CRecorderDoc serialization

void CRecorderDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CRecorderDoc diagnostics

#ifdef _DEBUG
void CRecorderDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CRecorderDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CRecorderDoc commands



DWORD WINAPI ControlAcqProc2(  LPVOID lpParameter  )
{
	//	IDTS_Product * tProduct = ( IDTS_Product * )lpParameter;
	
	int i,j;
	double randomNum;
	CRecorderDoc* pDoc = (CRecorderDoc*)(lpParameter);
	
	CTypedPtrList<CObList, CWaveData*>* pWaveList = ( CTypedPtrList<CObList, CWaveData*>* )(&(pDoc->m_WaveList));
	
	time_t t;
	time(&t);
	srand(t);
	
	while( pDoc -> m_bAcqing )
	{
		for ( i = 0 ; i < MAX_CHANNUM ; i++)
		{
			CWaveData* pWave = pWaveList->GetAt(pWaveList->FindIndex(i));
			//pWave->m_WaveData.RemoveAll();
			randomNum = (rand()%1000) / 100.0;
			for ( j = 0 ; j < g_nScreenWidth ; j++)
			{
				//随机波形
				//pWave->m_WaveData.SetAt(j,rand()%10000-5000);						//存入数字 0~100 之间
				//随机正弦波
				int save = (int)(5000* sin((j%200-100.0)/100*2*3.14+randomNum));
				pWave->m_WaveData.SetAt(j, (int)(5000* sin((j%200-100.0)/100*2*3.14+randomNum)));

			}
		}
		//每次循环休眠50ms  这里希望每50ms产生一组数据
		Sleep(50);
	}
	return 0;
}


void CRecorderDoc::StartCapture()
{
	if (1)
	{
		//随机波形
		//产生64组波形  存入m_WaveList
		//		int i,j;
		time_t t;
		time(&t);
		srand(t);
		
// 		CMainFrame *pMain = (CMainFrame*)AfxGetMainWnd();
// 		int maxWidth = pMain->m_nScreenWidth;							//最大取数个数按屏幕来定
		
		SECURITY_ATTRIBUTES saAttr;
		saAttr.nLength = sizeof( SECURITY_ATTRIBUTES );
		saAttr.bInheritHandle = TRUE; 
		saAttr.lpSecurityDescriptor = NULL; 
		DWORD ThreadId ;
		
		m_bAcqing = TRUE;
		VERIFY( CreateThread(&saAttr, 1000, ControlAcqProc2, (void *)(this), 0, &ThreadId ) );
		
	} 
	else
	{
	}
	
}

void CRecorderDoc::StopCapture()
{
	if (1)
	{
		//停止产生随机波形
		
		m_bAcqing = FALSE;
	} 
	else
	{
	}
}


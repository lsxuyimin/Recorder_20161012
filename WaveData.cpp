// WaveData.cpp : implementation file
//

#include "stdafx.h"
#include "Recorder.h"
#include "WaveData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWaveData

IMPLEMENT_DYNCREATE(CWaveData, CCmdTarget)

extern int g_nScreenWidth;

CWaveData::CWaveData()
{
	int i ;
	for (i=0 ; i<g_nScreenWidth; i++)
	{
		m_WaveData.Add(0);
	}
}

CWaveData::CWaveData(int nChnIndex)
{
	int i ;
	for (i=0 ; i<g_nScreenWidth; i++)
	{
		m_WaveData.Add(0);
	}

	m_nChnIndex = nChnIndex;
	GenerateColor(m_nChnIndex);
}


CWaveData::~CWaveData()
{
}


BEGIN_MESSAGE_MAP(CWaveData, CCmdTarget)
	//{{AFX_MSG_MAP(CWaveData)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWaveData message handlers

int CWaveData::GenerateColor(int nRandomSeed , int nWaveMode)
{
	//RGB的组合除了灰色系，还有6种组合：R,B,G,RG,RB,BG

	int nRed = 0;
	int nGreen = 0;
	int nBlue = 0;

	if( nWaveMode==0 )
	{
		//正常通道
		switch ( nRandomSeed%6 )
		{
		case 0:
			nRed = 255;
			break;
		case 1:
			nGreen = 255;
			break;
		case 2:
			nBlue = 255;
			break;
		case 3:
			nRed = 255;
			nGreen = 255;
			break;
		case 4:
			nRed = 255;
			nBlue = 255;
			break;
		case 5:
			nBlue = 255;
			nGreen = 255;
			break;
		default:
			break;
		}
		int nTemp = (int)(nRandomSeed/6);
		if ( nRed!=0 )
		{
			nRed = 255 - 20*nTemp;
		}
		if ( nGreen!=0 )
		{
			nGreen = 255 - 20*nTemp;
		}
		if ( nBlue!=0 )
		{
			nBlue = 255 - 20*nTemp;
		}
	}	
	else{
		//组合通道
		int nTemp = (int)(nRandomSeed%6);
		nRed	= 100 + 20*nTemp;
		nGreen	= 100 + 20*nTemp;
		nBlue	= 100 + 20*nTemp;
	}

	m_LineColor = RGB(nRed, nGreen, nBlue);


	return 1;

}

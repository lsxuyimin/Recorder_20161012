// MeasureItem.cpp : implementation file
//

#include "stdafx.h"
#include "recorder.h"
#include "MeasureItem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMeasureItem

IMPLEMENT_DYNCREATE(CMeasureItem, CCmdTarget)

CMeasureItem::CMeasureItem()
{
	pWaveData = NULL;					//数据（通道）
	nMeaType = -1;						//测量类型
	nMeaPart = -1;

	strMeaValue = "- -";						//测量计算结果
	strMin = "- -";							
	strMax = "- -";						
	strAverage = "- -";						//计算平均值
	strVariance = "- -";						//测量方差
}

CMeasureItem::~CMeasureItem()
{
//	TRACE("=============CMeasureItem::~CMeasureItem()==============================\n");
}


BEGIN_MESSAGE_MAP(CMeasureItem, CCmdTarget)
	//{{AFX_MSG_MAP(CMeasureItem)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMeasureItem message handlers

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
	pWaveData = NULL;					//���ݣ�ͨ����
	nMeaType = -1;						//��������
	nMeaPart = -1;

	strMeaValue = "- -";						//����������
	strMin = "- -";							
	strMax = "- -";						
	strAverage = "- -";						//����ƽ��ֵ
	strVariance = "- -";						//��������
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

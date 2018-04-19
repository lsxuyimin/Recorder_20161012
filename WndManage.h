// WndManage.h: interface for the CWndManage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WNDMANAGE_H__DC181133_6B54_455A_957B_E4724E6DD26E__INCLUDED_)
#define AFX_WNDMANAGE_H__DC181133_6B54_455A_957B_E4724E6DD26E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CWatchDlg;
class CBgDlg;

class CRecorderView;
class CRecorderDoc;
class CMainFrame;
class CProduct;

class CWndManage  
{
public:
	void AllWndRefresh_Complete();
	void AllWndRefresh();								//�������д�����ʾͼ�� ���������е�WDW���ο�
	void DefaultSetting();								//Ĭ�����ã������Ժ����ڡ�Ĭ�����á������ť�ϣ����򿪵�X��ͨ�����μ��뵽ǰX��BgDlg�У�ÿ��BgDlg��һ��
	bool GetVideoRect(int nSub, CRect& rc);
	int m_nModeSave;
	int m_nCurMode;
	int m_nSubWnd;										//�����Ӵ�����
	int m_nFirst;
	int m_nLast;
	void Postion();
	CRect m_rcFrameWnd;									//������ʾ�Ŀ��CRECT
	void Display(int nMode=0);
	CRecorderView* m_pOwnedViex;							//����DLG�ĸ�����ָ�� Ҳ����View����
	void Create(int nSubWnd, const RECT &rect, CRecorderView* pOwnedViex);
	int m_nCurSelect;

	CWatchDlg* m_pWatchDlg;// can be delete
	
	CBgDlg* m_pBgDlg;

	WavePlayCtrl  m_WPCUnified;						//20150507�汾�Ժ� ����watchdlg�ķŴ���С��ͬ���� ���Խ�������ʾ�Ľṹ�����������

	CWndManage();
	virtual ~CWndManage();

};

#endif // !defined(AFX_WNDMANAGE_H__DC181133_6B54_455A_957B_E4724E6DD26E__INCLUDED_)

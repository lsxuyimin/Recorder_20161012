// CheckCardDlg.cpp : implementation file
//

#include "stdafx.h"
#include "recorder.h"
#include "CheckCardDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
#include "MainFrm.h"
#include "RecorderDoc.h"
#include "Product.h"

#include "WCDPcie//WCDPcieDriver.h"
//#pragma comment(lib,"WCDPcie//WCDPcieDriver.lib")
/////////////////////////////////////////////////////////////////////////////
// CCheckCardDlg dialog


CCheckCardDlg::CCheckCardDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCheckCardDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCheckCardDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


CCheckCardDlg::CCheckCardDlg(CProduct* pProduct ,CWnd* pParent, int nInitialMode)
	: CDialog(CCheckCardDlg::IDD, pParent)
{
	m_nInitialMode = nInitialMode;
	m_pProduct = pProduct;
}



CCheckCardDlg::CCheckCardDlg(CProduct* pProduct , CWnd* pParent /*=NULL*/)
	: CDialog(CCheckCardDlg::IDD, pParent)
{
	m_pProduct = pProduct;
}


void CCheckCardDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCheckCardDlg)
	DDX_Control(pDX, IDC_BUTTON_Browse, m_buttonBrowse);
	DDX_Control(pDX, IDC_RADIO_Demo, m_RadioMode);
	DDX_Control(pDX, IDC_LIST_PhyCard, m_ListPhyCard);
	DDX_Control(pDX, IDC_COMBO_DemoChn, m_ComboDemoChn);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCheckCardDlg, CDialog)
	//{{AFX_MSG_MAP(CCheckCardDlg)
	ON_BN_CLICKED(IDC_RADIO_PhyCard, OnRADIOPhyCard)
	ON_BN_CLICKED(IDC_RADIO_Demo, OnRADIOMode)
	ON_BN_CLICKED(IDC_BUTTON_Browse, OnBUTTONBrowse)
	ON_BN_CLICKED(IDC_RADIO_OpenExistData, OnRADIOOpenExistData)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCheckCardDlg message handlers

BOOL CCheckCardDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	/*如果不是第一次调用CCheckCardDlg，说明之前有可能打开过物理卡，那么要进行检查并关闭打开的物理卡*/
	if ( 0 != m_nCardNum )
	{
		for(long nCardIndex=0;nCardIndex<m_nCardNum;nCardIndex++)
		{
			//启动采集
			WCDPcie_CloseDevice(nCardIndex);
		}
	}
	m_nCardNum = 0;
	m_pProduct->m_PhyCard.m_nPhyCardNum =0;


	CString strDebug;
	int i;
	CString str;
	for (i=1 ; i<=MAX_CHANNUM ; i++)
	{
		str.Format("%d",i);
		m_ComboDemoChn.InsertString(i-1,str);
	}
	
	if ( 2 == m_nInitialMode)
	{
		//是“打开”按钮激活的CCheckCardDlg
		
		((CButton *)(GetDlgItem(IDC_RADIO_Demo)))->SetCheck(FALSE);		
		((CButton *)(GetDlgItem(IDC_RADIO_PhyCard)))->SetCheck(FALSE);	
		((CButton *)(GetDlgItem(IDC_RADIO_OpenExistData)))->SetCheck(TRUE);		
		m_ListPhyCard.EnableWindow(FALSE);
		m_ComboDemoChn.EnableWindow(FALSE);
		m_buttonBrowse.EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_ExistDataPath)->EnableWindow(TRUE);

	}
	else{
		//默认选择demo模式
		((CButton *)GetDlgItem(IDC_RADIO_Demo))->SetCheck(TRUE);		
		((CButton *)GetDlgItem(IDC_RADIO_PhyCard))->SetCheck(FALSE);	
		((CButton *)GetDlgItem(IDC_RADIO_OpenExistData))->SetCheck(FALSE);		
		m_ListPhyCard.EnableWindow(FALSE);
		m_ComboDemoChn.EnableWindow(TRUE);
		m_buttonBrowse.EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_ExistDataPath)->EnableWindow(FALSE);
	}
	
	m_ComboDemoChn.SetCurSel(63);

	//检测当前物理采集卡的数量 并加入ListBox	
	m_pProduct->m_PhyCard.m_nPhyCardNum = WCDPcie_ScanfDevice(0x10ee,0x0007,m_pProduct->m_PhyCard.m_BusIDArray,m_pProduct->m_PhyCard.m_SlotIDArray);
//	m_pProduct->m_PhyCard.m_nPhyCardNum = 8;	//TEST 这行是测试用
	m_nCardNum = m_pProduct->m_PhyCard.m_nPhyCardNum ;

	strDebug.Format("Bunny Tips : the number of phy cards is %d.\n",m_nCardNum);
	TRACE(strDebug);

	CString strTemp;
	m_ListPhyCard.ResetContent();
	unsigned long nChNumInCard = 0;
	int nCount;

	/*如果有物理卡存在，那就打开设备，并显示在List上*/
	if ( 0 != m_nCardNum )
	{		
		WCDPcie_OpenAllDevice(0x10ee,0x0007);
	
		for(nCount=0;nCount<m_nCardNum;nCount++)
		{
			//针对每张卡获取卡的通道数，并打印出来
			nChNumInCard = WCDPcie_GetADCHNum(nCount);
			strTemp.Format("Card%d  %dch" , nCount+1 , nChNumInCard);
			m_ListPhyCard.AddString(strTemp);
		}

	}



	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCheckCardDlg::OnRADIOPhyCard() 
{
	// TODO: Add your control notification handler code here
	((CButton *)GetDlgItem(IDC_RADIO_Demo))->SetCheck(FALSE);		
	((CButton *)GetDlgItem(IDC_RADIO_PhyCard))->SetCheck(TRUE);		
	((CButton *)GetDlgItem(IDC_RADIO_OpenExistData))->SetCheck(FALSE);		
	m_ListPhyCard.EnableWindow(TRUE);
	m_ComboDemoChn.EnableWindow(FALSE);
	m_buttonBrowse.EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_ExistDataPath)->EnableWindow(FALSE);
}

void CCheckCardDlg::OnRADIOMode() 
{
	// TODO: Add your control notification handler code here
	((CButton *)GetDlgItem(IDC_RADIO_Demo))->SetCheck(TRUE);		
	((CButton *)GetDlgItem(IDC_RADIO_PhyCard))->SetCheck(FALSE);	
	((CButton *)GetDlgItem(IDC_RADIO_OpenExistData))->SetCheck(FALSE);		
	m_ListPhyCard.EnableWindow(FALSE);
	m_ComboDemoChn.EnableWindow(TRUE);
	m_buttonBrowse.EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_ExistDataPath)->EnableWindow(FALSE);
}

void CCheckCardDlg::OnRADIOOpenExistData() 
{
	// TODO: Add your control notification handler code here
	((CButton *)GetDlgItem(IDC_RADIO_Demo))->SetCheck(FALSE);		
	((CButton *)GetDlgItem(IDC_RADIO_PhyCard))->SetCheck(FALSE);	
	((CButton *)GetDlgItem(IDC_RADIO_OpenExistData))->SetCheck(TRUE);		
	m_ListPhyCard.EnableWindow(FALSE);
	m_ComboDemoChn.EnableWindow(FALSE);
	m_buttonBrowse.EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_ExistDataPath)->EnableWindow(TRUE);
}

void CCheckCardDlg::OnOK() 
{
	// TODO: Add extra validation here
	m_pProduct->AcqEnvInitialize();

	if (1 == ((CButton *)GetDlgItem(IDC_RADIO_Demo))->GetCheck())
	{	//选择中的是Demo
		m_pProduct->m_nRecorderMode = 1;
		int curSelect = m_ComboDemoChn.GetCurSel();
		CString curStr;
		m_ComboDemoChn.GetLBText(curSelect, curStr);
		m_pProduct->m_nOpenChannelNum = atoi(curStr);
		m_pProduct->m_VirtualCard.VirCard_Initialize(m_pProduct->m_nOpenChannelNum);

	}
	else if(1 == ((CButton *)GetDlgItem(IDC_RADIO_PhyCard))->GetCheck()){	
		//选中的是PhyCard
		m_pProduct->m_nRecorderMode = 0;
		m_pProduct->m_nOpenChannelNum = m_pProduct->m_PhyCard.m_nPhyCardNum * CH_NUM_IN_PER_CARD;		//每张卡有8个通道
	//	m_pProduct->m_PhyCard.PhyCard_Initialize(m_pProduct->m_nOpenChannelNum);		//这里应该打开8张卡的设备 但是其实已经在该dlg的初始化函数中通过WCDPcie_OpenAllDevice打开了
		//打开所有设备
	//	WCDPcie_OpenAllDevice(0x10ee,0x0007);

	}
	else{
		//选中的是OpenExist		
		m_pProduct->m_nRecorderMode = 2;
		//读setup.inf 获取播放参数 读入m_AcqInfo
		//禁用各种在0 1模式下的功能按钮
		
		//进行判断 若不是本软件使用的setup.inf
		int nResult = ReadInSettingFile();
		if ( 0 == nResult )	{
			AfxMessageBox("无法识别的配置文件！");
			return;
		}
		else if (2 == nResult)
		{
			AfxMessageBox("路径为空！");
			return;
		}
		else{
			//正确读出配置文件
//			m_pProduct->Initialize();
//			m_pProduct->PlayEnvInitialize();
		}
	}
		
	CDialog::OnOK();
}

void CCheckCardDlg::OnBUTTONBrowse() 
{
	// TODO: Add your control notification handler code here

	CFileDialog fileDlg(TRUE,NULL,NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,"inf Files(*.inf)|*.inf|All Files(*.*)|*.*");
	if (IDOK == fileDlg.DoModal())
	{
		m_strFilePath =  fileDlg.GetPathName();//取得文件路径及文件名
        SetDlgItemText(IDC_EDIT_ExistDataPath , m_strFilePath);
	}

}



int CCheckCardDlg::ReadInSettingFile()
{
	if (m_strFilePath.IsEmpty())
	{
		return 2;
	}
	//检查 RECORDER SETTING FILE
	CString str;
	char line[100];
	FILE* pSettingFile;
	pSettingFile = fopen(m_strFilePath, "r");
	fseek(pSettingFile , 0 , SEEK_SET);


	fgets( line, 100, pSettingFile );    
	str.Format("%s", line);
	str.TrimLeft();
	str.TrimRight();
	if ( str != "RECORDER SETTING FILE")
	{
		//不是本软件的setup.inf
		return 0;
	}

	//修改m_strCurWorkPath m_strWorkPlace
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CRecorderDoc *pDoc = (CRecorderDoc*)(pFrame->GetActiveDocument());
	CProduct* pProduct = &(pDoc->m_Product);
	int nPos;   
	nPos=m_strFilePath.ReverseFind('\\');   
	pProduct->m_strCurWorkPath=m_strFilePath.Left(nPos);
//	TRACE(pProduct->m_strCurWorkPath);


////////////写入格式//////////////////////////////////////////////////////////////
//	0.检查用行
//	1.通道数
//	2.量程
//	3.采样长度
//	4.采样速率
//	5.触发方式
//	6.触发通道
//	7.触发电平
//	8.采样延时
//////////////////////////////////////////////////////////////////////////
	fgets( line, 100, pSettingFile );    
	str.Format("%s", line);
	str.TrimLeft();
	str.TrimRight();
	pProduct->m_nOpenChannelNum = atoi(str);

	fgets( line, 100, pSettingFile );    
	str.Format("%s", line);
	str.TrimLeft();
	str.TrimRight();
	pProduct->m_AcqInfo.range = atoi(str);

	fgets( line, 100, pSettingFile );    
	str.Format("%s", line);
	str.TrimLeft();
	str.TrimRight();
	pProduct->m_AcqInfo.samplingLength = atoi(str);

	fgets( line, 100, pSettingFile );    
	str.Format("%s", line);
	str.TrimLeft();
	str.TrimRight();
	pProduct->m_AcqInfo.samplingRate = atoi(str);

	fgets( line, 100, pSettingFile );    
	str.Format("%s", line);
	str.TrimLeft();
	str.TrimRight();
	pProduct->m_AcqInfo.trigMode = atoi(str);

	fgets( line, 100, pSettingFile );    
	str.Format("%s", line);
	str.TrimLeft();
	str.TrimRight();
	pProduct->m_AcqInfo.trigChan = atoi(str);

	fgets( line, 100, pSettingFile );    
	str.Format("%s", line);
	str.TrimLeft();
	str.TrimRight();
	pProduct->m_AcqInfo.trigLevel = atoi(str);

	fgets( line, 100, pSettingFile );    
	str.Format("%s", line);
	str.TrimLeft();
	str.TrimRight();
	pProduct->m_AcqInfo.samplingDelay = atoi(str);

	fclose(pSettingFile);
	pSettingFile = NULL;

	return 1;

}

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

	/*������ǵ�һ�ε���CCheckCardDlg��˵��֮ǰ�п��ܴ򿪹���������ôҪ���м�鲢�رմ򿪵�����*/
	if ( 0 != m_nCardNum )
	{
		for(long nCardIndex=0;nCardIndex<m_nCardNum;nCardIndex++)
		{
			//�����ɼ�
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
		//�ǡ��򿪡���ť�����CCheckCardDlg
		
		((CButton *)(GetDlgItem(IDC_RADIO_Demo)))->SetCheck(FALSE);		
		((CButton *)(GetDlgItem(IDC_RADIO_PhyCard)))->SetCheck(FALSE);	
		((CButton *)(GetDlgItem(IDC_RADIO_OpenExistData)))->SetCheck(TRUE);		
		m_ListPhyCard.EnableWindow(FALSE);
		m_ComboDemoChn.EnableWindow(FALSE);
		m_buttonBrowse.EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_ExistDataPath)->EnableWindow(TRUE);

	}
	else{
		//Ĭ��ѡ��demoģʽ
		((CButton *)GetDlgItem(IDC_RADIO_Demo))->SetCheck(TRUE);		
		((CButton *)GetDlgItem(IDC_RADIO_PhyCard))->SetCheck(FALSE);	
		((CButton *)GetDlgItem(IDC_RADIO_OpenExistData))->SetCheck(FALSE);		
		m_ListPhyCard.EnableWindow(FALSE);
		m_ComboDemoChn.EnableWindow(TRUE);
		m_buttonBrowse.EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_ExistDataPath)->EnableWindow(FALSE);
	}
	
	m_ComboDemoChn.SetCurSel(63);

	//��⵱ǰ����ɼ��������� ������ListBox	
	m_pProduct->m_PhyCard.m_nPhyCardNum = WCDPcie_ScanfDevice(0x10ee,0x0007,m_pProduct->m_PhyCard.m_BusIDArray,m_pProduct->m_PhyCard.m_SlotIDArray);
//	m_pProduct->m_PhyCard.m_nPhyCardNum = 8;	//TEST �����ǲ�����
	m_nCardNum = m_pProduct->m_PhyCard.m_nPhyCardNum ;

	strDebug.Format("Bunny Tips : the number of phy cards is %d.\n",m_nCardNum);
	TRACE(strDebug);

	CString strTemp;
	m_ListPhyCard.ResetContent();
	unsigned long nChNumInCard = 0;
	int nCount;

	/*������������ڣ��Ǿʹ��豸������ʾ��List��*/
	if ( 0 != m_nCardNum )
	{		
		WCDPcie_OpenAllDevice(0x10ee,0x0007);
	
		for(nCount=0;nCount<m_nCardNum;nCount++)
		{
			//���ÿ�ſ���ȡ����ͨ����������ӡ����
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
	{	//ѡ���е���Demo
		m_pProduct->m_nRecorderMode = 1;
		int curSelect = m_ComboDemoChn.GetCurSel();
		CString curStr;
		m_ComboDemoChn.GetLBText(curSelect, curStr);
		m_pProduct->m_nOpenChannelNum = atoi(curStr);
		m_pProduct->m_VirtualCard.VirCard_Initialize(m_pProduct->m_nOpenChannelNum);

	}
	else if(1 == ((CButton *)GetDlgItem(IDC_RADIO_PhyCard))->GetCheck()){	
		//ѡ�е���PhyCard
		m_pProduct->m_nRecorderMode = 0;
		m_pProduct->m_nOpenChannelNum = m_pProduct->m_PhyCard.m_nPhyCardNum * CH_NUM_IN_PER_CARD;		//ÿ�ſ���8��ͨ��
	//	m_pProduct->m_PhyCard.PhyCard_Initialize(m_pProduct->m_nOpenChannelNum);		//����Ӧ�ô�8�ſ����豸 ������ʵ�Ѿ��ڸ�dlg�ĳ�ʼ��������ͨ��WCDPcie_OpenAllDevice����
		//�������豸
	//	WCDPcie_OpenAllDevice(0x10ee,0x0007);

	}
	else{
		//ѡ�е���OpenExist		
		m_pProduct->m_nRecorderMode = 2;
		//��setup.inf ��ȡ���Ų��� ����m_AcqInfo
		//���ø�����0 1ģʽ�µĹ��ܰ�ť
		
		//�����ж� �����Ǳ����ʹ�õ�setup.inf
		int nResult = ReadInSettingFile();
		if ( 0 == nResult )	{
			AfxMessageBox("�޷�ʶ��������ļ���");
			return;
		}
		else if (2 == nResult)
		{
			AfxMessageBox("·��Ϊ�գ�");
			return;
		}
		else{
			//��ȷ���������ļ�
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
		m_strFilePath =  fileDlg.GetPathName();//ȡ���ļ�·�����ļ���
        SetDlgItemText(IDC_EDIT_ExistDataPath , m_strFilePath);
	}

}



int CCheckCardDlg::ReadInSettingFile()
{
	if (m_strFilePath.IsEmpty())
	{
		return 2;
	}
	//��� RECORDER SETTING FILE
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
		//���Ǳ������setup.inf
		return 0;
	}

	//�޸�m_strCurWorkPath m_strWorkPlace
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CRecorderDoc *pDoc = (CRecorderDoc*)(pFrame->GetActiveDocument());
	CProduct* pProduct = &(pDoc->m_Product);
	int nPos;   
	nPos=m_strFilePath.ReverseFind('\\');   
	pProduct->m_strCurWorkPath=m_strFilePath.Left(nPos);
//	TRACE(pProduct->m_strCurWorkPath);


////////////д���ʽ//////////////////////////////////////////////////////////////
//	0.�������
//	1.ͨ����
//	2.����
//	3.��������
//	4.��������
//	5.������ʽ
//	6.����ͨ��
//	7.������ƽ
//	8.������ʱ
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

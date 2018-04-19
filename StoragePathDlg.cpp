// StoragePathDlg.cpp : implementation file
//

#include "stdafx.h"
#include "recorder.h"
#include "StoragePathDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
#include "MainFrm.h"
#include "Product.h"
#include "RecorderDoc.h"
//////////////////////////////////////////////////////////////////////////
// CStoragePathDlg dialog


CStoragePathDlg::CStoragePathDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CStoragePathDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStoragePathDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CStoragePathDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStoragePathDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStoragePathDlg, CDialog)
	//{{AFX_MSG_MAP(CStoragePathDlg)
	ON_BN_CLICKED(IDC_RADIO_OverrideDoc, OnRADIOOverrideDoc)
	ON_BN_CLICKED(IDC_RADIO_NewDoc, OnRADIONewDoc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStoragePathDlg message handlers

BOOL CStoragePathDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CRecorderDoc *pDoc = (CRecorderDoc*)(pFrame->GetActiveDocument());
	CProduct* pProduct = &(pDoc->m_Product);

/*
	GetDlgItem(IDC_EDIT_OverridePath)->SetWindowText(pProduct->m_strCurWorkPath);
	GetDlgItem(IDC_EDIT_OverridePath)->EnableWindow(FALSE);
*/
//	GetDlgItem(IDC_EDIT_NewPath)->SetWindowText(pProduct->m_strWorkPlace + "\\");
	GetDlgItem(IDC_STATIC_WorkPlace)->SetWindowText(pProduct->m_strWorkPlace + "\\");
	
	((CButton *)GetDlgItem(IDC_RADIO_OverrideDoc))->SetCheck(TRUE);
	GetDlgItem(IDC_EDIT_NewPath)->EnableWindow(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CStoragePathDlg::OnOK() 
{
	// TODO: Add extra validation here
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CRecorderDoc *pDoc = (CRecorderDoc*)(pFrame->GetActiveDocument());
	CProduct* pProduct = &(pDoc->m_Product);

	if (1 == ((CButton *)GetDlgItem(IDC_RADIO_OverrideDoc))->GetCheck())
	{
		//ѡ���˸���

		/*ɾ��ԭ�������д洢�ļ���setup.inf*/
		pProduct->m_strCurWorkPath = pProduct->m_strWorkPlace + "\\" + "Temp";
		CString strDirPath;				//Ҫɾ�����ļ���������ʽ
		CString strDocName;			//�ҵ���Ҫɾ�����ļ����ļ���������������·����
		CString strDocPath;		//�ҵ���Ҫɾ�����ļ���ȷ��·��
		strDirPath = pProduct->m_strCurWorkPath + "\\*";
		HANDLE file;
		WIN32_FIND_DATA fileData;
		file = FindFirstFile(strDirPath.GetBuffer(0), &fileData);
		if ( INVALID_HANDLE_VALUE == file)
		{
			return;
		}

		strDocName.Format("%s",fileData.cFileName);
		strDocPath = pProduct->m_strCurWorkPath + "\\" + strDocName;
		DeleteFile(strDocPath);		//ɾ����һ���ļ�

		bool bState = false;
		bState = FindNextFile(file, &fileData);
		while(bState){
			
			strDocName.Format("%s",fileData.cFileName);
			strDocPath = pProduct->m_strCurWorkPath + "\\" + strDocName;
			/*ѭ��ɾ��ʣ�µ��ļ�*/
			DeleteFile(strDocPath);
			bState = FindNextFile(file, &fileData);
		}

	} 
	else
	{
		//ѡ�����½�
		CString strTemp;
		GetDlgItem(IDC_EDIT_NewPath)->GetWindowText(strTemp);
		pProduct->m_strCurWorkPath = pProduct->m_strWorkPlace + "\\" + strTemp;


		if(!CreateDirectory(pProduct->m_strCurWorkPath , NULL))
		{
			AfxMessageBox("����Ϊ�ջ��ļ����Ѵ��ڣ�");
			return;
		}
	}


	CDialog::OnOK();
}

void CStoragePathDlg::OnRADIOOverrideDoc() 
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_EDIT_NewPath)->EnableWindow(FALSE);
}

void CStoragePathDlg::OnRADIONewDoc() 
{
	// TODO: Add your control notification handler code here
	
	GetDlgItem(IDC_EDIT_NewPath)->EnableWindow(TRUE);
}

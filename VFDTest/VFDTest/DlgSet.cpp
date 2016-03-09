// DlgSet.cpp : implementation file
//

#include "stdafx.h"
#include "VFDTest.h"
#include "DlgSet.h"
#include "afxdialogex.h"


// CDlgSet dialog

IMPLEMENT_DYNAMIC(CDlgSet, CDialogEx)

CDlgSet::CDlgSet(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgSet::IDD, pParent)
	, m_iTotalTime(120)
	, m_iComTime(65)
	, m_iWriteTime(5000)
	, m_strMeterCom(_T(""))
	, m_strCtrlCom(_T(""))
{

}

CDlgSet::~CDlgSet()
{
}

void CDlgSet::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_TOTALTIME, m_iTotalTime);
	DDX_Text(pDX, IDC_EDIT_COMTIME, m_iComTime);
	DDX_Text(pDX, IDC_EDIT_WRITETIME, m_iWriteTime);
	DDX_Control(pDX, IDC_CHECK_AUTOSTART, m_autoStart);
	DDX_Control(pDX, IDC_COMBO1, m_cbMeterCom);
	DDX_Control(pDX, IDC_COMBO2, m_cbCtrlCom);
	DDX_Control(pDX, IDC_COMBO_STARTMETHOD, m_cbStartMethod);
	DDX_CBString(pDX, IDC_COMBO1, m_strMeterCom);
	DDX_CBString(pDX, IDC_COMBO2, m_strCtrlCom);
}


BEGIN_MESSAGE_MAP(CDlgSet, CDialogEx)
	ON_BN_CLICKED(IDC_CHECK_AUTOSTART, &CDlgSet::OnClickedCheckAutostart)
END_MESSAGE_MAP()


// CDlgSet message handlers


void CDlgSet::OnClickedCheckAutostart()
{
	HKEY hKey;
	CString strRegPath = _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run");
	//找到系统的启动项  
	if (m_autoStart.GetCheck())
	{
		if (RegOpenKeyEx(HKEY_CURRENT_USER, strRegPath, 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS) //打开启动项       
		{
			TCHAR szModule[MAX_PATH];
			GetModuleFileName(NULL, szModule, MAX_PATH);//得到本程序自身的全路径  
			RegSetValueEx(hKey, _T("天银老化测试系统"), 0, REG_SZ, (LPBYTE)szModule, (lstrlen(szModule) + 1)*sizeof(TCHAR));
			//添加一个子Key,并设置值，"VFDTest"是应用程序名字（不加后缀.exe）  
			RegCloseKey(hKey); //关闭注册表  
		}
		else
		{
			AfxMessageBox(_T("系统参数错误,不能随系统启动"));
		}
	}
	else
	{
		if (RegOpenKeyEx( HKEY_CURRENT_USER, strRegPath, 0, KEY_ALL_ACCESS, &hKey ) == ERROR_SUCCESS)
		{
			RegDeleteValue(hKey, _T("天银老化测试系统"));
			RegCloseKey(hKey);
		}
	}

}


BOOL CDlgSet::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	GetCom();

	m_cbStartMethod.InsertString( 0, _T("扫描启动") );
	m_cbStartMethod.InsertString( 1, _T("按钮启动") );
	m_cbStartMethod.SetCurSel( m_iStartMethod - 1 );

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgSet::GetCom()
{
	//程序启动时获取全部可用串口
	HANDLE hCom;
	int i,num,k;
	CString str;
	BOOL flag;
		
	m_cbMeterCom.ResetContent();
	m_cbCtrlCom.ResetContent();

	flag = FALSE;
	num = 0;
	for (i = 1;i <= 10;i++)//此程序支持20个串口
	{
		str.Format( _T("\\\\.\\COM%d"), i );
		hCom = CreateFile( str, 0, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0 );
		if( INVALID_HANDLE_VALUE != hCom )//能打开该串口，则添加该串口
		{
			CloseHandle( hCom );
			str = str.Mid( 4 );
			m_cbMeterCom.AddString( str );
			m_cbCtrlCom.AddString( str );
			if (flag == FALSE)
			{
				flag = TRUE;
				num = i;
			}
		}
	}
	i = m_cbMeterCom.GetCount();

	if ( i == 0 )
	{//若找不到可用串口则禁用“打开串口”功能
		m_cbMeterCom.EnableWindow( FALSE );
		m_cbCtrlCom.EnableWindow( FALSE );
	}
	else
	{
		k = m_cbCtrlCom.GetCount();
		m_cbMeterCom.SetCurSel( k - 1 );
		while ( m_iMeterCom != m_strMeterCom.Mid(4) && k > 0 )
		{
			m_cbMeterCom.SetCurSel( k-- );
		}

		k = m_cbCtrlCom.GetCurSel();
		m_cbCtrlCom.SetCurSel( k - 1 );
		while( m_iCtrlCom != m_strCtrlCom.Mid(4) && k > 0 )
		{
			m_cbCtrlCom.SetCurSel( k-- );
		}
	}
}		
// DlgOptions.cpp : 实现文件
//

#include "stdafx.h"
#include "VFDTest.h"
#include "DlgOptions.h"
#include "afxdialogex.h"
#include "MainForm.h"

// DlgOptions 对话框

IMPLEMENT_DYNAMIC(DlgOptions, CDialogEx)

DlgOptions::DlgOptions(CWnd* pParent /*=NULL*/)
	: CDialogEx(DlgOptions::IDD, pParent)
	, m_rownumber(0)
	, m_columnsnumber(0)
	, m_strCom1(_T(""))
	, m_strCom2(_T(""))
	, m_strCom3(_T(""))
	, m_strCom4(_T(""))
	, m_strCtrlCom(_T(""))
{
	m_iCom0 = 0;
	m_iCom1 = 0;
	m_iCom2 = 0;
	m_iCom3 = 0;
	m_iPart0 = 0;
	m_iPart1 = 0;
	m_iPart2 = 0;
	m_iPart3 = 0;
	m_iComTime = 65;
	m_iTotalTime = 120;
	m_iWriteTime = 5000;
	m_iPartNumbers = 32;
	m_iCtrlCom = 0;
	m_iAutoStart = 0;
}

DlgOptions::~DlgOptions()
{
}

void DlgOptions::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT6, m_rownumber);
	DDV_MinMaxInt(pDX, m_rownumber, 1, 50);
	DDX_Text(pDX, IDC_EDIT7, m_columnsnumber);
	DDV_MinMaxInt(pDX, m_columnsnumber, 1, 50);
	DDX_Text(pDX, IDC_EDIT_PART1, m_iPart0);
	DDX_Text(pDX, IDC_EDIT_PART2, m_iPart1);
	DDX_Text(pDX, IDC_EDIT_PART3, m_iPart2);
	DDX_Text(pDX, IDC_EDIT_PART4, m_iPart3);
	DDX_Text(pDX, IDC_EDIT_COMTIME, m_iComTime);
	DDX_Text(pDX, IDC_EDIT_TOTALTIME, m_iTotalTime);
	DDX_Text(pDX, IDC_EDIT_WRITETIME, m_iWriteTime);
	DDX_Text(pDX, IDC_EDIT_PARTNUMBERS, m_iPartNumbers);
	DDX_Control(pDX, IDC_COMBO_STARTMETHOD, m_cbStartMethod);
	DDX_Control(pDX, IDC_CHECK_AUTOSTART, m_AutoStart);
	DDX_Control(pDX, IDC_COMBO_COM1, m_cbCom1);
	DDX_Control(pDX, IDC_COMBO_COM2, m_cbCom2);
	DDX_Control(pDX, IDC_COMBO_COM3, m_cbCom3);
	DDX_Control(pDX, IDC_COMBO_COM4, m_cbCom4);
	DDX_Control(pDX, IDC_COMBO_CTRLCOM, m_cbCtrlCom);
	DDX_CBString(pDX, IDC_COMBO_COM1, m_strCom1);
	DDX_CBString(pDX, IDC_COMBO_COM2, m_strCom2);
	DDX_CBString(pDX, IDC_COMBO_COM3, m_strCom3);
	DDX_CBString(pDX, IDC_COMBO_COM4, m_strCom4);
	DDX_CBString(pDX, IDC_COMBO_CTRLCOM, m_strCtrlCom);
}


BEGIN_MESSAGE_MAP(DlgOptions, CDialogEx)
	ON_BN_CLICKED(IDC_RADIO1, &DlgOptions::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &DlgOptions::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO3, &DlgOptions::OnBnClickedRadio3)
	ON_BN_CLICKED(IDC_RADIO4, &DlgOptions::OnBnClickedRadio4)
	ON_CBN_SELCHANGE(IDC_COMBO_STARTMETHOD, &DlgOptions::OnSelchangeComboStartmethod)
	ON_BN_CLICKED(IDC_CHECK_AUTOSTART, &DlgOptions::OnClickedCheckAutostart)
	ON_BN_CLICKED(IDC_CHECK_AUTOSTART, &DlgOptions::OnClickedCheckAutostart)
	ON_CBN_SELCHANGE(IDC_COMBO_COM1, &DlgOptions::OnSelchangeComboCom1)
	ON_CBN_SELCHANGE(IDC_COMBO_COM2, &DlgOptions::OnSelchangeComboCom2)
	ON_CBN_SELCHANGE(IDC_COMBO_COM3, &DlgOptions::OnSelchangeComboCom3)
	ON_CBN_SELCHANGE(IDC_COMBO_COM4, &DlgOptions::OnSelchangeComboCom4)
	ON_CBN_SELCHANGE(IDC_COMBO_CTRLCOM, &DlgOptions::OnSelchangeComboCtrlcom)
END_MESSAGE_MAP()


// DlgOptions 消息处理程序

BOOL DlgOptions::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	AddCom();

	if (m_iParts == 4)
	{
		CheckDlgButton(IDC_RADIO4, 1);
		OnBnClickedRadio4();
		
		InitComBoBox( m_cbCom1, m_iCom0 );
		InitComBoBox( m_cbCom2, m_iCom1 );
		InitComBoBox( m_cbCom3, m_iCom2 );
		InitComBoBox( m_cbCom4, m_iCom3 );
	}
	else if (m_iParts == 3)
	{
		CheckDlgButton(IDC_RADIO3, 1);
		OnBnClickedRadio3();

		InitComBoBox( m_cbCom1, m_iCom0 );
		InitComBoBox( m_cbCom2, m_iCom1 );
		InitComBoBox( m_cbCom3, m_iCom2 );
	}
	else if (m_iParts == 2)
	{
		CheckDlgButton(IDC_RADIO2, 1);
		OnBnClickedRadio2();

		InitComBoBox( m_cbCom1, m_iCom0 );
		InitComBoBox( m_cbCom2, m_iCom1 );
	}
	else
	{
		CheckDlgButton(IDC_RADIO1, 1);
		OnBnClickedRadio1();

		InitComBoBox( m_cbCom1, m_iCom0 );
	}

	m_iCom[0] = m_iCtrlCom;
	m_iCom[1] = m_iCom0;
	m_iCom[2] = m_iCom1;
	m_iCom[3] = m_iCom2;
	m_iCom[4] = m_iCom3;

	InitComBoBox( m_cbCtrlCom, m_iCtrlCom );

	m_cbStartMethod.InsertString( 0, _T("扫描启动") );
	m_cbStartMethod.InsertString( 1, _T("按钮启动") );
	m_cbStartMethod.SetCurSel( m_iStartMethod - 1 );

	m_iAutoStart = theApp.GetInt( _T("AutoStart"), m_iAutoStart );		// 开机自启动

	if ( m_iAutoStart )
	{
		m_AutoStart.SetCheck( TRUE );
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
/*
串口选择框初始化
*/
void DlgOptions::InitComBoBox( CComboBox &cb, int i )
{
	CString strCom;
	strCom.Format( _T("COM%d"), i );
	int nIndex = cb.FindString( 0, strCom );
	cb.SetCurSel( nIndex );
}

void DlgOptions::OnBnClickedRadio1()
{
	// TODO: Add your control notification handler code here
	InitEditStatus();
	GetDlgItem(IDC_COMBO_COM1)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_PART1)->EnableWindow(TRUE);
	m_iParts = 1;
	m_rownumber = 8;
	m_columnsnumber = (( m_iPartNumbers * m_iParts ) - 1 ) / m_rownumber + 1;
	UpdateData(FALSE);
}

void DlgOptions::OnBnClickedRadio2()
{
	// TODO: Add your control notification handler code here
	OnBnClickedRadio1();
	GetDlgItem(IDC_COMBO_COM2)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_PART2)->EnableWindow(TRUE);
	m_iParts = 2;
	m_rownumber = 8;
	m_columnsnumber = (( m_iPartNumbers * m_iParts ) - 1 ) / m_rownumber + 1;
	UpdateData(FALSE);
}

void DlgOptions::OnBnClickedRadio3()
{
	// TODO: Add your control notification handler code here
	OnBnClickedRadio2();
	GetDlgItem(IDC_COMBO_COM3)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_PART3)->EnableWindow(TRUE);
	m_iParts = 3;
	m_rownumber = 12;
	m_columnsnumber = (( m_iPartNumbers * m_iParts ) - 1 ) / m_rownumber + 1;
	UpdateData(FALSE);
}

void DlgOptions::OnBnClickedRadio4()
{
	// TODO: Add your control notification handler code here
	OnBnClickedRadio3();
	GetDlgItem(IDC_COMBO_COM4)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_PART4)->EnableWindow(TRUE);
	m_iParts = 4;
	m_rownumber = 16;
	m_columnsnumber = (( m_iPartNumbers * m_iParts ) - 1 ) / m_rownumber + 1;
	UpdateData(FALSE);
}

void DlgOptions::InitEditStatus() 
{   
	// 全部禁用
	GetDlgItem(IDC_COMBO_COM1)->EnableWindow(FALSE);
	GetDlgItem(IDC_COMBO_COM2)->EnableWindow(FALSE);
	GetDlgItem(IDC_COMBO_COM3)->EnableWindow(FALSE);
	GetDlgItem(IDC_COMBO_COM4)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_PART1)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_PART2)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_PART3)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_PART4)->EnableWindow(FALSE);
}

void DlgOptions::OnSelchangeComboStartmethod()
{
	// TODO: Add your control notification handler code here
	m_iStartMethod = m_cbStartMethod.GetCurSel() + 1;
}
/*
初始化串口列表框
*/
void DlgOptions::AddCom(void)
{
	EnumerateSerialPorts(ports,portse,portsu);
	unsigned short uicounter;
	unsigned short uisetcom;
	CString str;

	//获取可用串口个数
	uicounter = portse.GetSize(); 

	//获取串口总数
	uicounter = ports.GetSize();

	//如果串口总数大于0
	if(uicounter > 0)
	{
		//初始化串口列表框
		for(int i=0; i<uicounter; i++)
		{
			uisetcom = ports.ElementAt(i);
			str.Format(_T("COM%d "),uisetcom);
			m_cbCom1.AddString(str);
			m_cbCom2.AddString(str);
			m_cbCom3.AddString(str);
			m_cbCom4.AddString(str);
			m_cbCtrlCom.AddString(str);
		}
	}
}
/*
枚举串口号
*/
void DlgOptions::EnumerateSerialPorts(CUIntArray& ports, CUIntArray& portse, CUIntArray& portsu)
{
	//清除串口数组内容
	ports.RemoveAll();
	portse.RemoveAll();
	portsu.RemoveAll();
	//因为至多有10个串口，所以依次检查各串口是否存在
	//如果能打开某一串口，或打开串口不成功，但返回的是 ERROR_ACCESS_DENIED错误信息，
	//都认为串口存在，只不过后者表明串口已经被占用
	//否则串口不存在
	for (int i=1; i<11; i++)
	{
		//Form the Raw device name
		CString sPort;
		sPort.Format(_T("\\\\.\\COM%d"), i);

		//Try to open the port
		BOOL bSuccess = FALSE;
		HANDLE hPort = ::CreateFile(sPort, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, 0, 0);
		if (hPort == INVALID_HANDLE_VALUE)
		{
			DWORD dwError = GetLastError();

			if (dwError == ERROR_ACCESS_DENIED)
			{
				bSuccess = TRUE;
				portsu.Add(i);       //已占用的串口
			}
		}
		else
		{
			//The port was opened successfully
			bSuccess = TRUE;
			portse.Add(i);      ////可用的串口
			//Don't forget to close the port, since we are going to do nothing with it anyway
			CloseHandle(hPort);
		}

		//Add the port number to the array which will be returned
		if (bSuccess)
			ports.Add(i);   //所有存在的串口
	}
}

void DlgOptions::OnClickedCheckAutostart()
{
	// TODO: Add your control notification handler code here
	HKEY hKey;
	CString strRegPath = _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run");
	//找到系统的启动项  
	if (m_AutoStart.GetCheck())
	{
		if (RegOpenKeyEx(HKEY_CURRENT_USER, strRegPath, 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS) //打开启动项       
		{
			TCHAR szModule[MAX_PATH];
			GetModuleFileName(NULL, szModule, MAX_PATH);//得到本程序自身的全路径  
			RegSetValueEx(hKey, _T("天银老化测试系统"), 0, REG_SZ, (LPBYTE)szModule, (lstrlen(szModule) + 1)*sizeof(TCHAR));
			//添加一个子Key,并设置值，"VFDTest"是应用程序名字（不加后缀.exe）  
			RegCloseKey(hKey); //关闭注册表
			m_iAutoStart = 1;
			theApp.WriteInt( _T("AutoStart"), m_iAutoStart );
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
			m_iAutoStart = 0;
			theApp.WriteInt( _T("AutoStart"), m_iAutoStart );
		}
	}
}

void DlgOptions::OnSelchangeComboCom1()
{
	m_cbCom1.GetLBText( m_cbCom1.GetCurSel(), m_strCom1 );
	if( ComTest( m_strCom1 ) )
	{
		m_iCom0 = atoi( m_strCom1.Mid( 3 ) );
	}
	
}

void DlgOptions::OnSelchangeComboCom2()
{
	m_cbCom2.GetLBText( m_cbCom2.GetCurSel(), m_strCom2 );
	if ( ComTest( m_strCom2 ) )
	{
		m_iCom1 = atoi( m_strCom2.Mid( 3 ) );
	}
}


void DlgOptions::OnSelchangeComboCom3()
{
	m_cbCom3.GetLBText( m_cbCom3.GetCurSel(), m_strCom3 );
	if ( ComTest( m_strCom3) )
	{
		m_iCom2 = atoi( m_strCom3.Mid( 3 ) );
	}
	
}

void DlgOptions::OnSelchangeComboCom4()
{
	m_cbCom4.GetLBText( m_cbCom4.GetCurSel(), m_strCom4 );
	if ( ComTest( m_strCom4 ) )
	{
		m_iCom3 = atoi( m_strCom4.Mid( 3 ) );
	}
	
}

void DlgOptions::OnSelchangeComboCtrlcom()
{
	m_cbCtrlCom.GetLBText( m_cbCtrlCom.GetCurSel(), m_strCtrlCom );
	
	if ( ComTest( m_strCtrlCom ) )
	{
		m_iCtrlCom = atoi( m_strCtrlCom.Mid( 3 ) );
	}
}

BOOL DlgOptions::ComTest( CString strPort )
{
	HANDLE hPort = ::CreateFile( strPort, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, 0, 0 );// 这句话导致串口重新设置时出错,在返回前删除该句柄
	if (hPort == INVALID_HANDLE_VALUE)
	{
		DWORD dwError = GetLastError();

		if ( dwError == ERROR_ACCESS_DENIED )
		{
			int i = atoi( strPort.Mid(3) );
			//if ( i != m_iCom[0] && i != m_iCom[1] && i != m_iCom[2] && i != m_iCom[3] && i != m_iCom[4] )
			{
				AfxMessageBox( _T("串口已被其他程序占用") );	// 已占用的串口
				return FALSE;
			}

		}
	}
	CloseHandle(hPort);
	return TRUE;
}
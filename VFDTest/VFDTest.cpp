
// VFDTest.cpp : ����Ӧ�ó��������Ϊ��
//
// ��ȫ�Զ������

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "VFDTest.h"
#include "MainForm.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// VFDTestApp

BEGIN_MESSAGE_MAP(VFDTestApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &VFDTestApp::OnAppAbout)
END_MESSAGE_MAP()


// VFDTestApp ����

VFDTestApp::VFDTestApp()
{
	// ȡ��Ļ����
	//m_nScreenW = GetSystemMetrics( SM_CXSCREEN );   
	//m_nScreenH = GetSystemMetrics( SM_CYSCREEN );
	//m_nScreenW = 980;   
	//m_nScreenH = 650;
	m_nFullScreen = 1;	// Ĭ��ȫ����ʾ
	m_nMaximized = 0;
	*m_szAppPath = 0;
	*m_szTempPath = 0;

	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;
#ifdef _MANAGED
	// ���Ӧ�ó��������ù�����������ʱ֧��(/clr)�����ģ���:
	//     1) �����д˸������ã�������������������֧�ֲ�������������
	//     2) ��������Ŀ�У������밴������˳���� System.Windows.Forms ������á�
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: ������Ӧ�ó��� ID �ַ����滻ΪΨһ�� ID �ַ�����������ַ�����ʽ
	//Ϊ CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("VFDTest.1000"));


	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}

// Ψһ��һ�� VFDTestApp ����

VFDTestApp theApp;


// VFDTestApp ��ʼ��

BOOL VFDTestApp::InitInstance()
{
	// ��ֹ�ظ�������ɨ������ظ�����ʱ�쳣��
	// Add this code in InistInstance function
	TCHAR *lpszName = _T("VFDTEST");
	HANDLE hMutex = ::CreateMutex(NULL, FALSE, lpszName);
	DWORD dwRet = ::GetLastError();
	if (hMutex)
	{
		if (ERROR_ALREADY_EXISTS == dwRet)
		{
			AfxMessageBox(_T("VFDTEST has been running."));
			CloseHandle(hMutex);  // should be closed
			hMutex = 0;
			return FALSE;
		}
	}
	else
	{
		AfxMessageBox(_T("Create Mutex Error"));
	}



	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	// ��ʼ�� OLE ��
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	
	//ȡӦ�ó���·��
	GetModuleFileName(NULL, m_szAppPath, MAX_PATH);
	PathRemoveFileSpec( m_szAppPath );

	// ȡ��ʱ�ļ���
	GetTempPath(MAX_PATH-1, m_szTempPath );//��ȡ��ʱ�ļ���·��

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// ʹ�� RichEdit �ؼ���Ҫ  AfxInitRichEdit2()	
	// AfxInitRichEdit2();

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("VFDTest"));

	/* ��Ϊ����һ����ͨ��������

	// ��Ҫ���������ڣ��˴��뽫�����µĿ�ܴ���
	// ����Ȼ��������ΪӦ�ó���������ڶ���
	CMainFrame* pFrame = new CMainFrame;
	if (!pFrame)
		return FALSE;
	m_pMainWnd = pFrame;
	// ���������ؿ�ܼ�����Դ
	pFrame->LoadFrame(IDR_MAINFRAME,
		WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, NULL,
		NULL);
	*/
	
    // Register your unique class name that you wish to use
    WNDCLASS wndcls;
    memset(&wndcls, 0, sizeof(WNDCLASS));   // start with NULL
                                            // defaults
    //wndcls.style = CS_DBLCLKS; //˫��

    //you can specify your own window procedure
    wndcls.lpfnWndProc = ::DefWindowProc; 
    wndcls.hInstance = AfxGetInstanceHandle();
    wndcls.hIcon = LoadIcon(IDR_MAINFRAME); // or load a different icon
    wndcls.hCursor = LoadStandardCursor( IDC_ARROW );

	//backGrush.CreateSolidBrush(RGB(247,247,247));   // Blue brush.
	//wndcls.hbrBackground = (HBRUSH)backGrush.m_hObject;
    wndcls.lpszMenuName = NULL;

    // Specify your own class name for using FindWindow later
    wndcls.lpszClassName = _T("VFDTest");

    // Register the new class and exit if it fails
    if(!AfxRegisterClass(&wndcls))
    {
       TRACE(_T("Class Registration Failed\n"));
       return FALSE;
	}

	// ������
	MainForm* pFrame = new MainForm();

	// ȡϵͳ������ȷ����ʾλ�úʹ�С
	CRect drc; 	// ���湤������С������������ 
	SystemParametersInfo(SPI_GETWORKAREA,0,drc,0);
	int dwidth = drc.Width();		// ���湤������
	int dheight = drc.Height();		// ���湤������

	// �ϴα���Ĵ��ڴ�С
	CRect rc;
	rc.right = dwidth;		// ��һ�ν�����״̬���������棬�б߿򣬿ɸı��С��
	rc.bottom = dheight;
	//this->GetInt();
	rc.left = GetInt( _T("WindowLeft"), rc.left );
	rc.top = GetInt( _T("WindowTop"), rc.top );
	rc.right = GetInt( _T("WindowRight"), rc.right );
	rc.bottom = GetInt( _T("WindowBottom"), rc.bottom );

	// ��С�ߴ�200x200
	const int s = 300;		// ?? Ӧ�ÿ���ϵͳDPI
	if( rc.Width() < s )
		rc.right = rc.left + s;
	if( rc.Height() < s )
		rc.bottom = rc.top + s;

	// �������ཻ����200
	int dx = 0, dy = 0, d = 0;
	if( rc.right < s )
		dx = s - rc.right;
	else if( (d = dwidth - rc.left) < s )
		dx = d - s;
	if( rc.bottom < s )
		dy = s - rc.bottom;
	else if( (d = dheight - rc.top) < s )
		dy = d - s;

	// ���ڵ����Ҷ��������棬�����Ƶ�������
	if( rc.left < 0 && rc.right > dwidth )
		dx = -rc.left;
	if( rc.top < 0 && rc.bottom > dheight )
		dy = -rc.top;

	// �ƶ���ʾ��
	if( dx || dy )
		rc.OffsetRect( dx, dy );

	// ��������
	// Ψһ��һ�������ѳ�ʼ���������ʾ����������и���
	m_nFullScreen = GetInt( _T("FullScreen"), m_nFullScreen );		// ȫ��״̬
	m_nMaximized = GetInt( _T("ShowMaximized"), m_nMaximized );	// ���״̬
	DWORD style = (m_nFullScreen ? WS_POPUP : WS_OVERLAPPEDWINDOW);
	if( m_nFullScreen | m_nMaximized )
		style |= WS_MAXIMIZE;
	if( !pFrame->CreateEx( NULL, wndcls.lpszClassName, _T("VFDTest"), style, rc, NULL, NULL, NULL ) )	// �ޱ߿򣬳�����
	{
		delete pFrame;
		return FALSE;
	}

	m_pMainWnd = pFrame;


	pFrame->ShowWindow(SW_SHOW);
	//pFrame->UpdateWindow();
	// �������к�׺ʱ�ŵ��� DragAcceptFiles
	//  �� SDI Ӧ�ó����У���Ӧ�� ProcessShellCommand ֮����

	return TRUE;
}

int VFDTestApp::ExitInstance()
{
	AfxOleTerm(FALSE);
	TRACE(_T("VFDTest ExitInstance!\r\n"));
	return CWinAppEx::ExitInstance();
}

// VFDTestApp ��Ϣ�������


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// �������жԻ����Ӧ�ó�������
void VFDTestApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// VFDTestApp ��Ϣ�������




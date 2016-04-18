
// VFDTest.cpp : 定义应用程序的类行为。
//
// 完全自定义办类

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


// VFDTestApp 构造

VFDTestApp::VFDTestApp()
{
	// 取屏幕像素
	//m_nScreenW = GetSystemMetrics( SM_CXSCREEN );   
	//m_nScreenH = GetSystemMetrics( SM_CYSCREEN );
	//m_nScreenW = 980;   
	//m_nScreenH = 650;
	m_nFullScreen = 1;	// 默认全屏显示
	m_nMaximized = 0;
	*m_szAppPath = 0;
	*m_szTempPath = 0;

	// 支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;
#ifdef _MANAGED
	// 如果应用程序是利用公共语言运行时支持(/clr)构建的，则:
	//     1) 必须有此附加设置，“重新启动管理器”支持才能正常工作。
	//     2) 在您的项目中，您必须按照生成顺序向 System.Windows.Forms 添加引用。
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: 将以下应用程序 ID 字符串替换为唯一的 ID 字符串；建议的字符串格式
	//为 CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("VFDTest.1000"));


	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}

// 唯一的一个 VFDTestApp 对象

VFDTestApp theApp;


// VFDTestApp 初始化

BOOL VFDTestApp::InitInstance()
{
	// 防止重复启动，扫描程序重复启动时异常。
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



	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	// 初始化 OLE 库
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	
	//取应用程序路径
	GetModuleFileName(NULL, m_szAppPath, MAX_PATH);
	PathRemoveFileSpec( m_szAppPath );

	// 取临时文件夹
	GetTempPath(MAX_PATH-1, m_szTempPath );//获取临时文件夹路径

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// 使用 RichEdit 控件需要  AfxInitRichEdit2()	
	// AfxInitRichEdit2();

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("VFDTest"));

	/* 改为创建一个普通的主窗口

	// 若要创建主窗口，此代码将创建新的框架窗口
	// 对象，然后将其设置为应用程序的主窗口对象
	CMainFrame* pFrame = new CMainFrame;
	if (!pFrame)
		return FALSE;
	m_pMainWnd = pFrame;
	// 创建并加载框架及其资源
	pFrame->LoadFrame(IDR_MAINFRAME,
		WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, NULL,
		NULL);
	*/
	
    // Register your unique class name that you wish to use
    WNDCLASS wndcls;
    memset(&wndcls, 0, sizeof(WNDCLASS));   // start with NULL
                                            // defaults
    //wndcls.style = CS_DBLCLKS; //双击

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

	// 主窗口
	MainForm* pFrame = new MainForm();

	// 取系统参数，确定显示位置和大小
	CRect drc; 	// 桌面工作区大小，不算任务栏 
	SystemParametersInfo(SPI_GETWORKAREA,0,drc,0);
	int dwidth = drc.Width();		// 桌面工作区宽
	int dheight = drc.Height();		// 桌面工作区高

	// 上次保存的窗口大小
	CRect rc;
	rc.right = dwidth;		// 第一次进窗口状态，充满桌面，有边框，可改变大小。
	rc.bottom = dheight;
	//this->GetInt();
	rc.left = GetInt( _T("WindowLeft"), rc.left );
	rc.top = GetInt( _T("WindowTop"), rc.top );
	rc.right = GetInt( _T("WindowRight"), rc.right );
	rc.bottom = GetInt( _T("WindowBottom"), rc.bottom );

	// 最小尺寸200x200
	const int s = 300;		// ?? 应该考虑系统DPI
	if( rc.Width() < s )
		rc.right = rc.left + s;
	if( rc.Height() < s )
		rc.bottom = rc.top + s;

	// 与桌面相交最少200
	int dx = 0, dy = 0, d = 0;
	if( rc.right < s )
		dx = s - rc.right;
	else if( (d = dwidth - rc.left) < s )
		dx = d - s;
	if( rc.bottom < s )
		dy = s - rc.bottom;
	else if( (d = dheight - rc.top) < s )
		dy = d - s;

	// 窗口的左右都超出桌面，窗口移到桌面内
	if( rc.left < 0 && rc.right > dwidth )
		dx = -rc.left;
	if( rc.top < 0 && rc.bottom > dheight )
		dy = -rc.top;

	// 移动显示区
	if( dx || dy )
		rc.OffsetRect( dx, dy );

	// 创建窗口
	// 唯一的一个窗口已初始化，因此显示它并对其进行更新
	m_nFullScreen = GetInt( _T("FullScreen"), m_nFullScreen );		// 全屏状态
	m_nMaximized = GetInt( _T("ShowMaximized"), m_nMaximized );	// 最大化状态
	DWORD style = (m_nFullScreen ? WS_POPUP : WS_OVERLAPPEDWINDOW);
	if( m_nFullScreen | m_nMaximized )
		style |= WS_MAXIMIZE;
	if( !pFrame->CreateEx( NULL, wndcls.lpszClassName, _T("VFDTest"), style, rc, NULL, NULL, NULL ) )	// 无边框，充满屏
	{
		delete pFrame;
		return FALSE;
	}

	m_pMainWnd = pFrame;


	pFrame->ShowWindow(SW_SHOW);
	//pFrame->UpdateWindow();
	// 仅当具有后缀时才调用 DragAcceptFiles
	//  在 SDI 应用程序中，这应在 ProcessShellCommand 之后发生

	return TRUE;
}

int VFDTestApp::ExitInstance()
{
	AfxOleTerm(FALSE);
	TRACE(_T("VFDTest ExitInstance!\r\n"));
	return CWinAppEx::ExitInstance();
}

// VFDTestApp 消息处理程序


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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

// 用于运行对话框的应用程序命令
void VFDTestApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// VFDTestApp 消息处理程序




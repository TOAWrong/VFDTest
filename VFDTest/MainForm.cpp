// MainForm.cpp : 实现文件

// 继承HWnd后不能使用onpaint消息

#include "stdafx.h"
#include "VFDTest.h"
#include "MainForm.h"
#include "DlgOptions.h"
#include <math.h>
#include "CtrlComm.h"
#include "Common.h"
// MainForm
IMPLEMENT_DYNAMIC(MainForm, HWnd)

MainForm::MainForm()
{
	IniConfig();// 初始化配置数据
}

MainForm::~MainForm()
{
}

BEGIN_MESSAGE_MAP(MainForm, HWnd)
	ON_MESSAGE(WM_COMM_RECEIVE_STRING, OnCommunication) // 串口消息处理
	ON_COMMAND(TID_FULLSCREEN, OnFullScreen)	// 全屏切换
	ON_COMMAND(TID_EXIT, OnExit)
	ON_COMMAND(TID_OPTIONS, OnOptions)				// 每扫描一幅，发送扫描数据到主窗口，PHNM(pNMHDR)->lParam 指向扫描数据
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// MainForm 消息处理程序

BOOL MainForm::CreateEx(DWORD dwExStyle, LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, LPVOID lpParam)
{

//	m_ctrFrame.setFont( 24, 0, _T("微软雅黑") );
//	m_ctrFrame.setDock( HDOCK_FULLHORIZONTAL | HDOCK_FULLVERTICAL, 0, 0, 800, 600 );
//	m_ctrFrame.setBackColor( RGB( 0, 0, 0 ) );

	// 先加控件, 右框架控件
	m_pPanlCtr = new PanlCtr( CRect() );	// 在onsize中自适应大小
	m_pPanlCtr->setDock( HDOCK_RIGHT | HDOCK_FULLVERTICAL );
	m_pDynamicScan = m_pPanlCtr->m_pDynamicScan;
	m_ctrFrame.addCtrl( m_pPanlCtr );	// 右边控制栏

	// 左框架控件
	m_pPanlImg = new PanlImg( CRect() );	// 在onsize中自适应大小
	m_pPanlImg->setRowNumber( m_iRowNumber ) ;
	m_pPanlImg->setColumnNumber( m_iColumnNumber ) ;
	m_pPanlImg->init();	// 连续显示图像
	//m_pPanlImg->initSingle();	// 单幅显示图像
	m_ctrFrame.addCtrl( m_pPanlImg );	// 左边图像栏

	// 后建窗口
	if( !HWnd::CreateEx(dwExStyle, lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, lpParam) )
		return FALSE;

	m_pPanlCtr->m_pLog->m_wndEdit.LimitText(0);
	setAccelerators( IDR_MAINFRAME );

	LogDisp(_T("运行日志"));

	CString strNotemp,strStartMethod;
	if ( m_iStartMethod == 1 )
	{
		strStartMethod.Format( _T("当前模式：扫描启动") );
	}
	else
	{
		strStartMethod.Format( _T("当前模式：按钮启动") );
	}

	LogDisp( strStartMethod );// 显示当前工作模式

	OpenComm();

	m_pPanlCtr->m_pScan->setFocus(); // 焦点调至扫描输入框
	
	if ( m_bOpenPort[m_iCtrlCom] )// 判断继电器串口是否打开成功
	{
		if ( m_iStartMethod == 1 )
		{
			m_pCtrlThread.m_bFun = FALSE;
		}
		m_pCtrlThread.Start();// 当开始运行时即刻开启查询线程
	}
	//for ( int i = 0; i < m_iParts; i++ )
	//{
	//	if ( m_bOpenPort[m_iCom[i]] )
	//	{
	//		m_pMeterThread[i].Start();
	//		//SetTimer(0,200,NULL);
	//	}
	//}
	return TRUE;
}

/*
全屏切换
*/
void MainForm::OnFullScreen()
{
	if( theApp.m_nFullScreen )		// 从全屏到窗口模式
	{
		if( theApp.m_nMaximized )
		{
			// 如果窗口模式是最大化，先转到普通模式，产生size事件，保持m_nFullScreen状态，转到普通模式时在size事件中不改变m_nMaximized状态。
			ShowWindow( SW_RESTORE );
			theApp.m_nFullScreen = 0;	// 转到窗口模式，有边框
			ModifyStyle( WS_POPUP, WS_OVERLAPPEDWINDOW);
			ShowWindow( SW_SHOWMAXIMIZED );
		}
		else
		{
			theApp.m_nFullScreen = 0;	// 转到窗口模式，有边框
			ModifyStyle( WS_POPUP, WS_OVERLAPPEDWINDOW);
			ShowWindow( SW_RESTORE );
		}
	}
	else
	{
		theApp.m_nFullScreen = 1;	// 转到全屏模式，无边框，覆盖标题栏
		ModifyStyle( WS_OVERLAPPEDWINDOW, WS_POPUP);
		if( theApp.m_nMaximized )	// 如果原来是最大化，先还原，否则不触发size事件，不重排。
			ShowWindow( SW_RESTORE );
		ShowWindow( SW_SHOWMAXIMIZED );
	}

	theApp.WriteInt(_T("FullScreen"), theApp.m_nFullScreen);
}

/*
对话框和主窗口交换数据,并保存配置参数
*/
void MainForm::OnOptions()
{
	DlgOptions op;
	op.m_rownumber = m_pPanlImg->getRowNumber();	// 显示图像数量，行数
	op.m_columnsnumber = m_pPanlImg->getColumnNumber();	// 显示图像数量，列数
	
	op.m_iTotalTime = m_iTotalTime;
	op.m_iComTime = m_iComTime;
	op.m_iWriteTime = m_iWriteTime;

	op.m_iPartNumbers = m_iPartNumbers;

	op.m_iParts = m_iParts;

	if (m_iParts > 0)
	{
		op.m_iCom0 = m_iCom[0];
		op.m_iPart0 = m_iPart[0];
	}
	if (m_iParts > 1)
	{
		op.m_iCom1 = m_iCom[1];
		op.m_iPart1 = m_iCom[1];
	}
	if (m_iParts > 2)
	{
		op.m_iCom2 = m_iCom[2];
		op.m_iPart2 = m_iCom[2];
	}
	if (m_iParts > 3)
	{
		op.m_iCom3 = m_iCom[3];
		op.m_iPart3 = m_iCom[3];
	}
	
	op.m_iCtrlCom = m_iCtrlCom;

	op.m_iStartMethod = m_iStartMethod;
	
	m_pCtrlThread.m_bFun = FALSE;
	CloseComm();
	
	if( op.DoModal() != IDOK )
	{
		OpenComm();
		m_pCtrlThread.m_bFun = TRUE;
		return;
	}

	KillTimer((UINT_PTR)this);

	LogDisp( _T("*******************************") );
	LogDisp( _T("参数设置成功") );

	
	if (m_pCtrlThread.m_bIsThread)
	{
		m_pCtrlThread.End();// 修改参数之后先关闭之前的线程
	}
	for(int i = 0; i < m_iParts; i++)
	{
		if (m_pMeterThread[i].m_bIsThread)
		{
			m_pMeterThread->End();
		}
	}
	
	

	m_iTotalTime = op.m_iTotalTime;// 设置完成，读取参数
	m_iComTime = op.m_iComTime;
	m_iWriteTime = op.m_iWriteTime;

	m_iPartNumbers = op.m_iPartNumbers;

	m_iParts = op.m_iParts;
	if (m_iParts > 0)
	{
		m_iCom[0] = op.m_iCom0;
		m_iPart[0] = op.m_iPart0;
	}
	if (m_iParts > 1)
	{
		m_iCom[1] = op.m_iCom1;
		m_iPart[1] = op.m_iPart1;
	}
	if (m_iParts > 2)
	{
		m_iCom[2] = op.m_iCom2;
		m_iPart[2] = op.m_iPart2;
	}
	if (m_iParts > 3)
	{
		m_iCom[3] = op.m_iCom3;
		m_iPart[3] = op.m_iPart3;
	}

	m_iCtrlCom = op.m_iCtrlCom;

	m_iStartMethod = op.m_iStartMethod;

	m_iColumnNumber = op.m_columnsnumber;
	m_iRowNumber = op.m_rownumber;

	SaveIniConfig();// 保存参数到配置文件
	IniConfig();// 参数初始化

	CString strNotemp;
	strNotemp.Format(_T("起始工位号：%i,终止工位号：%i"),m_iNoBegin,m_iNoEnd-1);
	LogDisp(strNotemp);

	OpenComm();// 打开新设置的串口

	m_pPanlImg->setRowNumber(op.m_rownumber);
	m_pPanlImg->setColumnNumber(op.m_columnsnumber);
	m_pPanlImg->init();
	m_pPanlImg->layerout();

	if ( m_bOpenPort[m_iCtrlCom] )// 判断串口是否打开成功
	{
		if ( m_iStartMethod == 1 )
		{
			LogDisp( _T("当前模式：扫描启动") );
			if( m_pCtrlThread.m_bIsThread )// 选择扫描启动，即刻停止扫描继电器状态
			m_pCtrlThread.End();
		}
		if ( m_iStartMethod == 2 )// 工位启动方式判断
		{
			LogDisp( _T("当前模式：按钮启动") );
			m_pCtrlThread.m_bFun = TRUE;
			m_pCtrlThread.Start(); // 当选择按钮启动即刻开始查询继电器状态
		}
	}
	for (int i = 0; i < m_iParts; i++)
	{
		if (m_bOpenPort[m_iCom[i]])
		{
			if (!m_pMeterThread[i].m_bIsThread)
			{
				m_pMeterThread[i].Start();
			}
		}
	}
}
/*
退出
*/
void MainForm::OnExit()
{
	//m_pCtrlThread.m_bFun = FALSE;
	PostMessage(WM_CLOSE);
}


void MainForm::OnClose()
{
	if( AfxMessageBox( _T("确定退出？"), MB_YESNO ) == IDNO )
	{
		//m_pCtrlThread.m_bFun = TRUE;
		return;
	}
	KillTimer( 0 );
	for ( int i = 0; i < m_iParts; i++)
	{
		KillTimer(m_iCom[i]);
	}

	if(m_bIsWriteThread)// 退出写线程
	{
		::PostThreadMessage( m_Thread->m_nThreadID, WM_THREAD_STOP, 0, 0 );
		LogDisp( _T("退出写线程") );
	}
	
	//m_pCtrlThread.End(); // 退出继电器线程
	LogDisp( _T("退出继电器线程") );

	//::PostThreadMessage( m_pCtrlThread.m_Thread->m_nThreadID, WM_THREAD_SHUT, NULL, NULL );// 关闭所有工位
	m_pCtrlThread.m_Port.SendCtrlOrderALL( CTRL_ORDER_CLOSE );
	LogDisp( _T("关闭所有工位") );

	//m_pCtrlThread.m_Port.StopMonitoring();
	//m_pCtrlThread.m_Port.ClosePort();

	for (int i = 0;i < m_iParts; i++)
	{
		//m_pMeterThread[i].m_Port.StopMonitoring();
		m_pMeterThread[i].m_Port.ClosePort();
		m_pMeterThread[i].End(); // 退出数据采集线程
		m_bOpenPort[m_iCom[i]] = FALSE;
		m_bStart[m_iCom[i]] = FALSE;
	}
	LogDisp( _T("退出数据采集线程") );

	WINDOWPLACEMENT wp;
	GetWindowPlacement( &wp );	// 取窗口的位置、大小及最大化最小化等
	theApp.WriteInt( _T("WindowLeft"), wp.rcNormalPosition.left );
	theApp.WriteInt( _T("WindowTop"), wp.rcNormalPosition.top );
	theApp.WriteInt( _T("WindowRight"), wp.rcNormalPosition.right );
	theApp.WriteInt( _T("WindowBottom"), wp.rcNormalPosition.bottom );
	theApp.WriteInt( _T("ShowMaximized"), theApp.m_nMaximized );

	LogDisp( _T("退出软件\r\n") );

	//Sleep( 200 );
	//_CrtDumpMemoryLeaks();
	HWnd::OnClose();
}


void MainForm::OnSize(UINT nType, int cx, int cy)
{
	// 重排控件
	// 控制面板放右边
	const int leftwidth = 330;
	CRect rcleft;
	rcleft.left = cx - leftwidth;
	rcleft.right = cx;
	rcleft.top = 0;
	rcleft.bottom = cy < 600 ? 600 : cy;	// 左边窗口最小高度600，右边也是
	m_pPanlCtr->setRect( rcleft, true );	// 不重排，不重画窗口，在随后的ONSIZE中重排重画

	int x = 0;
	int r = cx - leftwidth;
	if( cx < 800 )	// 全宽最小值800
	{
		int d = cx - 800;	// d是负值
		x = d;
	}
	CRect rcright( x, 0, r, rcleft.bottom );
	m_pPanlImg->setRect( rcright, false );

	if( !theApp.m_nFullScreen )	// 非全屏时记录窗口最大化状态
	{
		if( nType == SIZE_RESTORED )
			theApp.m_nMaximized = 0;
		if( nType == SIZE_MAXIMIZED )
			theApp.m_nMaximized = 1;
	}

	HWnd::OnSize(nType, cx, cy);
}
/*
读取配置文件，以初始化参数
*/
BOOL MainForm::IniConfig()
{
	CString strPath;
	strPath.Format(_T("%s//Config.ini"),theApp.m_szAppPath);

	CIni ini(strPath);

	m_iTotalTime = ini.GetIntValue(_T("TIME"),_T("TOTALTIME"));
	m_iComTime = ini.GetIntValue(_T("TIME"),_T("COMTIME"));
	m_iWriteTime = ini.GetIntValue(_T("TIME"),_T("WRITETIME"));

	m_iParts = ini.GetIntValue(_T("DISP"),_T("PARTS"));
	m_iPartNumbers = ini.GetIntValue(_T("DISP"),_T("PARTNUMBERS"));
	m_iRowNumber = ini.GetIntValue(_T("DISP"),_T("ROW"));
	m_iColumnNumber = ini.GetIntValue(_T("DISP"),_T("COLUMN"));

	for (int i = 0;i < m_iParts;i++)
	{ 
		CString strCom,strPart;
		strCom.Format(_T("COM[%i]"),i);
		strPart.Format(_T("PART[%i]"),i); 
		m_iCom[i] = ini.GetIntValue(_T("COM"),strCom);
		m_iPart[i] = ini.GetIntValue(_T("COM"),strPart);
	}
	m_iCtrlCom = ini.GetIntValue(_T("COM"),_T("CTRLCOM"));

	m_iStartMethod = ini.GetIntValue(_T("METHOD"),_T("STARTMETHOD"));

	//参数初始化
	m_bAutoDelete = FALSE;
	m_bIsWriteThread = FALSE;
	m_iWriteTimeTemp = 0;
	for(int i = 1;i < 11;i++)
	{
		m_bStart[i] = FALSE;
		m_bOpenPort[i] = FALSE;
		m_communicationSign[i] = FALSE;// 表示通信没有开始

		m_addr[i] = 1;// 初始仪表地址
		m_end[i] = TRUE;
		m_listflag[i] = 0;// 使得列表模拟显示多路仪表的标志，为0时，列表为空，执行插入数据指令；不为0时，列表不为空，执行修改列表参数命令
	}

	t[6] = 0;
	CString strSNText; 

	m_i = 0;// 本次运行接收数据数量
	m_j = 0;// 本次运行错误数据数量
	m_l = 0;// 本次运行发送断线次数
	m_addrtemp = 1;// 继电器控制板地址变量

	m_iNoBegin = (m_iPart[0] - 1) * m_iPartNumbers + 1;// 该组首个工位号
	m_iNoEnd = (m_iParts + m_iPart[0] -1)*m_iPartNumbers +1;// 该组末位工位号

	for(int i = m_iNoBegin;i < m_iNoEnd;i++)// 初始化参数
	{
		m_volVal[i] = 0;// 电压
		m_curVal[i] = 0;// 电流
		m_freVal[i] = 0;// 频率
		m_tpqVal[i] = 0;// 功率因数
		m_tawVal[i] = 0;// 有功功率
		m_trwVal[i] = 0;// 无功功率
		m_bStartWork[i] = FALSE;
		m_bRunning[i] = FALSE;
		m_bPass[i] = FALSE;
		m_bTestEnd[i] = FALSE;
		m_iTestEnd[i] = 0;
		m_offtimes[i] = 0;// 断线次数
	}

	m_productID = 0;// 变频板编号
	m_iWrite = 0;

	m_flag = 1;
	m_cNo = 0;// 仪表编号

	for (int i = 0;i < 20;i++)
	{
		m_bTimer[i] = FALSE;
	}
	iExit = 0;

	return TRUE;
}
/*
保存配置参数
*/
BOOL MainForm::SaveIniConfig()
{
	CString strPath;
	strPath.Format(_T("%s//Config.ini"),theApp.m_szAppPath);

	CIni ini(strPath);

	ini.DelSection(_T("COM"));// 删除所有COM字段

	CString strTotalTime,strComTime,strWriteTime;// time字段
	strTotalTime.Format(_T("%i"),m_iTotalTime);
	strComTime.Format(_T("%i"),m_iComTime);
	strWriteTime.Format(_T("%i"),m_iWriteTime);

	ini.SetValue(_T("TIME"),_T("TOTALTIME"),strTotalTime);
	ini.SetValue(_T("TIME"),_T("COMTIME"),strComTime);
	ini.SetValue(_T("TIME"),_T("WRITETIME"),strWriteTime);

	CString strParts,strPartNumbers,strCtrlCom;// Disp字段
	strParts.Format(_T("%i"),m_iParts);
	strPartNumbers.Format(_T("%i"),m_iPartNumbers);
	strCtrlCom.Format(_T("%i"),m_iCtrlCom);

	ini.SetValue(_T("DISP"),_T("PARTS"),strParts);
	ini.SetValue(_T("DISP"),_T("PARTNUMBERS"),strPartNumbers);
	ini.SetValue(_T("COM"),_T("CTRLCOM"),strCtrlCom);

	CString strRow,strColumn;// Disp行列数
	strRow.Format(_T("%i"),m_iRowNumber);
	strColumn.Format(_T("%i"),m_iColumnNumber);

	ini.SetValue(_T("DISP"),_T("ROW"),strRow);
	ini.SetValue(_T("DISP"),_T("COLUMN"),strColumn);

	CString strCom;// COM字段
	for (int i = 0;i < m_iParts;i++)
	{
		CString strCom,strPart,strComNo,strPartNo;
		strCom.Format(_T("COM[%i]"),i);
		strPart.Format(_T("PART[%i]"),i);
		strComNo.Format((_T("%i")),m_iCom[i]);
		strPartNo.Format((_T("%i")),m_iPart[i]);
		ini.SetValue(_T("COM"),strCom,strComNo);
		ini.SetValue(_T("COM"),strPart,strPartNo);
	}

	CString strStartMethod;
	strStartMethod.Format( _T("%i"), m_iStartMethod );
	
	ini.SetValue( _T("METHOD"), _T("STARTMETHOD"), strStartMethod );

	return TRUE;
}

/*
定时器消息处理函数，定时器i为发送查询指令定时器
其中m_offtimes为断线次数，表示仪表没有信号返回
*/
BOOL MainForm::SetTimerEvent(int port)
{
	
	KillTimer(port);
	int iNo = ComToPart(port); // 计算当下工位号
	CString str;
	if(m_bStartWork[iNo])
	{
		if (!m_communicationSign[port])// 没有查询到数据 
		{ 
			m_offtimes[iNo]++;
			m_bRunning[iNo] = FALSE;
			m_l++;
			OnStatusDisconnect(iNo);
			str.Format(_T("工位%i离线%i次"),iNo,m_offtimes[iNo]);
			LogDisp(str);
		}
		else// 接收到返回数据
		{
			iExit = 0;
			m_offtimes[iNo]=0;
			m_communicationSign[port]=FALSE;// 用于标志SendOrder是否成功
//			str.Format(_T("工位%i在线"),iNo);
//			LogDisp(str);
		}
		if ( m_offtimes[iNo] == 3 )// 设置断线容许断线次数，以及断线之后的事件处理
		{
			::PostThreadMessage( m_pCtrlThread.m_Thread->m_nThreadID, WM_THREAD_MSG, 1, iNo );
			LogDisp( _T("工位离线，测试不合格") );
			m_pPanlImg->m_iv[iNo]->m_pText->setText( _T("_._ _") );
			m_pPanlImg->m_iv[iNo]->m_pPrompt->setText( _T("断开连接") );
			m_pPanlImg->m_iv[iNo]->m_pText->setTextColor( 0x0000ff );
			m_pPanlImg->m_iv[iNo]->m_pPrompt->setTextColor( 0x0000ff );
//			m_pPanlImg->m_iv[iNo]->setBackColor( 0x5555ff );
			m_bStartWork[iNo] = FALSE;// 工位3次未返回数据，清除开始运行标志
			m_offtimes[iNo] = 0;// 重置离线次数
			str.Format(_T("工位%i离线3次断开"),iNo);
			LogDisp(str);
		}
	}
	// 仪表地址位递增，如果该工位未打开，则顺延；
	// 当递增次数达到每组工位数上限时，则地址位不变；
	int addrTemp = m_addr[port]++;
	m_addr[port] = ( m_addr[port] - 1 ) % m_iPartNumbers + 1;
	while( !m_bStartWork[m_addr[port]] && m_addr[port] != addrTemp )
	{
		m_addr[port]++;
		m_addr[port] = ( m_addr[port] - 1 ) % m_iPartNumbers + 1;
	}

	iNo = ComToPart(port);

	// 如果没有工位处于工作状态，则关闭定时器
	BOOL bStartWork = FALSE;

	for ( int i = m_iNoBegin; i < m_iNoEnd; i++ )
	{
		bStartWork |= m_bStartWork[i];
	}

	if ( !bStartWork )
	{
		//m_pMeterThread[port].End();
		m_bTimer[port] = FALSE;// 定时器关闭
		LogDisp(_T("关闭定时器，退出循环")); 
		iExit = 0;
		str.Format(_T("共接收%d,共离线%d,共错误%d"),m_i,m_l,m_j);
		LogDisp(str);
		for (int i = m_iNoBegin; i <= m_iNoEnd; i++)
		{
			m_iTestEnd[i] = 0;// 工位启动延迟计数清零
			m_bTestEnd[i] = FALSE;
		}
		//if(m_bIsWriteThread)// 退出写线程
		//{
		//	::PostThreadMessage( m_Thread->m_nThreadID, WM_THREAD_STOP, 0, 0 );
		//}
		return FALSE;
	}
	
	::PostThreadMessage( m_pMeterThread[port].m_Thread->m_nThreadID, WM_THREAD_MSG, 0, m_addr[port] ); // 查询指令，查询端口为port,仪表地址为m_addr[port]

//	str.Format(_T("发送%i"),m_addr[port]);
//	LogDisp(str);
	SetTimer( port, m_iComTime, NULL );// 启动定时器port
	return TRUE;
}
/*
串口缓冲响应消息，串口接收到数据时调用此函数
ch信息中保存的是接收到的数据，port为串口号
所有的数据显示和保存功能都在此函数中调用
*/
LONG MainForm::OnCommunication(WPARAM ch, LPARAM port)
{
/*	t[3] = GetTickCount();*/
	if (port < 1 || port > 10)
		return -1;
	if (!m_bStart[port])// 串口未打开，退出消息
		return -1;

	// 如果选择了"十六进制显示"，则显示十六进制值
	BSTR b = (BSTR)ch;
	CString str(b);
	SysFreeString(b);
	//	str.Format("%c",ch);
	m_strReceiveData[port] += str;

	if ( port == m_iCtrlCom )// 继电器控制板消息处理，主要判断继电器状态
	{
		/*CString str = "";
		str.Format("%02x",ch);

		m_strReceiveData[port] += str;*/
		if ( m_strReceiveData[port] )
		{
			if ( m_strReceiveData[port].Left( 2 ) == _T("81") )
			{
				CString strErr;
				strErr.Format( _T("Error：0x81 查询继电器状态返回错误") );
			}
			if ( m_strReceiveData[port].Left( 2 ) == _T("82") )
			{
				CString strErr;
				strErr.Format( _T("Error：0x82 控制继电器返回错误") );
			}
		}
		if ( m_strReceiveData[port].GetLength() >= 4)
		{
			if (m_strReceiveData[port].Mid(2,2) != _T("01") && m_strReceiveData[port].Mid(2,2) != _T("05") )
			{
				CString strErr;
				strErr.Format( _T("继电器返回数据错误") );
				m_strReceiveData[port].Empty();
				return 0;
			}
		}
		if ( m_strReceiveData[port].GetLength() == 14)
		{
			if ( m_strReceiveData[port].Mid(2,2) == _T("01"))// 01H为查询继电器返回码
			{
				CString strRecMidData = m_strReceiveData[port].Mid(0, 10);
				char buf[5];
				methord::Str2Hex( strRecMidData, buf );
				USHORT CRC = methord::MBCRC16( (UCHAR *)buf, 5 );// 查表获得CRC校验码
				int dataCrc=4096*methord::HexChar( m_strReceiveData[port][12])+256*methord::HexChar(m_strReceiveData[port][13])+
					16*methord::HexChar(m_strReceiveData[port][10])+methord::HexChar(m_strReceiveData[port][11]);// 计算接收到的CRC校验码

				if (CRC!=dataCrc)
				{
					return 0;
				}

				int iAddr = methord::Hex2Dec( m_strReceiveData[port].Left( 2 ) );

				int iStatus = methord::Hex2Dec( m_strReceiveData[port].Mid(6,2) );
				CtrlStatus( iStatus, iAddr, FALSE );// 高八位
				iStatus = methord::Hex2Dec( m_strReceiveData[port].Mid(8,2) );
				CtrlStatus( iStatus, iAddr, TRUE );// 低八位
				m_strReceiveData[port].Empty();// 清空串口缓存
			}
		}
		else if ( m_strReceiveData[port].GetLength() == 16 )// 数据长度为16位，为查询继电器返回数据
		{
			if ( m_strReceiveData[port].Mid(2,2) == _T("05"))
			{
				/*CString strRec;
				strRec.Format( _T("接收：%s"),m_strReceiveData[port] );
				LogDisp( strRec );*/
				m_strReceiveData[port].Empty();
			}
		}
		return 0;
	}
	else
	{
		//LogDisp(m_strReceiveData[port]);
 		if ( m_strReceiveData[port].GetLength() == 4 )
 		{
			if (m_strReceiveData[port].Mid( 2, 2 ) != _T("03"))// 数据第2个字为03   表明返回的是查询结果
			{
				m_strReceiveData[port].Empty();
				m_j++;
			}
		}
		if ( m_strReceiveData[port].GetLength() == DATA_LENGTH_COMMUNICATION )// 返回数据一个26个字
		{
			int iNo = ComToPart(port);// 计算工位编号;
			if (DATA_LENGTH_COMMUNICATION == 26)
			{
				CString strRecMidData = m_strReceiveData[port].Mid(0, 22);
				char buf[11];
				methord::Str2Hex(strRecMidData, buf);
				USHORT CRC = methord::MBCRC16((UCHAR *)buf, 11);// 查表获得CRC校验码
				int dataCrc = 4096 * methord::HexChar(m_strReceiveData[port][24]) + 256 * methord::HexChar(m_strReceiveData[port][25]) + 16 * methord::HexChar(m_strReceiveData[port][22]) + methord::HexChar(m_strReceiveData[port][23]);// 计算接收到的CRC校验码

				if (CRC != dataCrc)
				{
					LogDisp(_T("Error：仪表数据CRC校验出错"));
					m_j++;
					m_strReceiveData[port].Empty();
					return 0;
				}
				m_i++;

				//数值转换，将读取的仪表数据中包含的电参数提取出来
				m_volVal[iNo] = methord::Hex2Float(m_strReceiveData[port].Mid(6, 8));
				m_curVal[iNo] = methord::Hex2Float(m_strReceiveData[port].Mid(14, 8));
			}
			else
			{
				CString strRecMidData = m_strReceiveData[port].Mid(0, 62);
				char buf[31];
				methord::Str2Hex(strRecMidData, buf);
				USHORT CRC = methord::MBCRC16((UCHAR *)buf, 31);// 查表获得CRC校验码
				int dataCrc = 4096 * methord::HexChar(m_strReceiveData[port][64]) + 256 * methord::HexChar(m_strReceiveData[port][65]) + 16 * methord::HexChar(m_strReceiveData[port][62]) + methord::HexChar(m_strReceiveData[port][63]);// 计算接收到的CRC校验码

				if (CRC != dataCrc)
				{
					LogDisp(_T("Error：仪表数据CRC校验出错"));
					m_j++;
					m_strReceiveData[port].Empty();
					return 0;
				}
				m_i++;

				//数值转换，将读取的仪表数据中包含的电参数提取出来
				m_volVal[iNo] = methord::Hex2Float(m_strReceiveData[port].Mid(6, 8));
				m_curVal[iNo] = methord::Hex2Float(m_strReceiveData[port].Mid(14, 8));
				m_tawVal[iNo] = methord::Hex2Float(m_strReceiveData[port].Mid(22,8))*0.001;
				m_trwVal[iNo] = methord::Hex2Float(m_strReceiveData[port].Mid(30,8))*0.001;
				m_tpqVal[iNo] = methord::Hex2Float(m_strReceiveData[port].Mid(46,8));
				m_freVal[iNo] = methord::Hex2Float(m_strReceiveData[port].Mid(54,8)); 
			}
			m_strproductID = ArrayID[iNo];// 变频板编号 = 编号数组[仪表编号]

			if(m_bStartWork[iNo])
			{
				m_communicationSign[port]=TRUE;
				if (m_curVal[iNo] >= 0.1)//电流大于0.1A表明已启动成功
				{
					if (!m_bRunning[iNo])// 记录启动时间
					{
						CTime systemp = CTime::GetCurrentTime(); 
						CTimeSpan ts = systemp - m_startTime[iNo];// 计算已运行时间
						CString strStart;
						CString str;
						str = TimeToStr( iNo );
						strStart.Format(_T("%s，启动耗时：%i秒\r\n"),str ,ts.GetTotalSeconds());
						CStdioFile SFile;
						CFileException fileException;

						CString strTextPath;
						strTextPath.Format(_T("%s//test.txt"), theApp.m_szAppPath);

						if(SFile.Open( strTextPath, CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite), &fileException )
						{
							SFile.SeekToEnd();// 先定位到文件尾部
							SFile.WriteString(strStart);// 写入文件
						}
						else
						{
							TRACE("Can't open file %s,error=%u\n",_T("test.txt"),fileException.m_cause);
						}
					}
					m_bRunning[iNo] = TRUE;// 电流大于0.1，工位已正常启动
					OnStatusRunning(iNo);// 更新工位状态
					if ( m_curVal[iNo] > 0.6 )// 电流大于0.6A，电流过大，软件指示电流过高
					{
						m_pPanlImg->m_iv[iNo]->m_pPrompt->setText( _T("电流过高") );
						m_pPanlImg->m_iv[iNo]->m_pText->setTextColor( 0x0000ff );// 暗红色
					}
					else if ( m_curVal[iNo] < 0.4 )// 电流小于0.4A，电流过小，软件指示电流过低
					{
						m_pPanlImg->m_iv[iNo]->m_pPrompt->setText( _T("电流过低") );
						m_pPanlImg->m_iv[iNo]->m_pText->setTextColor( 0xff0000 );// 深蓝色
					}
				}
				else
				{
					m_bRunning[iNo] = FALSE;// 电流不大于0.1，工位尚未启动
					CTime systemp = CTime::GetCurrentTime(); 
					CTimeSpan ts = systemp - m_startTime[iNo];// 计算已运行时间
					m_pPanlImg->m_iv[iNo]->m_pPrompt->setText(_T("starting"));
					if( ts.GetTotalSeconds() > 12 )// 如果已运行超过12秒，且电流值仍小于0.1，则工位启动失败
					{
						m_bStartWork[iNo] = FALSE;
						m_pPanlImg->m_iv[iNo]->m_pPrompt->setText(_T("不合格"));
						m_pPanlImg->m_iv[iNo]->m_pPrompt->setTextColor( 0x0000ff );
						::PostThreadMessage( m_pCtrlThread.m_Thread->m_nThreadID, WM_THREAD_MSG, 1, iNo );

						CString strEnd;
						CString str;
						str = TimeToStr(iNo);
						strEnd.Format(_T("%s，已运行时间：%i秒，启动不成功，测试不合格"), str, ts.GetTotalSeconds());

						CStdioFile SFile;
						CFileException fileException;

						CString strTextPath;
						strTextPath.Format( _T("%s//test.txt"), theApp.m_szAppPath );

						if(SFile.Open( strTextPath, CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite),&fileException)
						{
							SFile.SeekToEnd();// 先定位到文件尾部
							SFile.WriteString(strEnd);// 写入文件
						}
						else
						{
							TRACE("Can't open file %s,error=%u\n",_T("test.txt"),fileException.m_cause);
						}
					}
				}
				OnDisp(iNo,port);// 实时显示数据
			}
			else
			{
				m_communicationSign[port]=FALSE;
			}
			CString strTemp = _T("");
			m_strReceiveData[port].Empty();
		}
//		return 1;
		
	}
	return 0;
}
/*
串口数据处理，判断继电器工位状态
*/
void MainForm::CtrlStatus( int iStatus, int iAddr, BOOL bLow )
{
	CString strStatus;
	int iPartNo = ( iAddr - 1 ) / 6;// 该组继电器地址编号,6是因为每32个工位为一个串口，每个串口2台设备，每台设备3个工控板
	int iNo = m_iNoBegin + iPartNo * m_iPartNumbers;
	if ( bLow )// 表示为寄存器低八位，寄存器高八位分别为继电器1-8，低八位分别为继电器9-16
	{
		iNo += 8;
	}
	if ( iAddr % 6 == 4 )// 每组中第二台设备号
	{
		iNo += 16;
	}
	for (int i = 0; i < 8; i++)
	{
		if ( iStatus % 2  )
		{
			if ( !m_bStartWork[iNo + i] && !m_bTestEnd[iNo + i] )// 工位尚未启动，并且工位并非在定时结束处理阶段，则该工位已启动
			{
				strStatus.Format( _T("工位%d启动"), iNo + i );
				LogDisp( strStatus );
				StartNewTest( iNo + i );
				CTime sys = CTime::GetCurrentTime();
				m_startTime[iNo + i] = sys;// 记录工位启动时间
				m_bStartWork[iNo +i] = TRUE;// 继电器状态为1，对应工位开启，标志置为1
				if ( !m_bTimer[ m_iCom[iPartNo] ] )// 如果对应端口定时器未启动，则开启定时器
				{
					BeginToRun(iNo + i);// 启动对应工位，iNo + i为工位号
				}
			}
			if ( m_bTestEnd[iNo + i] )
			{
				if (m_iTestEnd[iNo + i] == 3)// 结束阶段处理超过1个回合，则可以判断结束处理完成，以防误判造成不正确显示结果
				{
					m_bTestEnd[iNo + i] = FALSE;
					m_iTestEnd[iNo + i] = 0;
				}
				m_iTestEnd[iNo + i]++;
			}
		}
		else
		{
			if ( m_bStartWork[iNo + i] )
			{
				m_bStartWork[iNo + i] = FALSE;// 继电器状态为0，对应工位断开，标志置为0
				::PostThreadMessageA( m_pCtrlThread.m_Thread->m_nThreadID, WM_THREAD_MSG, 1, iNo+i);// 不合格指示
				LogDisp( _T("工位断开，测试不合格") );
			}
		}
		iStatus = iStatus / 2;// iStatus右移一位
	}
}

/*
数据存储,写入TXT文档
*/
void MainForm::SaveData(float vol,float cur,float tpq,float fre,float taw,float trw,int addr)
{
	CString str,strID,strDate,strTime,strDateTime;
	SYSTEMTIME st;
	GetLocalTime( &st ); 
	strID = ArrayID[addr];
	strDate.Format( "%04d%02d%02d", st.wYear, st.wMonth, st.wDay );
	strTime.Format( "%02d:%02d:%02d.%03d", st.wHour, st.wMinute, st.wSecond, st.wMilliseconds ) ;
	strDateTime.Format("%02d/%02d/%02d %02d:%02d:%02d.%03d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
	if ( m_iStartMethod == 1 )// 扫描启动方式，保存数据包含产品序列号
	{
		str.Format(_T("%s	%04d	%s	%0.4f	%0.1f	%0.4f	%0.4f	%0.4f\r\n"), strDateTime, addr, strID, vol, cur, tpq, taw, trw );
	}
	else// 按钮启动方式，保存数据不包含产品序列号
	{
		str.Format(_T("%s	%04d	%0.4f	%0.1f	%0.4f	%0.4f	%0.4f\r\n"), strDateTime, addr, cur, vol, tpq, taw, trw );
	}
	CStdioFile SFile;
	CFileException fileException;
	CString strTextPath;
	strTextPath.Format(_T("%s//data.txt"), theApp.m_szAppPath);
	if(SFile.Open( strTextPath, CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite), &fileException )
	{
		SFile.SeekToEnd();// 先定位到文件尾部
		SFile.WriteString(str);// 写入文件
	}
	else
	{
		TRACE("Can't open file %s,error=%u\n",_T("data.txt"),fileException.m_cause);
	}
}
/*
数据存储,写入TXT文档
*/
void MainForm::SaveData( float vol, float cur, int addr)
{
	CString str, strID, strDate, strTime, strDateTime;
	SYSTEMTIME st;
	GetLocalTime(&st);
	strID = ArrayID[addr];
	strDate.Format("%04d%02d%02d", st.wYear, st.wMonth, st.wDay);
	strTime.Format("%02d:%02d:%02d.%03d", st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
	strDateTime.Format("%02d/%02d/%02d %02d:%02d:%02d.%03d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
	if ( m_iStartMethod == 1 )// 扫描启动方式，保存数据包含产品序列号
	{
		str.Format(_T("%s %s	%04d	%s	%0.4f	%0.1f\r\n"), strDate, strTime, addr, strID, cur, vol/*, fre*/);
	}
	else// 按钮启动方式，保存数据不包含产品序列号
	{
		str.Format(_T("%s %s	%04d	%0.4f	%0.1f\r\n"), strDate, strTime, addr, cur, vol/*, fre*/);
	}
	CStdioFile SFile;
	CFileException fileException;

	CString strTextPath;
	strTextPath.Format(_T("%s//data.txt"), theApp.m_szAppPath);

	if(SFile.Open( strTextPath, CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite), &fileException )
	{
		SFile.SeekToEnd();// 先定位到文件尾部
		SFile.WriteString(str);// 写入文件
	}
	else
	{
		TRACE("Can't open file %s,error=%u\n",_T("data.txt"),fileException.m_cause);
	}

}
/*
功能说明：开始运行按钮，执行设定定时器指令
参数说明：addr为对应工位号
*/
void MainForm::BeginToRun(int No)
{
	
	int port = PartToCOM(No);// 串口号
	m_addr[port] = (No % m_iPartNumbers) == 0 ? m_iPartNumbers : No % m_iPartNumbers;
	if(m_bOpenPort[port])
	{
		if(m_bStart[port])
		{
			m_pMeterThread[port].Start();
			::PostThreadMessage( m_pMeterThread[port].m_Thread->m_nThreadID, WM_THREAD_MSG, NULL, m_addr[port] );
			t[1] = GetTickCount();// 开始运行时间
			SetTimer( port, m_iComTime, NULL );
			m_bTimer[port] = TRUE;
			CString str;
			str.Format( _T("开启定时器%d"),port );
			LogDisp( str );
		}
	}
	if(!m_bIsWriteThread)// 当开始运行时即刻记录数据
	{
		m_Thread = AfxBeginThread(WriteThread,this,0,CREATE_SUSPENDED,NULL);//开辟保存数据线程
	}
}
/*
打开串口
*/
void MainForm::OpenComm()
{
	CString str = _T("打开串口错误");
	CString str1;
	BOOL bFlag = TRUE;
	for (int i = 0;i < m_iParts; i++)
	{
		str1.Format(_T(",COM%i"),m_iCom[i]);
		OpenComm(m_iCom[i]);// 打开对应串口
		if (!m_bOpenPort[m_iCom[i]])
		{
			str += str1;
		}
		bFlag = bFlag & m_bOpenPort[m_iCom[i]];
	}
	OpenComm( m_iCtrlCom );
	str1.Format(_T("COM%i,其中继电器串口COM%i"),m_iCtrlCom,m_iCtrlCom);
	if ( !m_bOpenPort[m_iCtrlCom] )
	{
		str += str1;
	}
	if( !bFlag )
	{
//		AfxMessageBox(str);
		LogDisp( str );
	}
}
/*
打开串口m_nCom
*/
void MainForm::OpenComm( int m_nCom )
{
	m_bStart[m_nCom] = TRUE;
	m_dwCommEvents = EV_RXFLAG | EV_RXCHAR;
	m_nBaud = 9600;
	m_cParity = 'N';
	m_nDatabits = 8;
	m_nStopbits = 1;
	if ( !m_bOpenPort[m_nCom] )//串口未打开
	{	
		if ( m_nCom == m_iCtrlCom )
		{
			if (m_pCtrlThread.m_Port.InitPort(this->m_hWnd, m_nCom, m_nBaud,m_cParity,m_nDatabits,m_nStopbits,m_dwCommEvents,512))
			{
				m_pCtrlThread.m_Port.StartMonitoring();// 启动串口监视线程
				m_bOpenPort[m_nCom] = TRUE;
				m_EditComMsg.Format(_T("打开串口COM%d成功"),m_nCom,m_nBaud,m_cParity,m_nDatabits,m_nStopbits);
				LogDisp(m_EditComMsg);
			}
			else
			{
				m_EditComMsg.Format(_T("没有发现串口COM%d或被占用"),m_nCom);
				LogDisp(m_EditComMsg);
				m_bStart[m_nCom] = FALSE;
			}
		}
		else
		{
			if ( m_pMeterThread[m_nCom].m_Port.InitPort(this->m_hWnd, m_nCom, m_nBaud,m_cParity,m_nDatabits,m_nStopbits,m_dwCommEvents,512))
			{
				m_pMeterThread[m_nCom].m_Port.StartMonitoring();// 启动串口监视线程
				m_bOpenPort[m_nCom] = TRUE;
				m_EditComMsg.Format(_T("打开串口COM%d成功"),m_nCom,m_nBaud,m_cParity,m_nDatabits,m_nStopbits);
				LogDisp(m_EditComMsg);
			}
			else
			{
				m_EditComMsg.Format(_T("没有发现串口COM%d或被占用"),m_nCom);
				LogDisp(m_EditComMsg);
				m_bStart[m_nCom] = FALSE;
			}
		}
	}
	else
	{
		m_EditComMsg.Format(_T("串口COM%d已打开"),m_nCom);
		LogDisp(m_EditComMsg);
	}
}
/*
关闭串口
*/
void MainForm::CloseComm()
{
	CString strCom,str = _T("串口关闭：");
	for (int i = 0;i < m_iParts; i++)
	{
		if (m_bOpenPort[m_iCom[i]])
		{
			m_pMeterThread[m_iCom[i]].m_Port.ClosePort();// 关闭对应串口;
			/*m_bOpenPort[m_iCom[i]] = FALSE;*/
			strCom.Format( _T("COM%d "),m_iCom[i]);
			str += strCom;
		}
	}
	if ( m_bOpenPort[m_iCtrlCom] )
	{
		m_pCtrlThread.m_Port.ClosePort();
		//m_bOpenPort[m_iCtrlCom] = FALSE;
		strCom.Format( _T("COM%d"),m_iCtrlCom );
		str += strCom;
	}
	LogDisp(str);
}

void MainForm::OnDisp(int iMeter,int m_nCom)
{
	CTime systemp = CTime::GetCurrentTime(); 
	CTimeSpan ts = systemp - m_startTime[iMeter];// 计算已运行时间
	m_sysTime.Format( _T("%02d:%02d:%02d"),ts.GetHours(),ts.GetMinutes(),ts.GetSeconds()); 

	m_strcNo.Format(_T("%i"),iMeter);
	m_strvolVal.Format(_T("%0.1f"),m_volVal[iMeter]);
	m_strcurVal.Format(_T("%0.3f"),m_curVal[iMeter]);
	if (DATA_LENGTH_COMMUNICATION == 66)
	{
		m_strfreVal.Format(_T("%0.2f"), m_freVal[iMeter]);
		m_strtrwVal.Format(_T("%0.4f"), m_trwVal[iMeter]);
		m_strtawVal.Format(_T("%0.4f"), m_tawVal[iMeter]);
		m_strtpqVal.Format(_T("%0.4f"), m_tpqVal[iMeter]);
	}
	
	CString str;
	str.Format(_T( "电流%s已运行%s" ),m_strcurVal,m_sysTime);
	str.Format(_T("电压:%s 功率:%s\n电流:%s 功因:%s"), m_strvolVal, m_strtawVal, m_strcurVal, m_strtpqVal);
	m_pPanlImg->m_iv[iMeter]->m_pText->setText( m_strcurVal );
	m_pPanlImg->m_iv[iMeter]->m_pTime->setText( m_sysTime );
	
//	LogDisp(str);

	if ( ts.GetTotalMinutes() >= m_iTotalTime )// 测试时间到，执行相应的合格操作
	{
		m_bStartWork[iMeter] = FALSE;
		::PostThreadMessage( m_pCtrlThread.m_Thread->m_nThreadID, WM_THREAD_MSG, 2, iMeter );

		m_bTestEnd[iMeter] = TRUE;

		m_pPanlImg->m_iv[iMeter]->m_pPrompt->setText( _T( "测试合格" ) );
		OnStatus(iMeter);
		str.Format( _T("工位%i测试时间到"), iMeter );
		LogDisp( str );
		//m_bStartWork[iMeter] = FALSE;
		//m_bRunning[iMeter] = FALSE;
		CString strEnd;
		CString str;
		str = TimeToStr(iMeter);
		strEnd.Format(_T("%s，运行时间：%i分，测试合格\r\n"), str, ts.GetTotalMinutes());
		CStdioFile SFile;
		CFileException fileException;
		CString strTextPath;
		strTextPath.Format(_T("%s//test.txt"), theApp.m_szAppPath );
		if(SFile.Open( strTextPath, CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite), &fileException )
		{
			SFile.SeekToEnd();// 先定位到文件尾部
			SFile.WriteString( strEnd );// 写入文件
		}
		else
		{
			TRACE("Can't open file %s,error=%u\n",_T("test.txt"),fileException.m_cause);
		}

	}
}
void MainForm::TestPassed(int iMeter, CTimeSpan ts)
{

}
/*
开始新一轮测试
*/
void MainForm::StartNewTest( int iMeter )
{
	m_pPanlImg->m_iv[iMeter]->m_pText->setTextColor( 0x000000 );
	m_pPanlImg->m_iv[iMeter]->m_pPrompt->setTextColor( 0x000000 );
	
	if ( m_iStartMethod == 1 )// 扫描启动，执行全关-蓝开
	{
		::PostThreadMessage( m_pCtrlThread.m_Thread->m_nThreadID, WM_THREAD_MSG, 3, iMeter );
	}
	else // 按钮启动，执行红关-绿关
	{
		::PostThreadMessage( m_pCtrlThread.m_Thread->m_nThreadID, WM_THREAD_MSG, 4, iMeter );
	}
}
/*
功能描述：显示对应的工位状态
参数说明：addr为工位编号
*/
void MainForm::OnStatus( int addr )
{
	m_pPanlImg->m_iv[addr]->m_pText->setTextColor( 0x00ff00 );// 中间电流设为绿色
//	Invalidate(TRUE);
}
void MainForm::OnStatusRunning( int addr )// 正常运行状态指示
{
	m_pPanlImg->m_iv[addr]->m_pText->setTextColor( 0x00ff00 );// 中间电流设为绿色
	m_pPanlImg->m_iv[addr]->m_pPrompt->setText( _T("运行") );
	m_pPanlImg->m_iv[addr]->m_pPrompt->setTextColor( 0x000000 );// 左上提示设为黑色
//	Invalidate(TRUE);
}
void MainForm::OnStatusDisconnect( int addr )
{
	m_pPanlImg->m_iv[addr]->m_pText->setTextColor( 0x0000ff );// 中间电流设为红色
	m_pPanlImg->m_iv[addr]->m_pPrompt->setText( _T("断线") );
	Invalidate(TRUE);
}
/*
保存数据线程，定时保存数据
*/
UINT MainForm::WriteThread(LPVOID pParam)
{
	MainForm *pFrm = (MainForm *)pParam;
	pFrm->m_bIsWriteThread = TRUE;
	MSG msg;// 增加一个MSG的变量msg来接收消息
	while(!pFrm->m_bAutoDelete) 
	{
		while (PeekMessage(&msg,NULL,0,0,PM_REMOVE))// 将消息队列里的消息逐个读入msg
		{
			if(msg.message == WM_THREAD_STOP)// 如果收到终止消息则退出
			{	
				//TODO：放在堆里的变量要在这里手动清理
				return 0;// 线程正常返回，会释放局部变量等内存资源
			}
			else
			{
				DispatchMessage(&msg);
			}
		}
		Sleep( pFrm->m_iWriteTime - pFrm->m_iWriteTimeTemp );// 设定时间 - 写文件耗时
		int t1 = GetTickCount();
		pFrm->WriteData();// 存储数据
		int t2 = GetTickCount();
		pFrm->m_iWrite++;// 存储数据组数
		pFrm->m_iWriteTimeTemp = t2 - t1;
	}
	return 0;
}
void MainForm::WriteData()
{
//	int iNos = m_iParts * m_iPartNumbers;
	for ( int i = m_iNoBegin; i < m_iNoEnd; i++ )
	{
		if ( m_bStartWork[i])
		{
			if ( m_bRunning[i])// 工位正常启动才记录数据
			{
				if ( DATA_LENGTH_COMMUNICATION == 66 )
				{
					SaveData( m_volVal[i], m_curVal[i], m_tpqVal[i], m_freVal[i], m_tawVal[i], m_trwVal[i], i);
				}
				else if( DATA_LENGTH_COMMUNICATION == 26 )
				{
					SaveData( m_volVal[i], m_curVal[i], i);
				}
			}
		}
	}
}

/*
扫描处理
*/
void MainForm::ScanMsg()
{
	CString str;
	CString strMsg,strNextMsg;// 扫描提示信息
	str = m_pPanlCtr->m_pScan->getText();
	if ( str != _T("") )
//	if(!(atoi(str) == m_cNo || str == m_strproductID))// 如果此组数据已录入，则跳过此次扫描
	{
		UpdateData(TRUE);
		if(m_flag == 1)
		{
			strNo1 = m_pPanlCtr->m_pScan->getText();
			strMsg.Format(_T("扫描%i次：%s"),m_flag,strNo1);
			LogDisp(strMsg);
		}
		else
		{
			strNo2 = m_pPanlCtr->m_pScan->getText();
			strMsg.Format(_T("扫描%i次：%s"),m_flag,strNo2);
			LogDisp(strMsg);
		}
		m_pPanlCtr->m_pScan->setText( _T("") );

		if(m_flag > 1)// 扫描次数大于1,则进行以下操作
		{
			if(!(strNo1 == strNo2) && (strNo1.GetLength() == 8 || strNo2.GetLength() == 8))//　两次扫描结果不相同，并且有一个为8位
			{
				if(strNo1.GetLength() == 8)// strNo1长度为8，则为变频板编号
				{
					m_cNo = atoi(strNo2);
					m_strproductID = strNo1;
				}
				else
				{
					m_cNo = atoi(strNo1);
					m_strproductID = strNo2;
				}
				ArrayID[m_cNo] = m_strproductID;
				ScanToStart(m_cNo);// 配对成功，启动对应工位
				strNo1 = _T("");// 清空缓存
				strNo2 = _T("");
				m_flag = 0;	
				str.Format( _T("扫描配对成功，工位号：%i,变频板编号：%s"), m_cNo, m_strproductID );
				LogDisp(str);
			}
		}
		m_flag++;
	}
	else
	{
		strMsg.Format(_T("%s重复扫描：%s"),strMsg,str);
		LogDisp(strMsg);
	}
	m_pPanlCtr->m_pScan->setText( _T("") );
}

/*
扫描完成，启动对应工位
*/
void MainForm::ScanToStart( int iNo )
{
	int port = PartToCOM( iNo );// 工位对应串口号
	int addr = iNo % m_iPartNumbers;// 仪表地址
	if (addr == 0)
	{
		addr = m_iPartNumbers;
	}
//	m_addr[port] = addr;
	if ( m_bStart[port] )
	{
		CTime sys = CTime::GetCurrentTime();
		//::PostThreadMessage( m_pCtrlThread.m_Thread->m_nThreadID, WM_THREAD_MSG, 3, iNo );
		//Sleep( 15 );
		StartNewTest( iNo ); // 启动对应工位，255为闭合命令代码
		m_startTime[iNo] = sys;// 记录该工位启动时的系统时间
		m_bStartWork[iNo] = TRUE;// 设置该工位已启动，标志位置为TRUE
		if ( !m_bTimer[port] )// 如果对应端口定时器未启动，则开启定时器
		{
			BeginToRun( iNo );
		}
	}	
}

/*
消息预处理  此处用来捕捉扫描枪消息
*/
BOOL MainForm::PreTranslateMessage( MSG* pMsg )
{
	// TODO: Add your specialized code here and/or call the base class
	if ( m_iStartMethod == 2 )// 启动方式为按钮启动，不处理扫描信息
	{
		return FALSE;
	}
	if( pMsg -> message == WM_KEYDOWN && ( pMsg -> wParam == VK_RETURN ) )//判断是否有按键按下，并且按键为enter
	{
		if( !( m_bOpenPort[1] || m_bOpenPort[2] || m_bOpenPort[3] || m_bOpenPort[4] || m_bOpenPort[5] || m_bOpenPort[6] ) )
		{
			AfxMessageBox( _T("请检查串口") );
			return FALSE;
		}
		else
		{
			ScanMsg();
			return TRUE;
		}
	}
	else
	{
		return FALSE;
	}
	return HWnd::PreTranslateMessage( pMsg );
}

/*
由工位号 计算出其对应的串口号
参数说明：No为工位号
*/
int MainForm::PartToCOM( int No )
{
	int Port, tempPart;
	tempPart = ( No - 1 ) / m_iPartNumbers + 1;
	int i = 0;
	while( m_iPart[i] != tempPart && i < m_iParts )
	{
		i++;
	}
	if ( i != m_iParts )
	{
		Port = m_iCom[i];
		return Port;
	}
	else
	{
		return -1;
	}
}
/*
由串口号 计算出其对应的工位号
参数说明：port为串口号
*/
int MainForm::ComToPart( int port )
{
	int No, tempPart;
	for ( int i = 0; i < m_iParts; i++ )
	{
		if ( m_iCom[i] == port )
		{
			tempPart = m_iPart[i] - 1;
			No = tempPart * m_iPartNumbers + m_addr[port];
			return No;
		}
	}
	return -1;
}

/*
Log编辑框显示，并且实时保存到日志
*/
void MainForm::LogDisp( CString str )
{
	CString strDate,strTime;
	SYSTEMTIME st;
	GetLocalTime( &st ); 
	m_sysTime.Format( "%02d:%02d:%02d.%03d ", st.wHour, st.wMinute, st.wSecond, st.wMilliseconds );
	/*m_strEditLog += m_sysTime;
	m_strEditLog += str;
	m_strEditLog += _T("\r\n");
	m_pPanlCtr->m_pLog->setText( m_strEditLog );
	m_pPanlCtr->m_pLog->m_wndEdit.LineScroll( m_pPanlCtr->m_pLog->m_wndEdit.GetLineCount() - 1 );*/

	CString strEditLog;
	strEditLog += m_sysTime;
	strEditLog += str;
	strEditLog += _T("\r\n");

	int len = m_pPanlCtr->m_pLog->m_wndEdit.GetWindowTextLengthA();
	m_pPanlCtr->m_pLog->m_wndEdit.SetSel(len,len);
	m_pPanlCtr->m_pLog->m_wndEdit.ReplaceSel(strEditLog);

	CString strTextPath;
	strTextPath.Format( _T("%s//Log//%02d%02d%02d.txt"), theApp.m_szAppPath, st.wYear, st.wMonth, st.wDay );

	CString strFolderPath;
	strFolderPath.Format( _T("%s//Log"), theApp.m_szAppPath );

	if ( !PathIsDirectory( strFolderPath ) )// 如果日志文件夹不存在，则新建日志文件夹
	{
		if ( !CreateDirectory( strFolderPath, NULL ) )
		{
			TRACE( _T(" Can't Create Folder %s "), strFolderPath );
		}
	}

	CStdioFile SFile;
	CFileException fileException;
	if(SFile.Open( strTextPath, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite ), &fileException )
	{
		SFile.SeekToEnd();// 先定位到文件尾部
		SFile.WriteString( strEditLog );// 写入文件
	}
	else
	{
		TRACE( _T("Can't open file %s,error=%u\n"), strTextPath, fileException.m_cause );
	}
}

void MainForm::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	SetTimerEvent(nIDEvent);

	HWnd::OnTimer(nIDEvent);
}

CString MainForm::TimeToStr(int iNo)
{
	CString strID;
	if (m_iStartMethod == 1)
	{
		strID.Format(_T(" 产品号:%s,工位号:%i"), ArrayID[iNo], iNo);
	}
	else
	{
		strID.Format(_T(" 工位号:%i"), iNo);
	}

	CString strTime;
	SYSTEMTIME st;
	GetLocalTime(&st);
	strTime.Format("%02d/%02d/%02d %02d:%02d:%02d.%03d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

	return strTime + strID;
}
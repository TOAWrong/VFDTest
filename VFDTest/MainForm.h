#pragma once
#include "Hui.h"
#include "ImgView.h"
#include "DynamicScan.h"
#include "PanlCtr.h"
#include "PanlImg.h"
#include "SerialPort.h"
#include "CtrlComm.h"
#include "ini.h"
#include "CtrlThread.h"
#include "MeterThread.h"
// MainForm

class MainForm : public HWnd
{
	DECLARE_DYNAMIC(MainForm)

// Construction
public:
	MainForm();
	virtual ~MainForm();

protected:
	DECLARE_MESSAGE_MAP()
// Overflow
public:
	virtual BOOL	CreateEx(DWORD dwExStyle, LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, LPVOID lpParam = NULL);

	void			EnableAutoStart();// 开机自启动

	//启动方式
	int			m_iStartMethod;// 工作方式标志位，为1时采用扫描启动方式：此时通过扫描工位号和产品序列号配对启动工位，按钮不能启动，只能紧急停止；
						// 为2时采用按钮启动方式：此时通过按钮启动工位，扫描只能存储对应序列号，不能启动

	// 串口初始化
	int			m_nBaud;       //波特率
	int			m_nCom;        //串口号
	char		m_cParity;    //校验
	int			m_nDatabits;   //数据位
	int			m_nStopbits;   //停止位
	DWORD		m_dwCommEvents;//串口事件标志
	CString		m_strReceiveData[10];//串口接收缓存

	// 串口变量
	//CCtrlComm	m_Port[11];// 串口对象，每个串口建立一个对象，并启用一条线程来监视串口状态
	int			m_addr[11];// 仪表地址--->每串口1-32，记录时共4位数字，前2位表示组号，后2位为实际地址编号

	//串口标志
	BOOL		m_bOpenPort[11];// 状态标志
	BOOL		m_bStart[11];// 实时开始标志
	BOOL		m_end[11];// 实时停止标志
	BOOL		m_communicationSign[11];// 数据接收成功标志，在接收到正确数据后值为TRUE，默认为FALSE

	// 工位标志
	BOOL		m_bStartWork[1000];// 仪表工位启动标志
	BOOL		m_bRunning[1000];// 仪表工位正常运行标志
	int			m_offtimes[1000];// 断线重连次数
	BOOL		m_bTestEnd[1000];// 工位是否测试结束，防止接收数据延迟造成误判
	BOOL		m_bPass[1000];// 工位合格标志
	int			m_iTestEnd[1000];

	// 定时器开启标志
	BOOL		m_bTimer[20];

	// 打开串口
	void		OpenComm();
	void		OpenComm(int m_nCom);
	
	// 关闭串口
	void		CloseComm();

	// 仪表数据
	float		m_volVal[1000];// 电压
	float		m_curVal[1000];// 电流
	float		m_freVal[1000];// 频率
	float		m_tpqVal[1000];// 功率因数
	float		m_tawVal[1000];// 有功功率
	float		m_trwVal[1000];// 无功功率
	UINT		m_productID;// 变频板编号

	CString		m_sysTime,// 系统时间
				m_strcNo;
	CString		m_strvolVal;
	CString		m_strcurVal;
	CString		m_strtawVal;
	CString		m_strtrwVal;
	CString		m_strtpqVal;
	CString		m_strfreVal;
	CString		m_strproductID;

	//定时信息处理
	//static void CALLBACK myTimerClock(HWND, UINT, UINT_PTR, DWORD); // 自定义定时器回调函数
	BOOL		SetTimerEvent(int);
	int			iExit;// 所有仪表均断线时跳出循环，关闭定时器

	// 串口数据处理
	void		CtrlStatus( int iStatus, int iAddr,BOOL bLow );// 判断继电器工位状态

	// 数据保存
	void		WriteData();
	void		SaveData(float vol,float cur, int no);// 将数据保存到数据库
	void		SaveData(float vol,float cur,float tpq,float fre,float taw,float trw,int no);

	// 数据状态指示
	void		OnDisp(int iMeter,int m_nCom);// 更改列表视图中仪表编号为no的行
	void		StartNewTest(int iMeter);// 开始新一轮测试
	void		OnStatus(int i);// 实时显示对应工位的状态
	void		OnStatusRunning(int addr);// 设置对应工位状态为工作中
	void		OnStatusDisconnect(int addr);// 设置对应工位状态为不合格
	void		OnStatusDone(int addr);// 设置对应工位状态为合格

	// 采集性能监测
	long		m_i;// 记录接收数据组数
	long		m_j;// 记录接收数据错误数
	long		m_l;// 记录接收过程断线次数

	CString		m_status1;
	CString		m_status2;// 临时显示仪表状态
	CRect		rcDlgs;

	int			m_addrtemp;// 继电器控制板地址变量
	int			m_listflag[11];// 标志列表视图中是否为空
	int			m_porttemp;// 临时保存串口号
	float		m_voltemp;
	float		m_curtemp;
	CTime		m_startTime[1000];

	long		t[10];

	// 扫描操作
	void		ScanMsg();// 扫描处理函数
	void		ScanToStart(int No);// 扫描配对成功，启动对应工位
	CString		strNo;
	CString		strNo1;
	CString		strNo2;
	CString		ArrayID[1000];// 数组用来存储变频板编号，序号为仪表编号
	int			m_flag;// 记录接收键盘按键次数，为1则对应存到strSN1,大于1则存到strSN2;
	int			m_cNo;// 仪表编号

	// 启动状态监测
	BOOL		StartSuccess(int addr);// 工位启动成功，执行显示和时间记录操作

	// 启停操作
	void		OnBnClickedButton1();
	void		OnBnClickedButton2();
	void		BeginToRun(int No);// 启动对应工位，记录启动时刻
	void		StopRunning();// 定时时间到，关闭对应工位

	// 配置文件，初始化参数
	BOOL		IniConfig();// 读取配置文件
	BOOL		SaveIniConfig();// 保存配置参数

	// TIME字段
	int			m_iTotalTime;// 测试总时间
	int			m_iComTime;// 查询间隔时间
	int			m_iWriteTime;// 数据保存间隔时间

	// COM字段
	int			m_iCom[20];// 串口号
	int			m_iPart[20];// 与串口对应的分组号，即m_iniPart[m_iniCom]为对应分组号
	int			m_iCtrlCom;// 继电器对应串口号
	int			m_iMeterCom;// 仪表对应串口号

	// DISP字段
	int			m_iParts;// 组数
	int			m_iPartNumbers;// 每组个数 

	// 起止工位号
	int			m_iNoBegin;
	int			m_iNoEnd;

	// 串口号与分组号对应关系
	int			PartToCOM(int No);// 返回串口号
	int			ComToPart(int port);// 返回工位号

	// 图像显示行列数
	int			m_iRowNumber;// 行数
	int			m_iColumnNumber;// 列数

	// log编辑框显示
	void		LogDisp(CString str);

	void		TestPassed(int iMeter,CTimeSpan ts);

	//串口线程
	CCtrlThread		m_pCtrlThread; // 继电器串口
	CMeterThread	m_pMeterThread[11]; // 仪表串口

// Methods
private:
	void			imgJoin( int * pData );	// 拼接数据

	// 数据保存线程
	CWinThread*		m_Thread;
	BOOL			m_bAutoDelete;
	BOOL			m_bIsWriteThread;
	static UINT		WriteThread(LPVOID pParam);
	int				m_iWrite;// 记录存储数据组数
	int				m_iWriteTimeTemp;// 写文件操作所需时间

// Properties
private:
	DynamicScan *	m_pDynamicScan;		// 动态图像说明
	PanlImg *		m_pPanlImg;			// 左边窗口
	PanlCtr *		m_pPanlCtr;			// 右边窗口
protected:
//	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Messages
public:
	afx_msg LONG OnCommunication(WPARAM ch,LPARAM port);
	afx_msg void OnFullScreen();	// 全屏切换
	afx_msg void OnExit();
	afx_msg void OnOptions();	// 显示选项对话框

	afx_msg void OnClose();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CString m_strEditLog;
	CString m_EditComMsg;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

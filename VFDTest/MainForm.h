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

	void			EnableAutoStart();// ����������

	//������ʽ
	int			m_iStartMethod;// ������ʽ��־λ��Ϊ1ʱ����ɨ��������ʽ����ʱͨ��ɨ�蹤λ�źͲ�Ʒ���к����������λ����ť����������ֻ�ܽ���ֹͣ��
						// Ϊ2ʱ���ð�ť������ʽ����ʱͨ����ť������λ��ɨ��ֻ�ܴ洢��Ӧ���кţ���������

	// ���ڳ�ʼ��
	int			m_nBaud;       //������
	int			m_nCom;        //���ں�
	char		m_cParity;    //У��
	int			m_nDatabits;   //����λ
	int			m_nStopbits;   //ֹͣλ
	DWORD		m_dwCommEvents;//�����¼���־
	CString		m_strReceiveData[10];//���ڽ��ջ���

	// ���ڱ���
	//CCtrlComm	m_Port[11];// ���ڶ���ÿ�����ڽ���һ�����󣬲�����һ���߳������Ӵ���״̬
	int			m_addr[11];// �Ǳ��ַ--->ÿ����1-32����¼ʱ��4λ���֣�ǰ2λ��ʾ��ţ���2λΪʵ�ʵ�ַ���

	//���ڱ�־
	BOOL		m_bOpenPort[11];// ״̬��־
	BOOL		m_bStart[11];// ʵʱ��ʼ��־
	BOOL		m_end[11];// ʵʱֹͣ��־
	BOOL		m_communicationSign[11];// ���ݽ��ճɹ���־���ڽ��յ���ȷ���ݺ�ֵΪTRUE��Ĭ��ΪFALSE

	// ��λ��־
	BOOL		m_bStartWork[1000];// �Ǳ�λ������־
	BOOL		m_bRunning[1000];// �Ǳ�λ�������б�־
	int			m_offtimes[1000];// ������������
	BOOL		m_bTestEnd[1000];// ��λ�Ƿ���Խ�������ֹ���������ӳ��������
	BOOL		m_bPass[1000];// ��λ�ϸ��־
	int			m_iTestEnd[1000];

	// ��ʱ��������־
	BOOL		m_bTimer[20];

	// �򿪴���
	void		OpenComm();
	void		OpenComm(int m_nCom);
	
	// �رմ���
	void		CloseComm();

	// �Ǳ�����
	float		m_volVal[1000];// ��ѹ
	float		m_curVal[1000];// ����
	float		m_freVal[1000];// Ƶ��
	float		m_tpqVal[1000];// ��������
	float		m_tawVal[1000];// �й�����
	float		m_trwVal[1000];// �޹�����
	UINT		m_productID;// ��Ƶ����

	CString		m_sysTime,// ϵͳʱ��
				m_strcNo;
	CString		m_strvolVal;
	CString		m_strcurVal;
	CString		m_strtawVal;
	CString		m_strtrwVal;
	CString		m_strtpqVal;
	CString		m_strfreVal;
	CString		m_strproductID;

	//��ʱ��Ϣ����
	//static void CALLBACK myTimerClock(HWND, UINT, UINT_PTR, DWORD); // �Զ��嶨ʱ���ص�����
	BOOL		SetTimerEvent(int);
	int			iExit;// �����Ǳ������ʱ����ѭ�����رն�ʱ��

	// �������ݴ���
	void		CtrlStatus( int iStatus, int iAddr,BOOL bLow );// �жϼ̵�����λ״̬

	// ���ݱ���
	void		WriteData();
	void		SaveData(float vol,float cur, int no);// �����ݱ��浽���ݿ�
	void		SaveData(float vol,float cur,float tpq,float fre,float taw,float trw,int no);

	// ����״ָ̬ʾ
	void		OnDisp(int iMeter,int m_nCom);// �����б���ͼ���Ǳ���Ϊno����
	void		StartNewTest(int iMeter);// ��ʼ��һ�ֲ���
	void		OnStatus(int i);// ʵʱ��ʾ��Ӧ��λ��״̬
	void		OnStatusRunning(int addr);// ���ö�Ӧ��λ״̬Ϊ������
	void		OnStatusDisconnect(int addr);// ���ö�Ӧ��λ״̬Ϊ���ϸ�
	void		OnStatusDone(int addr);// ���ö�Ӧ��λ״̬Ϊ�ϸ�

	// �ɼ����ܼ��
	long		m_i;// ��¼������������
	long		m_j;// ��¼�������ݴ�����
	long		m_l;// ��¼���չ��̶��ߴ���

	CString		m_status1;
	CString		m_status2;// ��ʱ��ʾ�Ǳ�״̬
	CRect		rcDlgs;

	int			m_addrtemp;// �̵������ư��ַ����
	int			m_listflag[11];// ��־�б���ͼ���Ƿ�Ϊ��
	int			m_porttemp;// ��ʱ���洮�ں�
	float		m_voltemp;
	float		m_curtemp;
	CTime		m_startTime[1000];

	long		t[10];

	// ɨ�����
	void		ScanMsg();// ɨ�账����
	void		ScanToStart(int No);// ɨ����Գɹ���������Ӧ��λ
	CString		strNo;
	CString		strNo1;
	CString		strNo2;
	CString		ArrayID[1000];// ���������洢��Ƶ���ţ����Ϊ�Ǳ���
	int			m_flag;// ��¼���ռ��̰���������Ϊ1���Ӧ�浽strSN1,����1��浽strSN2;
	int			m_cNo;// �Ǳ���

	// ����״̬���
	BOOL		StartSuccess(int addr);// ��λ�����ɹ���ִ����ʾ��ʱ���¼����

	// ��ͣ����
	void		OnBnClickedButton1();
	void		OnBnClickedButton2();
	void		BeginToRun(int No);// ������Ӧ��λ����¼����ʱ��
	void		StopRunning();// ��ʱʱ�䵽���رն�Ӧ��λ

	// �����ļ�����ʼ������
	BOOL		IniConfig();// ��ȡ�����ļ�
	BOOL		SaveIniConfig();// �������ò���

	// TIME�ֶ�
	int			m_iTotalTime;// ������ʱ��
	int			m_iComTime;// ��ѯ���ʱ��
	int			m_iWriteTime;// ���ݱ�����ʱ��

	// COM�ֶ�
	int			m_iCom[20];// ���ں�
	int			m_iPart[20];// �봮�ڶ�Ӧ�ķ���ţ���m_iniPart[m_iniCom]Ϊ��Ӧ�����
	int			m_iCtrlCom;// �̵�����Ӧ���ں�
	int			m_iMeterCom;// �Ǳ��Ӧ���ں�

	// DISP�ֶ�
	int			m_iParts;// ����
	int			m_iPartNumbers;// ÿ����� 

	// ��ֹ��λ��
	int			m_iNoBegin;
	int			m_iNoEnd;

	// ���ں������Ŷ�Ӧ��ϵ
	int			PartToCOM(int No);// ���ش��ں�
	int			ComToPart(int port);// ���ع�λ��

	// ͼ����ʾ������
	int			m_iRowNumber;// ����
	int			m_iColumnNumber;// ����

	// log�༭����ʾ
	void		LogDisp(CString str);

	void		TestPassed(int iMeter,CTimeSpan ts);

	//�����߳�
	CCtrlThread		m_pCtrlThread; // �̵�������
	CMeterThread	m_pMeterThread[11]; // �Ǳ���

// Methods
private:
	void			imgJoin( int * pData );	// ƴ������

	// ���ݱ����߳�
	CWinThread*		m_Thread;
	BOOL			m_bAutoDelete;
	BOOL			m_bIsWriteThread;
	static UINT		WriteThread(LPVOID pParam);
	int				m_iWrite;// ��¼�洢��������
	int				m_iWriteTimeTemp;// д�ļ���������ʱ��

// Properties
private:
	DynamicScan *	m_pDynamicScan;		// ��̬ͼ��˵��
	PanlImg *		m_pPanlImg;			// ��ߴ���
	PanlCtr *		m_pPanlCtr;			// �ұߴ���
protected:
//	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Messages
public:
	afx_msg LONG OnCommunication(WPARAM ch,LPARAM port);
	afx_msg void OnFullScreen();	// ȫ���л�
	afx_msg void OnExit();
	afx_msg void OnOptions();	// ��ʾѡ��Ի���

	afx_msg void OnClose();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CString m_strEditLog;
	CString m_EditComMsg;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

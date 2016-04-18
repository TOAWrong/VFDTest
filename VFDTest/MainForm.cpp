// MainForm.cpp : ʵ���ļ�

// �̳�HWnd����ʹ��onpaint��Ϣ

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
	IniConfig();// ��ʼ����������
}

MainForm::~MainForm()
{
}

BEGIN_MESSAGE_MAP(MainForm, HWnd)
	ON_MESSAGE(WM_COMM_RECEIVE_STRING, OnCommunication) // ������Ϣ����
	ON_COMMAND(TID_FULLSCREEN, OnFullScreen)	// ȫ���л�
	ON_COMMAND(TID_EXIT, OnExit)
	ON_COMMAND(TID_OPTIONS, OnOptions)				// ÿɨ��һ��������ɨ�����ݵ������ڣ�PHNM(pNMHDR)->lParam ָ��ɨ������
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// MainForm ��Ϣ�������

BOOL MainForm::CreateEx(DWORD dwExStyle, LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, LPVOID lpParam)
{

//	m_ctrFrame.setFont( 24, 0, _T("΢���ź�") );
//	m_ctrFrame.setDock( HDOCK_FULLHORIZONTAL | HDOCK_FULLVERTICAL, 0, 0, 800, 600 );
//	m_ctrFrame.setBackColor( RGB( 0, 0, 0 ) );

	// �ȼӿؼ�, �ҿ�ܿؼ�
	m_pPanlCtr = new PanlCtr( CRect() );	// ��onsize������Ӧ��С
	m_pPanlCtr->setDock( HDOCK_RIGHT | HDOCK_FULLVERTICAL );
	m_pDynamicScan = m_pPanlCtr->m_pDynamicScan;
	m_ctrFrame.addCtrl( m_pPanlCtr );	// �ұ߿�����

	// ���ܿؼ�
	m_pPanlImg = new PanlImg( CRect() );	// ��onsize������Ӧ��С
	m_pPanlImg->setRowNumber( m_iRowNumber ) ;
	m_pPanlImg->setColumnNumber( m_iColumnNumber ) ;
	m_pPanlImg->init();	// ������ʾͼ��
	//m_pPanlImg->initSingle();	// ������ʾͼ��
	m_ctrFrame.addCtrl( m_pPanlImg );	// ���ͼ����

	// �󽨴���
	if( !HWnd::CreateEx(dwExStyle, lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, lpParam) )
		return FALSE;

	m_pPanlCtr->m_pLog->m_wndEdit.LimitText(0);
	setAccelerators( IDR_MAINFRAME );

	LogDisp(_T("������־"));

	CString strNotemp,strStartMethod;
	if ( m_iStartMethod == 1 )
	{
		strStartMethod.Format( _T("��ǰģʽ��ɨ������") );
	}
	else
	{
		strStartMethod.Format( _T("��ǰģʽ����ť����") );
	}

	LogDisp( strStartMethod );// ��ʾ��ǰ����ģʽ

	OpenComm();

	m_pPanlCtr->m_pScan->setFocus(); // �������ɨ�������
	
	if ( m_bOpenPort[m_iCtrlCom] )// �жϼ̵��������Ƿ�򿪳ɹ�
	{
		if ( m_iStartMethod == 1 )
		{
			m_pCtrlThread.m_bFun = FALSE;
		}
		m_pCtrlThread.Start();// ����ʼ����ʱ���̿�����ѯ�߳�
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
ȫ���л�
*/
void MainForm::OnFullScreen()
{
	if( theApp.m_nFullScreen )		// ��ȫ��������ģʽ
	{
		if( theApp.m_nMaximized )
		{
			// �������ģʽ����󻯣���ת����ͨģʽ������size�¼�������m_nFullScreen״̬��ת����ͨģʽʱ��size�¼��в��ı�m_nMaximized״̬��
			ShowWindow( SW_RESTORE );
			theApp.m_nFullScreen = 0;	// ת������ģʽ���б߿�
			ModifyStyle( WS_POPUP, WS_OVERLAPPEDWINDOW);
			ShowWindow( SW_SHOWMAXIMIZED );
		}
		else
		{
			theApp.m_nFullScreen = 0;	// ת������ģʽ���б߿�
			ModifyStyle( WS_POPUP, WS_OVERLAPPEDWINDOW);
			ShowWindow( SW_RESTORE );
		}
	}
	else
	{
		theApp.m_nFullScreen = 1;	// ת��ȫ��ģʽ���ޱ߿򣬸��Ǳ�����
		ModifyStyle( WS_OVERLAPPEDWINDOW, WS_POPUP);
		if( theApp.m_nMaximized )	// ���ԭ������󻯣��Ȼ�ԭ�����򲻴���size�¼��������š�
			ShowWindow( SW_RESTORE );
		ShowWindow( SW_SHOWMAXIMIZED );
	}

	theApp.WriteInt(_T("FullScreen"), theApp.m_nFullScreen);
}

/*
�Ի���������ڽ�������,���������ò���
*/
void MainForm::OnOptions()
{
	DlgOptions op;
	op.m_rownumber = m_pPanlImg->getRowNumber();	// ��ʾͼ������������
	op.m_columnsnumber = m_pPanlImg->getColumnNumber();	// ��ʾͼ������������
	
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
	LogDisp( _T("�������óɹ�") );

	
	if (m_pCtrlThread.m_bIsThread)
	{
		m_pCtrlThread.End();// �޸Ĳ���֮���ȹر�֮ǰ���߳�
	}
	for(int i = 0; i < m_iParts; i++)
	{
		if (m_pMeterThread[i].m_bIsThread)
		{
			m_pMeterThread->End();
		}
	}
	
	

	m_iTotalTime = op.m_iTotalTime;// ������ɣ���ȡ����
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

	SaveIniConfig();// ��������������ļ�
	IniConfig();// ������ʼ��

	CString strNotemp;
	strNotemp.Format(_T("��ʼ��λ�ţ�%i,��ֹ��λ�ţ�%i"),m_iNoBegin,m_iNoEnd-1);
	LogDisp(strNotemp);

	OpenComm();// �������õĴ���

	m_pPanlImg->setRowNumber(op.m_rownumber);
	m_pPanlImg->setColumnNumber(op.m_columnsnumber);
	m_pPanlImg->init();
	m_pPanlImg->layerout();

	if ( m_bOpenPort[m_iCtrlCom] )// �жϴ����Ƿ�򿪳ɹ�
	{
		if ( m_iStartMethod == 1 )
		{
			LogDisp( _T("��ǰģʽ��ɨ������") );
			if( m_pCtrlThread.m_bIsThread )// ѡ��ɨ������������ֹͣɨ��̵���״̬
			m_pCtrlThread.End();
		}
		if ( m_iStartMethod == 2 )// ��λ������ʽ�ж�
		{
			LogDisp( _T("��ǰģʽ����ť����") );
			m_pCtrlThread.m_bFun = TRUE;
			m_pCtrlThread.Start(); // ��ѡ��ť�������̿�ʼ��ѯ�̵���״̬
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
�˳�
*/
void MainForm::OnExit()
{
	//m_pCtrlThread.m_bFun = FALSE;
	PostMessage(WM_CLOSE);
}


void MainForm::OnClose()
{
	if( AfxMessageBox( _T("ȷ���˳���"), MB_YESNO ) == IDNO )
	{
		//m_pCtrlThread.m_bFun = TRUE;
		return;
	}
	KillTimer( 0 );
	for ( int i = 0; i < m_iParts; i++)
	{
		KillTimer(m_iCom[i]);
	}

	if(m_bIsWriteThread)// �˳�д�߳�
	{
		::PostThreadMessage( m_Thread->m_nThreadID, WM_THREAD_STOP, 0, 0 );
		LogDisp( _T("�˳�д�߳�") );
	}
	
	//m_pCtrlThread.End(); // �˳��̵����߳�
	LogDisp( _T("�˳��̵����߳�") );

	//::PostThreadMessage( m_pCtrlThread.m_Thread->m_nThreadID, WM_THREAD_SHUT, NULL, NULL );// �ر����й�λ
	m_pCtrlThread.m_Port.SendCtrlOrderALL( CTRL_ORDER_CLOSE );
	LogDisp( _T("�ر����й�λ") );

	//m_pCtrlThread.m_Port.StopMonitoring();
	//m_pCtrlThread.m_Port.ClosePort();

	for (int i = 0;i < m_iParts; i++)
	{
		//m_pMeterThread[i].m_Port.StopMonitoring();
		m_pMeterThread[i].m_Port.ClosePort();
		m_pMeterThread[i].End(); // �˳����ݲɼ��߳�
		m_bOpenPort[m_iCom[i]] = FALSE;
		m_bStart[m_iCom[i]] = FALSE;
	}
	LogDisp( _T("�˳����ݲɼ��߳�") );

	WINDOWPLACEMENT wp;
	GetWindowPlacement( &wp );	// ȡ���ڵ�λ�á���С�������С����
	theApp.WriteInt( _T("WindowLeft"), wp.rcNormalPosition.left );
	theApp.WriteInt( _T("WindowTop"), wp.rcNormalPosition.top );
	theApp.WriteInt( _T("WindowRight"), wp.rcNormalPosition.right );
	theApp.WriteInt( _T("WindowBottom"), wp.rcNormalPosition.bottom );
	theApp.WriteInt( _T("ShowMaximized"), theApp.m_nMaximized );

	LogDisp( _T("�˳����\r\n") );

	//Sleep( 200 );
	//_CrtDumpMemoryLeaks();
	HWnd::OnClose();
}


void MainForm::OnSize(UINT nType, int cx, int cy)
{
	// ���ſؼ�
	// ���������ұ�
	const int leftwidth = 330;
	CRect rcleft;
	rcleft.left = cx - leftwidth;
	rcleft.right = cx;
	rcleft.top = 0;
	rcleft.bottom = cy < 600 ? 600 : cy;	// ��ߴ�����С�߶�600���ұ�Ҳ��
	m_pPanlCtr->setRect( rcleft, true );	// �����ţ����ػ����ڣ�������ONSIZE�������ػ�

	int x = 0;
	int r = cx - leftwidth;
	if( cx < 800 )	// ȫ����Сֵ800
	{
		int d = cx - 800;	// d�Ǹ�ֵ
		x = d;
	}
	CRect rcright( x, 0, r, rcleft.bottom );
	m_pPanlImg->setRect( rcright, false );

	if( !theApp.m_nFullScreen )	// ��ȫ��ʱ��¼�������״̬
	{
		if( nType == SIZE_RESTORED )
			theApp.m_nMaximized = 0;
		if( nType == SIZE_MAXIMIZED )
			theApp.m_nMaximized = 1;
	}

	HWnd::OnSize(nType, cx, cy);
}
/*
��ȡ�����ļ����Գ�ʼ������
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

	//������ʼ��
	m_bAutoDelete = FALSE;
	m_bIsWriteThread = FALSE;
	m_iWriteTimeTemp = 0;
	for(int i = 1;i < 11;i++)
	{
		m_bStart[i] = FALSE;
		m_bOpenPort[i] = FALSE;
		m_communicationSign[i] = FALSE;// ��ʾͨ��û�п�ʼ

		m_addr[i] = 1;// ��ʼ�Ǳ��ַ
		m_end[i] = TRUE;
		m_listflag[i] = 0;// ʹ���б�ģ����ʾ��·�Ǳ�ı�־��Ϊ0ʱ���б�Ϊ�գ�ִ�в�������ָ���Ϊ0ʱ���б�Ϊ�գ�ִ���޸��б��������
	}

	t[6] = 0;
	CString strSNText; 

	m_i = 0;// �������н�����������
	m_j = 0;// �������д�����������
	m_l = 0;// �������з��Ͷ��ߴ���
	m_addrtemp = 1;// �̵������ư��ַ����

	m_iNoBegin = (m_iPart[0] - 1) * m_iPartNumbers + 1;// �����׸���λ��
	m_iNoEnd = (m_iParts + m_iPart[0] -1)*m_iPartNumbers +1;// ����ĩλ��λ��

	for(int i = m_iNoBegin;i < m_iNoEnd;i++)// ��ʼ������
	{
		m_volVal[i] = 0;// ��ѹ
		m_curVal[i] = 0;// ����
		m_freVal[i] = 0;// Ƶ��
		m_tpqVal[i] = 0;// ��������
		m_tawVal[i] = 0;// �й�����
		m_trwVal[i] = 0;// �޹�����
		m_bStartWork[i] = FALSE;
		m_bRunning[i] = FALSE;
		m_bPass[i] = FALSE;
		m_bTestEnd[i] = FALSE;
		m_iTestEnd[i] = 0;
		m_offtimes[i] = 0;// ���ߴ���
	}

	m_productID = 0;// ��Ƶ����
	m_iWrite = 0;

	m_flag = 1;
	m_cNo = 0;// �Ǳ���

	for (int i = 0;i < 20;i++)
	{
		m_bTimer[i] = FALSE;
	}
	iExit = 0;

	return TRUE;
}
/*
�������ò���
*/
BOOL MainForm::SaveIniConfig()
{
	CString strPath;
	strPath.Format(_T("%s//Config.ini"),theApp.m_szAppPath);

	CIni ini(strPath);

	ini.DelSection(_T("COM"));// ɾ������COM�ֶ�

	CString strTotalTime,strComTime,strWriteTime;// time�ֶ�
	strTotalTime.Format(_T("%i"),m_iTotalTime);
	strComTime.Format(_T("%i"),m_iComTime);
	strWriteTime.Format(_T("%i"),m_iWriteTime);

	ini.SetValue(_T("TIME"),_T("TOTALTIME"),strTotalTime);
	ini.SetValue(_T("TIME"),_T("COMTIME"),strComTime);
	ini.SetValue(_T("TIME"),_T("WRITETIME"),strWriteTime);

	CString strParts,strPartNumbers,strCtrlCom;// Disp�ֶ�
	strParts.Format(_T("%i"),m_iParts);
	strPartNumbers.Format(_T("%i"),m_iPartNumbers);
	strCtrlCom.Format(_T("%i"),m_iCtrlCom);

	ini.SetValue(_T("DISP"),_T("PARTS"),strParts);
	ini.SetValue(_T("DISP"),_T("PARTNUMBERS"),strPartNumbers);
	ini.SetValue(_T("COM"),_T("CTRLCOM"),strCtrlCom);

	CString strRow,strColumn;// Disp������
	strRow.Format(_T("%i"),m_iRowNumber);
	strColumn.Format(_T("%i"),m_iColumnNumber);

	ini.SetValue(_T("DISP"),_T("ROW"),strRow);
	ini.SetValue(_T("DISP"),_T("COLUMN"),strColumn);

	CString strCom;// COM�ֶ�
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
��ʱ����Ϣ����������ʱ��iΪ���Ͳ�ѯָ�ʱ��
����m_offtimesΪ���ߴ�������ʾ�Ǳ�û���źŷ���
*/
BOOL MainForm::SetTimerEvent(int port)
{
	
	KillTimer(port);
	int iNo = ComToPart(port); // ���㵱�¹�λ��
	CString str;
	if(m_bStartWork[iNo])
	{
		if (!m_communicationSign[port])// û�в�ѯ������ 
		{ 
			m_offtimes[iNo]++;
			m_bRunning[iNo] = FALSE;
			m_l++;
			OnStatusDisconnect(iNo);
			str.Format(_T("��λ%i����%i��"),iNo,m_offtimes[iNo]);
			LogDisp(str);
		}
		else// ���յ���������
		{
			iExit = 0;
			m_offtimes[iNo]=0;
			m_communicationSign[port]=FALSE;// ���ڱ�־SendOrder�Ƿ�ɹ�
//			str.Format(_T("��λ%i����"),iNo);
//			LogDisp(str);
		}
		if ( m_offtimes[iNo] == 3 )// ���ö���������ߴ������Լ�����֮����¼�����
		{
			::PostThreadMessage( m_pCtrlThread.m_Thread->m_nThreadID, WM_THREAD_MSG, 1, iNo );
			LogDisp( _T("��λ���ߣ����Բ��ϸ�") );
			m_pPanlImg->m_iv[iNo]->m_pText->setText( _T("_._ _") );
			m_pPanlImg->m_iv[iNo]->m_pPrompt->setText( _T("�Ͽ�����") );
			m_pPanlImg->m_iv[iNo]->m_pText->setTextColor( 0x0000ff );
			m_pPanlImg->m_iv[iNo]->m_pPrompt->setTextColor( 0x0000ff );
//			m_pPanlImg->m_iv[iNo]->setBackColor( 0x5555ff );
			m_bStartWork[iNo] = FALSE;// ��λ3��δ�������ݣ������ʼ���б�־
			m_offtimes[iNo] = 0;// �������ߴ���
			str.Format(_T("��λ%i����3�ζϿ�"),iNo);
			LogDisp(str);
		}
	}
	// �Ǳ��ַλ����������ù�λδ�򿪣���˳�ӣ�
	// �����������ﵽÿ�鹤λ������ʱ�����ַλ���䣻
	int addrTemp = m_addr[port]++;
	m_addr[port] = ( m_addr[port] - 1 ) % m_iPartNumbers + 1;
	while( !m_bStartWork[m_addr[port]] && m_addr[port] != addrTemp )
	{
		m_addr[port]++;
		m_addr[port] = ( m_addr[port] - 1 ) % m_iPartNumbers + 1;
	}

	iNo = ComToPart(port);

	// ���û�й�λ���ڹ���״̬����رն�ʱ��
	BOOL bStartWork = FALSE;

	for ( int i = m_iNoBegin; i < m_iNoEnd; i++ )
	{
		bStartWork |= m_bStartWork[i];
	}

	if ( !bStartWork )
	{
		//m_pMeterThread[port].End();
		m_bTimer[port] = FALSE;// ��ʱ���ر�
		LogDisp(_T("�رն�ʱ�����˳�ѭ��")); 
		iExit = 0;
		str.Format(_T("������%d,������%d,������%d"),m_i,m_l,m_j);
		LogDisp(str);
		for (int i = m_iNoBegin; i <= m_iNoEnd; i++)
		{
			m_iTestEnd[i] = 0;// ��λ�����ӳټ�������
			m_bTestEnd[i] = FALSE;
		}
		//if(m_bIsWriteThread)// �˳�д�߳�
		//{
		//	::PostThreadMessage( m_Thread->m_nThreadID, WM_THREAD_STOP, 0, 0 );
		//}
		return FALSE;
	}
	
	::PostThreadMessage( m_pMeterThread[port].m_Thread->m_nThreadID, WM_THREAD_MSG, 0, m_addr[port] ); // ��ѯָ���ѯ�˿�Ϊport,�Ǳ��ַΪm_addr[port]

//	str.Format(_T("����%i"),m_addr[port]);
//	LogDisp(str);
	SetTimer( port, m_iComTime, NULL );// ������ʱ��port
	return TRUE;
}
/*
���ڻ�����Ӧ��Ϣ�����ڽ��յ�����ʱ���ô˺���
ch��Ϣ�б�����ǽ��յ������ݣ�portΪ���ں�
���е�������ʾ�ͱ��湦�ܶ��ڴ˺����е���
*/
LONG MainForm::OnCommunication(WPARAM ch, LPARAM port)
{
/*	t[3] = GetTickCount();*/
	if (port < 1 || port > 10)
		return -1;
	if (!m_bStart[port])// ����δ�򿪣��˳���Ϣ
		return -1;

	// ���ѡ����"ʮ��������ʾ"������ʾʮ������ֵ
	BSTR b = (BSTR)ch;
	CString str(b);
	SysFreeString(b);
	//	str.Format("%c",ch);
	m_strReceiveData[port] += str;

	if ( port == m_iCtrlCom )// �̵������ư���Ϣ������Ҫ�жϼ̵���״̬
	{
		/*CString str = "";
		str.Format("%02x",ch);

		m_strReceiveData[port] += str;*/
		if ( m_strReceiveData[port] )
		{
			if ( m_strReceiveData[port].Left( 2 ) == _T("81") )
			{
				CString strErr;
				strErr.Format( _T("Error��0x81 ��ѯ�̵���״̬���ش���") );
			}
			if ( m_strReceiveData[port].Left( 2 ) == _T("82") )
			{
				CString strErr;
				strErr.Format( _T("Error��0x82 ���Ƽ̵������ش���") );
			}
		}
		if ( m_strReceiveData[port].GetLength() >= 4)
		{
			if (m_strReceiveData[port].Mid(2,2) != _T("01") && m_strReceiveData[port].Mid(2,2) != _T("05") )
			{
				CString strErr;
				strErr.Format( _T("�̵����������ݴ���") );
				m_strReceiveData[port].Empty();
				return 0;
			}
		}
		if ( m_strReceiveData[port].GetLength() == 14)
		{
			if ( m_strReceiveData[port].Mid(2,2) == _T("01"))// 01HΪ��ѯ�̵���������
			{
				CString strRecMidData = m_strReceiveData[port].Mid(0, 10);
				char buf[5];
				methord::Str2Hex( strRecMidData, buf );
				USHORT CRC = methord::MBCRC16( (UCHAR *)buf, 5 );// �����CRCУ����
				int dataCrc=4096*methord::HexChar( m_strReceiveData[port][12])+256*methord::HexChar(m_strReceiveData[port][13])+
					16*methord::HexChar(m_strReceiveData[port][10])+methord::HexChar(m_strReceiveData[port][11]);// ������յ���CRCУ����

				if (CRC!=dataCrc)
				{
					return 0;
				}

				int iAddr = methord::Hex2Dec( m_strReceiveData[port].Left( 2 ) );

				int iStatus = methord::Hex2Dec( m_strReceiveData[port].Mid(6,2) );
				CtrlStatus( iStatus, iAddr, FALSE );// �߰�λ
				iStatus = methord::Hex2Dec( m_strReceiveData[port].Mid(8,2) );
				CtrlStatus( iStatus, iAddr, TRUE );// �Ͱ�λ
				m_strReceiveData[port].Empty();// ��մ��ڻ���
			}
		}
		else if ( m_strReceiveData[port].GetLength() == 16 )// ���ݳ���Ϊ16λ��Ϊ��ѯ�̵�����������
		{
			if ( m_strReceiveData[port].Mid(2,2) == _T("05"))
			{
				/*CString strRec;
				strRec.Format( _T("���գ�%s"),m_strReceiveData[port] );
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
			if (m_strReceiveData[port].Mid( 2, 2 ) != _T("03"))// ���ݵ�2����Ϊ03   �������ص��ǲ�ѯ���
			{
				m_strReceiveData[port].Empty();
				m_j++;
			}
		}
		if ( m_strReceiveData[port].GetLength() == DATA_LENGTH_COMMUNICATION )// ��������һ��26����
		{
			int iNo = ComToPart(port);// ���㹤λ���;
			if (DATA_LENGTH_COMMUNICATION == 26)
			{
				CString strRecMidData = m_strReceiveData[port].Mid(0, 22);
				char buf[11];
				methord::Str2Hex(strRecMidData, buf);
				USHORT CRC = methord::MBCRC16((UCHAR *)buf, 11);// �����CRCУ����
				int dataCrc = 4096 * methord::HexChar(m_strReceiveData[port][24]) + 256 * methord::HexChar(m_strReceiveData[port][25]) + 16 * methord::HexChar(m_strReceiveData[port][22]) + methord::HexChar(m_strReceiveData[port][23]);// ������յ���CRCУ����

				if (CRC != dataCrc)
				{
					LogDisp(_T("Error���Ǳ�����CRCУ�����"));
					m_j++;
					m_strReceiveData[port].Empty();
					return 0;
				}
				m_i++;

				//��ֵת��������ȡ���Ǳ������а����ĵ������ȡ����
				m_volVal[iNo] = methord::Hex2Float(m_strReceiveData[port].Mid(6, 8));
				m_curVal[iNo] = methord::Hex2Float(m_strReceiveData[port].Mid(14, 8));
			}
			else
			{
				CString strRecMidData = m_strReceiveData[port].Mid(0, 62);
				char buf[31];
				methord::Str2Hex(strRecMidData, buf);
				USHORT CRC = methord::MBCRC16((UCHAR *)buf, 31);// �����CRCУ����
				int dataCrc = 4096 * methord::HexChar(m_strReceiveData[port][64]) + 256 * methord::HexChar(m_strReceiveData[port][65]) + 16 * methord::HexChar(m_strReceiveData[port][62]) + methord::HexChar(m_strReceiveData[port][63]);// ������յ���CRCУ����

				if (CRC != dataCrc)
				{
					LogDisp(_T("Error���Ǳ�����CRCУ�����"));
					m_j++;
					m_strReceiveData[port].Empty();
					return 0;
				}
				m_i++;

				//��ֵת��������ȡ���Ǳ������а����ĵ������ȡ����
				m_volVal[iNo] = methord::Hex2Float(m_strReceiveData[port].Mid(6, 8));
				m_curVal[iNo] = methord::Hex2Float(m_strReceiveData[port].Mid(14, 8));
				m_tawVal[iNo] = methord::Hex2Float(m_strReceiveData[port].Mid(22,8))*0.001;
				m_trwVal[iNo] = methord::Hex2Float(m_strReceiveData[port].Mid(30,8))*0.001;
				m_tpqVal[iNo] = methord::Hex2Float(m_strReceiveData[port].Mid(46,8));
				m_freVal[iNo] = methord::Hex2Float(m_strReceiveData[port].Mid(54,8)); 
			}
			m_strproductID = ArrayID[iNo];// ��Ƶ���� = �������[�Ǳ���]

			if(m_bStartWork[iNo])
			{
				m_communicationSign[port]=TRUE;
				if (m_curVal[iNo] >= 0.1)//��������0.1A�����������ɹ�
				{
					if (!m_bRunning[iNo])// ��¼����ʱ��
					{
						CTime systemp = CTime::GetCurrentTime(); 
						CTimeSpan ts = systemp - m_startTime[iNo];// ����������ʱ��
						CString strStart;
						CString str;
						str = TimeToStr( iNo );
						strStart.Format(_T("%s��������ʱ��%i��\r\n"),str ,ts.GetTotalSeconds());
						CStdioFile SFile;
						CFileException fileException;

						CString strTextPath;
						strTextPath.Format(_T("%s//test.txt"), theApp.m_szAppPath);

						if(SFile.Open( strTextPath, CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite), &fileException )
						{
							SFile.SeekToEnd();// �ȶ�λ���ļ�β��
							SFile.WriteString(strStart);// д���ļ�
						}
						else
						{
							TRACE("Can't open file %s,error=%u\n",_T("test.txt"),fileException.m_cause);
						}
					}
					m_bRunning[iNo] = TRUE;// ��������0.1����λ����������
					OnStatusRunning(iNo);// ���¹�λ״̬
					if ( m_curVal[iNo] > 0.6 )// ��������0.6A�������������ָʾ��������
					{
						m_pPanlImg->m_iv[iNo]->m_pPrompt->setText( _T("��������") );
						m_pPanlImg->m_iv[iNo]->m_pText->setTextColor( 0x0000ff );// ����ɫ
					}
					else if ( m_curVal[iNo] < 0.4 )// ����С��0.4A��������С�����ָʾ��������
					{
						m_pPanlImg->m_iv[iNo]->m_pPrompt->setText( _T("��������") );
						m_pPanlImg->m_iv[iNo]->m_pText->setTextColor( 0xff0000 );// ����ɫ
					}
				}
				else
				{
					m_bRunning[iNo] = FALSE;// ����������0.1����λ��δ����
					CTime systemp = CTime::GetCurrentTime(); 
					CTimeSpan ts = systemp - m_startTime[iNo];// ����������ʱ��
					m_pPanlImg->m_iv[iNo]->m_pPrompt->setText(_T("starting"));
					if( ts.GetTotalSeconds() > 12 )// ��������г���12�룬�ҵ���ֵ��С��0.1����λ����ʧ��
					{
						m_bStartWork[iNo] = FALSE;
						m_pPanlImg->m_iv[iNo]->m_pPrompt->setText(_T("���ϸ�"));
						m_pPanlImg->m_iv[iNo]->m_pPrompt->setTextColor( 0x0000ff );
						::PostThreadMessage( m_pCtrlThread.m_Thread->m_nThreadID, WM_THREAD_MSG, 1, iNo );

						CString strEnd;
						CString str;
						str = TimeToStr(iNo);
						strEnd.Format(_T("%s��������ʱ�䣺%i�룬�������ɹ������Բ��ϸ�"), str, ts.GetTotalSeconds());

						CStdioFile SFile;
						CFileException fileException;

						CString strTextPath;
						strTextPath.Format( _T("%s//test.txt"), theApp.m_szAppPath );

						if(SFile.Open( strTextPath, CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite),&fileException)
						{
							SFile.SeekToEnd();// �ȶ�λ���ļ�β��
							SFile.WriteString(strEnd);// д���ļ�
						}
						else
						{
							TRACE("Can't open file %s,error=%u\n",_T("test.txt"),fileException.m_cause);
						}
					}
				}
				OnDisp(iNo,port);// ʵʱ��ʾ����
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
�������ݴ����жϼ̵�����λ״̬
*/
void MainForm::CtrlStatus( int iStatus, int iAddr, BOOL bLow )
{
	CString strStatus;
	int iPartNo = ( iAddr - 1 ) / 6;// ����̵�����ַ���,6����Ϊÿ32����λΪһ�����ڣ�ÿ������2̨�豸��ÿ̨�豸3�����ذ�
	int iNo = m_iNoBegin + iPartNo * m_iPartNumbers;
	if ( bLow )// ��ʾΪ�Ĵ����Ͱ�λ���Ĵ����߰�λ�ֱ�Ϊ�̵���1-8���Ͱ�λ�ֱ�Ϊ�̵���9-16
	{
		iNo += 8;
	}
	if ( iAddr % 6 == 4 )// ÿ���еڶ�̨�豸��
	{
		iNo += 16;
	}
	for (int i = 0; i < 8; i++)
	{
		if ( iStatus % 2  )
		{
			if ( !m_bStartWork[iNo + i] && !m_bTestEnd[iNo + i] )// ��λ��δ���������ҹ�λ�����ڶ�ʱ��������׶Σ���ù�λ������
			{
				strStatus.Format( _T("��λ%d����"), iNo + i );
				LogDisp( strStatus );
				StartNewTest( iNo + i );
				CTime sys = CTime::GetCurrentTime();
				m_startTime[iNo + i] = sys;// ��¼��λ����ʱ��
				m_bStartWork[iNo +i] = TRUE;// �̵���״̬Ϊ1����Ӧ��λ��������־��Ϊ1
				if ( !m_bTimer[ m_iCom[iPartNo] ] )// �����Ӧ�˿ڶ�ʱ��δ������������ʱ��
				{
					BeginToRun(iNo + i);// ������Ӧ��λ��iNo + iΪ��λ��
				}
			}
			if ( m_bTestEnd[iNo + i] )
			{
				if (m_iTestEnd[iNo + i] == 3)// �����׶δ�����1���غϣ�������жϽ���������ɣ��Է�������ɲ���ȷ��ʾ���
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
				m_bStartWork[iNo + i] = FALSE;// �̵���״̬Ϊ0����Ӧ��λ�Ͽ�����־��Ϊ0
				::PostThreadMessageA( m_pCtrlThread.m_Thread->m_nThreadID, WM_THREAD_MSG, 1, iNo+i);// ���ϸ�ָʾ
				LogDisp( _T("��λ�Ͽ������Բ��ϸ�") );
			}
		}
		iStatus = iStatus / 2;// iStatus����һλ
	}
}

/*
���ݴ洢,д��TXT�ĵ�
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
	if ( m_iStartMethod == 1 )// ɨ��������ʽ���������ݰ�����Ʒ���к�
	{
		str.Format(_T("%s	%04d	%s	%0.4f	%0.1f	%0.4f	%0.4f	%0.4f\r\n"), strDateTime, addr, strID, vol, cur, tpq, taw, trw );
	}
	else// ��ť������ʽ���������ݲ�������Ʒ���к�
	{
		str.Format(_T("%s	%04d	%0.4f	%0.1f	%0.4f	%0.4f	%0.4f\r\n"), strDateTime, addr, cur, vol, tpq, taw, trw );
	}
	CStdioFile SFile;
	CFileException fileException;
	CString strTextPath;
	strTextPath.Format(_T("%s//data.txt"), theApp.m_szAppPath);
	if(SFile.Open( strTextPath, CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite), &fileException )
	{
		SFile.SeekToEnd();// �ȶ�λ���ļ�β��
		SFile.WriteString(str);// д���ļ�
	}
	else
	{
		TRACE("Can't open file %s,error=%u\n",_T("data.txt"),fileException.m_cause);
	}
}
/*
���ݴ洢,д��TXT�ĵ�
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
	if ( m_iStartMethod == 1 )// ɨ��������ʽ���������ݰ�����Ʒ���к�
	{
		str.Format(_T("%s %s	%04d	%s	%0.4f	%0.1f\r\n"), strDate, strTime, addr, strID, cur, vol/*, fre*/);
	}
	else// ��ť������ʽ���������ݲ�������Ʒ���к�
	{
		str.Format(_T("%s %s	%04d	%0.4f	%0.1f\r\n"), strDate, strTime, addr, cur, vol/*, fre*/);
	}
	CStdioFile SFile;
	CFileException fileException;

	CString strTextPath;
	strTextPath.Format(_T("%s//data.txt"), theApp.m_szAppPath);

	if(SFile.Open( strTextPath, CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite), &fileException )
	{
		SFile.SeekToEnd();// �ȶ�λ���ļ�β��
		SFile.WriteString(str);// д���ļ�
	}
	else
	{
		TRACE("Can't open file %s,error=%u\n",_T("data.txt"),fileException.m_cause);
	}

}
/*
����˵������ʼ���а�ť��ִ���趨��ʱ��ָ��
����˵����addrΪ��Ӧ��λ��
*/
void MainForm::BeginToRun(int No)
{
	
	int port = PartToCOM(No);// ���ں�
	m_addr[port] = (No % m_iPartNumbers) == 0 ? m_iPartNumbers : No % m_iPartNumbers;
	if(m_bOpenPort[port])
	{
		if(m_bStart[port])
		{
			m_pMeterThread[port].Start();
			::PostThreadMessage( m_pMeterThread[port].m_Thread->m_nThreadID, WM_THREAD_MSG, NULL, m_addr[port] );
			t[1] = GetTickCount();// ��ʼ����ʱ��
			SetTimer( port, m_iComTime, NULL );
			m_bTimer[port] = TRUE;
			CString str;
			str.Format( _T("������ʱ��%d"),port );
			LogDisp( str );
		}
	}
	if(!m_bIsWriteThread)// ����ʼ����ʱ���̼�¼����
	{
		m_Thread = AfxBeginThread(WriteThread,this,0,CREATE_SUSPENDED,NULL);//���ٱ��������߳�
	}
}
/*
�򿪴���
*/
void MainForm::OpenComm()
{
	CString str = _T("�򿪴��ڴ���");
	CString str1;
	BOOL bFlag = TRUE;
	for (int i = 0;i < m_iParts; i++)
	{
		str1.Format(_T(",COM%i"),m_iCom[i]);
		OpenComm(m_iCom[i]);// �򿪶�Ӧ����
		if (!m_bOpenPort[m_iCom[i]])
		{
			str += str1;
		}
		bFlag = bFlag & m_bOpenPort[m_iCom[i]];
	}
	OpenComm( m_iCtrlCom );
	str1.Format(_T("COM%i,���м̵�������COM%i"),m_iCtrlCom,m_iCtrlCom);
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
�򿪴���m_nCom
*/
void MainForm::OpenComm( int m_nCom )
{
	m_bStart[m_nCom] = TRUE;
	m_dwCommEvents = EV_RXFLAG | EV_RXCHAR;
	m_nBaud = 9600;
	m_cParity = 'N';
	m_nDatabits = 8;
	m_nStopbits = 1;
	if ( !m_bOpenPort[m_nCom] )//����δ��
	{	
		if ( m_nCom == m_iCtrlCom )
		{
			if (m_pCtrlThread.m_Port.InitPort(this->m_hWnd, m_nCom, m_nBaud,m_cParity,m_nDatabits,m_nStopbits,m_dwCommEvents,512))
			{
				m_pCtrlThread.m_Port.StartMonitoring();// �������ڼ����߳�
				m_bOpenPort[m_nCom] = TRUE;
				m_EditComMsg.Format(_T("�򿪴���COM%d�ɹ�"),m_nCom,m_nBaud,m_cParity,m_nDatabits,m_nStopbits);
				LogDisp(m_EditComMsg);
			}
			else
			{
				m_EditComMsg.Format(_T("û�з��ִ���COM%d��ռ��"),m_nCom);
				LogDisp(m_EditComMsg);
				m_bStart[m_nCom] = FALSE;
			}
		}
		else
		{
			if ( m_pMeterThread[m_nCom].m_Port.InitPort(this->m_hWnd, m_nCom, m_nBaud,m_cParity,m_nDatabits,m_nStopbits,m_dwCommEvents,512))
			{
				m_pMeterThread[m_nCom].m_Port.StartMonitoring();// �������ڼ����߳�
				m_bOpenPort[m_nCom] = TRUE;
				m_EditComMsg.Format(_T("�򿪴���COM%d�ɹ�"),m_nCom,m_nBaud,m_cParity,m_nDatabits,m_nStopbits);
				LogDisp(m_EditComMsg);
			}
			else
			{
				m_EditComMsg.Format(_T("û�з��ִ���COM%d��ռ��"),m_nCom);
				LogDisp(m_EditComMsg);
				m_bStart[m_nCom] = FALSE;
			}
		}
	}
	else
	{
		m_EditComMsg.Format(_T("����COM%d�Ѵ�"),m_nCom);
		LogDisp(m_EditComMsg);
	}
}
/*
�رմ���
*/
void MainForm::CloseComm()
{
	CString strCom,str = _T("���ڹرգ�");
	for (int i = 0;i < m_iParts; i++)
	{
		if (m_bOpenPort[m_iCom[i]])
		{
			m_pMeterThread[m_iCom[i]].m_Port.ClosePort();// �رն�Ӧ����;
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
	CTimeSpan ts = systemp - m_startTime[iMeter];// ����������ʱ��
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
	str.Format(_T( "����%s������%s" ),m_strcurVal,m_sysTime);
	str.Format(_T("��ѹ:%s ����:%s\n����:%s ����:%s"), m_strvolVal, m_strtawVal, m_strcurVal, m_strtpqVal);
	m_pPanlImg->m_iv[iMeter]->m_pText->setText( m_strcurVal );
	m_pPanlImg->m_iv[iMeter]->m_pTime->setText( m_sysTime );
	
//	LogDisp(str);

	if ( ts.GetTotalMinutes() >= m_iTotalTime )// ����ʱ�䵽��ִ����Ӧ�ĺϸ����
	{
		m_bStartWork[iMeter] = FALSE;
		::PostThreadMessage( m_pCtrlThread.m_Thread->m_nThreadID, WM_THREAD_MSG, 2, iMeter );

		m_bTestEnd[iMeter] = TRUE;

		m_pPanlImg->m_iv[iMeter]->m_pPrompt->setText( _T( "���Ժϸ�" ) );
		OnStatus(iMeter);
		str.Format( _T("��λ%i����ʱ�䵽"), iMeter );
		LogDisp( str );
		//m_bStartWork[iMeter] = FALSE;
		//m_bRunning[iMeter] = FALSE;
		CString strEnd;
		CString str;
		str = TimeToStr(iMeter);
		strEnd.Format(_T("%s������ʱ�䣺%i�֣����Ժϸ�\r\n"), str, ts.GetTotalMinutes());
		CStdioFile SFile;
		CFileException fileException;
		CString strTextPath;
		strTextPath.Format(_T("%s//test.txt"), theApp.m_szAppPath );
		if(SFile.Open( strTextPath, CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite), &fileException )
		{
			SFile.SeekToEnd();// �ȶ�λ���ļ�β��
			SFile.WriteString( strEnd );// д���ļ�
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
��ʼ��һ�ֲ���
*/
void MainForm::StartNewTest( int iMeter )
{
	m_pPanlImg->m_iv[iMeter]->m_pText->setTextColor( 0x000000 );
	m_pPanlImg->m_iv[iMeter]->m_pPrompt->setTextColor( 0x000000 );
	
	if ( m_iStartMethod == 1 )// ɨ��������ִ��ȫ��-����
	{
		::PostThreadMessage( m_pCtrlThread.m_Thread->m_nThreadID, WM_THREAD_MSG, 3, iMeter );
	}
	else // ��ť������ִ�к��-�̹�
	{
		::PostThreadMessage( m_pCtrlThread.m_Thread->m_nThreadID, WM_THREAD_MSG, 4, iMeter );
	}
}
/*
������������ʾ��Ӧ�Ĺ�λ״̬
����˵����addrΪ��λ���
*/
void MainForm::OnStatus( int addr )
{
	m_pPanlImg->m_iv[addr]->m_pText->setTextColor( 0x00ff00 );// �м������Ϊ��ɫ
//	Invalidate(TRUE);
}
void MainForm::OnStatusRunning( int addr )// ��������״ָ̬ʾ
{
	m_pPanlImg->m_iv[addr]->m_pText->setTextColor( 0x00ff00 );// �м������Ϊ��ɫ
	m_pPanlImg->m_iv[addr]->m_pPrompt->setText( _T("����") );
	m_pPanlImg->m_iv[addr]->m_pPrompt->setTextColor( 0x000000 );// ������ʾ��Ϊ��ɫ
//	Invalidate(TRUE);
}
void MainForm::OnStatusDisconnect( int addr )
{
	m_pPanlImg->m_iv[addr]->m_pText->setTextColor( 0x0000ff );// �м������Ϊ��ɫ
	m_pPanlImg->m_iv[addr]->m_pPrompt->setText( _T("����") );
	Invalidate(TRUE);
}
/*
���������̣߳���ʱ��������
*/
UINT MainForm::WriteThread(LPVOID pParam)
{
	MainForm *pFrm = (MainForm *)pParam;
	pFrm->m_bIsWriteThread = TRUE;
	MSG msg;// ����һ��MSG�ı���msg��������Ϣ
	while(!pFrm->m_bAutoDelete) 
	{
		while (PeekMessage(&msg,NULL,0,0,PM_REMOVE))// ����Ϣ���������Ϣ�������msg
		{
			if(msg.message == WM_THREAD_STOP)// ����յ���ֹ��Ϣ���˳�
			{	
				//TODO�����ڶ���ı���Ҫ�������ֶ�����
				return 0;// �߳��������أ����ͷžֲ��������ڴ���Դ
			}
			else
			{
				DispatchMessage(&msg);
			}
		}
		Sleep( pFrm->m_iWriteTime - pFrm->m_iWriteTimeTemp );// �趨ʱ�� - д�ļ���ʱ
		int t1 = GetTickCount();
		pFrm->WriteData();// �洢����
		int t2 = GetTickCount();
		pFrm->m_iWrite++;// �洢��������
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
			if ( m_bRunning[i])// ��λ���������ż�¼����
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
ɨ�账��
*/
void MainForm::ScanMsg()
{
	CString str;
	CString strMsg,strNextMsg;// ɨ����ʾ��Ϣ
	str = m_pPanlCtr->m_pScan->getText();
	if ( str != _T("") )
//	if(!(atoi(str) == m_cNo || str == m_strproductID))// �������������¼�룬�������˴�ɨ��
	{
		UpdateData(TRUE);
		if(m_flag == 1)
		{
			strNo1 = m_pPanlCtr->m_pScan->getText();
			strMsg.Format(_T("ɨ��%i�Σ�%s"),m_flag,strNo1);
			LogDisp(strMsg);
		}
		else
		{
			strNo2 = m_pPanlCtr->m_pScan->getText();
			strMsg.Format(_T("ɨ��%i�Σ�%s"),m_flag,strNo2);
			LogDisp(strMsg);
		}
		m_pPanlCtr->m_pScan->setText( _T("") );

		if(m_flag > 1)// ɨ���������1,��������²���
		{
			if(!(strNo1 == strNo2) && (strNo1.GetLength() == 8 || strNo2.GetLength() == 8))//������ɨ��������ͬ��������һ��Ϊ8λ
			{
				if(strNo1.GetLength() == 8)// strNo1����Ϊ8����Ϊ��Ƶ����
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
				ScanToStart(m_cNo);// ��Գɹ���������Ӧ��λ
				strNo1 = _T("");// ��ջ���
				strNo2 = _T("");
				m_flag = 0;	
				str.Format( _T("ɨ����Գɹ�����λ�ţ�%i,��Ƶ���ţ�%s"), m_cNo, m_strproductID );
				LogDisp(str);
			}
		}
		m_flag++;
	}
	else
	{
		strMsg.Format(_T("%s�ظ�ɨ�裺%s"),strMsg,str);
		LogDisp(strMsg);
	}
	m_pPanlCtr->m_pScan->setText( _T("") );
}

/*
ɨ����ɣ�������Ӧ��λ
*/
void MainForm::ScanToStart( int iNo )
{
	int port = PartToCOM( iNo );// ��λ��Ӧ���ں�
	int addr = iNo % m_iPartNumbers;// �Ǳ��ַ
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
		StartNewTest( iNo ); // ������Ӧ��λ��255Ϊ�պ��������
		m_startTime[iNo] = sys;// ��¼�ù�λ����ʱ��ϵͳʱ��
		m_bStartWork[iNo] = TRUE;// ���øù�λ����������־λ��ΪTRUE
		if ( !m_bTimer[port] )// �����Ӧ�˿ڶ�ʱ��δ������������ʱ��
		{
			BeginToRun( iNo );
		}
	}	
}

/*
��ϢԤ����  �˴�������׽ɨ��ǹ��Ϣ
*/
BOOL MainForm::PreTranslateMessage( MSG* pMsg )
{
	// TODO: Add your specialized code here and/or call the base class
	if ( m_iStartMethod == 2 )// ������ʽΪ��ť������������ɨ����Ϣ
	{
		return FALSE;
	}
	if( pMsg -> message == WM_KEYDOWN && ( pMsg -> wParam == VK_RETURN ) )//�ж��Ƿ��а������£����Ұ���Ϊenter
	{
		if( !( m_bOpenPort[1] || m_bOpenPort[2] || m_bOpenPort[3] || m_bOpenPort[4] || m_bOpenPort[5] || m_bOpenPort[6] ) )
		{
			AfxMessageBox( _T("���鴮��") );
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
�ɹ�λ�� ��������Ӧ�Ĵ��ں�
����˵����NoΪ��λ��
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
�ɴ��ں� ��������Ӧ�Ĺ�λ��
����˵����portΪ���ں�
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
Log�༭����ʾ������ʵʱ���浽��־
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

	if ( !PathIsDirectory( strFolderPath ) )// �����־�ļ��в����ڣ����½���־�ļ���
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
		SFile.SeekToEnd();// �ȶ�λ���ļ�β��
		SFile.WriteString( strEditLog );// д���ļ�
	}
	else
	{
		TRACE( _T("Can't open file %s,error=%u\n"), strTextPath, fileException.m_cause );
	}
}

void MainForm::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	SetTimerEvent(nIDEvent);

	HWnd::OnTimer(nIDEvent);
}

CString MainForm::TimeToStr(int iNo)
{
	CString strID;
	if (m_iStartMethod == 1)
	{
		strID.Format(_T(" ��Ʒ��:%s,��λ��:%i"), ArrayID[iNo], iNo);
	}
	else
	{
		strID.Format(_T(" ��λ��:%i"), iNo);
	}

	CString strTime;
	SYSTEMTIME st;
	GetLocalTime(&st);
	strTime.Format("%02d/%02d/%02d %02d:%02d:%02d.%03d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

	return strTime + strID;
}
//CtrlThread.cpp

#include "stdafx.h"
#include "CtrlThread.h"
#include "CtrlComm.h"

CCtrlThread::CCtrlThread()
{
	m_bFun = TRUE;
}

CCtrlThread::~CCtrlThread()
{
	TRACE(_T("CtrlThread Ended\r\n"));
}

void CCtrlThread::RunMsg()
{
 	m_csForVec.Lock();
 	switch( m_iStatus )
 	{
	case 0: // ȫ��
		m_Port.SendCtrlOrderALL( CTRL_ORDER_CLOSE );
		TRACE(_T("close all\r\n"));
		Sleep(CTRL_DELAY);
		break;
 	case 1: // ȫ��-Red��--160420��ΪBLUE��-RED��
		m_Port.SendCtrlOrder(CTRL_ORDER_CLOSE, 254, m_iNo);
		Sleep(CTRL_DELAY);
		/*m_Port.SendCtrlOrder(CTRL_ORDER_CLOSE, LED_BLUE, m_iNo);
		Sleep( CTRL_DELAY );*/
		m_Port.SendCtrlOrder( CTRL_ORDER_OPEN, LED_RED, m_iNo);
		Sleep(CTRL_DELAY);
		break;
 	case 2: // ȫ��-Green��--160420��ΪBLUE��-GREEN��
		m_Port.SendCtrlOrder(CTRL_ORDER_CLOSE, 254, m_iNo);
		Sleep(CTRL_DELAY);
		/*m_Port.SendCtrlOrder(CTRL_ORDER_CLOSE, LED_BLUE, m_iNo);
		Sleep(CTRL_DELAY);*/
		m_Port.SendCtrlOrder( CTRL_ORDER_OPEN, LED_GREEN, m_iNo );
		Sleep(CTRL_DELAY);
		break;
	case 3: // ȫ��-Blue��
		m_Port.SendCtrlOrder( CTRL_ORDER_CLOSE, 254, m_iNo );
		Sleep(CTRL_DELAY);
		m_Port.SendCtrlOrder( CTRL_ORDER_OPEN, LED_BLUE, m_iNo );
		Sleep(CTRL_DELAY);
		break;
	case 4: // Red��-Green��
		m_Port.SendCtrlOrder( CTRL_ORDER_CLOSE, LED_GREEN, m_iNo );
		Sleep(CTRL_DELAY);
		m_Port.SendCtrlOrder( CTRL_ORDER_CLOSE, LED_RED, m_iNo );
		Sleep(CTRL_DELAY);
	case 5: // ��ѯ��λ״̬
		m_Port.SendCtrlOrder( 1 );// ��ѯ�̵���״̬
		Sleep( 200 );
		break;
	default:
		break;
 	}
 	m_csForVec.Unlock();
}

void CCtrlThread::Fun()
{
	//if(	m_bFun )
	{
		m_Port.SendCtrlOrder( 1 );
		Sleep( 200 );
	}
}

void CCtrlThread::Shut()
{
	TRACE(_T("Shut All\r\n"));
	m_Port.SendCtrlOrderALL( CTRL_ORDER_CLOSE );
	Sleep( 50 );
}
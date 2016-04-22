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
	case 0: // 全关
		m_Port.SendCtrlOrderALL( CTRL_ORDER_CLOSE );
		TRACE(_T("close all\r\n"));
		Sleep(CTRL_DELAY);
		break;
 	case 1: // 全关-Red开--160420改为BLUE关-RED开
		m_Port.SendCtrlOrder(CTRL_ORDER_CLOSE, 254, m_iNo);
		Sleep(CTRL_DELAY);
		/*m_Port.SendCtrlOrder(CTRL_ORDER_CLOSE, LED_BLUE, m_iNo);
		Sleep( CTRL_DELAY );*/
		m_Port.SendCtrlOrder( CTRL_ORDER_OPEN, LED_RED, m_iNo);
		Sleep(CTRL_DELAY);
		break;
 	case 2: // 全关-Green开--160420改为BLUE关-GREEN开
		m_Port.SendCtrlOrder(CTRL_ORDER_CLOSE, 254, m_iNo);
		Sleep(CTRL_DELAY);
		/*m_Port.SendCtrlOrder(CTRL_ORDER_CLOSE, LED_BLUE, m_iNo);
		Sleep(CTRL_DELAY);*/
		m_Port.SendCtrlOrder( CTRL_ORDER_OPEN, LED_GREEN, m_iNo );
		Sleep(CTRL_DELAY);
		break;
	case 3: // 全关-Blue开
		m_Port.SendCtrlOrder( CTRL_ORDER_CLOSE, 254, m_iNo );
		Sleep(CTRL_DELAY);
		m_Port.SendCtrlOrder( CTRL_ORDER_OPEN, LED_BLUE, m_iNo );
		Sleep(CTRL_DELAY);
		break;
	case 4: // Red关-Green关
		m_Port.SendCtrlOrder( CTRL_ORDER_CLOSE, LED_GREEN, m_iNo );
		Sleep(CTRL_DELAY);
		m_Port.SendCtrlOrder( CTRL_ORDER_CLOSE, LED_RED, m_iNo );
		Sleep(CTRL_DELAY);
	case 5: // 查询工位状态
		m_Port.SendCtrlOrder( 1 );// 查询继电器状态
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
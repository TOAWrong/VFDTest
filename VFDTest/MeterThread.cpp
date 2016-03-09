//CtrlThread.cpp

#include "stdafx.h"
#include "MeterThread.h"
#include "CtrlComm.h"
#include "Common.h"

CMeterThread::CMeterThread()
{
	m_fnCode = 3;// 查询命令代码
	m_readAddrCode = 22;// 查询首地址
	m_lenCode = 4;  // 查询寄存器个数
}

CMeterThread::~CMeterThread()
{
	TRACE(_T("MeterThread Ended\r\n"));
}

void CMeterThread::RunMsg()
{
	m_csForVec.Lock();
	
	m_Port.SendOrder();
	Sleep( 5 );
	m_Port.SendOrder( m_iNo, m_fnCode, m_readAddrCode, m_lenCode );
//	Sleep( 65 );

	m_csForVec.Unlock();
}
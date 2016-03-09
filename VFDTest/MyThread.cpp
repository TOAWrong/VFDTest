// MyThread.Cpp

#include "stdafx.h"
#include "MyThread.h"
#include <assert.h>

/////////////////////////////////////////////////////////////////////////////
// CMyThread
CMyThread::CMyThread()
{
	m_bFun = FALSE;
	m_bIsThread = FALSE;
	m_bExitThread = FALSE;
	m_dwWaitTimeOut = 5000;
}

CMyThread::~CMyThread()
{
	TRACE(_T("MyThread Ended!\r\n "));
}

BOOL CMyThread::Start()
{
	if ( !m_bIsThread )
	{
		if ( !( m_Thread = AfxBeginThread( Thread, this, THREAD_PRIORITY_NORMAL, 0, 0 ) ) )
			return FALSE;
	}
	TRACE( _T("Thread started\n") );
	return TRUE;
}

BOOL CMyThread::Restart()
{
	TRACE( _T("Thread resumed\n") );
	m_Thread->ResumeThread();
	return TRUE;
}

BOOL CMyThread::Stop()
{
	TRACE( _T("Thread suspended\n") );
	m_Thread->SuspendThread();
	return TRUE;
}

BOOL CMyThread::End()
{
	if( m_bIsThread )
	{
		m_bExitThread = TRUE;
		return TRUE;
	}
	return FALSE;
}

UINT CMyThread::Thread(LPVOID lpParam)
{
	CMyThread *pTrd = (CMyThread *)lpParam;
	pTrd->m_bIsThread = TRUE;
	MSG msg;
	while( !pTrd->m_bExitThread )
	{
		while ( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )// 将消息队列里的消息逐个读入msg
		{
			if( msg.message == WM_THREAD_STOP )// 如果收到终止消息则退出
			{
				pTrd->m_bIsThread = FALSE;
				TRACE(_T("MyThread Stoped\r\n"));
				return 0;// 线程正常返回，会释放局部变量等内存资源
			}
			else if ( msg.message == WM_THREAD_MSG )
			{
				pTrd->m_iNo = ( int )msg.lParam;
				pTrd->m_iStatus = ( int )msg.wParam;
				pTrd->RunMsg();
			}
			else if (msg.message == WM_THREAD_SHUT)
			{
				pTrd->Shut();
			}
			else
			{
				DispatchMessage( &msg );
			}
		}
		if ( pTrd->m_bFun )
		{
			pTrd->Fun();
		}	
	}

	return 0;
}

void CMyThread::RunOnceEnd()
{
	m_bExitThread = TRUE;
}

void CMyThread::RunMsg()
{
}

void CMyThread::Fun()
{
}

void CMyThread::Shut()
{

}

#include "stdafx.h" 
#include <objbase.h>   //主持COM的一个头文件
#include "TimerThread.h"

TimerThread::TimerThread(int nPriority)
{
	_TimerHandleCount = 0;
	_TerminateEvent = CreateEvent(0, TRUE, FALSE, NULL); //创建线程
	_TimerHandle = CreateWaitableTimer(NULL, FALSE, NULL);          //创建时间核心对象，自动重置定时器
	::InitializeCriticalSection(&_Mutex); //初始化临界区        
	unsigned int id;
	_hHandle = (HANDLE)_beginthreadex(NULL, 0, StaticThreadProc, this, 0, &id);
	_ThreadID = id;
	if(_hHandle != NULL)
		SetThreadPriority(_hHandle, nPriority); //设置线程优先级
	//else
	// MessageBox(NULL, "Create thread fail!", "TimerThread", MB_OK); 
}


TimerThread::~TimerThread()
{
	Terminate();   
	CloseHandle(_hHandle);  
	CloseHandle(_TimerHandle);
	CloseHandle(_TerminateEvent); 
	::DeleteCriticalSection(&_Mutex);
}


//子类需要重载的函数
void TimerThread::OnInit()
{
}


//OnTimer()函数需要在子类实现
//子类需要重载的函数
void TimerThread::OnExit()
{
}


BOOL TimerThread::SetTimer(LONG lPeriodTime)//ms
{
	_lPeriodTime = lPeriodTime;
	LARGE_INTEGER liUTC;
	memset(&liUTC, 0, sizeof(LARGE_INTEGER));
	return SetWaitableTimer(_TimerHandle, &liUTC, _lPeriodTime, NULL, NULL, FALSE)!=0;
}


BOOL TimerThread::SetTimer(SYSTEMTIME st, LONG lPeriodTime)//ms
{
	_lPeriodTime = lPeriodTime;
	_st = st; //SYSTEMTIME结构，用来设置第1次通知的时间
	FILETIME ftLocal, ftUTC;     //FILETIME结构，用来接受STSTEMTIME结构的转换
	LARGE_INTEGER liUTC;     //LARGE_INTEGER结构，作为SetWaitableTimer的参数,定时时间
	SystemTimeToFileTime(&_st, &ftLocal);           //将SYSTIME结构转换为FILETIME结构
	LocalFileTimeToFileTime(&ftLocal, &ftUTC);      //将本地时间转换为标准时间（UTC），SetWaitableTimer函数接受一个标准时间
	liUTC.LowPart  = ftUTC.dwLowDateTime;     // 设置LARGE_INTEGER结构，因为该结构数据要作为SetWaitableTimer的参数
	liUTC.HighPart = ftUTC.dwHighDateTime;
	return SetWaitableTimer(_TimerHandle, &liUTC, _lPeriodTime, NULL, NULL, FALSE) != 0;
}


BOOL TimerThread::SetTimer(int lSecond, LONG lPeriodTime)
{
	_lSecond = lSecond;
	_lPeriodTime = lPeriodTime;
	LARGE_INTEGER li;
	const int nTimerUnitsPerSecond = 1000000000 / 100;       //每1s中有多少个100ns
	li.QuadPart = -(_lSecond * nTimerUnitsPerSecond );        //负数，表示相对值lSecond秒
	return SetWaitableTimer(_TimerHandle, &li, _lPeriodTime, NULL, NULL, FALSE)!=0;
}


void TimerThread::Terminate()
{
	SetEvent(_TerminateEvent);
	if(WaitForSingleObject(_hHandle, 200) != WAIT_OBJECT_0)
		TerminateThread(_hHandle, 0);
}
void TimerThread::Run()
{
	HANDLE HandleArray[2];
	HandleArray[0] = _TimerHandle;
	HandleArray[1] = _TerminateEvent;
	for(;;)
	{
		DWORD ret = WaitForMultipleObjects(2, HandleArray, false, INFINITE); //当其中有一个信号量是往下执行
		if(ret == WAIT_OBJECT_0 + 1) //第二个事件发生
			break;
		if(ret == WAIT_OBJECT_0)     //第一个事件发生
		{
			try
			{
				OnTimer();
			}
			catch (...)
			{
			}


			InterlockedIncrement(&_TimerHandleCount);  //共享变量，互斥锁,时间内核对象被调用次数
		}
	}
}


HANDLE TimerThread::GetHandle()
{
	return _hHandle;
}


LONG TimerThread::GetFinishedCount()
{
	return _TimerHandleCount;
}




UINT WINAPI TimerThread::StaticThreadProc(LPVOID lpPara)
{
	TimerThread *pObj = (TimerThread*)lpPara;


	pObj->OnInit();
	pObj->Run();
	pObj->OnExit();
	return 0;
}
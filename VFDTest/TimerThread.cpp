
#include "stdafx.h" 
#include <objbase.h>   //����COM��һ��ͷ�ļ�
#include "TimerThread.h"

TimerThread::TimerThread(int nPriority)
{
	_TimerHandleCount = 0;
	_TerminateEvent = CreateEvent(0, TRUE, FALSE, NULL); //�����߳�
	_TimerHandle = CreateWaitableTimer(NULL, FALSE, NULL);          //����ʱ����Ķ����Զ����ö�ʱ��
	::InitializeCriticalSection(&_Mutex); //��ʼ���ٽ���        
	unsigned int id;
	_hHandle = (HANDLE)_beginthreadex(NULL, 0, StaticThreadProc, this, 0, &id);
	_ThreadID = id;
	if(_hHandle != NULL)
		SetThreadPriority(_hHandle, nPriority); //�����߳����ȼ�
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


//������Ҫ���صĺ���
void TimerThread::OnInit()
{
}


//OnTimer()������Ҫ������ʵ��
//������Ҫ���صĺ���
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
	_st = st; //SYSTEMTIME�ṹ���������õ�1��֪ͨ��ʱ��
	FILETIME ftLocal, ftUTC;     //FILETIME�ṹ����������STSTEMTIME�ṹ��ת��
	LARGE_INTEGER liUTC;     //LARGE_INTEGER�ṹ����ΪSetWaitableTimer�Ĳ���,��ʱʱ��
	SystemTimeToFileTime(&_st, &ftLocal);           //��SYSTIME�ṹת��ΪFILETIME�ṹ
	LocalFileTimeToFileTime(&ftLocal, &ftUTC);      //������ʱ��ת��Ϊ��׼ʱ�䣨UTC����SetWaitableTimer��������һ����׼ʱ��
	liUTC.LowPart  = ftUTC.dwLowDateTime;     // ����LARGE_INTEGER�ṹ����Ϊ�ýṹ����Ҫ��ΪSetWaitableTimer�Ĳ���
	liUTC.HighPart = ftUTC.dwHighDateTime;
	return SetWaitableTimer(_TimerHandle, &liUTC, _lPeriodTime, NULL, NULL, FALSE) != 0;
}


BOOL TimerThread::SetTimer(int lSecond, LONG lPeriodTime)
{
	_lSecond = lSecond;
	_lPeriodTime = lPeriodTime;
	LARGE_INTEGER li;
	const int nTimerUnitsPerSecond = 1000000000 / 100;       //ÿ1s���ж��ٸ�100ns
	li.QuadPart = -(_lSecond * nTimerUnitsPerSecond );        //��������ʾ���ֵlSecond��
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
		DWORD ret = WaitForMultipleObjects(2, HandleArray, false, INFINITE); //��������һ���ź���������ִ��
		if(ret == WAIT_OBJECT_0 + 1) //�ڶ����¼�����
			break;
		if(ret == WAIT_OBJECT_0)     //��һ���¼�����
		{
			try
			{
				OnTimer();
			}
			catch (...)
			{
			}


			InterlockedIncrement(&_TimerHandleCount);  //���������������,ʱ���ں˶��󱻵��ô���
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
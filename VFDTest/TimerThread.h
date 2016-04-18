/*用线程实现的时间核心对象类：*/
/*
作者:FreeBird
功能:时间核心对象线程类，该类是抽象类，需要子类实现OnTimer函数，实现定时器的功能
*/
#pragma once 
#include <winbase.h>   //包含了Windows平台专属的一些函数和宏的定义
#include <windows.h>
#include <process.h>   //一般用程序获得进程列表时会用这个文件中的API

#ifndef _TIMER_THREAD_H_
#define _TIMER_THREAD_H_

class TimerThread 
{
protected:
	HANDLE _TerminateEvent;			//事件句柄
	HANDLE _TimerHandle;			//时间内核对象
	HANDLE _hHandle;				//线程句柄
	unsigned _ThreadID;				//线程ID
	HWND _hNotifyWnd;				// 通知窗口
	LONG _TimerHandleCount;			//时间内核对象调用次数

	LONG _lPeriodTime;				//定时器周期
	SYSTEMTIME _st;					//指定开始执行时间
	int _lSecond;					//指定多少秒后开始执行

	CRITICAL_SECTION _Mutex;		//临界区

protected:	
	virtual void OnInit();		 //初始化，子类需要重载的函数
	virtual void OnTimer()=0;    //定时器函数，子类必须重载的函数
	virtual void OnExit();       //退出，子类需要重载的函数

	static UINT WINAPI StaticThreadProc(LPVOID lpPara);
	virtual void Run();         //运行

public:
	TimerThread(int nPriority = THREAD_PRIORITY_NORMAL);  //构造函数中创建事件对象、时间内核对象、初始化临界区并启动线程
	virtual ~TimerThread();

	BOOL SetTimer(LONG lPeriodTime);                //设置定时器,周期执行ms,该函数调用后立即按周期执行
	BOOL SetTimer(SYSTEMTIME st, LONG lPeriodTime); //调用该函数后，在st指定时间到时，按照周期执行
	BOOL SetTimer(int lSecond, LONG lPeriodTime);	//调用该函数后，在指定lSecond秒后开始执行
	void Terminate();					//终止定时器  
	HANDLE GetHandle(); 				//获取线程句柄

	inline void SetWnd(HWND hWnd);      //关联消息的窗口句柄
	LONG GetFinishedCount();            //获取线程被执行次数
};

void TimerThread::SetWnd(HWND hWnd)     //关联消息的窗口句柄
{
	//	assert(::IsWindow(hWnd));
	_hNotifyWnd = hWnd;
}
#endif _TIMER_THREAD_H_ 
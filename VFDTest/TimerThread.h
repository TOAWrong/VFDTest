/*���߳�ʵ�ֵ�ʱ����Ķ����ࣺ*/
/*
����:FreeBird
����:ʱ����Ķ����߳��࣬�����ǳ����࣬��Ҫ����ʵ��OnTimer������ʵ�ֶ�ʱ���Ĺ���
*/
#pragma once 
#include <winbase.h>   //������Windowsƽ̨ר����һЩ�����ͺ�Ķ���
#include <windows.h>
#include <process.h>   //һ���ó����ý����б�ʱ��������ļ��е�API

#ifndef _TIMER_THREAD_H_
#define _TIMER_THREAD_H_

class TimerThread 
{
protected:
	HANDLE _TerminateEvent;			//�¼����
	HANDLE _TimerHandle;			//ʱ���ں˶���
	HANDLE _hHandle;				//�߳̾��
	unsigned _ThreadID;				//�߳�ID
	HWND _hNotifyWnd;				// ֪ͨ����
	LONG _TimerHandleCount;			//ʱ���ں˶�����ô���

	LONG _lPeriodTime;				//��ʱ������
	SYSTEMTIME _st;					//ָ����ʼִ��ʱ��
	int _lSecond;					//ָ���������ʼִ��

	CRITICAL_SECTION _Mutex;		//�ٽ���

protected:	
	virtual void OnInit();		 //��ʼ����������Ҫ���صĺ���
	virtual void OnTimer()=0;    //��ʱ������������������صĺ���
	virtual void OnExit();       //�˳���������Ҫ���صĺ���

	static UINT WINAPI StaticThreadProc(LPVOID lpPara);
	virtual void Run();         //����

public:
	TimerThread(int nPriority = THREAD_PRIORITY_NORMAL);  //���캯���д����¼�����ʱ���ں˶��󡢳�ʼ���ٽ����������߳�
	virtual ~TimerThread();

	BOOL SetTimer(LONG lPeriodTime);                //���ö�ʱ��,����ִ��ms,�ú������ú�����������ִ��
	BOOL SetTimer(SYSTEMTIME st, LONG lPeriodTime); //���øú�������stָ��ʱ�䵽ʱ����������ִ��
	BOOL SetTimer(int lSecond, LONG lPeriodTime);	//���øú�������ָ��lSecond���ʼִ��
	void Terminate();					//��ֹ��ʱ��  
	HANDLE GetHandle(); 				//��ȡ�߳̾��

	inline void SetWnd(HWND hWnd);      //������Ϣ�Ĵ��ھ��
	LONG GetFinishedCount();            //��ȡ�̱߳�ִ�д���
};

void TimerThread::SetWnd(HWND hWnd)     //������Ϣ�Ĵ��ھ��
{
	//	assert(::IsWindow(hWnd));
	_hNotifyWnd = hWnd;
}
#endif _TIMER_THREAD_H_ 
// MyThread.h

#ifndef MY_THREAD_H
#define MY_THREAD_H

class CMyThread
{
public:
	CMyThread();
	virtual ~CMyThread();

	BOOL m_bIsThread;
	BOOL m_bFun;

	// 开启/停止线程
	BOOL				Start();
	BOOL				Restart();
	BOOL				Stop();
	BOOL				End();
	virtual void		RunMsg();
	virtual void		Fun();
	virtual void		Shut();

	int					m_iNo;
	int					m_iStatus;

	CCriticalSection	m_csForVec;

	// 线程
	CWinThread*			m_Thread;

protected:
	static UINT			Thread(LPVOID lpParam);
	void				RunOnceEnd();

	// 宏指令
	DWORD				m_dwWaitTimeOut;
	BOOL				m_bExitThread;
	LPVOID				m_lpParam;
  
	// 用户窗口
	CWnd*				m_pOwner;// 拥塞窗口
};

#endif
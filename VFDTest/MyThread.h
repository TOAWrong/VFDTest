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

	// ����/ֹͣ�߳�
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

	// �߳�
	CWinThread*			m_Thread;

protected:
	static UINT			Thread(LPVOID lpParam);
	void				RunOnceEnd();

	// ��ָ��
	DWORD				m_dwWaitTimeOut;
	BOOL				m_bExitThread;
	LPVOID				m_lpParam;
  
	// �û�����
	CWnd*				m_pOwner;// ӵ������
};

#endif
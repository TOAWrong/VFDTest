// CtrlThread

#include "MyThread.h"
#include "CtrlComm.h"

#ifndef CTRLTHREAD_H
#define CTRLTHREAD_H

class CCtrlThread : public CMyThread
{
public:
	CCtrlThread();
	~CCtrlThread();

	CCtrlComm m_Port;
	
	
	//int m_iStatus;
	//int m_iNo;

	virtual void RunMsg();
	virtual void Fun();
	virtual void Shut();
};

#endif
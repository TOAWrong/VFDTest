// CtrlThread

#include "MyThread.h"
#include "CtrlComm.h"

#ifndef _DATATHREAD_H_
#define _DATATHREAD_H_

class CMeterThread : public CMyThread
{
public:
	CMeterThread();
	~CMeterThread();

	CCtrlComm m_Port;

	/*int m_iNo;*/

	// 查询指令变量
	int m_readAddrCode;	//地址码
	int m_fnCode;		//命令码
	int m_lenCode;		//指令长度
	int m_datCode;		//数据

	virtual void RunMsg();
};

#endif
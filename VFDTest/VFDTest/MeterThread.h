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

	// ��ѯָ�����
	int m_readAddrCode;	//��ַ��
	int m_fnCode;		//������
	int m_lenCode;		//ָ���
	int m_datCode;		//����

	virtual void RunMsg();
};

#endif
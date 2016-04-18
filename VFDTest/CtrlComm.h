// CtrlComm.h: interface for the CtrlComm class.
//
//////////////////////////////////////////////////////////////////////
#include "SerialPort.h"

#ifndef __CTRLCOMM_H__
#define __CTRLCOMM_H__

class CCtrlComm : public CSerialPort
{
public:
	CCtrlComm();
	~CCtrlComm();

	int m_iNoBegin;		//��ʼ��λ��
	int m_iPartNumbers;	//ÿ�鹤λ��

	void SendOrder( int Address,int FunCode,int StartAddress,int Length);// ���Ͳ�ѯָ��
	void SendCtrlOrder( int FunCode,int Address,int addr);// ���ͼ̵�������ָ��
	void SendCtrlOrderALL( int FunCode);// ����ȫ���̵���ָ��
	void SendCtrlOrder( int addr );// ���Ͳ�ѯ�̵���ָ��
	void SendOrder();// ͬ���Ǳ�

};

#endif 
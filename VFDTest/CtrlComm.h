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

	int m_iNoBegin;		//起始工位号
	int m_iPartNumbers;	//每组工位数

	void SendOrder( int Address,int FunCode,int StartAddress,int Length);// 发送查询指令
	void SendCtrlOrder( int FunCode,int Address,int addr);// 发送继电器控制指令
	void SendCtrlOrderALL( int FunCode);// 操作全部继电器指令
	void SendCtrlOrder( int addr );// 发送查询继电器指令
	void SendOrder();// 同步仪表

};

#endif 
// CtrlComm.cpp: implementation of the CtrlComm class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CtrlComm.h"
#include "methord.h"
#include "MainForm.h"
#include "VFDTest.h"

CCtrlComm::CCtrlComm()
{
	CString strPath;
	strPath.Format(_T("%s//Config.ini"),theApp.m_szAppPath);

	CIni ini(strPath);

	m_iPartNumbers = ini.GetIntValue(_T("DISP"),_T("PARTNUMBERS"));
	CString strPart = _T("PART[0]");
	int m_iPart = ini.GetIntValue(_T("COM"),strPart);
	m_iNoBegin = (m_iPart - 1) * m_iPartNumbers + 1;// 该组首个工位号
}
CCtrlComm::~CCtrlComm()
{

}

/*
仪表操作	查询指令
查询方式:	先写入字符数组，再用WritePort函数写入串口
参数说明：	m_nCom	仪表串口号，通常为m_iCom
			Address 仪表地址，每组均为1-32
			FunCode 仪表操作指令，本程序只用到查询指令03H
			StartAddress 仪表寄存器起始地址
			Length	查询寄存器个数
*/
void CCtrlComm::SendOrder( int Address,int FunCode,int StartAddress,int Length)
{
 //   char data[10];
	char  Buf[8]={'\0'};
	Buf[0]=(CHAR)Address;
	Buf[1]=(CHAR)FunCode;
	Buf[2]=(CHAR)((StartAddress & 0xFF00)>>8);
	Buf[3]=(CHAR)(StartAddress & 0xFF);
	Buf[4]=(CHAR)((Length & 0xFF00)>>8);
	Buf[5]=(CHAR)(Length & 0xFF);
	USHORT CRC = methord::MBCRC16((UCHAR *)Buf,6);
	Buf[6]=(CHAR)(CRC & 0xFF);
	Buf[7]=(CHAR)((CRC & 0xFF00)>>8);
		
	WriteToPort((char*)Buf,8);
	//	m_Port.WriteToPort((LPCTSTR)m_strSendData);	//发送数据
}
/*
串口同步
写入8个FE，以防仪表互相干扰
*/
void CCtrlComm::SendOrder()
{
	char  Buf[8]={'FE'};

	WriteToPort((char*)Buf,8);
}
/*
继电器 操作指令
参数说明：	FunCode为FFH为打开，00H为闭合
			m_nCom为控制串口编号，通常为m_iCtrlCom
			Address为控制板地址，1、4为主电路控制，2、5为合格指示灯控制，3、6为不合格指示灯控制
			iNo为工位编号
*/
void CCtrlComm::SendCtrlOrder( int FunCode,int Address,int iNo )
{
//	m_bCtrlBusy = TRUE;
	char Buf[8]={'\0'};
	int buf = 0;
	int buf1 = 5;
	
	int ipart = ( iNo - m_iNoBegin ) / 32;// 如果工位组数不为1，则继电器控制板对应地址位要+6的倍数，即第二组+6，第三组+12，第四组+18
	Address = ( ipart * 6 ) + Address;

	int iCtrlNo = (iNo - 1) % m_iPartNumbers + 1;// 仪表地址位
	if ( (iCtrlNo - 1) / 16 != 0)// 如果仪表地址位大于16，表明仪表位于该组第二设备上，则继电器控制板相应地址位+3
	{
		Address += 3;
	}

	iCtrlNo = (iCtrlNo - 1) % 16;// 继电器地址位为0-15

	Buf[0]=(CHAR)Address;
	Buf[1]=(CHAR)buf1;
	Buf[2]=(CHAR)buf;
	Buf[3]=(CHAR)iCtrlNo;
	Buf[4]=(CHAR)FunCode;
	Buf[5]=(CHAR)buf;
	USHORT CRC = methord::MBCRC16((UCHAR *)Buf,6);
	Buf[6]=(CHAR)(CRC & 0xFF);
	Buf[7]=(CHAR)((CRC & 0xFF00)>>8);

	WriteToPort((char*)Buf,8);
}
/*
继电器 全开全关操作指令
参数说明：	FunCode为FFH为打开，00H为闭合
			m_nCom为控制串口编号，通常为m_iCtrlCom
*/
void CCtrlComm::SendCtrlOrderALL( int FunCode )
{
	char Buf[11]={'\0'};
	int buf1 = 254;
	int buf2 = 15;
	int buf3 = 0;
	int buf4 = 16;
	int buf5 = 2;

 	Buf[0] = (CHAR)buf1;	// 广播地址
 	Buf[1] = (CHAR)buf2;	// 写多个线圈寄存器
 	Buf[2] = (CHAR)buf3;
 	Buf[3] = (CHAR)buf3;
 	Buf[4] = (CHAR)buf3;
 	Buf[5] = (CHAR)buf4;	//继电器数量为16个
 	Buf[6] = (CHAR)buf5;	//发送命令字节数为2
	Buf[7] = (CHAR)FunCode;
	Buf[8] = (CHAR)FunCode;
	USHORT CRC = methord::MBCRC16(( UCHAR* )Buf,9);
	Buf[9]=(CHAR)(CRC & 0xFF);
	Buf[10]=(CHAR)((CRC & 0xFF00)>>8);

	WriteToPort((char*)Buf,11);
}
/*
继电器 查询线圈状态操作指令
*/
void CCtrlComm::SendCtrlOrder( int addr )
{
	char Buf[8]={'\0'};
	int buf1 = 1;
	int buf2 = 0;
	int buf3 = 16;

 	Buf[0] = (CHAR)addr;	// 工控板地址
 	Buf[1] = (CHAR)buf1;	// 查询线圈寄存器命令
 	Buf[2] = (CHAR)buf2;
 	Buf[3] = (CHAR)buf2;
 	Buf[4] = (CHAR)buf2;
 	Buf[5] = (CHAR)buf3;	//继电器数量为16个
	USHORT CRC = methord::MBCRC16(( UCHAR* )Buf,6);
	Buf[6]=(CHAR)(CRC & 0xFF);
	Buf[7]=(CHAR)((CRC & 0xFF00)>>8);

	WriteToPort((char*)Buf,8);
}
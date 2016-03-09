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
	m_iNoBegin = (m_iPart - 1) * m_iPartNumbers + 1;// �����׸���λ��
}
CCtrlComm::~CCtrlComm()
{

}

/*
�Ǳ����	��ѯָ��
��ѯ��ʽ:	��д���ַ����飬����WritePort����д�봮��
����˵����	m_nCom	�Ǳ��ںţ�ͨ��Ϊm_iCom
			Address �Ǳ��ַ��ÿ���Ϊ1-32
			FunCode �Ǳ����ָ�������ֻ�õ���ѯָ��03H
			StartAddress �Ǳ�Ĵ�����ʼ��ַ
			Length	��ѯ�Ĵ�������
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
	//	m_Port.WriteToPort((LPCTSTR)m_strSendData);	//��������
}
/*
����ͬ��
д��8��FE���Է��Ǳ������
*/
void CCtrlComm::SendOrder()
{
	char  Buf[8]={'FE'};

	WriteToPort((char*)Buf,8);
}
/*
�̵��� ����ָ��
����˵����	FunCodeΪFFHΪ�򿪣�00HΪ�պ�
			m_nComΪ���ƴ��ڱ�ţ�ͨ��Ϊm_iCtrlCom
			AddressΪ���ư��ַ��1��4Ϊ����·���ƣ�2��5Ϊ�ϸ�ָʾ�ƿ��ƣ�3��6Ϊ���ϸ�ָʾ�ƿ���
			iNoΪ��λ���
*/
void CCtrlComm::SendCtrlOrder( int FunCode,int Address,int iNo )
{
//	m_bCtrlBusy = TRUE;
	char Buf[8]={'\0'};
	int buf = 0;
	int buf1 = 5;
	
	int ipart = ( iNo - m_iNoBegin ) / 32;// �����λ������Ϊ1����̵������ư��Ӧ��ַλҪ+6�ı��������ڶ���+6��������+12��������+18
	Address = ( ipart * 6 ) + Address;

	int iCtrlNo = (iNo - 1) % m_iPartNumbers + 1;// �Ǳ��ַλ
	if ( (iCtrlNo - 1) / 16 != 0)// ����Ǳ��ַλ����16�������Ǳ�λ�ڸ���ڶ��豸�ϣ���̵������ư���Ӧ��ַλ+3
	{
		Address += 3;
	}

	iCtrlNo = (iCtrlNo - 1) % 16;// �̵�����ַλΪ0-15

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
�̵��� ȫ��ȫ�ز���ָ��
����˵����	FunCodeΪFFHΪ�򿪣�00HΪ�պ�
			m_nComΪ���ƴ��ڱ�ţ�ͨ��Ϊm_iCtrlCom
*/
void CCtrlComm::SendCtrlOrderALL( int FunCode )
{
	char Buf[11]={'\0'};
	int buf1 = 254;
	int buf2 = 15;
	int buf3 = 0;
	int buf4 = 16;
	int buf5 = 2;

 	Buf[0] = (CHAR)buf1;	// �㲥��ַ
 	Buf[1] = (CHAR)buf2;	// д�����Ȧ�Ĵ���
 	Buf[2] = (CHAR)buf3;
 	Buf[3] = (CHAR)buf3;
 	Buf[4] = (CHAR)buf3;
 	Buf[5] = (CHAR)buf4;	//�̵�������Ϊ16��
 	Buf[6] = (CHAR)buf5;	//���������ֽ���Ϊ2
	Buf[7] = (CHAR)FunCode;
	Buf[8] = (CHAR)FunCode;
	USHORT CRC = methord::MBCRC16(( UCHAR* )Buf,9);
	Buf[9]=(CHAR)(CRC & 0xFF);
	Buf[10]=(CHAR)((CRC & 0xFF00)>>8);

	WriteToPort((char*)Buf,11);
}
/*
�̵��� ��ѯ��Ȧ״̬����ָ��
*/
void CCtrlComm::SendCtrlOrder( int addr )
{
	char Buf[8]={'\0'};
	int buf1 = 1;
	int buf2 = 0;
	int buf3 = 16;

 	Buf[0] = (CHAR)addr;	// ���ذ��ַ
 	Buf[1] = (CHAR)buf1;	// ��ѯ��Ȧ�Ĵ�������
 	Buf[2] = (CHAR)buf2;
 	Buf[3] = (CHAR)buf2;
 	Buf[4] = (CHAR)buf2;
 	Buf[5] = (CHAR)buf3;	//�̵�������Ϊ16��
	USHORT CRC = methord::MBCRC16(( UCHAR* )Buf,6);
	Buf[6]=(CHAR)(CRC & 0xFF);
	Buf[7]=(CHAR)((CRC & 0xFF00)>>8);

	WriteToPort((char*)Buf,8);
}
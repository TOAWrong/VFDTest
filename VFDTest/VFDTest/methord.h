#ifndef __METHORD_H__
#define __METHORD_H__

class methord  
{
public:
	
	// ��������ת��
	static	int Hex2Dec(CString data);// ʮ������תʮ����
	static	int Str2Hex(CString str, char *data);// �ַ���תʮ������
	static	float Hex2Float(CString data);// ʮ������ת������
	static	char HexChar(char c);// �ַ���תʮ������
	static	USHORT MBCRC16( UCHAR * pucFrame, USHORT usLen );// �������CRCУ����

	static int methord::FindCommPort(CString *serial);
	static void StrSort(CString *StrList, int Comnum);

	methord();
	virtual ~methord();

};

#endif

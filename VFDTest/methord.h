#ifndef __METHORD_H__
#define __METHORD_H__

class methord  
{
public:
	
	// 数据类型转换
	static	int Hex2Dec(CString data);// 十六进制转十进制
	static	int Str2Hex(CString str, char *data);// 字符串转十六进制
	static	float Hex2Float(CString data);// 十六进制转浮点数
	static	char HexChar(char c);// 字符型转十六进制
	static	USHORT MBCRC16( UCHAR * pucFrame, USHORT usLen );// 查表法计算CRC校验码

	static int methord::FindCommPort(CString *serial);
	static void StrSort(CString *StrList, int Comnum);

	methord();
	virtual ~methord();

};

#endif


/*************************************************************************************\
�Զ�����治���κ�windows�ؼ���������ť����𡢴��ڷָ��̬����Ӧ��С��ȫ���л���F11������ݼ���S����
�Ի���������ڽ������ݡ����Զ�ȡ�����ã������ϴεĴ���״̬��˫������ʾ������˸���Զ���Ǵ������Զ�����ON_COMMAND�¼�
MFC2010
��������Hui.h
\**************************************************************************************/

// VFDTest.h : VFDTest Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������

// VFDTestApp:
// �йش����ʵ�֣������ VFDTest.cpp
//

class VFDTestApp : public CWinAppEx
{
public:
	VFDTestApp();

// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ʵ��
public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()

// Properties
public:
	int					m_nFullScreen;	// ��Ϊ��ʱ��ȫ����ʾ�����򴰿���ʾ��
	int					m_nMaximized;	// ȫ��֮ǰ�Ĵ������״̬

	TCHAR				m_szAppPath[MAX_PATH]; // MAX_PATH��API�ж����˰ɣ�������128
	TCHAR				m_szTempPath[MAX_PATH];	// ��ʱ�ļ���

// Methods
private:

public:
};

extern VFDTestApp theApp;

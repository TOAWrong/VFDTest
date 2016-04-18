
/*************************************************************************************\
自定义界面不用任何windows控件，包括按钮，标答、窗口分割，动态自适应大小，全屏切换（F11），快捷键（S），
对话框和主窗口交换数据、属性读取和设置，记忆上次的窗口状态，双缓存显示，不闪烁，自定义非窗口类自动处理ON_COMMAND事件
MFC2010
详见代码和Hui.h
\**************************************************************************************/

// VFDTest.h : VFDTest 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"       // 主符号

// VFDTestApp:
// 有关此类的实现，请参阅 VFDTest.cpp
//

class VFDTestApp : public CWinAppEx
{
public:
	VFDTestApp();

// 重写
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// 实现
public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()

// Properties
public:
	int					m_nFullScreen;	// 不为零时，全屏显示，否则窗口显示。
	int					m_nMaximized;	// 全屏之前的窗口最大化状态

	TCHAR				m_szAppPath[MAX_PATH]; // MAX_PATH在API中定义了吧，好象是128
	TCHAR				m_szTempPath[MAX_PATH];	// 临时文件夹

// Methods
private:

public:
};

extern VFDTestApp theApp;


#pragma once

#include "Hui.h"


// ImgView 拼接图形列表

class ImgView : public HCtrl
{
	DECLARE_DYNAMIC(ImgView)
	DECLARE_MESSAGE_MAP()

// Construction
public:
	ImgView(LPCRECT prc, int ID);	// 用区域和ID号初始化控件
	ImgView(LPCRECT prc);	// 用区域初始化控件
	virtual ~ImgView();

// Overflow
public:
	virtual void draw( CDC* pDC, CRect & rcClip );	// 画客户区
	virtual void OnLButtonDown( UINT nFlags, CPoint point );	// 返回true表示已经处理。
	virtual void	layerout();					//重排控件

// Properties
public:
	HLabel *		m_pText;		// 中间实时数据显示
	HLabel *		m_pPrompt;		// 左上角显示运行状态
	HLabel *		m_pNo;			// 左下角显示编号
	HLabel *		m_pTime;		// 右上角显示已运行时间
protected:
	int				m_nPos;			// 已拼接图像的行位置，向上拼时逐渐减小，向下拼时逐渐加大, -2表示不接收拼接图像，-1表示可以接收图像，当前图像是空。
	HBtn *			m_pPosBtn;		// 引导按钮

public:
	static ImgView*	m_pJoinView;	// 活动图像窗口, 正在拼接

// Methods
public:
	void save( CString & sdid );			// 在sdid中保存图像

protected:

// Messages
public:
	afx_msg void OnAbout();	//自定义非窗口类自动处理ON_COMMAND事件
};



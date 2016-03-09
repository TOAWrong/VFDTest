
#pragma once

#include "Hui.h"
#include "ImgView.h"
// PanlImg 拼接图形列表

class PanlImg : public HCtrl
{
	DECLARE_MESSAGE_MAP()
	DECLARE_DYNAMIC(PanlImg)

// Construction
public:
	PanlImg(LPCRECT prc);
	virtual ~PanlImg();
	ImgView *m_iv[1000];

// Properties
private:
	int m_nRowNumber;			// 图像窗口行数
	int m_nColumnNumber;		// 图像窗口列数
public:	//属性读取和设置
	int getRowNumber() { return m_nRowNumber; }
	void setRowNumber( int v ){m_nRowNumber = v; }
	int getColumnNumber() { return m_nColumnNumber; }
	void setColumnNumber( int v ) { m_nColumnNumber = v; }

// Methods
public:
	void init();		// 初始化2行4列，连续扫描方式

// Overwrite
	virtual void layerout();		//重排控件

// Messages
public:
	afx_msg void OnGuiderBtn( UINT nID );	// 自定义非窗口类自动处理ON_COMMAND事件
};



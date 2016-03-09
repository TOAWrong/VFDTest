// PanlCtr.cpp : 实现文件
//

#include "stdafx.h"
#include "VFDTest.h"
#include "Hui.h"
#include "PanlCtr.h"

// PanlCtr

IMPLEMENT_DYNAMIC(PanlCtr, HCtrl)

PanlCtr::PanlCtr(LPCRECT prc) : HCtrl(prc)
{
	// 背景颜色，钛金
//	setBackColor( RGB(60, 55, 40) );
//	setBackColor( RGB( 255, 255, 255) );
//	setBackColor( 0xffbf00 );// 深天蓝色
	setBackColor( 0xffaa00 );
	// 标题
	m_pTitle = new HLabel( CRect( 0, 0, 340, 120 ) );
	m_pTitle->setFont( 30, 20, _T("黑体") );
//	m_pTitle->setBackColor( RGB( 70, 65, 50 ) );
//	m_pTitle->setBackColor( 0xebce87 );// 天蓝色
//	m_pTitle->setBackColor( 0xf33403 );
//	m_pTitle->setTextColor( 0xffeedd );
	m_pTitle->setBackImg(MAKEINTRESOURCE(IDB_LOGO), _T("PNG"));
	m_pTitle->setTextColor( 0xffffff );// 浅黄色
	m_pTitle->setText( _T("\r\n天银机电\r\n老化测试系统") );
	m_pTitle->setShadowColor( 0x000000 );
	m_pTitle->setAlign( DT_CENTER | DT_VCENTER  );
	addCtrl( m_pTitle );

	// 信息提示框
	m_pDynamicScan = new DynamicScan( CRect(4, 220, 4+320+2, 220+240+2) );
	addCtrl ( m_pDynamicScan );

	// 加按钮
	m_pc = new HCtrl( CRect( 0, 110, 380, 210 ) );
	addCtrl( m_pc );
	CSize imgsize(57, 57);
	setBtnImage( IDB_BTN57X57, CSize( 57, 57 ) );	// 按钮图片

	const int rows = 1;
	const int cols = 4;
	int y = 20;
	int ids[rows*cols] = {TID_EXIT, TID_OPTIONS, TID_FULLSCREEN, TID_ABOUT};
	CString title[rows*cols] = {_T("退出"), _T("设置"), _T("全屏"), _T("关于")};
	for( int i = 0; i < rows; i++ )
	{
		for( int j = 0; j < cols; j++ )
		{
			int x = j * 70 + y;
			HBtn * btn = new HBtn(CRect(x, y, x + imgsize.cx, y + imgsize.cy), 0, 1, 2, ids[i*cols + j]);
			btn->setText( title[i*cols+j] );
			btn->setFont( 18, 0, _T("楷体") );
			btn->setTextColor( 0xffffff );
			m_pc->addCtrl( btn );
		}
	}

	// 运行日志提示框
	m_pLog = new HEdit( CRect());
	m_pLog->setTextColor( RGB( 0, 255, 0) );
	m_pLog->setFont( 14, 0, _T("楷体") );
//	m_pLog->setAlign( DT_LEFT );
	m_pLog->setBackColor( 0x000000 );
	addCtrl(m_pLog);

	// 扫描输入框
	m_pScan = new HEdit( CRect());
//	m_pScan->setAlign( DT_CENTER | DT_VCENTER );
	m_pScan->setFont( 14, 0, 0 );
	addCtrl(m_pScan);
}

PanlCtr::~PanlCtr()
{
}

//计算顶级控件位置。
void PanlCtr::layerout()
{
//	m_rcClient.bottom = m_rcWnd.bottom;
	int yy = m_rcClient.bottom - m_rcClient.top;
	int xx = m_rcClient.right - m_rcClient.left;
	if (m_pTitle)
	{
		m_pTitle->setRect( CRect( 0, 0, xx, 120 ),false );
	}
	if (m_pc)
	{
		m_pc->setRect( CRect( 0, 4 + 120, xx, 4 + 120 + 80 ),false );
	}
	if (m_pDynamicScan)
	{
		m_pDynamicScan->setRect( CRect( 4, 4 + 120 + 80 + 20, xx - 4, 4 + 120 + 80 + 20 + 120 ),false );
	}
	if ( m_pLog )
	{
		m_pLog->setRect( CRect(4,  4 + 120 + 80 + 20 + 120 + 4, xx - 4, yy - 34), false);
	}
	if ( m_pScan )
	{
		m_pScan->setRect( CRect( 4, yy -30, xx - 4, yy - 4), false);
	}
	HCtrl::layerout();
}

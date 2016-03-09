// DynamicScan.cpp : 实现文件
// 动态扫描控件

#include "stdafx.h"
#include "VFDTest.h"
#include "DynamicScan.h"


// DynamicScan
IMPLEMENT_DYNAMIC(DynamicScan, HCtrl)

BEGIN_MESSAGE_MAP(DynamicScan, HCtrl)
END_MESSAGE_MAP()

DynamicScan::DynamicScan(LPCRECT prc) : HCtrl(prc)
{
	// 信息提示框
	// 左上角提示框
	m_pPrompt = new HLabel(CRect( 8, 8, 300, 8+20 ));
	addCtrl( m_pPrompt );
	m_pPrompt->setText( _T("状态") ); 
	m_pPrompt->setFont( 22, 0, _T("微软雅黑") );
	m_pPrompt->setTextColor( 0x000000 );

	// 右上角提示框
	m_pTime = new HLabel(CRect());
	m_pTime->setText( _T("已运行时间(h:m:s)") ); 
	m_pTime->setAlign( DT_RIGHT );
	m_pTime->setFont( 22, 0, _T("微软雅黑") );
	m_pTime->setTextColor( 0x000000 );
	addCtrl( m_pTime );

	//中间显示电流
	int yy = ( m_rcClient.bottom - prc->top ) / 2 - 10;
	int xx = prc->right - prc->left;
	m_pText = new HLabel( CRect( 0, yy, xx , 20));
	m_pText->setAlign( DT_CENTER );
	m_pText->setText( _T("电流I(A)") );
	m_pText->setFont(30,0,_T("TIMES NEW ROMAN"));
	m_pText->setTextColor( 0x000000 );
	addCtrl( m_pText );

	// 左下角信息
	m_pNo = new HLabel(CRect( 8, prc->bottom - 20, prc->right, 28 ));
	addCtrl( m_pNo );
	m_pNo->setText( _T("工位号") );
	m_pNo->setTextColor( 0x000000 );
}

DynamicScan::~DynamicScan()
{
}

// 扫描，窗口建立完毕后初始化。
void DynamicScan::initCtrl()
{
	// TODO 初始化控件

	HCtrl::initCtrl();
}


// 画用户区
void DynamicScan::draw( CDC* pDC, CRect & rcClip )
{
	// TODO 画控件
	pDC->FillSolidRect( m_rcWnd, 0xffffff );

	HCtrl::draw( pDC, rcClip );
	
}
//计算顶级控件位置。
void DynamicScan::layerout()
{
	int yy = m_rcClient.Height();
	int xx = m_rcClient.Width();
	if (m_pTime)
	{
		m_pTime->setRect( CRect( xx/2,  8, xx, 8 + 20), false);
	}
	if ( m_pNo )
	{
		m_pNo->setRect( CRect(8, yy-20, xx, yy), false);
	}
	if ( m_pText )
	{
		m_pText->setRect( CRect( 0, yy / 2 - 15, xx, yy), false);
	}
	HCtrl::layerout();	
}
// HLabel.cpp : 实现文件
//
// 文字控件

#include "stdafx.h"
#include "Hui.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// HLabel
IMPLEMENT_DYNAMIC(HLabel, HCtrl)

HLabel::HLabel(LPCRECT prc)
: HCtrl(prc)
, m_bAutoSize( FALSE )	// 默认不自动计算大小
{
	m_nID = -1;	// 默认不接收鼠标消息
	m_bShadow = FALSE;	// 默认不显示阴影
}

HLabel::~HLabel()
{
}

// HLabel 成员函数

void HLabel::draw( CDC* pDC, CRect & rcClip )
{
	HCtrl::draw( pDC, rcClip );

	if( !m_strText.IsEmpty() )
		drawText( pDC );
}

// 画阴影文字
void HLabel::drawText( CDC* pDC )
{
	pDC->SetBkMode(TRANSPARENT);

	CFont * pOldfont = 0;
	CFont * pfont = getFont();
	if( pfont )
		pOldfont = pDC->SelectObject( pfont );

	CRect rc = m_rcWnd;

	if( m_bShadow )
	{
		rc.OffsetRect( 1, 1 );
		COLORREF oldColor = pDC->SetTextColor(m_coShadow);	// 先画阴影色，与TTMView中的同步 Color color3(0x00,0x00,0x00);	// 阴影色
		pDC->DrawText(m_strText, rc, m_nAlign);
		rc.OffsetRect( -1, -1 );
		pDC->SetTextColor(m_coText);	// 再画前景文字颜色， 与TTMView中的同步 Color color1(0x88,0x88,0x88);	// 非活动窗口色
		pDC->DrawText(m_strText, rc, m_nAlign);
		pDC->SetTextColor(oldColor);
	}
	else
	{
		COLORREF oldColor = pDC->SetTextColor(m_coText);	// 文字颜色
		pDC->DrawText(m_strText, rc, m_nAlign);
		pDC->SetTextColor(oldColor);
	}

	if( pOldfont )
		pDC->SelectObject( pOldfont );
}

// 设置阴影颜色
void HLabel::setShadowColor( COLORREF color )
{
	m_bShadow = TRUE;
	m_coShadow = color;
}

// 重写设置文本，计算大小。
void HLabel::setText(CString txt)
{
	HCtrl::setText( txt );

	if( !m_bAutoSize )
		return;

	CRect rc;
	if( getTextSize( rc ) )
	{
		if( m_bShadow )	// 如果有阴影，周围加大一个象素
			rc.DeflateRect( -1, -1 );

		//invalidate();	// 重画修改前的区域	//HCtrl::setText( txt )中有了

		m_rcClient.right = m_rcClient.left + rc.Width();	// 用户区，相对于父控件坐标
		m_rcClient.bottom = m_rcClient.top + rc.Height();
		
		m_rcWnd.right = m_rcWnd.left + rc.Width();			// 窗口区，相对于窗口坐标
		m_rcWnd.bottom = m_rcWnd.top + rc.Height();

		invalidate();	// 重画修后的区域
	}
}

// 按资源号取文本
void HLabel::setTextRes( UINT nID )
{
	CString str;
	str.LoadString( nID );
	setText( str );
}

// 窗口建立完毕后初始化
void HLabel::initCtrl()
{
	HCtrl::initCtrl();

	// 自动尺寸控件再设置一次文字，计算控件大小
	if (m_bAutoSize)
	{
		setText(m_strText);	// set size
		layerout();	// relayerout
	}
}



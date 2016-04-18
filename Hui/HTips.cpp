// HTips.cpp : 实现文件
//
// 悬停提示

#include "stdafx.h"
#include "Hui.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// HTips
IMPLEMENT_DYNAMIC(HTips, HCtrl)

HTips::HTips()
: HCtrl(CRect(0,0,0,0))
{
	// 默认背景样式
	setBackColor( 0xddeeee );	// 浅黄色底
	setBorderColor( 0x444444 );	// 深灰色边框
	setTextColor( 0x444444 );	// 深灰色文字
	setFont( 20, 0, 0);	// 文字大小
	m_nAlign = DT_CENTER | DT_VCENTER | DT_SINGLELINE;	// 单行，水平垂直剧中
}

HTips::~HTips()
{
}

// 画文字, int x, int y 左上角坐标, PCTSTR txt 要显示的文字
void HTips::drawText(int x, int y, PCTSTR txt)
{
	CDC * pDC = m_pWnd->GetDC();

	// 字体和颜色
	CFont * pOldfont = 0;
	CFont * pfont = getFont();
	if( pfont )
		pOldfont = pDC->SelectObject( pfont );
	COLORREF oldColor = pDC->SetTextColor(m_coText);	// 文字颜色

	// 根据文字计算大小
	if( pDC->DrawText( txt, m_rcWnd, DT_CALCRECT ) )	// 能计算尺寸则显示
	{
		int helfwidth = m_rcWnd.Width() / 2;
		m_rcWnd.right += x - helfwidth + 8;	// 留10mm边框
		m_rcWnd.bottom += y + 3;
		m_rcWnd.left = x - helfwidth;
		m_rcWnd.top = y;

		// 把显示区移到屏幕内
		CRect rc;
		m_pWnd->GetClientRect( rc );

		if( m_rcWnd.Width() > rc.Width() )	// 比屏幕宽，缩小到屏幕
		{
			m_rcWnd.left = 5;
			m_rcWnd.right = rc.Width() - 5;
		}
		else	// 否则移到屏幕内
		{
			int dx = rc.left < m_rcWnd.left ? rc.right < m_rcWnd.right ? rc.right - m_rcWnd.right : 0 : rc.left - m_rcWnd.left;
			int dy = m_rcWnd.bottom > rc.bottom ? -28 - m_rcWnd.Height() : 0;
			if( dx != 0 || dy != 0 )
				m_rcWnd.OffsetRect( dx, dy );
		}

		// 画背景
		HCtrl::draw( pDC, m_rcWnd );	// 画背景

		// 画文字
		pDC->SetBkMode(TRANSPARENT);
		pDC->DrawText(txt, m_rcWnd, m_nAlign);
	}
	pDC->SetTextColor(oldColor);

	if( pOldfont )
		pDC->SelectObject( pOldfont );

	m_pWnd->ReleaseDC( pDC );
}

void HTips::hidden()
{
	if( !isNull() )
	{
		invalidate();
		setNull();
	}

}
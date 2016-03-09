// HBtn.cpp : 实现文件
// 按钮控件，能设置图形和文字

#include "stdafx.h"
#include "Hui.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// HBtn
IMPLEMENT_DYNAMIC(HBtn, HCtrl)


HBtn::HBtn(LPCRECT prc, int idoverflow, int idup, int iddown, int nID)
: HCtrl(prc)
{
	m_lpszCursor = IDC_HAND;	//按钮默认手形鼠标
	m_nImgup = idup;
	m_nImgdown = iddown;
	m_nImgover = idoverflow;
	m_nID = nID;

	m_nAlign = DT_CENTER | DT_VCENTER | DT_SINGLELINE;

	setTextColor( 0xffffff );
}

HBtn::~HBtn()
{
}


// HBtn 成员函数

void HBtn::mouseEntered( UINT nFlags, const CPoint & point )
{
	HCtrl::mouseEntered( nFlags, point );
	invalidate();
}

void HBtn::mouseLeave()
{
	HCtrl::mouseLeave();
	invalidate();
}

void HBtn::draw( CDC* pDC, CRect & /*rcClip*/ )
{
	//HCtrl::draw( pDC, rcClip );	// 不用重画背景

	int nid = !(m_nCheck || (m_pctrCapture == this)) ? (m_pctrMouseEnter == this) ? m_nImgover : m_nImgup : m_nImgdown;
	
	CMFCToolBarImages * pi = getImglist();
	if( pi )
		pi->DrawEx( pDC, rcToWnd(), nid );

	pDC->SetBkMode(TRANSPARENT);

	CFont * pOldfont = 0;
	CFont * pfont = getFont();
	if( pfont )
		pOldfont = pDC->SelectObject( pfont );

	COLORREF oldColor = pDC->SetTextColor(m_bEnabled ? m_coText : m_coDisabled);
	pDC->DrawText(m_strText, m_rcWnd, m_nAlign);
	pDC->SetTextColor(oldColor);

	if( pOldfont )
		pDC->SelectObject( pOldfont );
}

void HBtn::lbuttonUp( UINT nFlags, const CPoint & point )
{
	HCtrl::lbuttonUp( nFlags, point );

	//if( m_pctrMouseEnter == this )	// 鼠标压下后这个条件总成立，改为直接判断
	if( m_rcWnd.PtInRect( point ) )		// 不在按钮内不发送命令
	{
		m_pWnd->PostMessage( WM_COMMAND, this->m_nID, 0);
	}
	invalidate();
}

void HBtn::lbuttonDown( UINT nFlags, const CPoint & point )
{
	HCtrl::lbuttonDown( nFlags, point );

	invalidate();
}

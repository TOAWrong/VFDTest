// HWndEdit.cpp : 实现文件
//
// 编辑控件，内嵌CEdit

#include "stdafx.h"
#include "Hui.h"
#include "HWndEdit.h"


// HWndEdit

IMPLEMENT_DYNAMIC(HWndEdit, CEdit)

BEGIN_MESSAGE_MAP(HWndEdit, CEdit)
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSELEAVE()
	ON_WM_MOUSEHOVER()
END_MESSAGE_MAP()

HWndEdit::HWndEdit( HEdit * pEdit )
	: m_bMouseTracking( FALSE )
{
	m_pctrEdit = pEdit;
}

HWndEdit::~HWndEdit()
{
}


// HWndEdit 消息处理程序

void HWndEdit::OnMouseMove(UINT nFlags, CPoint point)
{
	// 激活鼠标悬停和离开消息
	if (!m_bMouseTracking)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(TRACKMOUSEEVENT);
		tme.dwFlags = TME_LEAVE | TME_HOVER;//要触发的消息
		tme.hwndTrack = this->m_hWnd;
		tme.dwHoverTime = HOVERTIME;	//HOVER_DEFAULT;// 若不设此参数，则无法触发mouseHover

		if (::_TrackMouseEvent(&tme)) //MOUSELEAVE|MOUSEHOVER消息由此函数触发
		{
			m_bMouseTracking = TRUE;   
		}
	}


	((HTips*)HCtrl::m_pctrTips)->hidden();

	CWnd::OnMouseMove(nFlags, point);
}

void HWndEdit::OnMouseHover(UINT nFlags, CPoint point)
{
	m_bMouseTracking = FALSE;

	m_pctrEdit->mouseHover( nFlags, clientToMain(point) );
}

void HWndEdit::OnMouseLeave()
{
	m_bMouseTracking = FALSE;
	m_pctrEdit->mouseLeave();
}

// 把编辑窗口的点转换到主窗口
CPoint HWndEdit::clientToMain( const CPoint & point )
{
	CPoint pt = point;
	ClientToScreen( &pt );
	m_pctrEdit->m_pWnd->ScreenToClient( &pt );

	return pt;
}





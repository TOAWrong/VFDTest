// HWndEdit.cpp : ʵ���ļ�
//
// �༭�ؼ�����ǶCEdit

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


// HWndEdit ��Ϣ�������

void HWndEdit::OnMouseMove(UINT nFlags, CPoint point)
{
	// ���������ͣ���뿪��Ϣ
	if (!m_bMouseTracking)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(TRACKMOUSEEVENT);
		tme.dwFlags = TME_LEAVE | TME_HOVER;//Ҫ��������Ϣ
		tme.hwndTrack = this->m_hWnd;
		tme.dwHoverTime = HOVERTIME;	//HOVER_DEFAULT;// ������˲��������޷�����mouseHover

		if (::_TrackMouseEvent(&tme)) //MOUSELEAVE|MOUSEHOVER��Ϣ�ɴ˺�������
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

// �ѱ༭���ڵĵ�ת����������
CPoint HWndEdit::clientToMain( const CPoint & point )
{
	CPoint pt = point;
	ClientToScreen( &pt );
	m_pctrEdit->m_pWnd->ScreenToClient( &pt );

	return pt;
}





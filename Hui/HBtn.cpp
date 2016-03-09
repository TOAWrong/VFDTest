// HBtn.cpp : ʵ���ļ�
// ��ť�ؼ���������ͼ�κ�����

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
	m_lpszCursor = IDC_HAND;	//��ťĬ���������
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


// HBtn ��Ա����

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
	//HCtrl::draw( pDC, rcClip );	// �����ػ�����

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

	//if( m_pctrMouseEnter == this )	// ���ѹ�º���������ܳ�������Ϊֱ���ж�
	if( m_rcWnd.PtInRect( point ) )		// ���ڰ�ť�ڲ���������
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

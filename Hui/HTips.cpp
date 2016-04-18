// HTips.cpp : ʵ���ļ�
//
// ��ͣ��ʾ

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
	// Ĭ�ϱ�����ʽ
	setBackColor( 0xddeeee );	// ǳ��ɫ��
	setBorderColor( 0x444444 );	// ���ɫ�߿�
	setTextColor( 0x444444 );	// ���ɫ����
	setFont( 20, 0, 0);	// ���ִ�С
	m_nAlign = DT_CENTER | DT_VCENTER | DT_SINGLELINE;	// ���У�ˮƽ��ֱ����
}

HTips::~HTips()
{
}

// ������, int x, int y ���Ͻ�����, PCTSTR txt Ҫ��ʾ������
void HTips::drawText(int x, int y, PCTSTR txt)
{
	CDC * pDC = m_pWnd->GetDC();

	// �������ɫ
	CFont * pOldfont = 0;
	CFont * pfont = getFont();
	if( pfont )
		pOldfont = pDC->SelectObject( pfont );
	COLORREF oldColor = pDC->SetTextColor(m_coText);	// ������ɫ

	// �������ּ����С
	if( pDC->DrawText( txt, m_rcWnd, DT_CALCRECT ) )	// �ܼ���ߴ�����ʾ
	{
		int helfwidth = m_rcWnd.Width() / 2;
		m_rcWnd.right += x - helfwidth + 8;	// ��10mm�߿�
		m_rcWnd.bottom += y + 3;
		m_rcWnd.left = x - helfwidth;
		m_rcWnd.top = y;

		// ����ʾ���Ƶ���Ļ��
		CRect rc;
		m_pWnd->GetClientRect( rc );

		if( m_rcWnd.Width() > rc.Width() )	// ����Ļ����С����Ļ
		{
			m_rcWnd.left = 5;
			m_rcWnd.right = rc.Width() - 5;
		}
		else	// �����Ƶ���Ļ��
		{
			int dx = rc.left < m_rcWnd.left ? rc.right < m_rcWnd.right ? rc.right - m_rcWnd.right : 0 : rc.left - m_rcWnd.left;
			int dy = m_rcWnd.bottom > rc.bottom ? -28 - m_rcWnd.Height() : 0;
			if( dx != 0 || dy != 0 )
				m_rcWnd.OffsetRect( dx, dy );
		}

		// ������
		HCtrl::draw( pDC, m_rcWnd );	// ������

		// ������
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
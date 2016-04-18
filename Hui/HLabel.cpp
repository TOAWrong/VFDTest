// HLabel.cpp : ʵ���ļ�
//
// ���ֿؼ�

#include "stdafx.h"
#include "Hui.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// HLabel
IMPLEMENT_DYNAMIC(HLabel, HCtrl)

HLabel::HLabel(LPCRECT prc)
: HCtrl(prc)
, m_bAutoSize( FALSE )	// Ĭ�ϲ��Զ������С
{
	m_nID = -1;	// Ĭ�ϲ����������Ϣ
	m_bShadow = FALSE;	// Ĭ�ϲ���ʾ��Ӱ
}

HLabel::~HLabel()
{
}

// HLabel ��Ա����

void HLabel::draw( CDC* pDC, CRect & rcClip )
{
	HCtrl::draw( pDC, rcClip );

	if( !m_strText.IsEmpty() )
		drawText( pDC );
}

// ����Ӱ����
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
		COLORREF oldColor = pDC->SetTextColor(m_coShadow);	// �Ȼ���Ӱɫ����TTMView�е�ͬ�� Color color3(0x00,0x00,0x00);	// ��Ӱɫ
		pDC->DrawText(m_strText, rc, m_nAlign);
		rc.OffsetRect( -1, -1 );
		pDC->SetTextColor(m_coText);	// �ٻ�ǰ��������ɫ�� ��TTMView�е�ͬ�� Color color1(0x88,0x88,0x88);	// �ǻ����ɫ
		pDC->DrawText(m_strText, rc, m_nAlign);
		pDC->SetTextColor(oldColor);
	}
	else
	{
		COLORREF oldColor = pDC->SetTextColor(m_coText);	// ������ɫ
		pDC->DrawText(m_strText, rc, m_nAlign);
		pDC->SetTextColor(oldColor);
	}

	if( pOldfont )
		pDC->SelectObject( pOldfont );
}

// ������Ӱ��ɫ
void HLabel::setShadowColor( COLORREF color )
{
	m_bShadow = TRUE;
	m_coShadow = color;
}

// ��д�����ı��������С��
void HLabel::setText(CString txt)
{
	HCtrl::setText( txt );

	if( !m_bAutoSize )
		return;

	CRect rc;
	if( getTextSize( rc ) )
	{
		if( m_bShadow )	// �������Ӱ����Χ�Ӵ�һ������
			rc.DeflateRect( -1, -1 );

		//invalidate();	// �ػ��޸�ǰ������	//HCtrl::setText( txt )������

		m_rcClient.right = m_rcClient.left + rc.Width();	// �û���������ڸ��ؼ�����
		m_rcClient.bottom = m_rcClient.top + rc.Height();
		
		m_rcWnd.right = m_rcWnd.left + rc.Width();			// ������������ڴ�������
		m_rcWnd.bottom = m_rcWnd.top + rc.Height();

		invalidate();	// �ػ��޺������
	}
}

// ����Դ��ȡ�ı�
void HLabel::setTextRes( UINT nID )
{
	CString str;
	str.LoadString( nID );
	setText( str );
}

// ���ڽ�����Ϻ��ʼ��
void HLabel::initCtrl()
{
	HCtrl::initCtrl();

	// �Զ��ߴ�ؼ�������һ�����֣�����ؼ���С
	if (m_bAutoSize)
	{
		setText(m_strText);	// set size
		layerout();	// relayerout
	}
}



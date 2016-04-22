// ImgView.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "VFDTest.h"
#include "ImgView.h"
//#include "HComm.h"

// ImgView

IMPLEMENT_DYNAMIC(ImgView, HCtrl)

BEGIN_MESSAGE_MAP(ImgView, HCtrl)
	ON_COMMAND(TID_ABOUT, OnAbout)
END_MESSAGE_MAP()

ImgView * ImgView::m_pJoinView = 0;		// �ͼ�񴰿�
ImgView::ImgView(LPCRECT prc, int ID ) : HCtrl(prc)
{
	setID( ID );
	m_nPos = 0;
	CString str;
	str.Format(_T("TYLH%04i"),ID);
	// ���Ͻ���ʾ��
	m_pPrompt = new HLabel(CRect( 8, 8, 300, 8+20 ));
	addCtrl( m_pPrompt );
	m_pPrompt->setText( _T("Waiting") ); 
	m_pPrompt->setFont( 18, 0, _T("����") );
	m_pPrompt->setTextColor( 0x000000 );

	//�м���ʾ��ֵ
	int yy = ( m_rcClient.bottom - prc->top ) / 2 - 10;
	int xx = prc->right - prc->left - 10;
	m_pText = new HLabel( CRect( 5, yy, xx , 20));
	m_pText->setAlign( DT_CENTER );
	m_pText->setText( _T("_._ _") );
	m_pText->setFont(30,0,_T("��Բ"));
	m_pText->setTextColor( 0x000000 );
	addCtrl( m_pText );

	// ���Ͻ�������ʱ��
	m_pTime = new HLabel( CRect() );
	m_pTime->setText( _T("_: _: _") );
	m_pTime->setFont( 18, 0, _T("����") );
	m_pTime->setTextColor( 0x000000 );
	m_pTime->setAlign( DT_RIGHT );
	addCtrl( m_pTime );

	// ���½ǹ�λ��
	m_pNo = new HLabel(CRect( 8, prc->bottom - 20, prc->right, 28 ));
	m_pNo->setFont( 18, 0, _T("����") );
	m_pNo->setTextColor( 0x000000 );
	addCtrl( m_pNo );
	m_pNo->setText( str );
}

// �����ؼ�
ImgView::ImgView(LPCRECT prc) : HCtrl(prc)
{

	m_pPosBtn = 0;

	// ���Ͻ���ʾ��
	m_pPrompt = new HLabel(CRect( 8, 8, 300, 8+20 ));
	addCtrl( m_pPrompt );
	m_pPrompt->setText( _T("Waiting") );
}

ImgView::~ImgView()
{
}

// ���û���
void ImgView::draw( CDC* pDC, CRect & rcClip )
{
	if( Intersect( rcClip ) )// ���ص������ػ���
	{
		CPen penBlack;
//		penBlack.CreatePen(PS_SOLID, 1, RGB(60, 55, 40));// �߿�ɫ�� RGB(60, 55, 40)
		penBlack.CreatePen(PS_SOLID, 1, 0xffbf00);
		CPen* pOldPen = pDC->SelectObject(&penBlack);
	
		pDC->MoveTo( m_rcWnd.left, m_rcWnd.top );
		pDC->LineTo( m_rcWnd.right-1, m_rcWnd.top );
		pDC->LineTo( m_rcWnd.right-1, m_rcWnd.bottom-1 );
		pDC->LineTo( m_rcWnd.left, m_rcWnd.bottom-1 );
		pDC->LineTo( m_rcWnd.left, m_rcWnd.top );

		pDC->SelectObject(pOldPen);

		HCtrl::draw( pDC, rcClip );
	}
}

// ����true��ʾ�Ѿ�����
void ImgView::OnLButtonDown( UINT nFlags, CPoint point )
{
	HCtrl::lbuttonDown( nFlags, point );	// �ȸ��ӿؼ����������ӿؼ��ͱ��ؼ��У�����false����һ�ؼ���������
}

//���㶥���ؼ�λ�á�
void ImgView::layerout()
{
	int yy = m_rcClient.Height();
	int xx = m_rcClient.Width();
	if (m_pPrompt)
	{
		m_pPrompt->setRect( CRect( 8, 8, xx/2, 8 + 25 ), false );
	}
	if (m_pTime)
	{
		m_pTime->setRect( CRect( xx / 2, 8, xx - 8, 8 + 25 ), false );
	}
	if ( m_pNo )
	{	
		m_pNo->setRect( CRect( 8, yy-24, xx, yy-4 ), false);
	}

	if ( m_pText )
	{
		m_pText->setRect( CRect( 2, yy / 2 - 15, xx-2, yy-2 ), false);
	}
	HCtrl::layerout();	
}

//�Զ���Ǵ������Զ�����ON_COMMAND�¼�
void ImgView::OnAbout()
{
	theApp.OnAppAbout();
}

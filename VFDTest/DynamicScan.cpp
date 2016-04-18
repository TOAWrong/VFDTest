// DynamicScan.cpp : ʵ���ļ�
// ��̬ɨ��ؼ�

#include "stdafx.h"
#include "VFDTest.h"
#include "DynamicScan.h"


// DynamicScan
IMPLEMENT_DYNAMIC(DynamicScan, HCtrl)

BEGIN_MESSAGE_MAP(DynamicScan, HCtrl)
END_MESSAGE_MAP()

DynamicScan::DynamicScan(LPCRECT prc) : HCtrl(prc)
{
	// ��Ϣ��ʾ��
	// ���Ͻ���ʾ��
	m_pPrompt = new HLabel(CRect( 8, 8, 300, 8+20 ));
	addCtrl( m_pPrompt );
	m_pPrompt->setText( _T("״̬") ); 
	m_pPrompt->setFont( 22, 0, _T("΢���ź�") );
	m_pPrompt->setTextColor( 0x000000 );

	// ���Ͻ���ʾ��
	m_pTime = new HLabel(CRect());
	m_pTime->setText( _T("������ʱ��(h:m:s)") ); 
	m_pTime->setAlign( DT_RIGHT );
	m_pTime->setFont( 22, 0, _T("΢���ź�") );
	m_pTime->setTextColor( 0x000000 );
	addCtrl( m_pTime );

	//�м���ʾ����
	int yy = ( m_rcClient.bottom - prc->top ) / 2 - 10;
	int xx = prc->right - prc->left;
	m_pText = new HLabel( CRect( 0, yy, xx , 20));
	m_pText->setAlign( DT_CENTER );
	m_pText->setText( _T("����I(A)") );
	m_pText->setFont(30,0,_T("TIMES NEW ROMAN"));
	m_pText->setTextColor( 0x000000 );
	addCtrl( m_pText );

	// ���½���Ϣ
	m_pNo = new HLabel(CRect( 8, prc->bottom - 20, prc->right, 28 ));
	addCtrl( m_pNo );
	m_pNo->setText( _T("��λ��") );
	m_pNo->setTextColor( 0x000000 );
}

DynamicScan::~DynamicScan()
{
}

// ɨ�裬���ڽ�����Ϻ��ʼ����
void DynamicScan::initCtrl()
{
	// TODO ��ʼ���ؼ�

	HCtrl::initCtrl();
}


// ���û���
void DynamicScan::draw( CDC* pDC, CRect & rcClip )
{
	// TODO ���ؼ�
	pDC->FillSolidRect( m_rcWnd, 0xffffff );

	HCtrl::draw( pDC, rcClip );
	
}
//���㶥���ؼ�λ�á�
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
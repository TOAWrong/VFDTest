// ImgView.cpp : 实现文件
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

ImgView * ImgView::m_pJoinView = 0;		// 活动图像窗口
ImgView::ImgView(LPCRECT prc, int ID ) : HCtrl(prc)
{
	setID( ID );
	m_nPos = 0;
	CString str;
	str.Format(_T("TYLH%04i"),ID);
	// 左上角提示框
	m_pPrompt = new HLabel(CRect( 8, 8, 300, 8+20 ));
	addCtrl( m_pPrompt );
	m_pPrompt->setText( _T("Waiting") ); 
	m_pPrompt->setFont( 18, 0, _T("楷体") );
	m_pPrompt->setTextColor( 0x000000 );

	//中间显示数值
	int yy = ( m_rcClient.bottom - prc->top ) / 2 - 10;
	int xx = prc->right - prc->left - 10;
	m_pText = new HLabel( CRect( 5, yy, xx , 20));
	m_pText->setAlign( DT_CENTER );
	m_pText->setText( _T("_._ _") );
	m_pText->setFont(30,0,_T("幼圆"));
	m_pText->setTextColor( 0x000000 );
	addCtrl( m_pText );

	// 右上角已运行时间
	m_pTime = new HLabel( CRect() );
	m_pTime->setText( _T("_: _: _") );
	m_pTime->setFont( 18, 0, _T("楷体") );
	m_pTime->setTextColor( 0x000000 );
	m_pTime->setAlign( DT_RIGHT );
	addCtrl( m_pTime );

	// 左下角工位号
	m_pNo = new HLabel(CRect( 8, prc->bottom - 20, prc->right, 28 ));
	m_pNo->setFont( 18, 0, _T("楷体") );
	m_pNo->setTextColor( 0x000000 );
	addCtrl( m_pNo );
	m_pNo->setText( str );
}

// 单幅控件
ImgView::ImgView(LPCRECT prc) : HCtrl(prc)
{

	m_pPosBtn = 0;

	// 左上角提示框
	m_pPrompt = new HLabel(CRect( 8, 8, 300, 8+20 ));
	addCtrl( m_pPrompt );
	m_pPrompt->setText( _T("Waiting") );
}

ImgView::~ImgView()
{
}

// 画用户区
void ImgView::draw( CDC* pDC, CRect & rcClip )
{
	if( Intersect( rcClip ) )// 有重叠区才重画。
	{
		CPen penBlack;
//		penBlack.CreatePen(PS_SOLID, 1, RGB(60, 55, 40));// 边框色彩 RGB(60, 55, 40)
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

// 返回true表示已经处理。
void ImgView::OnLButtonDown( UINT nFlags, CPoint point )
{
	HCtrl::lbuttonDown( nFlags, point );	// 先给子控件处理，不在子控件和本控件中，返回false，下一控件继续处理
}

//计算顶级控件位置。
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

//自定义非窗口类自动处理ON_COMMAND事件
void ImgView::OnAbout()
{
	theApp.OnAppAbout();
}

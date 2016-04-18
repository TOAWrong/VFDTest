// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

#include "stdafx.h"
#include <afxpriv.h>	//WM_IDLEUPDATECMDUI
#include "Hui.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// HWnd : Main window

//IMPLEMENT_DYNAMIC(HWnd, CWnd)
IMPLEMENT_DYNCREATE(HWnd, CWnd)

BEGIN_MESSAGE_MAP(HWnd, CWnd)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_SETCURSOR()
	ON_WM_NCMOUSEMOVE()
	ON_WM_MOUSELEAVE()
	ON_WM_MOUSEHOVER()
	ON_MESSAGE(WM_IDLEUPDATECMDUI, OnIdleUpdateCmdUI)	// WM_IDLEUPDATECMDUI 类视图中没有这个消息
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()

HWnd::HWnd() : m_ctrFrame()
{
	m_hAccel = 0;
	m_bMouseTracking = FALSE;
	HCtrl::m_pWnd = this;
	HCtrl::m_pctrTips = new HTips();
}

HWnd::~HWnd()
{
	delete HCtrl::m_pctrTips;
}

void HWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// 不为绘图消息调用 CWnd::OnPaint()

	// 双缓冲绘图http://blog.163.com/yuanlong_zheng@126/blog/static/49846449201033093333394/

	// 客户区
	CRect rect;
	GetClientRect(&rect);

	// 取重画区
	CRect rcClip;
	dc.GetClipBox( rcClip );	//?? 有没有更好的方法，取重多边形画区
	//if( rcClip.IsRectNull() )
	//	rcClip = rect;

	if( HCtrl::m_cTestColor )
		m_ctrFrame.setTestColor();// 强制显示控件背景色，用于判断重画区域是否合理高效，测试用，为0时不显示。

	CDC dcMem;		//用于缓冲作图的内存DC
	CBitmap bmp;	//内存中承载临时图象的位图
	dcMem.CreateCompatibleDC(&dc);	//依附窗口DC创建兼容内存DC
	//bmp.CreateCompatibleBitmap(&dcMem,rect.Width(),rect.Height());//创建兼容位图，用&dcMem只显示黑白的图像
	bmp.CreateCompatibleBitmap(&dc,rect.Width(),rect.Height());//创建兼容位图，用&dcMem只显示黑白的图像
	dcMem.SelectObject(&bmp);	//将位图选择进内存DC

	m_ctrFrame.preDraw( &dcMem, rcClip );	// 画控件

	//if( !((HTips*)HCtrl::m_pctrTips)->isNull() )	// 画悬停提示
	//	((HTips*)HCtrl::m_pctrTips)->draw(&dcMem, rcClip);


	dc.BitBlt(rcClip.left, rcClip.top, rcClip.Width(),rcClip.Height(), &dcMem, rcClip.left, rcClip.top, SRCCOPY);//将内存DC上的图象拷贝到前台
	dcMem.DeleteDC();	//删除DC
	bmp.DeleteObject();	//删除位图
}

void HWnd::PostNcDestroy()
{
	// 销毁窗口
	CWnd::PostNcDestroy();

	delete this;	// 释放主窗口
}

void HWnd::OnSize(UINT nType, int cx, int cy)
{
	//?? 如果有WS_MAXIMIZE样式会2-3次执行onsize事件
	//if( (GetStyle() & WS_MAXIMIZE) && (nType != SIZE_MAXIMIZED ) )
	//	return;

	CWnd::OnSize(nType, cx, cy);

	CRect rc;
	GetClientRect( rc );	
	rc.left *= DEFAULTDPI / HCtrl::m_nDpiX;
	rc.right *= DEFAULTDPI / HCtrl::m_nDpiX;
	rc.top *= DEFAULTDPI / HCtrl::m_nDpiY;
	rc.bottom *= DEFAULTDPI / HCtrl::m_nDpiY;
	m_ctrFrame.setRect( rc, TRUE );	// 主控件充满全部窗口，执行layerout。*/

	m_ctrFrame.layerout();
}

void HWnd::OnMouseMove(UINT nFlags, CPoint point)
{
	//#pragma message("********测试有没有相同点，有没有问题")  
	//if( m_ptPrevious == point )	// 同一个点则不处理，提高效率，?? 系统有时也发同一点的消息，会不会有问题
	//	return;		// 20140328 系统不会发同一点，测试中是调试状态下，vs和被调窗口相互切时产生的mousemove事件

	//m_ptPrevious = point;

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


	// 隐藏悬停提示框
	((HTips*)HCtrl::m_pctrTips)->hidden();

	if( HCtrl::m_pctrCapture )
	{
		HCtrl::m_pctrCapture->mouseMove( nFlags, point );
		return;
	}
	else	// 通知所有子控件
		if( m_ctrFrame.OnMouseMove(nFlags, point) )
			return;

	CWnd::OnMouseMove(nFlags, point);
}


void HWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	SetCapture();

	// 隐藏悬停提示框
	((HTips*)HCtrl::m_pctrTips)->hidden();

	if( HCtrl::m_pctrMouseEnter )
		HCtrl::m_pctrMouseEnter->lbuttonDown( nFlags, point );

	CWnd::OnLButtonDown(nFlags, point);
}


void HWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
	//ASSERT( HCtrl::m_pctrCapture );	// 鼠标双击时，第二个up，m_pctrCapture=0

	// 给锁定子控件处理
	if( HCtrl::m_pctrCapture )
	{
		HCtrl::m_pctrCapture->lbuttonUp(nFlags, point);
		HCtrl::m_pctrCapture = 0;
		ReleaseCapture();
	}

	CWnd::OnLButtonUp(nFlags, point);
}

// 向控件传递命令
BOOL HWnd::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// 先到Application
	if( AfxGetApp()->OnCmdMsg( nID, nCode, pExtra, pHandlerInfo ) )
		return TRUE;

	// 让自定义类第一次尝试该命令，不加这一句，收不到命令
	if( m_ctrFrame.OnCmdMsg( nID, nCode, pExtra, pHandlerInfo) )
		return TRUE;

	 // 否则，执行默认处理
	return CWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}


BOOL HWnd::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	if( m_ctrFrame.OnNotify(wParam, lParam, pResult) )
		return TRUE;

	return CWnd::OnNotify(wParam, lParam, pResult);
}

BOOL HWnd::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	if( HCtrl::m_pszCursor )
	{
		//HCURSOR hCursor = AfxGetApp()->LoadCursor(HCtrl::m_pszCursor);	// 不成功
		HCURSOR hCursor = LoadCursor(NULL, HCtrl::m_pszCursor);	//IDC_HAND是你在资源里画的那只“手”的光标。 
		::SetCursor(hCursor);
		return TRUE;
	}

	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}


BOOL HWnd::CreateEx(DWORD dwExStyle, LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, LPVOID lpParam)
{
	if( !CWnd::CreateEx(dwExStyle, lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, lpParam) )
		return FALSE;

	m_ctrFrame.initCtrl();		// 窗口建立后，显示之前，初始化所有子控件。

	return TRUE;
}

// 鼠标离开
void HWnd::OnMouseLeave()
{
	m_bMouseTracking = FALSE;

	// 隐藏悬停提示框
	((HTips*)HCtrl::m_pctrTips)->hidden();

	if( HCtrl::m_pctrMouseEnter )
		HCtrl::m_pctrMouseEnter->mouseLeave();	// 鼠标离开窗口时假设鼠标在最大绝对值负数处

	CWnd::OnMouseLeave();
}

// 鼠标悬停
void HWnd::OnMouseHover(UINT nFlags, CPoint point)
{
	m_bMouseTracking = FALSE;
	if( !HCtrl::m_pctrCapture && HCtrl::m_pctrMouseEnter )	// 没有锁定鼠标并且在某控件内，执行悬停。
		HCtrl::m_pctrMouseEnter->mouseHover( nFlags, point );	// 指向悬停控件

	CWnd::OnMouseHover(nFlags, point);
}

// 激活鼠标悬停离开事件，每次悬停离开都要重新激活
//void HWnd::trackMouseEvent(DWORD dwFlags)
//{
//	TRACKMOUSEEVENT tme;   
//	tme.cbSize		= sizeof(TRACKMOUSEEVENT);   
//	tme.dwFlags		= dwFlags;   
//	tme.hwndTrack	= m_hWnd;
//	tme.dwHoverTime =  HOVERTIME;
//	_TrackMouseEvent(&tme);
//}

// 载入快捷键资源
void HWnd::setAccelerators( UINT id )
{
	// 载入快捷键资源http://blog.163.com/lvlijuan001@126/blog/static/77997198201110147301101/
	m_hAccel = ::LoadAccelerators(AfxGetInstanceHandle(),MAKEINTRESOURCE(id));
}


BOOL HWnd::PreTranslateMessage(MSG* pMsg)
{
	// 处理快捷键事件http://blog.163.com/lvlijuan001@126/blog/static/77997198201110147301101/
	if( m_hAccel )
	{
		if (TranslateAccelerator(m_hWnd, m_hAccel, pMsg))   
			return TRUE;
	}

	m_idle.beforeUpdate( pMsg->message );

	return CWnd::PreTranslateMessage(pMsg);
}

BOOL HWnd::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// 所有消息先给子控件
	//if( m_ctrFrame.OnWndMsg(message, wParam, lParam, pResult) )
	//	return TRUE;

	// ?? 只给鼠标控件
	if( HCtrl::m_pctrCapture && HCtrl::m_pctrCapture->OnWndMsg(message, wParam, lParam, pResult) )
		return TRUE;

	// 再执行默认处理
	return CWnd::OnWndMsg(message, wParam, lParam, pResult);
}

// 用户控件状态更新消息 
// LRESULT CBasePane::OnIdleUpdateCmdUI(WPARAM wParam, LPARAM) @ afxbasepane.cpp
// void CMFCRibbonBar::OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler) @ afxribbonbar.cpp
// 网页http://blog.csdn.net/ltag0110rtag/article/details/7524289
LRESULT HWnd::OnIdleUpdateCmdUI(WPARAM wParam, LPARAM)
{
	// 不可见，不更新
	if (!(GetStyle() & WS_VISIBLE))
		return 0L;

	// 不允许，不更新
	if( !m_idle.canUpdate() )
		return 0L;

	BOOL bDisableIfNoHndler = (BOOL)wParam;

	HCmdUI state;
	state.m_pOther = this;

	m_ctrFrame.OnUpdateCmdUI( &state, bDisableIfNoHndler );

	// 更新完毕
	m_idle.endUpdate();

	//
	//TRACE( COleDateTime::GetCurrentTime().Format( L"****************************%Y%m%d %H%M%S\r\n" ) );


	return 0L;
}

// 鼠标左键双击事件，窗口类注册时加入wndcls.style = CS_DBLCLKS; 双击中第二个次鼠标按下没有down事件，产生双击事件。
void HWnd::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	if( HCtrl::m_pctrMouseEnter )
		HCtrl::m_pctrMouseEnter->lbuttonDblClk( nFlags, point );

	CWnd::OnLButtonDblClk(nFlags, point);
}



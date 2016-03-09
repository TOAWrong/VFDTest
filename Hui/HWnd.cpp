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
	ON_MESSAGE(WM_IDLEUPDATECMDUI, OnIdleUpdateCmdUI)	// WM_IDLEUPDATECMDUI ����ͼ��û�������Ϣ
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
	// ��Ϊ��ͼ��Ϣ���� CWnd::OnPaint()

	// ˫�����ͼhttp://blog.163.com/yuanlong_zheng@126/blog/static/49846449201033093333394/

	// �ͻ���
	CRect rect;
	GetClientRect(&rect);

	// ȡ�ػ���
	CRect rcClip;
	dc.GetClipBox( rcClip );	//?? ��û�и��õķ�����ȡ�ض���λ���
	//if( rcClip.IsRectNull() )
	//	rcClip = rect;

	if( HCtrl::m_cTestColor )
		m_ctrFrame.setTestColor();// ǿ����ʾ�ؼ�����ɫ�������ж��ػ������Ƿ�����Ч�������ã�Ϊ0ʱ����ʾ��

	CDC dcMem;		//���ڻ�����ͼ���ڴ�DC
	CBitmap bmp;	//�ڴ��г�����ʱͼ���λͼ
	dcMem.CreateCompatibleDC(&dc);	//��������DC���������ڴ�DC
	//bmp.CreateCompatibleBitmap(&dcMem,rect.Width(),rect.Height());//��������λͼ����&dcMemֻ��ʾ�ڰ׵�ͼ��
	bmp.CreateCompatibleBitmap(&dc,rect.Width(),rect.Height());//��������λͼ����&dcMemֻ��ʾ�ڰ׵�ͼ��
	dcMem.SelectObject(&bmp);	//��λͼѡ����ڴ�DC

	m_ctrFrame.preDraw( &dcMem, rcClip );	// ���ؼ�

	//if( !((HTips*)HCtrl::m_pctrTips)->isNull() )	// ����ͣ��ʾ
	//	((HTips*)HCtrl::m_pctrTips)->draw(&dcMem, rcClip);


	dc.BitBlt(rcClip.left, rcClip.top, rcClip.Width(),rcClip.Height(), &dcMem, rcClip.left, rcClip.top, SRCCOPY);//���ڴ�DC�ϵ�ͼ�󿽱���ǰ̨
	dcMem.DeleteDC();	//ɾ��DC
	bmp.DeleteObject();	//ɾ��λͼ
}

void HWnd::PostNcDestroy()
{
	// ���ٴ���
	CWnd::PostNcDestroy();

	delete this;	// �ͷ�������
}

void HWnd::OnSize(UINT nType, int cx, int cy)
{
	//?? �����WS_MAXIMIZE��ʽ��2-3��ִ��onsize�¼�
	//if( (GetStyle() & WS_MAXIMIZE) && (nType != SIZE_MAXIMIZED ) )
	//	return;

	CWnd::OnSize(nType, cx, cy);

	CRect rc;
	GetClientRect( rc );	
	rc.left *= DEFAULTDPI / HCtrl::m_nDpiX;
	rc.right *= DEFAULTDPI / HCtrl::m_nDpiX;
	rc.top *= DEFAULTDPI / HCtrl::m_nDpiY;
	rc.bottom *= DEFAULTDPI / HCtrl::m_nDpiY;
	m_ctrFrame.setRect( rc, TRUE );	// ���ؼ�����ȫ�����ڣ�ִ��layerout��*/

	m_ctrFrame.layerout();
}

void HWnd::OnMouseMove(UINT nFlags, CPoint point)
{
	//#pragma message("********������û����ͬ�㣬��û������")  
	//if( m_ptPrevious == point )	// ͬһ�����򲻴������Ч�ʣ�?? ϵͳ��ʱҲ��ͬһ�����Ϣ���᲻��������
	//	return;		// 20140328 ϵͳ���ᷢͬһ�㣬�������ǵ���״̬�£�vs�ͱ��������໥��ʱ������mousemove�¼�

	//m_ptPrevious = point;

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


	// ������ͣ��ʾ��
	((HTips*)HCtrl::m_pctrTips)->hidden();

	if( HCtrl::m_pctrCapture )
	{
		HCtrl::m_pctrCapture->mouseMove( nFlags, point );
		return;
	}
	else	// ֪ͨ�����ӿؼ�
		if( m_ctrFrame.OnMouseMove(nFlags, point) )
			return;

	CWnd::OnMouseMove(nFlags, point);
}


void HWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	SetCapture();

	// ������ͣ��ʾ��
	((HTips*)HCtrl::m_pctrTips)->hidden();

	if( HCtrl::m_pctrMouseEnter )
		HCtrl::m_pctrMouseEnter->lbuttonDown( nFlags, point );

	CWnd::OnLButtonDown(nFlags, point);
}


void HWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
	//ASSERT( HCtrl::m_pctrCapture );	// ���˫��ʱ���ڶ���up��m_pctrCapture=0

	// �������ӿؼ�����
	if( HCtrl::m_pctrCapture )
	{
		HCtrl::m_pctrCapture->lbuttonUp(nFlags, point);
		HCtrl::m_pctrCapture = 0;
		ReleaseCapture();
	}

	CWnd::OnLButtonUp(nFlags, point);
}

// ��ؼ���������
BOOL HWnd::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// �ȵ�Application
	if( AfxGetApp()->OnCmdMsg( nID, nCode, pExtra, pHandlerInfo ) )
		return TRUE;

	// ���Զ������һ�γ��Ը����������һ�䣬�ղ�������
	if( m_ctrFrame.OnCmdMsg( nID, nCode, pExtra, pHandlerInfo) )
		return TRUE;

	 // ����ִ��Ĭ�ϴ���
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
		//HCURSOR hCursor = AfxGetApp()->LoadCursor(HCtrl::m_pszCursor);	// ���ɹ�
		HCURSOR hCursor = LoadCursor(NULL, HCtrl::m_pszCursor);	//IDC_HAND��������Դ�ﻭ����ֻ���֡��Ĺ�ꡣ 
		::SetCursor(hCursor);
		return TRUE;
	}

	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}


BOOL HWnd::CreateEx(DWORD dwExStyle, LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, LPVOID lpParam)
{
	if( !CWnd::CreateEx(dwExStyle, lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, lpParam) )
		return FALSE;

	m_ctrFrame.initCtrl();		// ���ڽ�������ʾ֮ǰ����ʼ�������ӿؼ���

	return TRUE;
}

// ����뿪
void HWnd::OnMouseLeave()
{
	m_bMouseTracking = FALSE;

	// ������ͣ��ʾ��
	((HTips*)HCtrl::m_pctrTips)->hidden();

	if( HCtrl::m_pctrMouseEnter )
		HCtrl::m_pctrMouseEnter->mouseLeave();	// ����뿪����ʱ���������������ֵ������

	CWnd::OnMouseLeave();
}

// �����ͣ
void HWnd::OnMouseHover(UINT nFlags, CPoint point)
{
	m_bMouseTracking = FALSE;
	if( !HCtrl::m_pctrCapture && HCtrl::m_pctrMouseEnter )	// û��������겢����ĳ�ؼ��ڣ�ִ����ͣ��
		HCtrl::m_pctrMouseEnter->mouseHover( nFlags, point );	// ָ����ͣ�ؼ�

	CWnd::OnMouseHover(nFlags, point);
}

// ���������ͣ�뿪�¼���ÿ����ͣ�뿪��Ҫ���¼���
//void HWnd::trackMouseEvent(DWORD dwFlags)
//{
//	TRACKMOUSEEVENT tme;   
//	tme.cbSize		= sizeof(TRACKMOUSEEVENT);   
//	tme.dwFlags		= dwFlags;   
//	tme.hwndTrack	= m_hWnd;
//	tme.dwHoverTime =  HOVERTIME;
//	_TrackMouseEvent(&tme);
//}

// �����ݼ���Դ
void HWnd::setAccelerators( UINT id )
{
	// �����ݼ���Դhttp://blog.163.com/lvlijuan001@126/blog/static/77997198201110147301101/
	m_hAccel = ::LoadAccelerators(AfxGetInstanceHandle(),MAKEINTRESOURCE(id));
}


BOOL HWnd::PreTranslateMessage(MSG* pMsg)
{
	// �����ݼ��¼�http://blog.163.com/lvlijuan001@126/blog/static/77997198201110147301101/
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
	// ������Ϣ�ȸ��ӿؼ�
	//if( m_ctrFrame.OnWndMsg(message, wParam, lParam, pResult) )
	//	return TRUE;

	// ?? ֻ�����ؼ�
	if( HCtrl::m_pctrCapture && HCtrl::m_pctrCapture->OnWndMsg(message, wParam, lParam, pResult) )
		return TRUE;

	// ��ִ��Ĭ�ϴ���
	return CWnd::OnWndMsg(message, wParam, lParam, pResult);
}

// �û��ؼ�״̬������Ϣ 
// LRESULT CBasePane::OnIdleUpdateCmdUI(WPARAM wParam, LPARAM) @ afxbasepane.cpp
// void CMFCRibbonBar::OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler) @ afxribbonbar.cpp
// ��ҳhttp://blog.csdn.net/ltag0110rtag/article/details/7524289
LRESULT HWnd::OnIdleUpdateCmdUI(WPARAM wParam, LPARAM)
{
	// ���ɼ���������
	if (!(GetStyle() & WS_VISIBLE))
		return 0L;

	// ������������
	if( !m_idle.canUpdate() )
		return 0L;

	BOOL bDisableIfNoHndler = (BOOL)wParam;

	HCmdUI state;
	state.m_pOther = this;

	m_ctrFrame.OnUpdateCmdUI( &state, bDisableIfNoHndler );

	// �������
	m_idle.endUpdate();

	//
	//TRACE( COleDateTime::GetCurrentTime().Format( L"****************************%Y%m%d %H%M%S\r\n" ) );


	return 0L;
}

// ������˫���¼���������ע��ʱ����wndcls.style = CS_DBLCLKS; ˫���еڶ�������갴��û��down�¼�������˫���¼���
void HWnd::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	if( HCtrl::m_pctrMouseEnter )
		HCtrl::m_pctrMouseEnter->lbuttonDblClk( nFlags, point );

	CWnd::OnLButtonDblClk(nFlags, point);
}



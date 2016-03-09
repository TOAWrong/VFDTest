// HCtrl.cpp : 实现文件
// 控件基类

#include "stdafx.h"
#include "Hui.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// HCtrl
IMPLEMENT_DYNAMIC(HCtrl, CObject);

HCtrl * HCtrl::m_pctrCapture	= 0;
HCtrl * HCtrl::m_pctrTips		= 0;//new HTips(CRect(0, 0, 0, 0));
HCtrl *	HCtrl::m_pctrMouseEnter	= 0;
LPCTSTR HCtrl::m_pszCursor		= 0;
int		HCtrl::m_nDpiX			= DEFAULTDPI;	// 显示器X方向dpi
int		HCtrl::m_nDpiY			= DEFAULTDPI;	// 显示器Y方向dpi
CWnd *	HCtrl::m_pWnd			= 0;		// 控件所在的窗口。
COLORREF HCtrl::m_cTestColor	= 0;	// 初始化值为0，不显示测试背景。

// id < 0, 不接收鼠标消息

HCtrl::HCtrl()
{
	initParameter();
}

HCtrl::HCtrl(LPCRECT prc )
{
	m_rcClient = prc;

	initParameter();
}

// 公共初始化
void HCtrl::initParameter()
{
	setDock( HDOCK_NORMAL );

	m_nID = 0;			// id < 0, 不接收鼠标消息
	m_lpszCursor = 0;
	//m_bMouseEnter = FALSE;
	m_pImglist = 0;
	m_pParent = 0;

	m_nAlign = 0;
	m_coText = 0x00bbbbbb;

	m_strTips = L"";

	m_bEnabled = TRUE;	// 默认可用
	m_nCheck = 0;	// 默认未选中
	m_coDisabled = 0x888888;	// 默认禁止文本颜色

	//m_rcWnd = rcToWnd();			// 放到最后，因为rcToWnd()用到很多属性。 // 初始化时不能执行，因为窗口还没有建立，有放到layout里面。
}

HCtrl::~HCtrl()
{
	delete m_pImglist;
	
	removeAllChildren();
}

// 删除指定的子控件，i 控件索引号void HCtrl::removeAllChildren()
void HCtrl::removeChildrenAt( int i )
{
	ASSERT( i >= 0 && i <  m_arChildren.GetCount() );
	m_arChildren[i]->invalidate();
	delete m_arChildren[i];
	m_arChildren.RemoveAt(i);
}

// 删除全部子控件
void HCtrl::removeAllChildren()
{
	int n = m_arChildren.GetCount();
	if( !n )
		return;

	for( int i = 0; i < n; i++ )
	{
		m_arChildren[i]->invalidate();
		delete m_arChildren[i];
	}

	m_arChildren.RemoveAll();
}

// HCtrl 成员函数

// 返回TRUE表示已经处理。
BOOL HCtrl::OnMouseMove( UINT nFlags, const CPoint &point )
{
	int n = m_arChildren.GetCount();
	for( int i = 0; i < n; i++)		// 先查子控件
	{
		if( m_arChildren[i]->OnMouseMove( nFlags, point ) )
			return TRUE;	// 如果子控件处理，不再处理
	}

	if( !m_bEnabled || m_nID == -1 || !m_rcWnd.PtInRect( point ) )	// 如果禁止 或 m_nID==-1 或 鼠标不在控件中
		return FALSE;

	//#pragma message("********测试第一次启动有没有move事件") 有
	if( m_pctrMouseEnter != this )
	{
		if( m_pctrMouseEnter )
			m_pctrMouseEnter->mouseLeave();

		m_pctrMouseEnter = this;
		mouseEntered( nFlags, point );
	}

	mouseMove( nFlags, point );
	return TRUE;
}

BOOL HCtrl::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	int n = m_arChildren.GetCount();
	for( int i = 0; i < n; i++)		// 先查子控件
	{
		if( m_arChildren[i]->OnWndMsg(message, wParam, lParam, pResult) )
			return TRUE;	// 如果子控件处理，不再处理
	}

	return FALSE;
}


void HCtrl::addCtrl( HCtrl * pCtrl )	// 公开的添加方法。
{
	if( m_arChildren.IsEmpty() )
		m_arChildren.SetSize( 0, 20 );	// 第一次添加时预留20个元素，避免内存碎片
	//layerout();
	pCtrl->m_pParent = this;
	m_arChildren.Add( pCtrl );
	//TRACE( L"%x, %x\r\n", this, &m_arChildren[0] );

	pCtrl->added();
}

// 如果当前有m_font，取当前的，否则取父级的。
CFont * HCtrl::getFont()
{
	if( m_font.m_hObject )
		return &m_font;
	else if( m_pParent )
		return m_pParent->getFont();
	else
		return 0;
}

// 如果当前有m_pImglist，取当前的，否则取父级的。
CMFCToolBarImages * HCtrl::getImglist()
{
	if( m_pImglist )
		return m_pImglist;
	else if( m_pParent )
		return m_pParent->getImglist();
	else
		return 0;
}

// 调用父控件的rcToWnd，把当前rc转换成相对窗口的数据。
void HCtrl::rcToWnd( CRect &rc )
{
	rc.OffsetRect( m_rcClient.TopLeft() );
	if( m_pParent )
		m_pParent->rcToWnd( rc );
	else
	{
		// 磅转换成象素
		rc.left *= m_nDpiX / DEFAULTDPI;
		rc.right *= m_nDpiX / DEFAULTDPI;
		rc.top *= m_nDpiY / DEFAULTDPI;
		rc.bottom *= m_nDpiY / DEFAULTDPI;
	}
}

// 调用父控件的rcToWnd，把当前rc转换成相对窗口的数据。
CRect HCtrl::rcToWnd()
{
	CRect rc = m_rcClient;
	if( m_pParent )
		m_pParent->rcToWnd( rc );
	return rc;
}

// 调用父控件的rcToClientg，把当前的Wnd区域转换到UI坐标, 原点左上角，单位磅
void HCtrl::rcToClient( CRect &rc )
{
	rc.OffsetRect( -m_rcClient.left, -m_rcClient.top );
	if( m_pParent )
		m_pParent->rcToClient( rc );
	else
	{
		// 象素转换成磅
		rc.left *= DEFAULTDPI / m_nDpiX;
		rc.right *= DEFAULTDPI / m_nDpiX;
		rc.top *= DEFAULTDPI / m_nDpiY;
		rc.bottom *= DEFAULTDPI / m_nDpiY;
	}
}

// 把当前的Wnd区域转换到UI坐标, 原点左上角，单位磅
CRect HCtrl::rcToClient()
{
	CRect rc = m_rcWnd;
	if( m_pParent )
		m_pParent->rcToClient( rc );
	return rc;
}

//UINT nDock：停靠方式，右边和底部
void HCtrl::setDock(UINT nDock, int nMarginRight, int nMarginBottom, int nMinWidth, int nMinHeight)
{
	m_nDock = nDock;
	m_nMarginRight	= nMarginRight;
	m_nMarginBottom	= nMarginBottom;
	m_nMinWidth		= nMinWidth;
	m_nMinHeight	= nMinHeight;
}

//UINT nImg：控件图形资源ID号，可继承到子控件
void HCtrl::setBtnImage(UINT nImg, const CSize & sizeImage )
{
	if( nImg )
	{
		ASSERT( !m_pImglist );
		m_pImglist = new CMFCToolBarImages();
		m_pImglist->Load( nImg );
		m_pImglist->SetImageSize( sizeImage );
	}
	else
		m_pImglist = 0;
}

//LPCTSTR lpszCursor：设置光标，只支持系统光标，为0时不响应鼠标事件。
void HCtrl::setCursor(LPCTSTR lpszCursor)
{
	m_lpszCursor = lpszCursor;
}

// 预处理画图事件，先画底层
BOOL HCtrl::preDraw( CDC* pDC, CRect & rcClip )
{
	// 需不需要重画
	if( !Intersect( rcClip ) )
		return FALSE;				// 没有重叠区，不重画。

	// 先画底层
	draw( pDC, rcClip );

	// 后画子换件
	int n = m_arChildren.GetCount();
	for( int i = 0; i < n; i++)
		m_arChildren[i]->preDraw( pDC, rcClip );

	return TRUE;
}

// 返回 FALSE 不用重画，否则重画, CDC* pDC 设备, CRect & m_rcClip 重画区
void HCtrl::draw( CDC* pDC, CRect & /*rcClip*/ )
{
	// 重画
/*#ifdef _DEBUG	// 重画计数
	CTime time=CTime::GetCurrentTime();
	CString str = time.Format("%c\r\n");
	TRACE( str );
#endif*/

	// 画背景
	if( m_cTestColor )	// 画测试背景
	{
		CBrush br(m_cTestColor);
		pDC->FillRect( m_rcWnd, &br );

		CDC * pd = m_pWnd->GetDC();	// 直接在设备DC中画一道斜线，不管重画区多大
		// create and select a thick, black pen
		CPen penBlack;
		penBlack.CreatePen(PS_SOLID, 2, m_cTestColor);
		CPen* pOldPen = pd->SelectObject(&penBlack);
		pd->MoveTo( m_rcWnd.TopLeft());
		pd->LineTo( m_rcWnd.BottomRight());
		pd->SelectObject(pOldPen);
		m_pWnd->ReleaseDC( pd );
		return;
	}

	if( !m_imgBackground.IsNull() )	// 画背景图
		m_imgBackground.StretchBlt( pDC->m_hDC, m_rcWnd );
	else if( m_brBackground.m_hObject )	// 画实际背景
		pDC->FillRect( m_rcWnd, &m_brBackground );

	// 画边框
	if( m_pnBorder.m_hObject )
	{
		CPen * pOldPen = pDC->SelectObject( &m_pnBorder );
		pDC->MoveTo( m_rcWnd.left, m_rcWnd.top );
		pDC->LineTo( m_rcWnd.right-1, m_rcWnd.top );
		pDC->LineTo( m_rcWnd.right-1, m_rcWnd.bottom-1 );
		pDC->LineTo( m_rcWnd.left, m_rcWnd.bottom-1 );
		pDC->LineTo( m_rcWnd.left, m_rcWnd.top );
		pDC->SelectObject(pOldPen);
	}
}

// 重绘区是不是相交，相交返回TRUE，否则返回FALSE
BOOL HCtrl::Intersect( CRect & rcClip )
{
	return CRect().IntersectRect(rcClip, m_rcWnd) == TRUE;
}

// 排版。
void HCtrl::layerout()
{
	ASSERT( m_pWnd );
	CWnd  *pwnd = m_pWnd;
	CRect rc;
	if( m_pParent )// 顶级控件在HWnd的OnSize中自动充满	// 20140714 顶级控制也改到这里
		rc = m_pParent->getRect();	// 父控件的区域
	else
		
		m_pWnd->GetClientRect( rc );
	
	// 计算控件大小和位置
	if( m_nDock && (HDOCK_RIGHT || HDOCK_FULLHORIZONTAL) )	// 靠右或水平充满
	{
		int w = m_rcClient.Width();
		m_rcClient.right = rc.Width() - m_nMarginRight;
		if( m_nDock & HDOCK_FULLHORIZONTAL )	// 充满时检查最小宽度
		{
			if( m_nMinWidth && (m_rcClient.Width() < m_nMinWidth) )
				m_rcClient.right = m_rcClient.left + m_nMinWidth;
		}
		else // 不是充满，宽度不变
			m_rcClient.left = m_rcClient.right - w;
	}
	if( m_nDock & (HDOCK_BOTTOM | HDOCK_FULLVERTICAL) )	// 靠底或垂直充满
	{
		int h = m_rcClient.Height();
		m_rcClient.bottom = rc.Height() - m_nMarginBottom;
		if( m_nDock & HDOCK_FULLVERTICAL )		// 不是充满，高度不变
		{
			if( m_nMinHeight && (m_rcClient.Height() < m_nMinHeight) )
				m_rcClient.bottom = m_rcClient.top + m_nMinHeight;
		}
		else
			m_rcClient.top = m_rcClient.bottom - h;
	}

	m_rcWnd = rcToWnd();

	int n = m_arChildren.GetCount();
	for( int i = 0; i < n; i++)
		m_arChildren[i]->layerout();

	invalidate();	// 重排版后重画
}

BOOL HCtrl::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	int n = m_arChildren.GetCount();
	for( int i = 0; i < n; i++)
	{
		if( m_arChildren[i]->OnNotify(wParam, lParam, pResult) )
			return TRUE;
	}

	return FALSE;
}

// 自动更新控件状态
void HCtrl::OnUpdateCmdUI(HCmdUI* pCmdUI, BOOL bDisableIfNoHndler)
{
	ASSERT_VALID(m_pWnd);

	// 先更新子控件
	int n = m_arChildren.GetCount();
	for( int i = 0; i < n; i++)
	{
		m_arChildren[i]->OnUpdateCmdUI( pCmdUI, bDisableIfNoHndler );
	}

	((HCmdUI*)pCmdUI)->m_pUpdated = this;	// 本控件指针

	pCmdUI->m_nID = m_nID;
	pCmdUI->DoUpdate(m_pWnd, bDisableIfNoHndler);

	((HCmdUI*)pCmdUI)->m_pUpdated = NULL;
}

// 左上角移动到指定位置
void HCtrl::moveTo( int x, int y )
{
	//ASSERT( m_nDock == HDOCK_NORMAL );
	ASSERT( !m_pParent );

	//m_pWnd->RedrawWindow( m_rcWnd, 0, RDW_INTERNALPAINT );	// 重画当前区域
	//m_pWnd->InvalidateRect( m_rcWnd );	// 重画当前区域
	invalidate();

	m_rcClient.right += x - m_rcClient.left;
	m_rcClient.left = x;
	m_rcClient.bottom += y - m_rcClient.top;
	m_rcClient.top = y;

	invalidate();
	//m_pWnd->InvalidateRect( m_rcWnd );	// 重画新区域
	m_pWnd->UpdateWindow();	// 立即重画
}

// 设置背景颜色
void HCtrl::setBackColor( COLORREF col )
{
	if( m_brBackground.m_hObject )
		m_brBackground.DeleteObject();

	m_brBackground.CreateSolidBrush( col );
}

// 设置背景图片
void HCtrl::setBackImg( PCTSTR idResource, PCTSTR lpType )
{
	HRSRC hrsrc = FindResource(NULL, idResource, lpType );  
	if(hrsrc)  
	{  
		HGLOBAL hglb = LoadResource(NULL, hrsrc);  
		LPVOID lpsz = LockResource(hglb);  
		ULARGE_INTEGER srcSize;  
		srcSize.QuadPart = SizeofResource(NULL, hrsrc);  

		IStream * pStream = NULL;  
		CreateStreamOnHGlobal(NULL, TRUE, &pStream);   
		if(pStream)  
		{  
			pStream->SetSize(srcSize);  
			DWORD cbWritten = 0;  
			pStream->Write(lpsz, srcSize.LowPart, &cbWritten);  

			VERIFY( m_imgBackground.Load(pStream) == S_OK );
			pStream->Release();  
		}  
	}  
}

// 设置边框颜色
void HCtrl::setBorderColor( COLORREF col )
{
	if( m_pnBorder.m_hObject )
		m_pnBorder.DeleteObject();

	m_pnBorder.CreatePen(PS_SOLID, 1, col);
}

// 窗口建立完毕后初始化，两个自定义参数
void HCtrl::initCtrl()
{
	int n = m_arChildren.GetCount();
	for( int i = 0; i < n; i++)
	{
		m_arChildren[i]->initCtrl();
	}
}

// 设置字体
BOOL HCtrl::setFont( int nHeight, int nWidth, LPCTSTR lpszFacename )
{
	if( m_font.m_hObject )
		m_font.DeleteObject();

	return m_font.CreateFont(
		nHeight,                   // nHeight
		nWidth,                   // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		0,                   // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		lpszFacename);	//(lpszFacename && lpszFacename[0]) ? lpszFacename : L"System");	// 默认用系统字体	用系统字体大小不能变
}

// 设置用户区，layort 为真，重排控件，否则不重排。
void HCtrl::setRect( CRect & rc, BOOL layout)
{
	m_rcClient = rc;

	if( layout )	// 重排重画控件及子控件
		layerout();
}

// 下传命令消息
BOOL HCtrl::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	int n = m_arChildren.GetCount();
	for( int i = 0; i < n; i++)
	{
		if( m_arChildren[i]->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo) )
			return TRUE;
	}

	return CCmdTarget::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

// 重画控件区域，HCtrl * pctrDraw 只重画指定的控件，为零时重画所有底层控件; BOOL bDrawBack 为真时重画背景，否则不重画背景
void HCtrl::invalidate()
{
	if( !m_pWnd->GetSafeHwnd() )
		return;

	m_pWnd->InvalidateRect( m_rcWnd );
}

// 显示悬停信息
void HCtrl::mouseHover( UINT /*nFlags*/, const CPoint & point )
{
	if( !m_strTips.IsEmpty() ) // 如果有提示信息，显示
		((HTips*)m_pctrTips)->drawText( point.x, point.y+23, m_strTips);
}

// 取当前控件的文本的尺寸
BOOL HCtrl::getTextSize( CRect & rc )
{
	BOOL bResult = FALSE;
	if (!m_pWnd->GetSafeHwnd())
		return bResult;

	CDC * pDC = m_pWnd->GetDC();

	CFont * pOldfont = 0;
	CFont * pfont = getFont();
	if( pfont )
		pOldfont = pDC->SelectObject( pfont );

	bResult = pDC->DrawText(m_strText, rc, DT_CALCRECT);
	if( pOldfont )
		pDC->SelectObject( pOldfont );

	m_pWnd->ReleaseDC( pDC );

	return bResult;
}









/////////////////////////////////////////////////////////////////////////////
// CMFCRibbonBar idle update through CMFCRibbonCmdUI class
HCmdUI::HCmdUI()	//http://blog.csdn.net/tms_li/article/details/6150247
{
	m_pUpdated = NULL;
}

void HCmdUI::Enable(BOOL bOn)
{
	m_bEnableChanged = TRUE;

	ASSERT_VALID(m_pOther);
	ASSERT_VALID(m_pUpdated);

	if (m_pUpdated->isEnabled() != bOn)
		m_pUpdated->Enable(bOn);
}

void HCmdUI::SetCheck(int nCheck)
{
	ASSERT_VALID(m_pOther);
	ASSERT_VALID(m_pUpdated);

	if (m_pUpdated->getChecked() != nCheck)
		m_pUpdated->setChecked( nCheck );
}

/*void HCmdUI::SetRadio(BOOL bOn)
{
	ASSERT_VALID(m_pUpdated);

	m_pUpdated->m_bIsRadio = bOn;
	SetCheck(bOn ? 1 : 0);
}*/

void HCmdUI::SetText(CString lpszText)
{
	ENSURE(lpszText != _T(""));

	ASSERT_VALID(m_pOther);
	ASSERT_VALID(m_pUpdated);

	if (lstrcmp(m_pUpdated->getText(), lpszText) != 0)
	{
		m_pUpdated->setText(lpszText);
	}
}

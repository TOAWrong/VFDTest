// HCtrl.cpp : ʵ���ļ�
// �ؼ�����

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
int		HCtrl::m_nDpiX			= DEFAULTDPI;	// ��ʾ��X����dpi
int		HCtrl::m_nDpiY			= DEFAULTDPI;	// ��ʾ��Y����dpi
CWnd *	HCtrl::m_pWnd			= 0;		// �ؼ����ڵĴ��ڡ�
COLORREF HCtrl::m_cTestColor	= 0;	// ��ʼ��ֵΪ0������ʾ���Ա�����

// id < 0, �����������Ϣ

HCtrl::HCtrl()
{
	initParameter();
}

HCtrl::HCtrl(LPCRECT prc )
{
	m_rcClient = prc;

	initParameter();
}

// ������ʼ��
void HCtrl::initParameter()
{
	setDock( HDOCK_NORMAL );

	m_nID = 0;			// id < 0, �����������Ϣ
	m_lpszCursor = 0;
	//m_bMouseEnter = FALSE;
	m_pImglist = 0;
	m_pParent = 0;

	m_nAlign = 0;
	m_coText = 0x00bbbbbb;

	m_strTips = L"";

	m_bEnabled = TRUE;	// Ĭ�Ͽ���
	m_nCheck = 0;	// Ĭ��δѡ��
	m_coDisabled = 0x888888;	// Ĭ�Ͻ�ֹ�ı���ɫ

	//m_rcWnd = rcToWnd();			// �ŵ������ΪrcToWnd()�õ��ܶ����ԡ� // ��ʼ��ʱ����ִ�У���Ϊ���ڻ�û�н������зŵ�layout���档
}

HCtrl::~HCtrl()
{
	delete m_pImglist;
	
	removeAllChildren();
}

// ɾ��ָ�����ӿؼ���i �ؼ�������void HCtrl::removeAllChildren()
void HCtrl::removeChildrenAt( int i )
{
	ASSERT( i >= 0 && i <  m_arChildren.GetCount() );
	m_arChildren[i]->invalidate();
	delete m_arChildren[i];
	m_arChildren.RemoveAt(i);
}

// ɾ��ȫ���ӿؼ�
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

// HCtrl ��Ա����

// ����TRUE��ʾ�Ѿ�����
BOOL HCtrl::OnMouseMove( UINT nFlags, const CPoint &point )
{
	int n = m_arChildren.GetCount();
	for( int i = 0; i < n; i++)		// �Ȳ��ӿؼ�
	{
		if( m_arChildren[i]->OnMouseMove( nFlags, point ) )
			return TRUE;	// ����ӿؼ��������ٴ���
	}

	if( !m_bEnabled || m_nID == -1 || !m_rcWnd.PtInRect( point ) )	// �����ֹ �� m_nID==-1 �� ��겻�ڿؼ���
		return FALSE;

	//#pragma message("********���Ե�һ��������û��move�¼�") ��
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
	for( int i = 0; i < n; i++)		// �Ȳ��ӿؼ�
	{
		if( m_arChildren[i]->OnWndMsg(message, wParam, lParam, pResult) )
			return TRUE;	// ����ӿؼ��������ٴ���
	}

	return FALSE;
}


void HCtrl::addCtrl( HCtrl * pCtrl )	// ��������ӷ�����
{
	if( m_arChildren.IsEmpty() )
		m_arChildren.SetSize( 0, 20 );	// ��һ�����ʱԤ��20��Ԫ�أ������ڴ���Ƭ
	//layerout();
	pCtrl->m_pParent = this;
	m_arChildren.Add( pCtrl );
	//TRACE( L"%x, %x\r\n", this, &m_arChildren[0] );

	pCtrl->added();
}

// �����ǰ��m_font��ȡ��ǰ�ģ�����ȡ�����ġ�
CFont * HCtrl::getFont()
{
	if( m_font.m_hObject )
		return &m_font;
	else if( m_pParent )
		return m_pParent->getFont();
	else
		return 0;
}

// �����ǰ��m_pImglist��ȡ��ǰ�ģ�����ȡ�����ġ�
CMFCToolBarImages * HCtrl::getImglist()
{
	if( m_pImglist )
		return m_pImglist;
	else if( m_pParent )
		return m_pParent->getImglist();
	else
		return 0;
}

// ���ø��ؼ���rcToWnd���ѵ�ǰrcת������Դ��ڵ����ݡ�
void HCtrl::rcToWnd( CRect &rc )
{
	rc.OffsetRect( m_rcClient.TopLeft() );
	if( m_pParent )
		m_pParent->rcToWnd( rc );
	else
	{
		// ��ת��������
		rc.left *= m_nDpiX / DEFAULTDPI;
		rc.right *= m_nDpiX / DEFAULTDPI;
		rc.top *= m_nDpiY / DEFAULTDPI;
		rc.bottom *= m_nDpiY / DEFAULTDPI;
	}
}

// ���ø��ؼ���rcToWnd���ѵ�ǰrcת������Դ��ڵ����ݡ�
CRect HCtrl::rcToWnd()
{
	CRect rc = m_rcClient;
	if( m_pParent )
		m_pParent->rcToWnd( rc );
	return rc;
}

// ���ø��ؼ���rcToClientg���ѵ�ǰ��Wnd����ת����UI����, ԭ�����Ͻǣ���λ��
void HCtrl::rcToClient( CRect &rc )
{
	rc.OffsetRect( -m_rcClient.left, -m_rcClient.top );
	if( m_pParent )
		m_pParent->rcToClient( rc );
	else
	{
		// ����ת���ɰ�
		rc.left *= DEFAULTDPI / m_nDpiX;
		rc.right *= DEFAULTDPI / m_nDpiX;
		rc.top *= DEFAULTDPI / m_nDpiY;
		rc.bottom *= DEFAULTDPI / m_nDpiY;
	}
}

// �ѵ�ǰ��Wnd����ת����UI����, ԭ�����Ͻǣ���λ��
CRect HCtrl::rcToClient()
{
	CRect rc = m_rcWnd;
	if( m_pParent )
		m_pParent->rcToClient( rc );
	return rc;
}

//UINT nDock��ͣ����ʽ���ұߺ͵ײ�
void HCtrl::setDock(UINT nDock, int nMarginRight, int nMarginBottom, int nMinWidth, int nMinHeight)
{
	m_nDock = nDock;
	m_nMarginRight	= nMarginRight;
	m_nMarginBottom	= nMarginBottom;
	m_nMinWidth		= nMinWidth;
	m_nMinHeight	= nMinHeight;
}

//UINT nImg���ؼ�ͼ����ԴID�ţ��ɼ̳е��ӿؼ�
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

//LPCTSTR lpszCursor�����ù�ֻ֧꣬��ϵͳ��꣬Ϊ0ʱ����Ӧ����¼���
void HCtrl::setCursor(LPCTSTR lpszCursor)
{
	m_lpszCursor = lpszCursor;
}

// Ԥ����ͼ�¼����Ȼ��ײ�
BOOL HCtrl::preDraw( CDC* pDC, CRect & rcClip )
{
	// �費��Ҫ�ػ�
	if( !Intersect( rcClip ) )
		return FALSE;				// û���ص��������ػ���

	// �Ȼ��ײ�
	draw( pDC, rcClip );

	// ���ӻ���
	int n = m_arChildren.GetCount();
	for( int i = 0; i < n; i++)
		m_arChildren[i]->preDraw( pDC, rcClip );

	return TRUE;
}

// ���� FALSE �����ػ��������ػ�, CDC* pDC �豸, CRect & m_rcClip �ػ���
void HCtrl::draw( CDC* pDC, CRect & /*rcClip*/ )
{
	// �ػ�
/*#ifdef _DEBUG	// �ػ�����
	CTime time=CTime::GetCurrentTime();
	CString str = time.Format("%c\r\n");
	TRACE( str );
#endif*/

	// ������
	if( m_cTestColor )	// �����Ա���
	{
		CBrush br(m_cTestColor);
		pDC->FillRect( m_rcWnd, &br );

		CDC * pd = m_pWnd->GetDC();	// ֱ�����豸DC�л�һ��б�ߣ������ػ������
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

	if( !m_imgBackground.IsNull() )	// ������ͼ
		m_imgBackground.StretchBlt( pDC->m_hDC, m_rcWnd );
	else if( m_brBackground.m_hObject )	// ��ʵ�ʱ���
		pDC->FillRect( m_rcWnd, &m_brBackground );

	// ���߿�
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

// �ػ����ǲ����ཻ���ཻ����TRUE�����򷵻�FALSE
BOOL HCtrl::Intersect( CRect & rcClip )
{
	return CRect().IntersectRect(rcClip, m_rcWnd) == TRUE;
}

// �Ű档
void HCtrl::layerout()
{
	ASSERT( m_pWnd );
	CWnd  *pwnd = m_pWnd;
	CRect rc;
	if( m_pParent )// �����ؼ���HWnd��OnSize���Զ�����	// 20140714 ��������Ҳ�ĵ�����
		rc = m_pParent->getRect();	// ���ؼ�������
	else
		
		m_pWnd->GetClientRect( rc );
	
	// ����ؼ���С��λ��
	if( m_nDock && (HDOCK_RIGHT || HDOCK_FULLHORIZONTAL) )	// ���һ�ˮƽ����
	{
		int w = m_rcClient.Width();
		m_rcClient.right = rc.Width() - m_nMarginRight;
		if( m_nDock & HDOCK_FULLHORIZONTAL )	// ����ʱ�����С���
		{
			if( m_nMinWidth && (m_rcClient.Width() < m_nMinWidth) )
				m_rcClient.right = m_rcClient.left + m_nMinWidth;
		}
		else // ���ǳ�������Ȳ���
			m_rcClient.left = m_rcClient.right - w;
	}
	if( m_nDock & (HDOCK_BOTTOM | HDOCK_FULLVERTICAL) )	// ���׻�ֱ����
	{
		int h = m_rcClient.Height();
		m_rcClient.bottom = rc.Height() - m_nMarginBottom;
		if( m_nDock & HDOCK_FULLVERTICAL )		// ���ǳ������߶Ȳ���
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

	invalidate();	// ���Ű���ػ�
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

// �Զ����¿ؼ�״̬
void HCtrl::OnUpdateCmdUI(HCmdUI* pCmdUI, BOOL bDisableIfNoHndler)
{
	ASSERT_VALID(m_pWnd);

	// �ȸ����ӿؼ�
	int n = m_arChildren.GetCount();
	for( int i = 0; i < n; i++)
	{
		m_arChildren[i]->OnUpdateCmdUI( pCmdUI, bDisableIfNoHndler );
	}

	((HCmdUI*)pCmdUI)->m_pUpdated = this;	// ���ؼ�ָ��

	pCmdUI->m_nID = m_nID;
	pCmdUI->DoUpdate(m_pWnd, bDisableIfNoHndler);

	((HCmdUI*)pCmdUI)->m_pUpdated = NULL;
}

// ���Ͻ��ƶ���ָ��λ��
void HCtrl::moveTo( int x, int y )
{
	//ASSERT( m_nDock == HDOCK_NORMAL );
	ASSERT( !m_pParent );

	//m_pWnd->RedrawWindow( m_rcWnd, 0, RDW_INTERNALPAINT );	// �ػ���ǰ����
	//m_pWnd->InvalidateRect( m_rcWnd );	// �ػ���ǰ����
	invalidate();

	m_rcClient.right += x - m_rcClient.left;
	m_rcClient.left = x;
	m_rcClient.bottom += y - m_rcClient.top;
	m_rcClient.top = y;

	invalidate();
	//m_pWnd->InvalidateRect( m_rcWnd );	// �ػ�������
	m_pWnd->UpdateWindow();	// �����ػ�
}

// ���ñ�����ɫ
void HCtrl::setBackColor( COLORREF col )
{
	if( m_brBackground.m_hObject )
		m_brBackground.DeleteObject();

	m_brBackground.CreateSolidBrush( col );
}

// ���ñ���ͼƬ
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

// ���ñ߿���ɫ
void HCtrl::setBorderColor( COLORREF col )
{
	if( m_pnBorder.m_hObject )
		m_pnBorder.DeleteObject();

	m_pnBorder.CreatePen(PS_SOLID, 1, col);
}

// ���ڽ�����Ϻ��ʼ���������Զ������
void HCtrl::initCtrl()
{
	int n = m_arChildren.GetCount();
	for( int i = 0; i < n; i++)
	{
		m_arChildren[i]->initCtrl();
	}
}

// ��������
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
		lpszFacename);	//(lpszFacename && lpszFacename[0]) ? lpszFacename : L"System");	// Ĭ����ϵͳ����	��ϵͳ�����С���ܱ�
}

// �����û�����layort Ϊ�棬���ſؼ����������š�
void HCtrl::setRect( CRect & rc, BOOL layout)
{
	m_rcClient = rc;

	if( layout )	// �����ػ��ؼ����ӿؼ�
		layerout();
}

// �´�������Ϣ
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

// �ػ��ؼ�����HCtrl * pctrDraw ֻ�ػ�ָ���Ŀؼ���Ϊ��ʱ�ػ����еײ�ؼ�; BOOL bDrawBack Ϊ��ʱ�ػ������������ػ�����
void HCtrl::invalidate()
{
	if( !m_pWnd->GetSafeHwnd() )
		return;

	m_pWnd->InvalidateRect( m_rcWnd );
}

// ��ʾ��ͣ��Ϣ
void HCtrl::mouseHover( UINT /*nFlags*/, const CPoint & point )
{
	if( !m_strTips.IsEmpty() ) // �������ʾ��Ϣ����ʾ
		((HTips*)m_pctrTips)->drawText( point.x, point.y+23, m_strTips);
}

// ȡ��ǰ�ؼ����ı��ĳߴ�
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

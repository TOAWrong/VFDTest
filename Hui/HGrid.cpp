// HGrid.cpp : ʵ���ļ�
//
// ���ؼ�����һ����id��ʶ������ʾ������Ҫ

#include "stdafx.h"
#include "Hui.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// HGrid
IMPLEMENT_DYNAMIC(HGrid, HCtrl)

HGrid::HGrid(LPCRECT prc)
: HCtrl(prc)
{
	m_nRowNum = HGRID_RECERVATION_ROWS;	// ����Ĭ������������Ӧ�¼��м���ʵ��ֵ
	m_nRowHeight = 22;			// Ĭ���и�18
	m_arRows.SetSize(0, 100);		// Ԥ��100��
	m_Columns.items.SetSize(0,20);	// Ԥ��20��
	m_Columns.setHeight( m_nRowHeight * 5/4 );	// Ĭ�ϱ����и�һ��
	m_Columns.textColor = m_coText;
	m_nPadding = 4;	// �������ĸ���
	m_nMouse = -1;

	m_strTips = L"";	// �����մ���������ͣ�¼���
}

HGrid::~HGrid()
{
	for( int i = m_arRows.GetCount()-1; i >= 0; i-- )
		delete m_arRows[i];
	m_arRows.RemoveAll();
}

// HGrid ��Ա����
void HGrid::draw( CDC* pDC, CRect & rcClip )
{
	HCtrl::draw( pDC, rcClip );

	int h = m_rcWnd.Height();	// �ܸ�
	if( h < m_Columns.height )
		return;			// �߶Ȳ������⣬ֱ�ӷ���

	pDC->SetBkMode(TRANSPARENT);

	int x0 = m_rcWnd.left;		// ԭ�㣬���Ͻ�X
	int y0 = m_rcWnd.top;		// ԭ�㣬���Ͻ�Y
	int right = m_rcWnd.right - 1;	// �ұߡ���һ�����أ�����������

	int x1 = x0, y1 = y0;
	int x2 = 0, y2 = y0 + m_Columns.height - 1;	// ��һ�����أ�����������

	// �����б������ߺ�����
	pDC->SelectObject( &m_pnBorder );
	int columnNum = m_Columns.items.GetSize()-1;	// ������1
	pDC->SetTextColor(m_Columns.textColor);	// ������ɫ
	if( m_Columns.m_brBackground.m_hObject )	// �����б���
		pDC->FillRect( CRect(x0, y1, right, y2), &m_Columns.m_brBackground );
	pDC->MoveTo( x0, y1 );	// �����Ϸ�ֱ��
	pDC->LineTo( right, y1 );
	pDC->MoveTo( x0, y2 );	// �����·�ֱ�ߡ�
	pDC->LineTo( right, y2 );
	
	CFont * pOldfont = 0;
	CFont * pfont = getFont();
	if( pfont )
		pOldfont = pDC->SelectObject( pfont );

	for( int i = 1; i <= columnNum; i++ )	// ��һ����ID����ʾ
	{
		x2 = x1 + m_Columns.items[i].width;
		if( x2 > right || i == columnNum )
			x2 = right;	// ���һ�л����ؼ��ұ�
		pDC->DrawText( m_Columns.items[i].title, CRect( x1, y1, x2, y2+1 ), DT_CENTER | DT_VCENTER | DT_SINGLELINE );
		x1 = x2;
	}

	// ���֣������ߣ��Զ������иߣ������ؼ���������layerout�м���
	int bottom = m_rcWnd.bottom;
	h -= m_Columns.height;	// ���ݸ�
	if( h < m_nRowHeight )
		return;	// ����һ�����ݣ�����

	pDC->SetTextColor(m_coText);	// ������ɫ
	int y3 = y1 = y2;
	x1 = x0;
	//float h1 = (float)h / m_nRowNum;	// ��������ʾ���иߣ�ʵ�ֲ�ֵ����ÿһ��ʵ�ʸ߶ȣ������ؼ���
	for( int i = 0, n = m_arRows.GetCount(); i < m_nRowNum && i < n; i++)
	{
		y2 = int(y3 + ((i+1) * m_fRowHeight));

		// ѡ����λ��ͻ��
		if( !m_strSelectID.IsEmpty() && m_arRows[i]->GetAt(0).text == m_strSelectID && m_brSelect.m_hObject )	// �����б���
			pDC->FillRect( CRect(x0, y1+1, right, y2), &m_brSelect );
		// ���λ��ͻ��
		else if( i == m_nMouse && m_brMouse.m_hObject )	// �����б���
			pDC->FillRect( CRect(x0, y1+1, right, y2), &m_brMouse );

		// ������
		pDC->MoveTo( x0, y2 );	// �ø�������ʵ�ֲ�ֵ����ÿһ�и߶ȣ��Զ������ؼ���
		pDC->LineTo( right, y2 );

		// ����
		for( int j = 1; j <= columnNum && x1 < right; j++ )	// ��һ����id, ����ʾ
		{
			x2 = x1 + m_Columns.items[j].width;
			if( x2 > right || j == columnNum )
				x2 = right;	// ���һ�л����ؼ��ұ�
			CRect rc( x1 + m_nPadding, y1, x2-m_nPadding, y2 );
			Cell & cell = m_arRows[i]->GetAt(j);
			if( cell.breaked < 0 )
			{
				CRect rc2 = rc;
				pDC->DrawText( cell.text, rc2, DT_CALCRECT );
				cell.breaked = rc2.right > rc.right ? 1 : 0;
			}
			pDC->DrawText( cell.text, CRect( x1 + m_nPadding, y1, x2-m_nPadding, y2 ), m_Columns.items[j].style );
			x1 = x2;
		}
		x1 = x0;
		y1 = y2;
	}
	if( pOldfont )
		pDC->SelectObject( pOldfont );

	// �����ߣ����һ�г����ؼ�
	pDC->MoveTo( x0, y0 );
	pDC->LineTo( x0, bottom );
	int x = x0;
	for( int i = 0; i <= columnNum; i++ )
	{
		x += m_Columns.items[i].width;
		if( x > right || i == columnNum )
			x = right;	// ���һ�л����ؼ��ұ�

		pDC->MoveTo( x, y0 );
		pDC->LineTo( x, bottom );
	}
}

// ���һ�У�PCSTR title �б���; int width �п�ȣ����һ���Զ��������
void HGrid::addColumn( PCTSTR title, int width, UINT style )
{
	Column tit;
	tit.title = title;
	tit.width = width;
	tit.style = style;
	m_Columns.items.Add( tit );

	// ÿ�ж���һ��
	for( int i = m_arRows.GetCount()-1; i >= 0; i-- )
		m_arRows[i]->Add( Cell() );
}

/* ֱ���ÿؼ��ı߿���ɫ
// ���ñ������ɫ
void HGrid::setLineColor( COLORREF color )
{
	if( m_pnBorder.m_hObject )
		m_pnBorder.DeleteObject();

	m_pnBorder.CreatePen(PS_SOLID, 1, color);
}
*/

// ��������б�����ɫ
void HGrid::setMouseColor( COLORREF color )
{
	if( m_brMouse.m_hObject )
		m_brMouse.DeleteObject();

	m_brMouse.CreateSolidBrush(color);
}

// ����ѡ���б�����ɫ
void HGrid::setSelectColor( COLORREF color )
{
	if( m_brSelect.m_hObject )
		m_brSelect.DeleteObject();

	m_brSelect.CreateSolidBrush(color);
}

// �����������ÿո����
void HGrid::removeAll()
{
	int cs = m_Columns.items.GetCount();
	for( int i = m_arRows.GetCount()-1; i >= 0; i-- )
		for( int j = 0; j < cs; j++ )
			m_arRows[i]->GetAt(j).setEmpty();	// Ԥ��һ���ո񣬱����ڴ���Ƭ��Ч�ʵ���
}

// ȡָ���У����ָ���в����ڣ�������У���Ԥ���50�У������ڴ���Ƭ��Ч�ʵ��¡�
HGrid::Cells * HGrid::getRow( int index )
{
	int n = m_arRows.GetCount() - 1;
	if( index <= n )	// ������ڣ�ֱ�ӷ���
		return m_arRows[index];

	//���ָ���в����ڣ�������У���Ԥ���50��
	n = index - n + HGRID_RECERVATION_ROWS;
	for( int j = 0; j < n; j++ )
	{
		int cs = m_Columns.items.GetCount();
		Cells * prow = new Cells();
		prow->SetSize(cs);
		//for( int i = 0; i < cs; i++ )
		//	prow->GetAt(i, L" ");	// Ԥ��һ���ո񣬱����ڴ���Ƭ��Ч�ʵ���
		m_arRows.Add( prow );
	}

	return m_arRows[index];
}

// ���Ű棬��������
void HGrid::layerout()
{
	HCtrl::layerout();

	int h = m_rcWnd.Height();	// �ܸ�
	h -= m_Columns.height;	// ���ݸ�
	m_nRowNum = h / m_nRowHeight;	// ����
	m_fRowHeight = (float)h / m_nRowNum; // �����и�
}

// ����ƶ�
void HGrid::mouseMove( UINT /*nFlage*/, const CPoint & point )
{
	int y0 = m_rcWnd.top + m_Columns.height;
	int x = point.x;
	int y = point.y;

	int n = ( y >= y0 && y < m_rcWnd.bottom && x >= m_rcWnd.left && x < m_rcWnd.right )
		? int((y - y0) / m_fRowHeight) : -1;

	if( n < 0 || m_arRows.IsEmpty() )
		n = -1;
	else if( m_arRows[n]->GetAt(0).text.Trim().IsEmpty() )	// ���в���ɫ
		n = -1;

	if( n == m_nMouse )
		return;

	m_nMouse = n;
	invalidate(); // ?? �ĳɾֲ��ػ�
}

// ����뿪
void HGrid::mouseLeave()
{
	HCtrl::mouseLeave();
	if( m_nMouse >= 0 )
	{
		m_nMouse = -1;
		invalidate(); // ?? �ĳɾֲ��ػ�
	}
}

// ���������
void HGrid::lbuttonDown( UINT nFlags, const CPoint & point )
{
	HCtrl::lbuttonDown( nFlags, point );

	if( m_arRows.IsEmpty() )
		return;

	if( m_nMouse < 0 )
		m_strSelectID = L"";
	else
	{
		CString id = m_arRows[m_nMouse]->GetAt(0).text;
		if( id.Trim().IsEmpty() )
			m_strSelectID = L"";
		else
			m_strSelectID = id;
	}

	invalidate(); // ?? �ĳɾֲ��ػ�
}

// ��ʾ��ͣ��Ϣ
void HGrid::mouseHover( UINT /*nFlags*/, const CPoint & point )
{
	if( m_arRows.IsEmpty() )
		return;

	int nCol = -1;	// ��
	int nRow = -1;	// ��

	// ����
	int y = point.y - m_Columns.height - m_rcWnd.top;
	if( y < 0 )
		return;	// �ڵ�һ������
	nRow = int(y / m_fRowHeight);	// �ø���߶ȼ������ȷ
	if( nRow < 0 || nRow >= m_nRowNum )
		return;

	// ����
	CArray<Column> & items = m_Columns.items;
	int n = items.GetCount();
	ASSERT( n >= 2 );	// �������У�һ��ID������ʾ��һ�����ݡ�
	int w = m_rcWnd.left;	// �ӵڶ��п�ʼ����һ����ID������ʾ��
	for( int i = 1; i < n; i++ )
	{
		w += items[i].width;
		if( point.x < w )
		{
			nCol = i;
			break;
		}
	}

	if( nCol < 0 )
		return;

	// ��ʾ��ͣ��Ϣ
	Cell & cell = m_arRows[nRow]->GetAt(nCol);
	if( cell.breaked > 0 )	// ֻ�нض����ֵ�Ԫ����ʾ��ͣ
		((HTips*)m_pctrTips)->drawText( point.x, point.y+23, cell.text );
}

// ȡָ���������к�����
//CPoint HGrid::getPos( const CPoint & p )
//{
//	CPoint result( -1, -1 );
//	CArray<Column> & items = m_Columns.items;
//	int n = items.GetCount();
//	ASSERT( n > 0 );
//	int w = items[0].width;
//	for( int i = 1; i < n; w += items[i].width, i++ )
//	{
//		if( p.x < w )
//		{
//			result.x = i-1;
//			break;
//		}
//	}
//
//	//#pragma warning( "���Ա߽�" )
//	result.y = LONG((p.y - m_Columns.height) / m_fRowHeight);
//
//	return result;
//}
//
//

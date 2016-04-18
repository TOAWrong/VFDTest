// HGrid.cpp : 实现文件
//
// 表格控件，第一列是id标识，不显示，必需要

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
	m_nRowNum = HGRID_RECERVATION_ROWS;	// 设置默认行数，在相应事件中计算实际值
	m_nRowHeight = 22;			// 默认行高18
	m_arRows.SetSize(0, 100);		// 预留100行
	m_Columns.items.SetSize(0,20);	// 预留20列
	m_Columns.setHeight( m_nRowHeight * 5/4 );	// 默认标题行高一点
	m_Columns.textColor = m_coText;
	m_nPadding = 4;	// 左右留四个素
	m_nMouse = -1;

	m_strTips = L"";	// 赋个空串，激活悬停事件。
}

HGrid::~HGrid()
{
	for( int i = m_arRows.GetCount()-1; i >= 0; i-- )
		delete m_arRows[i];
	m_arRows.RemoveAll();
}

// HGrid 成员函数
void HGrid::draw( CDC* pDC, CRect & rcClip )
{
	HCtrl::draw( pDC, rcClip );

	int h = m_rcWnd.Height();	// 总高
	if( h < m_Columns.height )
		return;			// 高度不够标题，直接返回

	pDC->SetBkMode(TRANSPARENT);

	int x0 = m_rcWnd.left;		// 原点，左上角X
	int y0 = m_rcWnd.top;		// 原点，左上角Y
	int right = m_rcWnd.right - 1;	// 右边。减一个像素，画到区域内

	int x1 = x0, y1 = y0;
	int x2 = 0, y2 = y0 + m_Columns.height - 1;	// 减一个像素，画到区域内

	// 标题行背景横线和文字
	pDC->SelectObject( &m_pnBorder );
	int columnNum = m_Columns.items.GetSize()-1;	// 列数减1
	pDC->SetTextColor(m_Columns.textColor);	// 文字颜色
	if( m_Columns.m_brBackground.m_hObject )	// 标题行背景
		pDC->FillRect( CRect(x0, y1, right, y2), &m_Columns.m_brBackground );
	pDC->MoveTo( x0, y1 );	// 标题上方直线
	pDC->LineTo( right, y1 );
	pDC->MoveTo( x0, y2 );	// 标题下方直线。
	pDC->LineTo( right, y2 );
	
	CFont * pOldfont = 0;
	CFont * pfont = getFont();
	if( pfont )
		pOldfont = pDC->SelectObject( pfont );

	for( int i = 1; i <= columnNum; i++ )	// 第一行是ID不显示
	{
		x2 = x1 + m_Columns.items[i].width;
		if( x2 > right || i == columnNum )
			x2 = right;	// 最后一列画到控件右边
		pDC->DrawText( m_Columns.items[i].title, CRect( x1, y1, x2, y2+1 ), DT_CENTER | DT_VCENTER | DT_SINGLELINE );
		x1 = x2;
	}

	// 填字，画横线，自动调整行高，充满控件，行数在layerout中计算
	int bottom = m_rcWnd.bottom;
	h -= m_Columns.height;	// 内容高
	if( h < m_nRowHeight )
		return;	// 不够一行内容，返回

	pDC->SetTextColor(m_coText);	// 文字颜色
	int y3 = y1 = y2;
	x1 = x0;
	//float h1 = (float)h / m_nRowNum;	// 浮点数表示的行高，实现插值计算每一行实际高度，冲满控件。
	for( int i = 0, n = m_arRows.GetCount(); i < m_nRowNum && i < n; i++)
	{
		y2 = int(y3 + ((i+1) * m_fRowHeight));

		// 选择行位置突出
		if( !m_strSelectID.IsEmpty() && m_arRows[i]->GetAt(0).text == m_strSelectID && m_brSelect.m_hObject )	// 标题行背景
			pDC->FillRect( CRect(x0, y1+1, right, y2), &m_brSelect );
		// 鼠标位置突出
		else if( i == m_nMouse && m_brMouse.m_hObject )	// 标题行背景
			pDC->FillRect( CRect(x0, y1+1, right, y2), &m_brMouse );

		// 画横线
		pDC->MoveTo( x0, y2 );	// 用浮点数，实现插值计算每一行高度，自动充满控件。
		pDC->LineTo( right, y2 );

		// 填字
		for( int j = 1; j <= columnNum && x1 < right; j++ )	// 第一列是id, 不显示
		{
			x2 = x1 + m_Columns.items[j].width;
			if( x2 > right || j == columnNum )
				x2 = right;	// 最后一列画到控件右边
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

	// 画竖线，最后一列充满控件
	pDC->MoveTo( x0, y0 );
	pDC->LineTo( x0, bottom );
	int x = x0;
	for( int i = 0; i <= columnNum; i++ )
	{
		x += m_Columns.items[i].width;
		if( x > right || i == columnNum )
			x = right;	// 最后一列画到控件右边

		pDC->MoveTo( x, y0 );
		pDC->LineTo( x, bottom );
	}
}

// 添加一列，PCSTR title 列标题; int width 列宽度，最后一列自动充满外框。
void HGrid::addColumn( PCTSTR title, int width, UINT style )
{
	Column tit;
	tit.title = title;
	tit.width = width;
	tit.style = style;
	m_Columns.items.Add( tit );

	// 每行都加一列
	for( int i = m_arRows.GetCount()-1; i >= 0; i-- )
		m_arRows[i]->Add( Cell() );
}

/* 直接用控件的边框颜色
// 设置表格线颜色
void HGrid::setLineColor( COLORREF color )
{
	if( m_pnBorder.m_hObject )
		m_pnBorder.DeleteObject();

	m_pnBorder.CreatePen(PS_SOLID, 1, color);
}
*/

// 设置鼠标行背景颜色
void HGrid::setMouseColor( COLORREF color )
{
	if( m_brMouse.m_hObject )
		m_brMouse.DeleteObject();

	m_brMouse.CreateSolidBrush(color);
}

// 设置选择行背景颜色
void HGrid::setSelectColor( COLORREF color )
{
	if( m_brSelect.m_hObject )
		m_brSelect.DeleteObject();

	m_brSelect.CreateSolidBrush(color);
}

// 把所有内容用空格代替
void HGrid::removeAll()
{
	int cs = m_Columns.items.GetCount();
	for( int i = m_arRows.GetCount()-1; i >= 0; i-- )
		for( int j = 0; j < cs; j++ )
			m_arRows[i]->GetAt(j).setEmpty();	// 预存一个空格，避免内存碎片和效率低下
}

// 取指定行，如果指定行不存在，添加新行，并预添加50行，避免内存碎片和效率低下。
HGrid::Cells * HGrid::getRow( int index )
{
	int n = m_arRows.GetCount() - 1;
	if( index <= n )	// 如果存在，直接返回
		return m_arRows[index];

	//如果指定行不存在，添加新行，并预添加50行
	n = index - n + HGRID_RECERVATION_ROWS;
	for( int j = 0; j < n; j++ )
	{
		int cs = m_Columns.items.GetCount();
		Cells * prow = new Cells();
		prow->SetSize(cs);
		//for( int i = 0; i < cs; i++ )
		//	prow->GetAt(i, L" ");	// 预存一个空格，避免内存碎片和效率低下
		m_arRows.Add( prow );
	}

	return m_arRows[index];
}

// 重排版，计算行数
void HGrid::layerout()
{
	HCtrl::layerout();

	int h = m_rcWnd.Height();	// 总高
	h -= m_Columns.height;	// 内容高
	m_nRowNum = h / m_nRowHeight;	// 行数
	m_fRowHeight = (float)h / m_nRowNum; // 浮点行高
}

// 鼠标移动
void HGrid::mouseMove( UINT /*nFlage*/, const CPoint & point )
{
	int y0 = m_rcWnd.top + m_Columns.height;
	int x = point.x;
	int y = point.y;

	int n = ( y >= y0 && y < m_rcWnd.bottom && x >= m_rcWnd.left && x < m_rcWnd.right )
		? int((y - y0) / m_fRowHeight) : -1;

	if( n < 0 || m_arRows.IsEmpty() )
		n = -1;
	else if( m_arRows[n]->GetAt(0).text.Trim().IsEmpty() )	// 空行不变色
		n = -1;

	if( n == m_nMouse )
		return;

	m_nMouse = n;
	invalidate(); // ?? 改成局部重画
}

// 鼠标离开
void HGrid::mouseLeave()
{
	HCtrl::mouseLeave();
	if( m_nMouse >= 0 )
	{
		m_nMouse = -1;
		invalidate(); // ?? 改成局部重画
	}
}

// 点击鼠标左键
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

	invalidate(); // ?? 改成局部重画
}

// 显示悬停信息
void HGrid::mouseHover( UINT /*nFlags*/, const CPoint & point )
{
	if( m_arRows.IsEmpty() )
		return;

	int nCol = -1;	// 列
	int nRow = -1;	// 行

	// 求行
	int y = point.y - m_Columns.height - m_rcWnd.top;
	if( y < 0 )
		return;	// 在第一行上面
	nRow = int(y / m_fRowHeight);	// 用浮点高度计算更精确
	if( nRow < 0 || nRow >= m_nRowNum )
		return;

	// 求列
	CArray<Column> & items = m_Columns.items;
	int n = items.GetCount();
	ASSERT( n >= 2 );	// 至少两列，一列ID，不显示，一列内容。
	int w = m_rcWnd.left;	// 从第二列开始（第一列是ID，不显示）
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

	// 显示悬停信息
	Cell & cell = m_arRows[nRow]->GetAt(nCol);
	if( cell.breaked > 0 )	// 只有截断文字单元格显示悬停
		((HTips*)m_pctrTips)->drawText( point.x, point.y+23, cell.text );
}

// 取指定坐标点的行和列数
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
//	//#pragma warning( "测试边界" )
//	result.y = LONG((p.y - m_Columns.height) / m_fRowHeight);
//
//	return result;
//}
//
//

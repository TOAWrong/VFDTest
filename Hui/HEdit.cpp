// HEdit.cpp : 实现文件
//
// 编辑控件，内嵌CEdit

#include "stdafx.h"
#include "Hui.h"
#include <regex>	// 正则

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// HEdit
IMPLEMENT_DYNAMIC(HEdit, HCtrl)

HEdit::HEdit(LPCRECT prc)
: HCtrl(prc)
, m_dwStyle( WS_CHILD | WS_BORDER/* | ES_CENTER*/ | ES_AUTOVSCROLL | WS_VISIBLE | ES_MULTILINE )	// 默认样式
, m_enumType( HEDITString )	// 默认普通字符串
, m_nMaxCount( 10000 )	// 默认最大100个字符
, m_nMinCount( 0 )		// 默认可以为空
, m_bTrim( TRUE )		// 默认不要前后空格
, m_bAllowNull( TRUE )	// 默认允许空
, m_wndEdit( this )		// Windows窗口编辑控件
{
}

HEdit::~HEdit()
{
}

// HEdit 成员函数
void HEdit::draw( CDC* pDC, CRect & rcClip )
{
	// 不画背景，由CEdit控件绘制
	pDC->FillSolidRect( m_rcClient, RGB(0,0,0) );
	HCtrl::draw( pDC, rcClip );
}

//重排。
void HEdit::layerout()
{
	HCtrl::layerout();

	if( m_wndEdit.m_hWnd )	// 已经创建则移动
		m_wndEdit.MoveWindow( m_rcWnd );
	else	// 未创建，则先创建
	{
		m_wndEdit.Create( m_dwStyle, m_rcWnd, m_pWnd, m_nID );
		CFont * pfont = getFont();
		if( pfont )
			m_wndEdit.SetFont( pfont );	//?? 不能动态变字体

		m_wndEdit.SetWindowText( m_strText );
	}
}

void HEdit::setText( CString txt )
{
	 HCtrl::setText( txt ); //不重画
	if( m_wndEdit.m_hWnd )	// 已经创建则设置
		m_wndEdit.SetWindowText( txt );
}

CString HEdit::getText()
{
	ASSERT( m_wndEdit.m_hWnd );

	CString str;
	CString msg;
	m_wndEdit.GetWindowText( str );
	if( m_bTrim )
		str.Trim();

	if( m_enumType != HEDITString )
		verify( str );

	return str;
}

void HEdit::verify( CString & str )
{
	CString msg;
	int len = str.GetLength();

	if( !len && m_bAllowNull )	// 允许为空
		return;

	std::wregex rxMobile(L"^1\\d{10}$");	// 手机号正则
	std::wregex rxDate(L"^(?:(?!0000)[0-9]{4}(?:(?:0[1-9]|1[0-2])(?:0[1-9]|1[0-9]|2[0-8])|(?:0[13-9]|1[0-2])(?:29|30)|(?:0[13578]|1[02])31)|(?:[0-9]{2}(?:0[48]|[2468][048]|[13579][26])|(?:0[48]|[2468][048]|[13579][26])00)0229)$");	// 日期：yyyymmdd
	std::wregex rxIdentity(L"^(\\d{15}$|^\\d{18}$|^\\d{17}(\\d|X|x))$");		// 15或18位身份证号
	switch( m_enumType )
	{
	case HEDITLength:
		if( len > m_nMaxCount )
		{
			msg.Format( _T("最多 %i 个字符。"), m_nMaxCount );
			throw new HVerifyException( this, msg );
		}
		else if( len < m_nMinCount )
		{
			msg.Format( _T("最少 %i 个字符。"), m_nMinCount );
			throw new HVerifyException( this, msg );
		}

		break;
/*
	case HEDITMobile:
		if( !regex_match((PCTSTR)str, rxMobile) )
			throw new HVerifyException( this, _T("请正确填写手机号。") );
		break;

	case HEDITDate:
		if( !regex_match((PCTSTR)str, rxDate) )
			throw new HVerifyException( this, _T("请正确填写生日。") );
		break;

	case HEDITIdentity:
		if( !regex_match((PCTSTR)str, rxIdentity) )
			throw new HVerifyException( this, _T("请正确填写身份证号。") );
		break;
*/
	default:
		//break;
		throw new HVerifyException( this, _T("无效的编辑框类型") );
	}
}

void HEdit::Enable( BOOL bEnable )
{
	ASSERT( m_wndEdit.m_hWnd );

	m_bEnabled = bEnable;	// 不重画
	m_wndEdit.EnableWindow( bEnable );
}

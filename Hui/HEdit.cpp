// HEdit.cpp : ʵ���ļ�
//
// �༭�ؼ�����ǶCEdit

#include "stdafx.h"
#include "Hui.h"
#include <regex>	// ����

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// HEdit
IMPLEMENT_DYNAMIC(HEdit, HCtrl)

HEdit::HEdit(LPCRECT prc)
: HCtrl(prc)
, m_dwStyle( WS_CHILD | WS_BORDER/* | ES_CENTER*/ | ES_AUTOVSCROLL | WS_VISIBLE | ES_MULTILINE )	// Ĭ����ʽ
, m_enumType( HEDITString )	// Ĭ����ͨ�ַ���
, m_nMaxCount( 10000 )	// Ĭ�����100���ַ�
, m_nMinCount( 0 )		// Ĭ�Ͽ���Ϊ��
, m_bTrim( TRUE )		// Ĭ�ϲ�Ҫǰ��ո�
, m_bAllowNull( TRUE )	// Ĭ�������
, m_wndEdit( this )		// Windows���ڱ༭�ؼ�
{
}

HEdit::~HEdit()
{
}

// HEdit ��Ա����
void HEdit::draw( CDC* pDC, CRect & rcClip )
{
	// ������������CEdit�ؼ�����
	pDC->FillSolidRect( m_rcClient, RGB(0,0,0) );
	HCtrl::draw( pDC, rcClip );
}

//���š�
void HEdit::layerout()
{
	HCtrl::layerout();

	if( m_wndEdit.m_hWnd )	// �Ѿ��������ƶ�
		m_wndEdit.MoveWindow( m_rcWnd );
	else	// δ���������ȴ���
	{
		m_wndEdit.Create( m_dwStyle, m_rcWnd, m_pWnd, m_nID );
		CFont * pfont = getFont();
		if( pfont )
			m_wndEdit.SetFont( pfont );	//?? ���ܶ�̬������

		m_wndEdit.SetWindowText( m_strText );
	}
}

void HEdit::setText( CString txt )
{
	 HCtrl::setText( txt ); //���ػ�
	if( m_wndEdit.m_hWnd )	// �Ѿ�����������
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

	if( !len && m_bAllowNull )	// ����Ϊ��
		return;

	std::wregex rxMobile(L"^1\\d{10}$");	// �ֻ�������
	std::wregex rxDate(L"^(?:(?!0000)[0-9]{4}(?:(?:0[1-9]|1[0-2])(?:0[1-9]|1[0-9]|2[0-8])|(?:0[13-9]|1[0-2])(?:29|30)|(?:0[13578]|1[02])31)|(?:[0-9]{2}(?:0[48]|[2468][048]|[13579][26])|(?:0[48]|[2468][048]|[13579][26])00)0229)$");	// ���ڣ�yyyymmdd
	std::wregex rxIdentity(L"^(\\d{15}$|^\\d{18}$|^\\d{17}(\\d|X|x))$");		// 15��18λ���֤��
	switch( m_enumType )
	{
	case HEDITLength:
		if( len > m_nMaxCount )
		{
			msg.Format( _T("��� %i ���ַ���"), m_nMaxCount );
			throw new HVerifyException( this, msg );
		}
		else if( len < m_nMinCount )
		{
			msg.Format( _T("���� %i ���ַ���"), m_nMinCount );
			throw new HVerifyException( this, msg );
		}

		break;
/*
	case HEDITMobile:
		if( !regex_match((PCTSTR)str, rxMobile) )
			throw new HVerifyException( this, _T("����ȷ��д�ֻ��š�") );
		break;

	case HEDITDate:
		if( !regex_match((PCTSTR)str, rxDate) )
			throw new HVerifyException( this, _T("����ȷ��д���ա�") );
		break;

	case HEDITIdentity:
		if( !regex_match((PCTSTR)str, rxIdentity) )
			throw new HVerifyException( this, _T("����ȷ��д���֤�š�") );
		break;
*/
	default:
		//break;
		throw new HVerifyException( this, _T("��Ч�ı༭������") );
	}
}

void HEdit::Enable( BOOL bEnable )
{
	ASSERT( m_wndEdit.m_hWnd );

	m_bEnabled = bEnable;	// ���ػ�
	m_wndEdit.EnableWindow( bEnable );
}

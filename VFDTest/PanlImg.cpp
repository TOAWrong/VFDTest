// PanlImg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "VFDTest.h"
#include "PanlImg.h"
#include <math.h>
#include "ini.h"

// PanlImg

IMPLEMENT_DYNAMIC(PanlImg, HCtrl)

BEGIN_MESSAGE_MAP(PanlImg, HCtrl)
	ON_COMMAND_RANGE(TID_GUIDER1, TID_GUIDER1+10000, OnGuiderBtn)	// ������У��
END_MESSAGE_MAP()

PanlImg::PanlImg(LPCRECT prc) : HCtrl(prc)
{
	setBackColor( RGB(255, 255, 255) );

	setBtnImage( IDB_BTN69X57, CSize( 69, 57 ) );	// ÿ��ɨ�贰���½ǵİ�ťͼƬ

	m_nRowNumber = 8;
	m_nColumnNumber = 4;
}

PanlImg::~PanlImg()
{
}


//���㶥���ؼ�λ�á�
void PanlImg::layerout()
{
	int d = 0;	// ���5��
	int w = (m_rcClient.right - m_rcClient.left)/m_nColumnNumber - d;	// ͼ��ؼ����
	float fh = (float)(m_rcClient.bottom - m_rcClient.top - (m_nRowNumber-1) * d)/m_nRowNumber;	// ͼ��ؼ��߶�, ���������ڲ�ֵ
	int ih = (int)fh;	// ͼ��ؼ��߶� ����
	for( int i = 0, id = 0; i < m_nRowNumber; i++ )
	{
		int n = (int)ceil( i * (fh + d) );	// ����ȡ��
		for( int j = 0; j < m_nColumnNumber; j++, id++ )
		{
			int l = (w +d)*j;// + d;
			m_arChildren[id]->setRect( CRect( l, n, l+w, n+ih ), false );
		}
	}
	HCtrl::layerout();
}

// ��ʼ��4��8��
void PanlImg::init()
{
	removeAllChildren();

	CString strPath;
	strPath.Format(_T("%s//Config.ini"),theApp.m_szAppPath);// �����ļ���

	CIni ini(strPath);

	int iParts = ini.GetIntValue(_T("DISP"),_T("PARTS"));
	int iPartNumbers = ini.GetIntValue(_T("DISP"),_T("PARTNUMBERS"));

	CString strCom,strPart;
	int n = 0;
	strCom.Format(_T("COM[%i]"),n);
	strPart.Format(_T("PART[%i]"),n);
	int iPartBegin = ini.GetIntValue(_T("COM"),strPart);

	int iNoBegin = (iPartBegin - 1) * iPartNumbers + 1;
	int iNoEnd = (iParts + iPartBegin - 1) * iPartNumbers + 1;

	// ��ʾ32��ͼ��4��8��
	for( int i = 0, id = iNoBegin; i < m_nRowNumber; i++ )
	{
		for( int j = 0; j < m_nColumnNumber, id < iNoEnd; j++, id++ )
		{
			m_iv[id] = new ImgView( CRect(), id );	// ��layerout()�г�ʼ��λ��
			addCtrl( m_iv[id] );
		}
	}
//	layerout(); // �ڴ�������֮ǰ����Ҫ���ö����ؼ�λ�����ã��������ִ���m_pWndΪ�յĶ��Դ���
}

// �Զ���Ǵ������Զ�����ON_COMMAND�¼�
void PanlImg::OnGuiderBtn(UINT nID)
{
	CString str;
	str.Format( _T("View %i"), nID - TID_GUIDERSTART );
	AfxMessageBox( str );
}

// PanlImg.cpp : 实现文件
//

#include "stdafx.h"
#include "VFDTest.h"
#include "PanlImg.h"
#include <math.h>
#include "ini.h"

// PanlImg

IMPLEMENT_DYNAMIC(PanlImg, HCtrl)

BEGIN_MESSAGE_MAP(PanlImg, HCtrl)
	ON_COMMAND_RANGE(TID_GUIDER1, TID_GUIDER1+10000, OnGuiderBtn)	// 不均匀校正
END_MESSAGE_MAP()

PanlImg::PanlImg(LPCRECT prc) : HCtrl(prc)
{
	setBackColor( RGB(255, 255, 255) );

	setBtnImage( IDB_BTN69X57, CSize( 69, 57 ) );	// 每个扫描窗口下角的按钮图片

	m_nRowNumber = 8;
	m_nColumnNumber = 4;
}

PanlImg::~PanlImg()
{
}


//计算顶级控件位置。
void PanlImg::layerout()
{
	int d = 0;	// 间隔5磅
	int w = (m_rcClient.right - m_rcClient.left)/m_nColumnNumber - d;	// 图像控件宽度
	float fh = (float)(m_rcClient.bottom - m_rcClient.top - (m_nRowNumber-1) * d)/m_nRowNumber;	// 图像控件高度, 浮点数用于插值
	int ih = (int)fh;	// 图像控件高度 整数
	for( int i = 0, id = 0; i < m_nRowNumber; i++ )
	{
		int n = (int)ceil( i * (fh + d) );	// 向上取整
		for( int j = 0; j < m_nColumnNumber; j++, id++ )
		{
			int l = (w +d)*j;// + d;
			m_arChildren[id]->setRect( CRect( l, n, l+w, n+ih ), false );
		}
	}
	HCtrl::layerout();
}

// 初始化4行8列
void PanlImg::init()
{
	removeAllChildren();

	CString strPath;
	strPath.Format(_T("%s//Config.ini"),theApp.m_szAppPath);// 程序文件夹

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

	// 显示32幅图像，4行8列
	for( int i = 0, id = iNoBegin; i < m_nRowNumber; i++ )
	{
		for( int j = 0; j < m_nColumnNumber, id < iNoEnd; j++, id++ )
		{
			m_iv[id] = new ImgView( CRect(), id );	// 到layerout()中初始化位置
			addCtrl( m_iv[id] );
		}
	}
//	layerout(); // 在创建窗口之前不需要调用顶级控件位置设置，否则会出现窗口m_pWnd为空的断言错误
}

// 自定义非窗口类自动处理ON_COMMAND事件
void PanlImg::OnGuiderBtn(UINT nID)
{
	CString str;
	str.Format( _T("View %i"), nID - TID_GUIDERSTART );
	AfxMessageBox( str );
}

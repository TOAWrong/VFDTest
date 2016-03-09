
#pragma once

#include "Hui.h"
#include "ImgView.h"
// PanlImg ƴ��ͼ���б�

class PanlImg : public HCtrl
{
	DECLARE_MESSAGE_MAP()
	DECLARE_DYNAMIC(PanlImg)

// Construction
public:
	PanlImg(LPCRECT prc);
	virtual ~PanlImg();
	ImgView *m_iv[1000];

// Properties
private:
	int m_nRowNumber;			// ͼ�񴰿�����
	int m_nColumnNumber;		// ͼ�񴰿�����
public:	//���Զ�ȡ������
	int getRowNumber() { return m_nRowNumber; }
	void setRowNumber( int v ){m_nRowNumber = v; }
	int getColumnNumber() { return m_nColumnNumber; }
	void setColumnNumber( int v ) { m_nColumnNumber = v; }

// Methods
public:
	void init();		// ��ʼ��2��4�У�����ɨ�跽ʽ

// Overwrite
	virtual void layerout();		//���ſؼ�

// Messages
public:
	afx_msg void OnGuiderBtn( UINT nID );	// �Զ���Ǵ������Զ�����ON_COMMAND�¼�
};



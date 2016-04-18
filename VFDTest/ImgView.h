
#pragma once

#include "Hui.h"


// ImgView ƴ��ͼ���б�

class ImgView : public HCtrl
{
	DECLARE_DYNAMIC(ImgView)
	DECLARE_MESSAGE_MAP()

// Construction
public:
	ImgView(LPCRECT prc, int ID);	// �������ID�ų�ʼ���ؼ�
	ImgView(LPCRECT prc);	// �������ʼ���ؼ�
	virtual ~ImgView();

// Overflow
public:
	virtual void draw( CDC* pDC, CRect & rcClip );	// ���ͻ���
	virtual void OnLButtonDown( UINT nFlags, CPoint point );	// ����true��ʾ�Ѿ�����
	virtual void	layerout();					//���ſؼ�

// Properties
public:
	HLabel *		m_pText;		// �м�ʵʱ������ʾ
	HLabel *		m_pPrompt;		// ���Ͻ���ʾ����״̬
	HLabel *		m_pNo;			// ���½���ʾ���
	HLabel *		m_pTime;		// ���Ͻ���ʾ������ʱ��
protected:
	int				m_nPos;			// ��ƴ��ͼ�����λ�ã�����ƴʱ�𽥼�С������ƴʱ�𽥼Ӵ�, -2��ʾ������ƴ��ͼ��-1��ʾ���Խ���ͼ�񣬵�ǰͼ���ǿա�
	HBtn *			m_pPosBtn;		// ������ť

public:
	static ImgView*	m_pJoinView;	// �ͼ�񴰿�, ����ƴ��

// Methods
public:
	void save( CString & sdid );			// ��sdid�б���ͼ��

protected:

// Messages
public:
	afx_msg void OnAbout();	//�Զ���Ǵ������Զ�����ON_COMMAND�¼�
};



#pragma once

// HWndEdit

class HEdit;

class HWndEdit : public CEdit
{
	DECLARE_DYNAMIC(HWndEdit)
	DECLARE_MESSAGE_MAP()

// Construction:
public:
	HWndEdit( HEdit * p );
	virtual ~HWndEdit();

// Properties:
protected:
	BOOL m_bMouseTracking;	// ���������ͣ���뿪�¼�
	HEdit * m_pctrEdit;

// Methods:
	CPoint clientToMain( const CPoint & point );	// �ѱ༭���ڵĵ�ת����������

// Messages:
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg void OnMouseHover(UINT nFlags, CPoint point);
};



#pragma once
#include "afxwin.h"


// CDlgSet dialog

class CDlgSet : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgSet)

public:
	CDlgSet(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgSet();

// Dialog Data
	enum { IDD = IDD_SET };
	void GetCom();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int m_iTotalTime;
	int m_iComTime;
	int m_iWriteTime;
	int m_iStartMethod;
	int m_iCtrlCom;
	int m_iMeterCom;
	CButton m_autoStart;
	CComboBox m_cbMeterCom;
	CComboBox m_cbCtrlCom;
	afx_msg void OnClickedCheckAutostart();
	virtual BOOL OnInitDialog();
	CComboBox m_cbStartMethod;
	CString m_strMeterCom;
	CString m_strCtrlCom;
};

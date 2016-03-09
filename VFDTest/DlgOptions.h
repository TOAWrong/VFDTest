#pragma once
#include "afxwin.h"


// DlgOptions 对话框

class DlgOptions : public CDialogEx
{
// Constraction
	DECLARE_DYNAMIC(DlgOptions)
	DECLARE_MESSAGE_MAP()
public:
	DlgOptions(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~DlgOptions();

	void InitEditStatus();

	int m_iParts;

	void EnumerateSerialPorts(CUIntArray& ports, CUIntArray& portse, CUIntArray& portsu);
	void AddCom();
// 对话框数据
	enum { IDD = IDD_OPTIONS };

// 重建
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	virtual BOOL OnInitDialog();

// Properties
public:

	CUIntArray ports, portse, portsu;

	// 扫描图的行数
	int m_rownumber;
	// 扫描图列数
	int m_columnsnumber;

	int	m_iAutoStart;	// 不为零时，开机自启动

	BOOL ComTest( CString strPort );

	void InitComBoBox( CComboBox &cb, int i );

	int m_iCom[5];

// Messages
public:
	int m_iCom0;
	int m_iCom1;
	int m_iCom2;
	int m_iCom3;
	int m_iPart0;
	int m_iPart1;
	int m_iPart2;
	int m_iPart3;
	int m_iComTime;
	int m_iTotalTime;
	int m_iWriteTime;
	int m_iPartNumbers;
	int m_iStartMethod;
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio3();
	afx_msg void OnBnClickedRadio4();
	int m_iCtrlCom;
	CComboBox m_cbStartMethod;
	afx_msg void OnSelchangeComboStartmethod();
	CButton m_AutoStart;
	CComboBox m_cbCom1;
	CComboBox m_cbCom2;
	CComboBox m_cbCom3;
	CComboBox m_cbCom4;
	CComboBox m_cbCtrlCom;
	CString m_strCom1;
	CString m_strCom2;
	CString m_strCom3;
	CString m_strCom4;
	CString m_strCtrlCom;
	afx_msg void OnClickedCheckAutostart();
	afx_msg void OnSelchangeComboCom1();
	afx_msg void OnSelchangeComboCom2();
	afx_msg void OnSelchangeComboCom3();
	afx_msg void OnSelchangeComboCom4();
	afx_msg void OnSelchangeComboCtrlcom();
};

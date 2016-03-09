// ����һ���㶨�����MFC�࣬ʹ�÷������¡�
// 1 �½����ĵ�MFC����
// 2 ��Hui.h�ļ���Hui.lib�ļ����Ƶ�Դ�ļ�����
// 3 ��Hui.dll�ļ����Ƶ�exe�ļ�����
// 4 ����Ŀ����-��������-������-�������������м���Hui.lib��������б�����÷ֺŸ��롣
// 5 �޸������ڴ�HWnd�̳С�ɾ��ԭ����view���ڡ�

#pragma once

#include "HWndEdit.h"

// Note: This sample uses AFX_EXT_CLASS to export an entire class.
// This avoids creating a .DEF file with all the decorated names for
// the class.  Creating a .DEF file is more efficient since the names
// can be exported by ordinal.  To use that method of exporting, much
// like MFC does itself, uncomment the following two lines and add
// the appropriate lines to your .DEF file.
//#undef AFX_DATA
//#define AFX_DATA AFX_EXT_DATA

#define HOVERTIME 500	// ��ͣʱ��500ms

/////////////////////////////////////////////////////////////////////////////
// �Զ���֪ͨ����
typedef struct tagHNM{
	NMHDR hdr;
	LPARAM lParam;
	LRESULT result;	// ����ֵ
	tagHNM( CWnd * pWnd, UINT id, LPARAM param, BOOL send = TRUE )	//  CWnd * pWnd ����, UINT id ֪ͨID, LPVOID p ��Ӳ���, send == TRUE ʱ������Ϣ��
	{
		ASSERT(::IsWindow(pWnd->m_hWnd));
		hdr.hwndFrom = pWnd->m_hWnd;		// ���ھ��
		hdr.idFrom = pWnd->GetDlgCtrlID();	// ����ID
		hdr.code = id;						// ֪ͨID
		lParam = param;						// �Զ������
		result = 0;

		if( send )
			result = pWnd->SendMessage( WM_NOTIFY, 0, (LPARAM)this );
	}
} HNM, FAR *PHNM;


/////////////////////////////////////////////////////////////////////////////
// HCtrl �Զ���ؼ�����ʹ�ô���, ��CCmdTarget���Խ���������Ϣ��
/*�Զ����ദ��������Ϣ������hWnd��Ҳ���ô��ڣ���CDoc�ࡣҪ�����£�
 1. ������뺬 CCmdTarget
 2. ��DECLARE_MESSAGE_MAP()��BEGIN_MESSAGE_MAP
 3. �������ڵ�OnCmdMsg��Ϣ�м����´���
BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
    // ���Զ������һ�γ��Ը����������һ�䣬�ղ�������
    if (m_pHCtrl->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
        return TRUE;
 
    // ����ִ��Ĭ�ϴ���
    return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}
*/

#define HDOCK_NORMAL			0	// �ؼ�������ʽ����ͣ��
//#define HDOCK_LEFT			1	// ����m_nMarginLeft��������ľ���
//#define HDOCK_TOP				2	// ������m_nMarginTop���������ľ���
#define HDOCK_RIGHT				4	// ���ң�m_nMarginRight�������ұߵľ��룬�����m_rcClient����
#define HDOCK_BOTTOM			8	// ���ף�m_nMarginBottom�������ױߵľ��룬�߶���m_rcClient����
#define HDOCK_FULLHORIZONTAL	16	// ˮƽ������m_nMarginLeft��������ľ��룬m_nMarginRight�������ұߵľ��룬m_rcClient��left��right��Ч
#define HDOCK_FULLVERTICAL		32	// ��ֱ������m_nMarginTop���������ľ��룬m_nMarginBottom�������ױߵľ��룬m_rcClient��top��bottom��Ч

#define DEFAULTDPI 96		// Ĭ����ʾ��dpi, ����1/DEFAULTDPIΪ1pt(��)

class HCmdUI;


/////////////////////////////////////////////////////////////////////////////
// HCtrl ���пؼ�����
class AFX_EXT_CLASS HCtrl : public CCmdTarget
{
protected:
	DECLARE_DYNCREATE(HCtrl)	// ��ȡ����

public:

// ����
protected:
public:
	HCtrl();
	HCtrl(LPCRECT prc);	// ����ӿؼ���HCtrl * pParent�����ؼ���LPCRECT prc: �ؼ�����
	virtual ~HCtrl();
	virtual void initCtrl();	// ���ڽ�����Ϻ��ʼ������û����ʾ����

// Properties
public:
	//class HTips;
	// ����Ŀ�����á��ڹ��� DLL ��ʹ�� MFC����������������Ŀ����ʹ�����¾�̬����������ϵͳ��̬������
	static LPCTSTR	m_pszCursor;		// ����ꡣ
	static HCtrl *	m_pctrTips;			// ������ʾ�ؼ���
	static HCtrl *	m_pctrCapture;		// �������ؼ���
	static HCtrl *	m_pctrMouseEnter;	// ��ǰ���λ�ÿؼ���
	static int		m_nDpiX;			// ��ʾ��X����dpi
	static int		m_nDpiY;			// ��ʾ��Y����dpi
	static CWnd *	m_pWnd;				// �ؼ����ڵĴ��ڡ�
	//static CRect	m_rcWndClient;		// �����û�����С��
	static COLORREF m_cTestColor;		// ǿ����ʾ�ؼ�����ɫ�������ж��ػ������Ƿ�����Ч�������ã�Ϊ0ʱ����ʾ��

	void setTestColor(){ m_cTestColor = (m_cTestColor ? m_cTestColor += 0x111111 : 0x204080) & 0xffffff; }	// ���ܼӳ���

	//void setTopWnd( CWnd * pWnd )
	//{m_pTopWnd = pWnd;}	// ���ö����ڣ�����֪ͨ��Ϣ��

private:
		CFont			m_font;			// ���壬���ó�˽�еģ�ֻ����getFont����

protected:
	CMFCToolBarImages*	m_pImglist;	// ͼ����
	//BOOL			m_bMouseEnter;	// ������û�����
	CArray<HCtrl*>	m_arChildren;	// �����ӿؼ�
	CRect			m_rcClient;		// �û���������ڸ��ؼ�, ��λpt��
	CRect			m_rcWnd;		// �û������������꣬��λ���ء�
	int				m_nID;			// �ؼ�ID
	LPCTSTR			m_lpszCursor;	// �����״, Ϊ0ʱ����Ӧ����¼�
	HCtrl *			m_pParent;		// ���ؼ�
	UINT			m_nDock;		// ͣ����ʽ
	CString			m_strText;		// �ؼ�����
	CImage			m_imgBackground;// ����ͼ������б���ͼ�Ͳ��ñ���ɫ //?? ����CPngImage pngImage;if (pngImage.Load(lpszResourceName, 0/*hinstRes*/)){hbmp = (HBITMAP) pngImage.Detach();}
	CBrush			m_brBackground;	// ����ˢ�ӣ�����ɫ
	CPen			m_pnBorder;		// �߿��
	int				m_nAlign;		// ���뷽ʽ
	COLORREF		m_coText;		// �ı���ɫ
	COLORREF		m_coDisabled;	// ��ֹʱ�ı���ɫ
	CString			m_strTips;		// ��ͣ��ʾ����
	BOOL			m_bEnabled;		// ������
	int				m_nCheck;		// ѡ��״̬��1ѡ�У�����δѡ��
	//int				m_nMarginLeft;	// ˮƽ����ʱ����ߵľ���
	int				m_nMinWidth;	// ��С��ȣ�ˮƽ����ʱ��Ч
	int				m_nMarginRight;	// ���һ�ˮƽ����ʱ���ұߵľ���
	//int				m_nMarginTop;	// ��ֱ����ʱ�����ߵľ���
	int				m_nMinHeight;	// ��С�߶ȣ�����ʱ��Ч
	int				m_nMarginBottom;// ���׻�ֱ����ʱ���ױߵľ���

public:
	BOOL isEnabled(){ return m_bEnabled; }
	void setID( int id ){ m_nID = id; }
	int getID(){ return m_nID; }
	void setTextColor( COLORREF color ){ m_coText = color; }

// ��Ϣ����
public:
	virtual BOOL OnMouseMove( UINT nFlags, const CPoint &point );	// ����TRUE��ʾ�Ѿ�����
	virtual void mouseMove( UINT /*nFlags*/, const CPoint &/*point*/ ){}	// ����ƶ���ֻ��������ڵ����տؼ����յ���
	virtual void lbuttonDown( UINT /*nFlags*/, const CPoint &/*point*/ ){m_pctrCapture = this; /*�������*/}
	virtual void lbuttonDblClk( UINT /*nFlags*/, const CPoint &/*point*/ ){m_pctrCapture = this; /*�������*/}
	virtual void lbuttonUp( UINT /*nFlags*/, const CPoint &/*point*/ ){m_pctrCapture = 0;}
	virtual void mouseEntered( UINT /*nFlags*/, const CPoint &/*point*/ ){ m_pszCursor = m_lpszCursor; }
	virtual void mouseLeave(){m_pctrMouseEnter = 0, m_pszCursor = 0, m_pctrCapture = 0; }	// ����뿪��m_pctrCapture = 0ԭ��lbuttonUp�¼������ã�����ʱlbuttonUp�¼�ǰ����mouseLeave�¼����Ժ��û��lbuttonUp�¼��ˣ��������½��ȿ��ư�ť����ʱ�����Ի���ϵͳ�ѽ����Ƶ��¶Ի����У��Ժ��ٻص�ttmcaʱ�������ã���һ�²źá�
	virtual void mouseHover( UINT /*nFlags*/, const CPoint &/*point*/ );	// �����ͣ
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	virtual BOOL OnWndMsg(UINT /*message*/, WPARAM /*wParam*/, LPARAM /*lParam*/, LRESULT* /*pResult*/);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

// Methods
protected:
	CMFCToolBarImages * getImglist();
	CFont * getFont();	// �����ǰ��m_font��ȡ��ǰ�ģ�����ȡ�����ġ�
	void rcToWnd( CRect &rc );	// ת����Wnd����, ԭ�����Ͻǣ���λ����
	void rcToClient( CRect &rc );	// ת����UI����, ԭ�����Ͻǣ���λ��
	void initParameter();				// ������ʼ��

public:
	virtual BOOL preDraw( CDC* pDC, CRect & rcClip );	// Ԥ����ͼ�¼����Ȼ��ײ�
	virtual void draw( CDC* pDC, CRect & rcClip );	// ���ͻ���

	virtual CString	getText(){ return m_strText; }
	virtual void	setText(CString txt){ m_strText = txt; invalidate(); }
	virtual void	setTextRes( UINT nID ){ m_strText.LoadString( nID ); invalidate(); }
	virtual void	setBackColor( COLORREF col );
	virtual void	setBackImg( PCTSTR idResource, PCTSTR lpType );
	virtual void	setBorderColor( COLORREF col );
	virtual void	layerout();					// ���ſؼ�
	virtual void	Enable( BOOL bEnable ){ if( m_bEnabled == bEnable) return; m_bEnabled = bEnable; invalidate(); }	// bEnable Ϊ��ʱ����
	virtual int		getChecked(){ return m_nCheck; }
	virtual void	setChecked( int nCheck ){ if( m_nCheck == nCheck) return; m_nCheck = nCheck; invalidate(); }
	virtual void	setFocus(){}
	virtual void invalidate();	// �ػ��ؼ�����
	virtual void added(){}	// ������

	void addCtrl( HCtrl * pCtrl );	// m_arChildren����ӿؼ�
	CRect rcToWnd();	// ��ǰRCת����Wnd����
	CRect rcToClient();	// �ѵ�ǰ��Wnd����ת����UI����, ԭ�����Ͻǣ���λ��
	void setDock(UINT nDock, int nMarginRight = 0, int nMarginBottom = 0, int nMinWidth = 0, int nMinHeight = 0);	//UINT nDock ͣ����ʽ��HDOCK_X, int nMarginRight �ұ߼�϶, int nMarginBottom �ױ߼�϶, int nMinWidth ��С���, int nMinHeight ��С�߶�
	void setBtnImage(UINT nImg, const CSize & sizeImage );			//UINT nImg���ؼ�ͼ����ԴID�ţ��ɼ̳е��ӿؼ�, ����ͼ��ߴ硣
	void setCursor(LPCTSTR lpszCursor);	//LPCTSTR lpszCursor�����ù�ֻ֧꣬��ϵͳ��꣬Ϊ0ʱ����Ӧ����¼���
	void moveTo( int x, int y );		// ���Ͻ��ƶ���ָ��λ��
	//void invalidate( HCtrl * pctrDraw, BOOL bDrawBack );// �ػ��ؼ�����HCtrl * pctrDraw ֻ�ػ�ָ���Ŀؼ���Ϊ��ʱ�ػ����еײ�ؼ�; BOOL bDrawBack Ϊ��ʱ�ػ������������ػ�������20140408 �ػ���������ȡ������
	// �Զ����¿ؼ�����
	void OnUpdateCmdUI(HCmdUI* pCmdUI,BOOL bDisableIfNoHndler);

	void setAlign( int nAlign ){ m_nAlign = nAlign; }
	BOOL setFont( int nHeight, int nWeight = 0, LPCTSTR lpszFacename = 0 );

	const CRect & getRect(){return m_rcClient;} const	// ȡ�û���
	CRect getWinRect(){return m_rcWnd;}	// ȡ������
	void setRect( CRect & rc, BOOL layout);	// �����û�����layort Ϊ�棬���ſؼ����������š�
	
	CArray<HCtrl*> * getChildren(){ return &m_arChildren; }	// �����ӿؼ�

	void removeAllChildren();	// ɾ��ȫ���ӿؼ�
	void removeChildrenAt( int i );	// ɾ��ָ�����ӿؼ���i �ؼ�������

	BOOL Intersect( CRect & rcClip );	// �ػ����ǲ����ཻ���ཻ����TRUE�����򷵻�FALSE

	void setTips( PCTSTR str ){ m_strTips = str; }
	BOOL getTextSize( CRect & rc );	// ȡ�ؼ���ǰ�ı��ĳߴ�
};


/////////////////////////////////////////////////////////////////////////////
// HCmdUI idle update through HCmdUI class
class AFX_EXT_CLASS HCmdUI : public CCmdUI
{
public:
	HCmdUI();

	virtual void Enable(BOOL bOn);
	virtual void SetCheck(int nCheck);   // 0, 1 or 2 (indeterminate)
	virtual void SetText(CString lpszText);
	//virtual void SetRadio(BOOL bOn = TRUE);

	HCtrl* m_pUpdated;
};


/////////////////////////////////////////////////////////////////////////////
// HLabel ������ʾ
class AFX_EXT_CLASS HLabel : public HCtrl
{
// ����
protected:
	//HLabel();
public:
	HLabel(LPCRECT prc);	// ����ӿؼ���HCtrl * pParent�����ؼ���LPCRECT prc: �ؼ�����, int idup̧���ͼ��ID, int iddown���µ�ͼ��ID��
	virtual ~HLabel();

	DECLARE_DYNCREATE(HLabel)

// Properties
protected:
	COLORREF m_coShadow;	// ��Ӱ��ɫ
	BOOL m_bShadow;
	BOOL m_bAutoSize;	// ���������Զ������ߴ�

public:
	void setAutoSize( BOOL bAutoSize ){m_bAutoSize = bAutoSize;}

// Methods
private:
	void drawText( CDC* pDC );
public:
	void setShadowColor( COLORREF color );

// Overwrite
public:
	virtual void draw( CDC* pDC, CRect & rcClip );	// ���ͻ���
	virtual void setText(CString txt);
	virtual void setTextRes( UINT nID );
	virtual void initCtrl();	// ���ڽ�����Ϻ��ʼ������û����ʾ����
};

/////////////////////////////////////////////////////////////////////////////
// HEdit �༭�ؼ�����ǶMFC CEdit
class HException;
class HVerifyException : public CException	// ��֤�쳣
{
// Constrction
public:
	HVerifyException( HCtrl * pctrl, PCTSTR msg)	// HCtrl * pctrl �ؼ�, PCSTR msg ��Ϣ
		: m_strMessage( msg )
		, m_pCtrl( pctrl )
	{
	}
	//virtual ~HVerifyException(){};

// Overwrite
public:
	virtual BOOL GetErrorMessage( LPTSTR lpszError, UINT nMaxError, PUINT /*pnHelpContext = NULL*/ )
	{
		Checked::tcscpy_s( lpszError, nMaxError, m_strMessage );
		return TRUE;
	}

// Methods
public:
	void setFocus(){ if(m_pCtrl) m_pCtrl->setFocus(); }

// Properties
protected:
	HCtrl * m_pCtrl;
	CString m_strMessage;
};

class AFX_EXT_CLASS HEdit : public HCtrl
{
public:
	//�ؼ�����
	enum HEditType 
	{ 
		HEDITString,		// ��ͨ�ַ���������֤
		HEDITLength,		// �޶�����
		HEDITMobile,		// �й��ֻ���
		HEDITIdentity,		// �й����֤��
		//HEDITEmail,			// ����
		HEDITDate,			// ���� yyyymmdd
	}; 


// ����
protected:
	//HEdit();
public:
	HEdit(LPCRECT prc);	// ����ӿؼ���HCtrl * pParent�����ؼ���LPCRECT prc: �ؼ�����, int idup̧���ͼ��ID, int iddown���µ�ͼ��ID��
	virtual ~HEdit();

	DECLARE_DYNCREATE(HEdit)

// Properties
protected:

	DWORD m_dwStyle;		// MFC Edit�ؼ���ʽ��Ĭ��ΪWS_CHILD | WS_BORDER/* | ES_CENTER*/ | ES_AUTOHSCROLL | WS_VISIBLE
	HEditType m_enumType;	// ��������
	int m_nMaxCount;		// ����ַ���
	int m_nMinCount;		// ��С�ַ���
	BOOL m_bTrim;			// ��Ҫǰ��ո�
	BOOL m_bAllowNull;		// �����

public:
	HWndEdit m_wndEdit;		// MFC Edit�ؼ�
// Methods
private:
	//void drawText( CDC* pDC );
	virtual void	verify( CString & str );

public:
	void setStyle( DWORD dwStyle ){ m_dwStyle = dwStyle; }
	void setType( HEditType type ){ m_enumType = type, m_bAllowNull = FALSE; }
	void setMaxMin( int min, int max ){ m_nMaxCount = max, m_nMinCount = min, m_enumType = HEDITLength, m_bAllowNull = FALSE; }	// ͬʱ�ѿؼ�������Ϊ�޶�����
	void setTrim( BOOL trim ){ m_bTrim = trim; }
	void setAllowNull( BOOL bAllowNull ){ m_bAllowNull = bAllowNull; }

// Overwrite
public:
	virtual void	draw( CDC* pDC, CRect & rcClip );	// ���ͻ���
	virtual void	layerout();		//����
	virtual void	setText(CString txt);
	virtual CString	getText();
	virtual void	Enable( BOOL bEnable );		// bEnable Ϊ��ʱ����
	virtual void	setFocus(){m_wndEdit.SetFocus();}
	//virtual void	mouseHover( UINT /*nFlags*/, const CPoint &/*point*/ );	// �����ͣ
};


/////////////////////////////////////////////////////////////////////////////
// HTips �����ͣ�ؼ�
class AFX_EXT_CLASS HTips : public HCtrl
{
// ����
protected:
	//HTips();
public:
	HTips();	// ����ӿؼ���HCtrl * pParent�����ؼ���LPCRECT prc: �ؼ�����, int idup̧���ͼ��ID, int iddown���µ�ͼ��ID��
	virtual ~HTips();

	DECLARE_DYNCREATE(HTips)

// Properties
protected:
	CPen m_pnBorder;

public:
	//void setBorder( COLORREF color );

// Methods
private:

public:
	void drawText(int x, int y, PCTSTR txt);
	BOOL isNull(){ return m_rcWnd.IsRectNull(); }
	void setNull(){ m_rcWnd.SetRectEmpty(); }
	void hidden();	// ������ͣ��ʾ��
};


/////////////////////////////////////////////////////////////////////////////
// HGrid ���
#define HGRID_RECERVATION_ROWS 50 // Ԥ������
class AFX_EXT_CLASS HGrid : public HCtrl
{
// Type define
public:
	// �нṹ
	struct Column 
	{
		CString title;	// �б���
		int width;		// �п�ȣ����һ���Զ��������
		UINT style;		// ��ʽ
	};

	// �м���
	struct Columns
	{
		COLORREF	textColor;		// ������������ɫ
		CBrush		m_brBackground;	// �����б�����ɫ
		int			height;			// �����и߶�
		CArray<Column>	items;			// ���⼯��
		void setHeight( int h ){ height = h; }
		void setBackColor( COLORREF color )
		{
			if( m_brBackground.m_hObject )
				m_brBackground.DeleteObject();
			m_brBackground.CreateSolidBrush( color );
		}
	};

	// ��Ԫ��
	struct Cell
	{
		CString text;	// ����
		int breaked;	// �Ƿ�ضϣ�-1 ��֪����0 ���ضϣ�1 �ض�

		Cell()
		{
			setEmpty();
		}

		void setEmpty()
		{
			text = L" ";	// ��ʼ��һ���ո񣬱��ⷴ�������ڴ档
			breaked = -1;	// -1 ��֪����0 ���ضϣ�1 �ض�
		}
	};

	typedef CArray<Cell> Cells;	// �е�Ԫ�񼯺�
	typedef CArray<Cells *> Rows;	// �м���

// ����
protected:
	//HGrid();
public:
	HGrid(LPCRECT prc);	// LPCRECT prc: �ؼ�����
	virtual ~HGrid();

	DECLARE_DYNCREATE(HGrid)

// Properties
private:
	Rows m_arRows;	// �м��ϣ�˽�г�Ա��ֻ��getRow������ѯ

protected:
	int m_nRowHeight;			// �и�
	float m_fRowHeight;			// �����и�
	CBrush m_brMouse;	// �������ɫ
	CBrush m_brSelect;// ѡ������ɫ
	int m_nRowNum;	// ����
	int m_nPadding;	// ��������
	int m_nMouse;	// �����
	CString m_strSelectID;	// ѡ����ID

public:
	Columns m_Columns;	// ���⼯��
	void setRowHeight( int h ){ m_nRowHeight = h; }

// Methods
private:
	//CPoint getPos( const CPoint & p );
	//BOOL isBreakText( const CPoint & p );

public:
	void addColumn( PCTSTR title, int width, UINT style );	// ���һ�У�PCSTR title �б���; int width �п�ȣ����һ���Զ��������; UINT sytle ��������ʽ����DrawText
	void addRow( CStringArray row );			// ���һ�У�Ҫ��row����������������ͬ
	int getColumnNumber();		// ȡ����
	int getRowNumber(){ return m_nRowNum; }			// ȡ����
	Cells * getRow( int index );			// ȡָ����
	//void setLineColor( COLORREF color );	// ���ñ������ɫ
	void setMouseColor( COLORREF color );	// ��������б�����ɫ
	void setSelectColor( COLORREF color );	// ����ѡ���б�����ɫ
	void removeAll();
	

// Overwrite:
public:
	virtual void draw( CDC* pDC, CRect & rcClip );	// ���ͻ���
	virtual void layerout();		//����, ��������
	virtual void mouseMove( UINT nFlags, const CPoint & point );
	virtual void lbuttonDown( UINT nFlags, const CPoint & point );
	virtual void mouseLeave();
	virtual void mouseHover( UINT /*nFlags*/, const CPoint &/*point*/ );	// �����ͣ
};


/////////////////////////////////////////////////////////////////////////////
// HBtn ���尴ť
class AFX_EXT_CLASS HBtn : public HCtrl
{
// ����
protected:
	//HBtn();
public:
	// ����ӿؼ���HCtrl * pParent�����ؼ���LPCRECT prc: �ؼ�����, int idoverflow �ƶ����ؼ��ϵ�ͼ��ID, int idup̧���ͼ��ID, int iddown���µ�ͼ��ID��
	HBtn(LPCRECT prc, int idoverflow, int idup, int iddown, int nID);
	virtual ~HBtn();

	DECLARE_DYNCREATE(HBtn)

// Properties
protected:
	int m_nImgup;	// ̧��ͼ��
	int m_nImgdown;	// ѹ��ͼ��
	int m_nImgover;	// ������
public:

// Methods
private:

public:
	// ����ͼ��������int idup, int iddown
	//void setImgIndex( int idup, int iddown );

	virtual void lbuttonUp( UINT nFlags, const CPoint & point );
	virtual void lbuttonDown( UINT nFlags, const CPoint & point );
	virtual void mouseEntered( UINT nFlags, const CPoint & point );
	virtual void mouseLeave();
	//virtual void mouseHover( UINT /*nFlags*/, const CPoint &/*point*/ );	// �����ͣ
	virtual void draw( CDC* pDC, CRect & rcClip );	// ���ͻ���
	//virtual void calSize();	//����λ��
};


/////////////////////////////////////////////////////////////////////////////
// Ӧ�ó��򴰿�
class AFX_EXT_CLASS HWnd : public CWnd
{
	// ���и����ָ࣬������Ϣ�����£�Ĭ������WM_MOUSEMOVE, WM_TIMER, WM_PAINT���¼�
	// ����Ƶ�����ÿؼ�״̬����
	typedef class tagIdleUpdateCmdUI
	{
	private:
		CUIntArray msgs;
		BOOL bUpdate;

	public:
		tagIdleUpdateCmdUI()
			: bUpdate(FALSE)
		{
			msgs.SetSize( 0, 10 );		// Ԥ��10���ռ�
			msgs.Add( WM_MOUSEMOVE );	// Ĭ������WM_MOUSEMOVE, WM_TIMER, WM_PAINT���¼�
			msgs.Add( WM_PAINT );
			msgs.Add( WM_TIMER );
			msgs.Add( WM_NCMOUSEMOVE );
			msgs.Add( WM_NCMOUSEHOVER );
			msgs.Add( WM_NCMOUSELEAVE );
			msgs.Add( WM_MOUSEHOVER );
			msgs.Add( WM_MOUSELEAVE );
			msgs.Add( 0x0118 ); // WM_SYSTIMER (caret blink)http://bbs.csdn.net/topics/90170047
		}

		// ���һ�������¼�
		void Add( UINT msg )
		{
			msgs.Add( msg );
		}

		// �Ƴ�һ�������¼�
		void remove( UINT msg )
		{
			for( int i = msgs.GetCount()-1; i >= 0; i-- )
			{
				if( msgs[i] == msg )
					msgs.RemoveAt( i );
			}

		}

		// �ж���Ϣ�Ƿ����Update
		// msg��ǰ��Ϣ
		// ���msg�ڼ���msgs�У������¿ؼ���
		void beforeUpdate( UINT msg )
		{
			// �����ǰ�Ǹ���״̬�����ָ���
			if( bUpdate )
				return;

			// ���������msgs�����У�������
			for( int i = msgs.GetCount() - 1; i >= 0; i-- )
				if( msgs[i] == msg )
					return;
			bUpdate = TRUE;

			//
			//TRACE( "========================== %04x", msg );
		}

		// ������ɺ󣬰�״̬��Ϊ������
		void endUpdate(){ bUpdate = FALSE; }

		// �Ƿ����
		BOOL canUpdate(){ return bUpdate; }
	};

	tagIdleUpdateCmdUI m_idle;

protected:
	DECLARE_MESSAGE_MAP()
	//DECLARE_DYNAMIC(HWnd)
	DECLARE_DYNCREATE(HWnd)

// Constrction
public:
	HWnd();
	virtual ~HWnd();

// Methods
public:
	//void trackMouseEvent(DWORD dwFlags);
	void setAccelerators( UINT id );	// �����ݼ�

// Propertis
public:
	HCtrl m_ctrFrame;	// ���ؼ����������ں󴴽����ؼ������Ǵ����û�����

protected:
	HACCEL m_hAccel;	// ��ݼ���Դ
	BOOL m_bMouseTracking;	// ΪFALSEʱ�������ͣ���뿪�¼�
	//CPoint m_ptPrevious;	// �ƶ��������ϴε����λ��

// Overwrite
protected:
	virtual void PostNcDestroy();
	virtual BOOL CreateEx(DWORD dwExStyle, LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, LPVOID lpParam = NULL);
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);

// Messages
public:
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
//	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
//	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
//	virtual BOOL OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pLResult);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//afx_msg void OnNcMouseMove(UINT nHitTest, CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg void OnMouseHover(UINT nFlags, CPoint point);
	afx_msg LRESULT OnIdleUpdateCmdUI(WPARAM wParam, LPARAM);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};

#undef AFX_DATA
#define AFX_DATA

/////////////////////////////////////////////////////////////////////////////

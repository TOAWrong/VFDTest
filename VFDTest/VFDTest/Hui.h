// 这是一个算定义界面MFC类，使用方法如下“
// 1 新建单文档MFC工程
// 2 把Hui.h文件和Hui.lib文件复制到源文件夹中
// 3 把Hui.dll文件复制到exe文件夹中
// 4 在项目属性-》链接器-》输入-》附加依赖项中加入Hui.lib，如果还有别的项用分号隔离。
// 5 修改主窗口从HWnd继承。删除原来的view窗口。

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

#define HOVERTIME 500	// 悬停时间500ms

/////////////////////////////////////////////////////////////////////////////
// 自定义通知参数
typedef struct tagHNM{
	NMHDR hdr;
	LPARAM lParam;
	LRESULT result;	// 返回值
	tagHNM( CWnd * pWnd, UINT id, LPARAM param, BOOL send = TRUE )	//  CWnd * pWnd 窗口, UINT id 通知ID, LPVOID p 外加参数, send == TRUE 时发送消息。
	{
		ASSERT(::IsWindow(pWnd->m_hWnd));
		hdr.hwndFrom = pWnd->m_hWnd;		// 窗口句柄
		hdr.idFrom = pWnd->GetDlgCtrlID();	// 窗口ID
		hdr.code = id;						// 通知ID
		lParam = param;						// 自定义参数
		result = 0;

		if( send )
			result = pWnd->SendMessage( WM_NOTIFY, 0, (LPARAM)this );
	}
} HNM, FAR *PHNM;


/////////////////////////////////////////////////////////////////////////////
// HCtrl 自定义控件，不使用窗口, 从CCmdTarget可以接收命令消息。
/*自定义类处理命令消息不必有hWnd，也不用窗口，如CDoc类。要求如下：
 1. 基类必须含 CCmdTarget
 2. 加DECLARE_MESSAGE_MAP()和BEGIN_MESSAGE_MAP
 3. 在主窗口的OnCmdMsg消息中加如下代码
BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
    // 让自定义类第一次尝试该命令，不加这一句，收不到命令
    if (m_pHCtrl->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
        return TRUE;
 
    // 否则，执行默认处理
    return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}
*/

#define HDOCK_NORMAL			0	// 控件依靠方式，无停靠
//#define HDOCK_LEFT			1	// 靠左，m_nMarginLeft决定靠左的距离
//#define HDOCK_TOP				2	// 靠顶，m_nMarginTop决定靠顶的距离
#define HDOCK_RIGHT				4	// 靠右，m_nMarginRight决定到右边的距离，宽度由m_rcClient决定
#define HDOCK_BOTTOM			8	// 靠底，m_nMarginBottom决定到底边的距离，高度由m_rcClient决定
#define HDOCK_FULLHORIZONTAL	16	// 水平充满，m_nMarginLeft决定靠左的距离，m_nMarginRight决定到右边的距离，m_rcClient的left和right无效
#define HDOCK_FULLVERTICAL		32	// 垂直充满，m_nMarginTop决定靠顶的距离，m_nMarginBottom决定到底边的距离，m_rcClient的top和bottom无效

#define DEFAULTDPI 96		// 默认显示器dpi, 假设1/DEFAULTDPI为1pt(磅)

class HCmdUI;


/////////////////////////////////////////////////////////////////////////////
// HCtrl 所有控件基类
class AFX_EXT_CLASS HCtrl : public CCmdTarget
{
protected:
	DECLARE_DYNCREATE(HCtrl)	// 获取类名

public:

// 构造
protected:
public:
	HCtrl();
	HCtrl(LPCRECT prc);	// 添加子控件。HCtrl * pParent：父控件，LPCRECT prc: 控件区域。
	virtual ~HCtrl();
	virtual void initCtrl();	// 窗口建立完毕后初始化，还没有显示窗口

// Properties
public:
	//class HTips;
	// 主项目必需用“在共享 DLL 中使用 MFC”参数，否则主项目不能使用如下静态参数和其它系统静态参数。
	static LPCTSTR	m_pszCursor;		// 鼠标光标。
	static HCtrl *	m_pctrTips;			// 悬浮提示控件。
	static HCtrl *	m_pctrCapture;		// 锁定鼠标控件。
	static HCtrl *	m_pctrMouseEnter;	// 当前鼠标位置控件。
	static int		m_nDpiX;			// 显示器X方向dpi
	static int		m_nDpiY;			// 显示器Y方向dpi
	static CWnd *	m_pWnd;				// 控件所在的窗口。
	//static CRect	m_rcWndClient;		// 窗口用户区大小。
	static COLORREF m_cTestColor;		// 强制显示控件背景色，用于判断重画区域是否合理高效，测试用，为0时不显示。

	void setTestColor(){ m_cTestColor = (m_cTestColor ? m_cTestColor += 0x111111 : 0x204080) & 0xffffff; }	// 不能加成零

	//void setTopWnd( CWnd * pWnd )
	//{m_pTopWnd = pWnd;}	// 设置顶窗口，接收通知消息，

private:
		CFont			m_font;			// 字体，设置成私有的，只能用getFont方法

protected:
	CMFCToolBarImages*	m_pImglist;	// 图形条
	//BOOL			m_bMouseEnter;	// 鼠标在用户区。
	CArray<HCtrl*>	m_arChildren;	// 所有子控件
	CRect			m_rcClient;		// 用户区，相对于父控件, 单位pt。
	CRect			m_rcWnd;		// 用户区，窗口坐标，单位象素。
	int				m_nID;			// 控件ID
	LPCTSTR			m_lpszCursor;	// 鼠标形状, 为0时不响应鼠标事件
	HCtrl *			m_pParent;		// 父控件
	UINT			m_nDock;		// 停靠方式
	CString			m_strText;		// 控件文字
	CImage			m_imgBackground;// 背景图，如果有背景图就不用背景色 //?? 改用CPngImage pngImage;if (pngImage.Load(lpszResourceName, 0/*hinstRes*/)){hbmp = (HBITMAP) pngImage.Detach();}
	CBrush			m_brBackground;	// 背景刷子，背景色
	CPen			m_pnBorder;		// 边框笔
	int				m_nAlign;		// 对齐方式
	COLORREF		m_coText;		// 文本颜色
	COLORREF		m_coDisabled;	// 禁止时文本颜色
	CString			m_strTips;		// 悬停提示文字
	BOOL			m_bEnabled;		// 不可用
	int				m_nCheck;		// 选中状态，1选中，否则未选中
	//int				m_nMarginLeft;	// 水平充满时到左边的距离
	int				m_nMinWidth;	// 最小宽度，水平充满时有效
	int				m_nMarginRight;	// 靠右或水平充满时到右边的距离
	//int				m_nMarginTop;	// 垂直充满时到顶边的距离
	int				m_nMinHeight;	// 最小高度，充满时有效
	int				m_nMarginBottom;// 靠底或垂直充满时到底边的距离

public:
	BOOL isEnabled(){ return m_bEnabled; }
	void setID( int id ){ m_nID = id; }
	int getID(){ return m_nID; }
	void setTextColor( COLORREF color ){ m_coText = color; }

// 消息处理
public:
	virtual BOOL OnMouseMove( UINT nFlags, const CPoint &point );	// 返回TRUE表示已经处理。
	virtual void mouseMove( UINT /*nFlags*/, const CPoint &/*point*/ ){}	// 鼠标移动，只有鼠标所在的最终控件能收到。
	virtual void lbuttonDown( UINT /*nFlags*/, const CPoint &/*point*/ ){m_pctrCapture = this; /*锁定鼠标*/}
	virtual void lbuttonDblClk( UINT /*nFlags*/, const CPoint &/*point*/ ){m_pctrCapture = this; /*锁定鼠标*/}
	virtual void lbuttonUp( UINT /*nFlags*/, const CPoint &/*point*/ ){m_pctrCapture = 0;}
	virtual void mouseEntered( UINT /*nFlags*/, const CPoint &/*point*/ ){ m_pszCursor = m_lpszCursor; }
	virtual void mouseLeave(){m_pctrMouseEnter = 0, m_pszCursor = 0, m_pctrCapture = 0; }	// 鼠标离开，m_pctrCapture = 0原在lbuttonUp事件中设置，但有时lbuttonUp事件前出现mouseLeave事件，以后就没有lbuttonUp事件了，如上升下降等控制按钮按下时弹出对话框，系统把焦点移到新对话框中，以后再回到ttmca时不起作用，点一下才好。
	virtual void mouseHover( UINT /*nFlags*/, const CPoint &/*point*/ );	// 鼠标悬停
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	virtual BOOL OnWndMsg(UINT /*message*/, WPARAM /*wParam*/, LPARAM /*lParam*/, LRESULT* /*pResult*/);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

// Methods
protected:
	CMFCToolBarImages * getImglist();
	CFont * getFont();	// 如果当前有m_font，取当前的，否则取父级的。
	void rcToWnd( CRect &rc );	// 转换到Wnd坐标, 原点左上角，单位像素
	void rcToClient( CRect &rc );	// 转换到UI坐标, 原点左上角，单位磅
	void initParameter();				// 公共初始化

public:
	virtual BOOL preDraw( CDC* pDC, CRect & rcClip );	// 预处理画图事件，先画底层
	virtual void draw( CDC* pDC, CRect & rcClip );	// 画客户区

	virtual CString	getText(){ return m_strText; }
	virtual void	setText(CString txt){ m_strText = txt; invalidate(); }
	virtual void	setTextRes( UINT nID ){ m_strText.LoadString( nID ); invalidate(); }
	virtual void	setBackColor( COLORREF col );
	virtual void	setBackImg( PCTSTR idResource, PCTSTR lpType );
	virtual void	setBorderColor( COLORREF col );
	virtual void	layerout();					// 重排控件
	virtual void	Enable( BOOL bEnable ){ if( m_bEnabled == bEnable) return; m_bEnabled = bEnable; invalidate(); }	// bEnable 为真时可用
	virtual int		getChecked(){ return m_nCheck; }
	virtual void	setChecked( int nCheck ){ if( m_nCheck == nCheck) return; m_nCheck = nCheck; invalidate(); }
	virtual void	setFocus(){}
	virtual void invalidate();	// 重画控件区域
	virtual void added(){}	// 添加完成

	void addCtrl( HCtrl * pCtrl );	// m_arChildren中添加控件
	CRect rcToWnd();	// 当前RC转换到Wnd坐标
	CRect rcToClient();	// 把当前的Wnd区域转换到UI坐标, 原点左上角，单位磅
	void setDock(UINT nDock, int nMarginRight = 0, int nMarginBottom = 0, int nMinWidth = 0, int nMinHeight = 0);	//UINT nDock 停靠方式见HDOCK_X, int nMarginRight 右边间隙, int nMarginBottom 底边间隙, int nMinWidth 最小宽度, int nMinHeight 最小高度
	void setBtnImage(UINT nImg, const CSize & sizeImage );			//UINT nImg：控件图形资源ID号，可继承到子控件, 单个图像尺寸。
	void setCursor(LPCTSTR lpszCursor);	//LPCTSTR lpszCursor：设置光标，只支持系统光标，为0时不响应鼠标事件。
	void moveTo( int x, int y );		// 左上角移动到指定位置
	//void invalidate( HCtrl * pctrDraw, BOOL bDrawBack );// 重画控件区域，HCtrl * pctrDraw 只重画指定的控件，为零时重画所有底层控件; BOOL bDrawBack 为真时重画背景，否则不重画背景。20140408 重画不正常，取消参数
	// 自动更新控件数据
	void OnUpdateCmdUI(HCmdUI* pCmdUI,BOOL bDisableIfNoHndler);

	void setAlign( int nAlign ){ m_nAlign = nAlign; }
	BOOL setFont( int nHeight, int nWeight = 0, LPCTSTR lpszFacename = 0 );

	const CRect & getRect(){return m_rcClient;} const	// 取用户区
	CRect getWinRect(){return m_rcWnd;}	// 取窗口区
	void setRect( CRect & rc, BOOL layout);	// 设置用户区，layort 为真，重排控件，否则不重排。
	
	CArray<HCtrl*> * getChildren(){ return &m_arChildren; }	// 所有子控件

	void removeAllChildren();	// 删除全部子控件
	void removeChildrenAt( int i );	// 删除指定的子控件，i 控件索引号

	BOOL Intersect( CRect & rcClip );	// 重绘区是不是相交，相交返回TRUE，否则返回FALSE

	void setTips( PCTSTR str ){ m_strTips = str; }
	BOOL getTextSize( CRect & rc );	// 取控件当前文本的尺寸
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
// HLabel 文字显示
class AFX_EXT_CLASS HLabel : public HCtrl
{
// 构造
protected:
	//HLabel();
public:
	HLabel(LPCRECT prc);	// 添加子控件。HCtrl * pParent：父控件，LPCRECT prc: 控件区域, int idup抬起的图像ID, int iddown按下的图像ID。
	virtual ~HLabel();

	DECLARE_DYNCREATE(HLabel)

// Properties
protected:
	COLORREF m_coShadow;	// 阴影颜色
	BOOL m_bShadow;
	BOOL m_bAutoSize;	// 根据内容自动调整尺寸

public:
	void setAutoSize( BOOL bAutoSize ){m_bAutoSize = bAutoSize;}

// Methods
private:
	void drawText( CDC* pDC );
public:
	void setShadowColor( COLORREF color );

// Overwrite
public:
	virtual void draw( CDC* pDC, CRect & rcClip );	// 画客户区
	virtual void setText(CString txt);
	virtual void setTextRes( UINT nID );
	virtual void initCtrl();	// 窗口建立完毕后初始化，还没有显示窗口
};

/////////////////////////////////////////////////////////////////////////////
// HEdit 编辑控件，内嵌MFC CEdit
class HException;
class HVerifyException : public CException	// 验证异常
{
// Constrction
public:
	HVerifyException( HCtrl * pctrl, PCTSTR msg)	// HCtrl * pctrl 控件, PCSTR msg 消息
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
	//控件类型
	enum HEditType 
	{ 
		HEDITString,		// 普通字符串，不验证
		HEDITLength,		// 限定长度
		HEDITMobile,		// 中国手机号
		HEDITIdentity,		// 中国身份证号
		//HEDITEmail,			// 邮箱
		HEDITDate,			// 日期 yyyymmdd
	}; 


// 构造
protected:
	//HEdit();
public:
	HEdit(LPCRECT prc);	// 添加子控件。HCtrl * pParent：父控件，LPCRECT prc: 控件区域, int idup抬起的图像ID, int iddown按下的图像ID。
	virtual ~HEdit();

	DECLARE_DYNCREATE(HEdit)

// Properties
protected:

	DWORD m_dwStyle;		// MFC Edit控件样式，默认为WS_CHILD | WS_BORDER/* | ES_CENTER*/ | ES_AUTOHSCROLL | WS_VISIBLE
	HEditType m_enumType;	// 按件类型
	int m_nMaxCount;		// 最大字符数
	int m_nMinCount;		// 最小字符数
	BOOL m_bTrim;			// 不要前后空格
	BOOL m_bAllowNull;		// 允许空

public:
	HWndEdit m_wndEdit;		// MFC Edit控件
// Methods
private:
	//void drawText( CDC* pDC );
	virtual void	verify( CString & str );

public:
	void setStyle( DWORD dwStyle ){ m_dwStyle = dwStyle; }
	void setType( HEditType type ){ m_enumType = type, m_bAllowNull = FALSE; }
	void setMaxMin( int min, int max ){ m_nMaxCount = max, m_nMinCount = min, m_enumType = HEDITLength, m_bAllowNull = FALSE; }	// 同时把控件类型设为限定长度
	void setTrim( BOOL trim ){ m_bTrim = trim; }
	void setAllowNull( BOOL bAllowNull ){ m_bAllowNull = bAllowNull; }

// Overwrite
public:
	virtual void	draw( CDC* pDC, CRect & rcClip );	// 画客户区
	virtual void	layerout();		//重排
	virtual void	setText(CString txt);
	virtual CString	getText();
	virtual void	Enable( BOOL bEnable );		// bEnable 为真时可用
	virtual void	setFocus(){m_wndEdit.SetFocus();}
	//virtual void	mouseHover( UINT /*nFlags*/, const CPoint &/*point*/ );	// 鼠标悬停
};


/////////////////////////////////////////////////////////////////////////////
// HTips 鼠标悬停控件
class AFX_EXT_CLASS HTips : public HCtrl
{
// 构造
protected:
	//HTips();
public:
	HTips();	// 添加子控件。HCtrl * pParent：父控件，LPCRECT prc: 控件区域, int idup抬起的图像ID, int iddown按下的图像ID。
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
	void hidden();	// 隐藏悬停提示框
};


/////////////////////////////////////////////////////////////////////////////
// HGrid 表格
#define HGRID_RECERVATION_ROWS 50 // 预留行数
class AFX_EXT_CLASS HGrid : public HCtrl
{
// Type define
public:
	// 列结构
	struct Column 
	{
		CString title;	// 列标题
		int width;		// 列宽度，最后一列自动充满外框。
		UINT style;		// 样式
	};

	// 列集合
	struct Columns
	{
		COLORREF	textColor;		// 标题行文字颜色
		CBrush		m_brBackground;	// 标题行背景颜色
		int			height;			// 标题行高度
		CArray<Column>	items;			// 标题集合
		void setHeight( int h ){ height = h; }
		void setBackColor( COLORREF color )
		{
			if( m_brBackground.m_hObject )
				m_brBackground.DeleteObject();
			m_brBackground.CreateSolidBrush( color );
		}
	};

	// 单元格
	struct Cell
	{
		CString text;	// 文字
		int breaked;	// 是否截断，-1 不知道，0 不截断，1 截断

		Cell()
		{
			setEmpty();
		}

		void setEmpty()
		{
			text = L" ";	// 初始化一个空格，避免反复分配内存。
			breaked = -1;	// -1 不知道，0 不截断，1 截断
		}
	};

	typedef CArray<Cell> Cells;	// 行单元格集合
	typedef CArray<Cells *> Rows;	// 行集合

// 构造
protected:
	//HGrid();
public:
	HGrid(LPCRECT prc);	// LPCRECT prc: 控件区域。
	virtual ~HGrid();

	DECLARE_DYNCREATE(HGrid)

// Properties
private:
	Rows m_arRows;	// 行集合，私有成员，只用getRow方法查询

protected:
	int m_nRowHeight;			// 行高
	float m_fRowHeight;			// 浮点行高
	CBrush m_brMouse;	// 鼠标行颜色
	CBrush m_brSelect;// 选择行颜色
	int m_nRowNum;	// 行数
	int m_nPadding;	// 左右留白
	int m_nMouse;	// 鼠标行
	CString m_strSelectID;	// 选择行ID

public:
	Columns m_Columns;	// 标题集合
	void setRowHeight( int h ){ m_nRowHeight = h; }

// Methods
private:
	//CPoint getPos( const CPoint & p );
	//BOOL isBreakText( const CPoint & p );

public:
	void addColumn( PCTSTR title, int width, UINT style );	// 添加一列，PCSTR title 列标题; int width 列宽度，最后一列自动充满外框; UINT sytle 内容列样式，见DrawText
	void addRow( CStringArray row );			// 添加一行，要求row的项数与表格列数相同
	int getColumnNumber();		// 取列数
	int getRowNumber(){ return m_nRowNum; }			// 取行数
	Cells * getRow( int index );			// 取指定行
	//void setLineColor( COLORREF color );	// 设置表格线颜色
	void setMouseColor( COLORREF color );	// 设置鼠标行背景颜色
	void setSelectColor( COLORREF color );	// 设置选择行背景颜色
	void removeAll();
	

// Overwrite:
public:
	virtual void draw( CDC* pDC, CRect & rcClip );	// 画客户区
	virtual void layerout();		//重排, 计算行数
	virtual void mouseMove( UINT nFlags, const CPoint & point );
	virtual void lbuttonDown( UINT nFlags, const CPoint & point );
	virtual void mouseLeave();
	virtual void mouseHover( UINT /*nFlags*/, const CPoint &/*point*/ );	// 鼠标悬停
};


/////////////////////////////////////////////////////////////////////////////
// HBtn 定义按钮
class AFX_EXT_CLASS HBtn : public HCtrl
{
// 构造
protected:
	//HBtn();
public:
	// 添加子控件。HCtrl * pParent：父控件，LPCRECT prc: 控件区域, int idoverflow 移动到控件上的图像ID, int idup抬起的图像ID, int iddown按下的图像ID。
	HBtn(LPCRECT prc, int idoverflow, int idup, int iddown, int nID);
	virtual ~HBtn();

	DECLARE_DYNCREATE(HBtn)

// Properties
protected:
	int m_nImgup;	// 抬起图像
	int m_nImgdown;	// 压下图像
	int m_nImgover;	// 鼠标进入
public:

// Methods
private:

public:
	// 设置图像索引，int idup, int iddown
	//void setImgIndex( int idup, int iddown );

	virtual void lbuttonUp( UINT nFlags, const CPoint & point );
	virtual void lbuttonDown( UINT nFlags, const CPoint & point );
	virtual void mouseEntered( UINT nFlags, const CPoint & point );
	virtual void mouseLeave();
	//virtual void mouseHover( UINT /*nFlags*/, const CPoint &/*point*/ );	// 鼠标悬停
	virtual void draw( CDC* pDC, CRect & rcClip );	// 画客户区
	//virtual void calSize();	//重排位置
};


/////////////////////////////////////////////////////////////////////////////
// 应用程序窗口
class AFX_EXT_CLASS HWnd : public CWnd
{
	// 空闲更新类，指定的消息不更新，默认屏蔽WM_MOUSEMOVE, WM_TIMER, WM_PAINT等事件
	// 避免频繁调用控件状态更新
	typedef class tagIdleUpdateCmdUI
	{
	private:
		CUIntArray msgs;
		BOOL bUpdate;

	public:
		tagIdleUpdateCmdUI()
			: bUpdate(FALSE)
		{
			msgs.SetSize( 0, 10 );		// 预留10个空间
			msgs.Add( WM_MOUSEMOVE );	// 默认屏蔽WM_MOUSEMOVE, WM_TIMER, WM_PAINT等事件
			msgs.Add( WM_PAINT );
			msgs.Add( WM_TIMER );
			msgs.Add( WM_NCMOUSEMOVE );
			msgs.Add( WM_NCMOUSEHOVER );
			msgs.Add( WM_NCMOUSELEAVE );
			msgs.Add( WM_MOUSEHOVER );
			msgs.Add( WM_MOUSELEAVE );
			msgs.Add( 0x0118 ); // WM_SYSTIMER (caret blink)http://bbs.csdn.net/topics/90170047
		}

		// 添加一个屏蔽事件
		void Add( UINT msg )
		{
			msgs.Add( msg );
		}

		// 移除一个屏蔽事件
		void remove( UINT msg )
		{
			for( int i = msgs.GetCount()-1; i >= 0; i-- )
			{
				if( msgs[i] == msg )
					msgs.RemoveAt( i );
			}

		}

		// 判断消息是否进行Update
		// msg当前消息
		// 如果msg在集合msgs中，不更新控件。
		void beforeUpdate( UINT msg )
		{
			// 如果当前是更新状态，保持更新
			if( bUpdate )
				return;

			// 否则如果在msgs集合中，不更新
			for( int i = msgs.GetCount() - 1; i >= 0; i-- )
				if( msgs[i] == msg )
					return;
			bUpdate = TRUE;

			//
			//TRACE( "========================== %04x", msg );
		}

		// 更新完成后，把状态设为不更新
		void endUpdate(){ bUpdate = FALSE; }

		// 是否更新
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
	void setAccelerators( UINT id );	// 载入快捷键

// Propertis
public:
	HCtrl m_ctrFrame;	// 主控件，创建窗口后创建主控件，覆盖窗口用户区。

protected:
	HACCEL m_hAccel;	// 快捷键资源
	BOOL m_bMouseTracking;	// 为FALSE时激活窗口悬停和离开事件
	//CPoint m_ptPrevious;	// 移动过程中上次的鼠标位置

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

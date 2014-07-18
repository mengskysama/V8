#pragma once
#include "afxwin.h"
#include <vector>
#include <string>
#include "../Common.h"
#include "../../include/ui/SimpleTipCtrl.h"


namespace common { namespace ui
{

///////////////////////////////////////////////////////////
//            CToolItemBar

enum EArrowButtonType
{
	eArrowType_Left,				//	箭头方向朝左
	eArrowType_Right,			//	箭头方向朝右
	eArrowType_Up,				//	箭头方向朝上
	eArrowType_Down,				//	箭头方向朝下
};

enum EArrowButtonState
{
	eArrowButtonState_Normal,		//	正常状态
	eArrowButtonState_Hover,		//	鼠标划过状态
	eArrowButtonState_Down,		//	被按下的状态
	eArrowButtonState_Gray,		//	变灰色状态
};

class CArrowButton	:
	public	CWnd
{

	DECLARE_DYNAMIC( CArrowButton )

public:
	CArrowButton(void);
	virtual ~CArrowButton( void);

	BOOL Create(CWnd* pwndParent, CRect rc, UINT uId);

	HICON					m_hIcon;	//	对应的图标
	EArrowButtonType		m_eType;	//	按钮方向类型	
	EArrowButtonState		m_eState;	//	按钮的状态

protected:

	DECLARE_MESSAGE_MAP()

	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);

	LRESULT OnMouseLeave(WPARAM w, LPARAM l);

private:

	BOOL			m_bTrackMouse;
};


///////////////////////////////////////////////////////////
//	类名: CToolItem
//  类功能描述：
//  	用以描述Bar中每一个Item

// Item的状态描述
enum EIconButtonState
{
	eState_None,		//	无效状态
	eState_Normal,		//	正常状态
	eState_Hover,		//	鼠标划过状态
	eState_Down,		//	被按下的状态
	eState_Gray,		//	变灰色状态
};

/**
*	当CRBIconButton被点击的时候，该函数将被调用，用以执行一些特殊的操作
*	通常情况下pParam参数为CRBIconButton
*/
typedef void (*IconButtonFunc)(LPVOID pParam);


class COMMON_INTERFACE CToolItem	
{
public:
	CToolItem(void);
	CToolItem( CToolItem& barItem);
	CToolItem( CToolItem* pbarItem);
	virtual ~CToolItem(void);

	/**
	@fn SetWidth( UINT nWidth)
	@brief 设置当前Item的宽度
	@param [in] nWidth 设置的目标宽度
	@return 
	*/
	void	SetWidth( UINT nWidth) { m_nWidth = nWidth;}

	/**
	@fn GetWidth()
	@brief 获取当前Item的宽度
	@return 返回当前Item的宽度
	*/
	UINT	GetWidth() { return m_nWidth;}

	/**
	@fn SetHeight( UINT nHeight)
	@brief 设置当前Item的宽度
	@param [in] nWidth 设置的目标宽度
	@return 
	*/
	void	SetHeight( UINT nHeight) { m_nHeight = nHeight;}

	/**
	@fn GetHeight()
	@brief 获取当前Item的高度
	@return 返回当前Item的高度
	*/
	UINT	GetHeight() { return m_nHeight;}

	/**
	@fn 设置当前Item的tips
	@brief 获取当前Item的高度
	@param [in] strTips Item的tips
	*/
	void	SetTips(std::wstring	strTips);

	/**
	@fn GetTips()
	@brief 获取当前Item的关联tips
	@return 返回当前Item的tips
	*/
	std::wstring GetTips(){return m_strTips;}

	/**
	@fn SetState( EIconButtonState eState)
	@brief 设置当前Item的状态
	@param [in] eState 当前Item的状态
	*/	
	void	SetState( EIconButtonState eState){m_eState =  eState;}

	/**
	@fn GetState()
	@brief 获取当前Item的状态
	@param [in] eState 当前Item的状态
	*/
	EIconButtonState	GetState() { return m_eState;}

	/**
	@fn ShowState( BOOL  bShowState )
	@brief 是否需要显示三态效果
	@param [in] bShowState  TRUE表示需要显示三态效果,FALSE则不需要显示三态
	*/	
	void	ShowState( BOOL  bShowState = TRUE ){m_bShow3State =  bShowState;};

	/**
	@fn IsShow3State()
	@brief 获取当前是否需要显示三态
	*/	
	BOOL	IsShow3State() {return m_bShow3State;}
	
	/**
	@fn SetParent( CWnd* pParentWnd)
	@brief 设置当前Item所在的窗口
	@param [in] pParentWnd	当前的父窗口
	*/	
	void	SetParent( CWnd* pParentWnd) { m_pParentWnd = pParentWnd;}

	/**
	@fn IsShow3State()
	@brief 获取当前是否需要显示三态
	*/	
	CWnd*	GetParent() {return m_pParentWnd;}

	/**
	@fn Draw( CDC* pDC)
	@brief Item的绘制接口，所有的Item都必须实现该接口，以便ToolBar调用以便进行绘制
			该接口中提供默认的三态绘制效果，继承的类可以通过重写Draw实现自己需要的
			三态绘制
	*/	
	virtual void	Draw( CDC* pDC);

	/**
	@fn OnMouseMove(UINT nFlags, CPoint point)
	@brief 当鼠标在Item上移动的时候该Item的行为
	*/
	virtual void	OnMouseMove(CDC* pDC, UINT nFlags, CPoint point);

	/**
	@fn OnLButtonDown(UINT nFlags, CPoint point)
	@brief 当鼠标按下的时候Item的行为
	*/
	virtual void	OnLButtonDown(CDC* pDC, UINT nFlags, CPoint point);

	/**
	@fn OnLButtonUp(UINT nFlags, CPoint point)
	@brief 当鼠标按下弹起的时候Item的行为
	*/
	virtual void	OnLButtonUp(CDC* pDC, UINT nFlags, CPoint point);

	/**
	@fn OnMouseLeave(WPARAM w, LPARAM l)
	@brief 当鼠标按下弹起的时候Item的行为
	*/
	virtual LRESULT	OnMouseLeave(CDC* pDC, WPARAM w, LPARAM l);

	/**
	@fn OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
	@brief 光标设置行为
	*/
	virtual BOOL	OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);


	CRect				m_Rect;				//	当前Item的区域大小
	UINT				m_nItemIndex;		//	Item在ToolBar中的索引
	UINT				m_nLine;			//	当前Item所在的行或者页，如果是非折叠，则是所在的页
	IconButtonFunc		m_ClickFunc;		//	点击函数
	VOID*				m_pParam;			//	需要的额外参数，通常是父窗口
											//	如果是折叠，则是所在的行
protected:
	UINT				m_nWidth;			//	当前Item的宽度
	UINT				m_nHeight;			//	当前Item的高度
	std::wstring		m_strTips;			//	当鼠标移入到该Item上时候的提示消息
	EIconButtonState	m_eState;			//	当前Item的状态
	BOOL				m_bShow3State;		//	是否需要显示三态
	CWnd*				m_pParentWnd;		//	当前Item所在的窗口
};



///////////////////////////////////////////////////////////
//	类名: CToolItemBar
//  类功能描述：
//  	用以描述一个Item Bar，该bar仅仅是一些Item的组合，具有一个公共的属性


#define	H_ICON_SPACE	2			//	水平图标之间的间距
#define V_ICON_SPACE	2			//	垂直图标之间的间距
#define LEFT_SPACE		3			//	左侧的间距
#define RIGHT_SPACE		3			//	右侧的间距
#define	TOP_SPACE		2			//	上端的间距
#define BUTTON_SPACE	2			//	下端的间距

class COMMON_INTERFACE CToolItemBar :
	public CWnd
{
	DECLARE_DYNAMIC(CToolItemBar)

public:

	typedef std::vector<CToolItem*> CToolItems;

public:
	CToolItemBar(void);
	virtual ~CToolItemBar(void);

	/**
	@fn Create(CWnd* pwndParent, CRect rc, UINT uId)
	@brief 创建一个Bar
	@param [in] pwndParent	当前Bar所属的父窗口
	@param [in] rc			当前Bar的所占区域
	@param [in] uId			当前Bar的资源ID
	*/
	BOOL	Create(CWnd* pwndParent, CRect rc, UINT uId);

	/**
	@fn SetWrapper(BOOL bWrapper)
	@brief 当Item超出给定的矩形区域之后，它是自动切换到下一行还是进行滚动显示
	@param [in] bWrapper	是否允许行折叠
	*/
	void	SetWrapper(BOOL bWrapper = TRUE);

	/**
	@fn SetLineHeight( UINT nLineHeight)
	@brief 设置Item的高度
	@param [in] nLineHeight	设置的行高
	*/
	void	SetLineHeight( UINT nLineHeight = 20){m_nLineHeight = nLineHeight;}

	/**
	@fn GetLineHeight( )
	@brief 获取Item的高度
	@return 设置的Item的行高
	*/
	UINT	GetLineHeight()	{	return m_nLineHeight;}

	/**
	@fn AddButton( CToolItem* pIconButton);
	@brief 在Bar中增加一个新的Item
	@param [in] bWrapper	是否允许行折叠
	*/
	void	AddItem( CToolItem* pIconButton);

	/**
	@fn DeleteButtons;
	@brief 删除Bar中的某个Item
	@param [in] nIndex	需要删除的Item的索引
	*/
	void	DeleteItem( UINT nIndex);

	/**
	@fn DeleteButtons;
	@brief 删除Bar中的所有的bar
	*/
	void	DeleteItems();

	/**
	@fn SetBkgColor( COLORREF uBkgColor);
	@brief 设置当前Bar的背景色
	*/
	void	SetBkgColor( COLORREF uBkgColor);

	/**
	@fn SetTips( COLORREF uBkgColor);
	@brief 设置当前Bar的背景色
	*/
	void	SetTips( std::wstring strTips) { m_strTips = strTips; }


	CRect	GetLastIconRect() ;


protected:

	CRect		GetNewIconRect( UINT nWidth, UINT nHeight);
	CToolItem*	GetItem( CPoint pt, UINT* pnIdx = NULL);
	INT			GetFirstButtonAtLine( UINT nLine);
	UINT		GetIconBarRight();

	CToolItem*	GetSelectedItem( UINT* pnIdx = NULL);
	CToolItem*	GetHoverItem( UINT*	pnIdx = NULL);
	void		ChangeItemState( CToolItem* pItem, EIconButtonState eNewState);

protected:

	DECLARE_MESSAGE_MAP()

	afx_msg void	OnPaint();
	afx_msg void	OnSize(UINT, int, int);
	afx_msg void	OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void	OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void	OnLButtonUp(UINT nFlags, CPoint point);
	LRESULT			OnMouseLeave(WPARAM w, LPARAM l);
	BOOL			OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);

	BOOL			OnEraseBkgnd(CDC* pDC);
	LRESULT			OnRightArrowDown( WPARAM w, LPARAM l);
	LRESULT			OnLeftArrowDown( WPARAM w, LPARAM l);

protected:

	CToolItems			m_vItems;					//	保存所有的ICON
	BOOL				m_bWrapper;					//	是否允许折叠。如果图标超出给定的宽度，那么自动折叠到下一行
	UINT				m_nLineHeight;				//	设置行高，通常情况下对于wrapper模式会有效
													//	设置了行高只有，一行中的所有的Item的高度都不允许超出该行高

	CArrowButton		m_leftArrowButton;			//	左箭头按钮
	CArrowButton		m_rightArrowButton;			//	右箭头按钮
	UINT				m_nCurrentPage;				//	当前显示的页面索引
	CRect				m_rRightArrowButtonRect;	//	右侧箭头的区域
	CRect				m_rLeftArrowButtonRect;		//	左侧箭头区域
	COLORREF			m_nBkgColor;
	BOOL				m_bTransparent;				//	窗口是否需要透明
	UINT				m_nLine;					//	总的行数
	std::wstring		m_strTips;					//	当鼠标移动到整个Bar上的时候的提示信息
	BOOL				m_bTrackMouse;				

	CSimpleTipCtrl	m_ctrlButtonTip;	//	当鼠标移动到Button上的时候显示的提示信息
	CSimpleTipCtrl	m_ctrlBarTip;
};

}}
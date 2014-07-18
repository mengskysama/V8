
#pragma once
#include "afxwin.h"

#include <string>
#include <vector>
//#include "ImageManagerDefine.h"
#include "SkinUtil.h"
#include "../CommonLibrary/include/ui/SimpleTipCtrl.h"
#include "../CommonLibrary/include/ui/ImageImpl.h"
#include "../CommonLibrary/include/ui/ImageWrapper.h"

using namespace std;

#define WM_ITEM_SELECT		WM_USER + 1107		//	通知父窗口，该Item已经获取焦点
#define WM_ITEM_UNSELECT	WM_USER + 1108		//	通知父窗口，该Item已经获取焦点
#define WM_ITEM_CLICK		WM_USER + 1109		//	通知某一项已经被点击
#define WM_ITEM_DELETE		WM_USER + 1110		//	某一个Item被删除, WPARAM为删除的UIN

#define WM_LISTCTRL_SHOWHIDE    WM_USER + 1111      //  CLoginListCtrl 登陆账号下拉框隐藏

enum E_State
{
	ES_NORMAL,
	ES_HOVER,
	ES_DOWN,
};

/**********************************************************
*														  *
* 功能说明：描述登陆账号下拉框中的每一个Item项			  *
*														  *
**********************************************************/
class CLoginListCtrlItem:
	public CWnd
{
	DECLARE_DYNAMIC(CLoginListCtrlItem)

public:

	enum 
	{
		LI_NORMAL_HEIGHT	=	30,
		LI_HOVER_HEIGHT		=	46,

		TEXT_NORMAL_LEFT_OFFSET	=	30,			//	正常状态下Item上文字与边界的左偏移
		TEXT_HOVER_LEFT_OFFSET	=	48,			//	Hover状态下Item上文字与边界的左偏移

		IMAGE_NORMAL_LEFT_OFFSET	=	2,		//	正常情况下Item上头像与边界的左偏移
		IMAGE_HOVER_LEFT_OFFSET		=	3,		//	Hover状态下Item上头像与边界的左偏移

		IMAGE_NORMAL_TOP_OFFSET		=	2,		//	正常情况下Item上头像与边界的上偏移
		IMAGE_HOVER_TOP_OFFSET		=	3,		//	Hover状态下Item上头像与边界的上偏移


		THUMBNAIL_IMAGE_WIDTH	=	25,
		LARGE_IMAGE_WIDTH		=	40,


		CONTENT_TOP_GAP				=	2,		//	内部Item与边界的距离
		CONTENT_LEFT_GAP			=	2,		//	内部Item
		CONTENT_RIGHT_GAP			=	2,
		CONTENT_BOTTOM_GAP			=	2,
	};

	CLoginListCtrlItem();
	virtual ~CLoginListCtrlItem();

	/**
	* Function		创建一个子项目
	* @param		rc		子项目
	* @param		pParent	所在的父窗口
	* @return	
	**/
	BOOL	Create(const CRect& rc, CWnd* pParentWnd, UINT uId);


protected:

	DECLARE_MESSAGE_MAP()

	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

	LRESULT OnMouseLeave(WPARAM w, LPARAM l);

private:

	E_State		GetCloseBtnState() const { return m_eImageState;}

public:
	
	E_State		m_eState;
	E_State		m_eImageState;
	BOOL		m_bTrackMouse;

	UINT32		m_nUin;			//	给定用户的UIN
	CString		m_strShowAccount;	//		
	CString		m_strNickName;	

	UINT8		m_nIndex;
	common::ui::CImageImpl*		m_pLargeImage;	//	大图片
	CRect			m_rectCloseBtn;
    common::ui::CSimpleTipCtrl	m_ctrlTips;

	//			相关的Image对象
    common::ui::CImageImpl*		m_pHoverImage;	//	Item被选中的时候加载的图像
	common::ui::CImageImpl*		m_pCloseImage;	//	关闭按钮加载的图片
};



/**********************************************************
*														  *
* 功能说明：描述登陆账号下拉框							  *
*														  *
**********************************************************/
class CLoginListCtrl :
	public CWnd
{

	DECLARE_DYNAMIC(CLoginListCtrl)

public:
	CLoginListCtrl(void);
	~CLoginListCtrl(void);

	/**
	* Function		创建列表窗口
	* @param		rc		列表窗口矩形
	* @param		pParent	所在的父窗口
	* @return	
	**/
	BOOL	Create(const CRect& rc, CWnd* pParentWnd, UINT uId);
	BOOL	PreTranslateMessage(MSG* pMsg);

	/**
	* Function		设置与当前下拉框关联的窗口
	**/
	VOID	SetAssoicatedWnd( CWnd* pWnd);

	/**
	* Function		显示给定的账号下拉框
	* @param		point	左上角的弹出点
	* @return	
	**/
	VOID	Show( CPoint& point);

	/**
	* Function		隐藏下拉框
	* @return	
	**/
	VOID	Hide();

	/**
	* @function		增加一个新的Item
	* @param		nUin		Item对应的uin
	* @param		strShowAccount	对应的账号
	* @param		smallImageWrapper	用户的小头像
	* @param		largeImageWrapper	用户的大头像
	* @return		如果插入成功，返回TRUE，否则返回FALSE;
	*/
	BOOL	AddItem( UINT32	nUin, CString strShowAccount, CString strNickName,
		common::ui::CImageImpl* imageWrapper);

	/**
	* @function		在制定的索引位置插入一个Item
	* @param		nUin		Item对应的uin
	* @param		strShowAccount	对应的账号
	* @param		smallImageWrapper	用户的小头像
	* @param		largeImageWrapper	用户的大头像
	* @return		如果插入成功，返回TRUE，否则返回FALSE;
	*/
	BOOL	InsertItem( int nIndex, UINT32	nUin, CString strShowAccount,  CString strNickName,
					common::ui::CImageImpl* imageWrapper);

	/**
	* @function		删除一个给定用户的Item
	* @param		nUin	被删除用户的uin
	*/
	void	DeleteItem( UINT32	nUin);
	void	DeleteItem( CString	strShowAccount);
	void	DeleteAllItem();

	/**
	* @function		获取指定索引处的UIN
	* @param		nIndex	Item的索引项
	*/
	uin		GetUinAtIndex( UINT nIndex);

	/**
	* @function		获取指定索引处的账号
	* @param		nIndex	Item的索引项
	*/
	wstring	GetAccountAtIndex( UINT nIndex);
	wstring GetShowUinAtIndex( UINT nIndex);

	int SetCurSel(int nSelect);

	/**
	* @function		查找给定的UIN的索引
	* @param		nUin	需要查找的UIN
	* @return		返回给定UIN的索引，如果没有找到，则返回-1
	*/
	int		Find( uin nUin);

	/**
	* @function		查找给定的账号的索引
	* @param		strShowAccount	需要查找的账号
	* @return		返回给定账号的索引，如果没有找到，则返回-1
	*/
	int		Find( CString strShowAccount);

	/**
	* @function		获取当前下拉框中的总的Item数目
	*/
	int		GetCount() const;

	/**
	* @function		获取当前被选中的Item的索引
	*/
	int		GetCurSel() const;

	/**
	* @function		在下拉框的所有Item中查找给定的账号
	* @param		nStartAfter	查找的起始索引，通常情况下为0，表明从第一个Item开始查找
	* @param		lpszString	需要查找的账号
	* @return		返回对应Item的索引
	*/
	int		SelectAccount( int nStartAfter, LPCTSTR	lpszString);
	
	/**
	* @function		在下拉框的所有Item中查找给定的UIN
	* @param		nStartAfter	查找的起始索引，通常情况下为0，表明从第一个Item开始查找
	* @param		nUin		需要查找的UIN
	* @return		返回对应Item的索引
	*/
	int		SelectUin( int nStartAfter, uin nUin);

	BOOL	m_bHide;						

protected:

	DECLARE_MESSAGE_MAP()

	afx_msg UINT	OnGetDlgCode();
	afx_msg void	OnPaint();
	afx_msg LRESULT	OnItemSelected( WPARAM w, LPARAM l);
	afx_msg LRESULT	OnItemUnSelected( WPARAM w, LPARAM l);
	afx_msg LRESULT OnItemClick( WPARAM w, LPARAM l);
	afx_msg LRESULT OnItemDelete( WPARAM w, LPARAM l);

	afx_msg	void	OnKillFocus(CWnd* pNewWnd);
	afx_msg int		OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg	void	OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void	OnLButtonDown(UINT nFlags, CPoint point);
	LRESULT OnMouseLeave(WPARAM w, LPARAM l);

private:

	void	DrawItem();
	void	ChangeNextSelect();				//	切换选中项至下一个
	void	ChangePrevSelect();				//	切换选中项至上一个


	std::vector< CLoginListCtrlItem*>	m_vpLoginListItem;	//	保存所有的Item
	CWnd*	m_pAssoicatedWnd;							//	与当前下拉框广联的窗口，通常是编辑框
	INT		m_nSelIndex;								//	选中的Item的索引
	CComPtr<ISkinEngine> m_pSkinEngine;
	BOOL	m_bTrackMouse;
	CWnd* m_pParentWnd;
};

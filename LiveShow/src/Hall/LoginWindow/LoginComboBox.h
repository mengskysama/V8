
#pragma once
#include "afxwin.h"
#include "LoginList.h"
#include "RBExtEdit.h"
#include "TipCtrl.h"
//#include "ImageManagerDefine.h"

// COMBOBOX的消息
#define WM_COMBOBOX_SEL_CHANGE	WM_USER	+ 1000			//	ComboBox的选项发生了变化
#define WM_COMBOBOX_SET_FOCUS	WM_USER	+ 1001			//	ComboBox的获取到了焦点
#define WM_COMBOBOX_LOSE_FOCUS	WM_USER	+ 1002			//	ComboBox的失去了焦点
#define WM_COMBOBOX_EDIT_CHANGE	WM_USER	+ 1003			//	ComboBox的编辑文本发生变化

#define WM_COMBOBOX_BUTTON_CLICK	WM_USER + 1004		//	按下按钮

#define DEFAULT_ACCOUNT_TEXT _T("白兔KTV号码/用户名/邮箱")
/**********************************************************
*														  *
* 功能说明：输入账号的编辑框控件，它具有自动补全功能	  *
*														  *
**********************************************************/

#define ACCOUNT_EDIT_MOUSE_HOVER    WM_USER+250
#define ACCOUNT_EDIT_MOUSE_LEAVE    WM_USER+251

class CLoginAccountEdit:
	public CRBLimitTextEdit
{
	DECLARE_DYNAMIC(CLoginAccountEdit)

public:
	CLoginAccountEdit();
	virtual ~CLoginAccountEdit();

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void	OnEditUpdate();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg HRESULT OnMouseHover( WPARAM w, LPARAM l);
	afx_msg HRESULT OnMouseLeave( WPARAM w, LPARAM l);


protected:
	DECLARE_MESSAGE_MAP()

private:

};


/**********************************************************
*														  *
* 功能说明：账号登陆的ComboBox							  *
*														  *
**********************************************************/
#define  IDC_COMBOBOX_ACCOUNT		5000
#define	 IDC_COMBOBOX_BUTTON		5001
#define	 IDC_COMBOBOX_LIST			5002

#define  ACCOUNT_CTRL_MOUSE_HOVER   WM_USER+500
#define  ACCOUNT_CTRL_MOUSE_LEAVE   WM_USER+501

class C179TipCtrl;
class CLoginComboBox:
	public CWnd
{

	DECLARE_DYNAMIC(CLoginComboBox)

public:

	CLoginComboBox();
	virtual ~CLoginComboBox();

	BOOL	PreTranslateMessage(MSG* pMsg);
	BOOL	Create(const CRect& rc, CWnd* pParentWnd, UINT uId);

	/**
	* @function		在制定的索引位置插入一个Item
	* @param		nUin		Item对应的uin
	* @param		strShowAccount	对应的账号
	* @param		smallImageWrapper	用户的小头像
	* @param		largeImageWrapper	用户的大头像
	* @return		如果插入成功，返回TRUE，否则返回FALSE;
	*/
	BOOL	AddItem( UINT32	nUin, CString strShowAccount, CString strNickName, 
					 common::ui::CImageImpl* imageWrapper);

	/**
	* @function		删除一个给定用户的Item
	* @param		nUin	被删除用户的uin
	*/
	void	DeleteItem( UINT32	nUin);
	void	DeleteItem( CString strShowAccount);
	void	DeleteAllItem();

	/**
	* @function		获取指定索引处的UIN
	* @param		nIndex	Item的索引项
	*/
	uin		GetUinAtIndex( UINT nIndex);

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

	//================================================
	//           下面是模拟的CComboBox的接口
	//================================================


	/**
	* @function		Deletes (clears) the current selection ( if any) in the edit control
	*/
	void	Clear();

	/**
	* @function		Retrieves the number of items in the list box of a combo box
	*/
	int		GetCount() const;

	/**
	* @function		Call thie member function to determine which item in the combo box is selected;
	*/
	int		GetCurSel()	const;

	/**
	* @function		Gets the starting and ending character positions of the current selection in 
					the edit control of a combo box
	* @return		The return value is a DWORD value that contains the starting position in the
					low-order and the position of the first nonselected character after the end
					of the selectiono in the high-order word.
	*/
	DWORD	GetEditSel() const;


	/**
	* @function		Selects characters in the edit control of a combo box
	* @param		nStartChar specifies the starting position, if the starting postion is set to -1, then any existing selection
					is removed;
	*				nEndChar	specifies the ending position, if the ending position is set to -1, then all text from the starting
					position to the last character in the edit control is selected.
	* @return		Nonzero if the menber function is successful; otherwise 0.
	of the selectiono in the high-order word.
	*/
	void	SetEditSel( int nStartChar, int nEndChar);

	/**
	* @function		Gets account string from the list box of a combo box, in 
	* @return			
	*/
	int		GetLBAccountText( int nIndex, CString&	rString) ;

	/**
	* @function		Gets a uin string from the list box of a combo box, in 
	* @return			
	*/
	int		GetLBUinText( int nIndex, CString&	rString) ;

	/**
	* @function		Limites the length of the text that the user can enter into the edit control of a combo box
	* @return			
	*/
	BOOL	LimitText( int nMaxChars);

	/**
	* @function		Selects a string in the list box of a combo box
	* @param		nSelect	specifies the zero-based index of the string to select, 
					if -1, any current selection in the list box is removed and the edit control is cleared
	* @return		the zero-based index of the item selected if the message is successful. The return value is CB_ERR
					if nSelect is greater than the number of items in the list or if nSelect is set to -1, which clears the selection
	*/
	int		SetCurSel( int nSelect);

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

	/**
	* @function		设置Combo编辑框中的文本
	*/
	void	SetWindowText(LPCTSTR lpszString);

	/**
	* @function		获取Combo编辑框中的文本
	*/
	void	GetWindowText(CString& rString);

	/**
	* @function		将combo中的编辑框设置为获取焦点状态
	*/
	void	SetFocus();

	/**
	* @function		设置字体颜色
	*/
	void	SetTextColor( COLORREF textColor);

	void	SetLastText( CString strLastText) { m_strLastInputText = strLastText;}
	CString	GetLastText() { return m_strLastInputText;}

protected:

	DECLARE_MESSAGE_MAP()

	afx_msg void	OnPaint();
	afx_msg	void	OnSetFocus(CWnd* pOldWnd);
	afx_msg void    OnKillFocus(CWnd* pNewWnd);
	afx_msg void	OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg	void	OnButtonClick();
	afx_msg HRESULT	OnComboBoxClick(WPARAM w, LPARAM l);
	afx_msg void	OnAccountEditChange();
	afx_msg void	OnAccountEditKillFocus();
	afx_msg void	OnAccountEditSetFocus();
	afx_msg HRESULT	OnDeleteAccount( WPARAM w, LPARAM l);
	afx_msg HRESULT	OnLoginListCtrlShowHide( WPARAM w, LPARAM l);

	afx_msg LRESULT	OnItemClick(WPARAM w, LPARAM l);
	afx_msg void	OnSize(UINT nType, int cx, int cy);

	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg HRESULT OnMouseHover( WPARAM w, LPARAM l);
	afx_msg HRESULT OnMouseLeave( WPARAM w, LPARAM l);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);  //鼠标点击

	void KeyDownSpace(); 

private:
	CLoginAccountEdit	m_InnerEdit;	//	内部的编辑框控件
	CLoginListCtrl		m_InnerList;	//	内部的下拉框控件
	CButton				m_InnerButton;	//	内部的按钮
	CFont				m_EditFont;
	BOOL				m_bEditChangeRecursive;

	BOOL				m_bAutoComplete;		//	是否需要自动补全
	CString				m_strOldText;			//	原有的文本
	CString				m_strLastInputText;		//	最后一次输入的文本数据

	//以下由DragonZ注释
	//CTipCtrl	        m_ctrlTip;
	C179TipCtrl         *m_pTipCtrl;

};
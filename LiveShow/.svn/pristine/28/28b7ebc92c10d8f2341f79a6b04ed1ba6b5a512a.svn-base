#pragma once
#include "../../include/ExportMacro.h"
//#include "../CommonUi/TipCtrl.h"

#include <string>

using namespace std;

#define  WM_PWD_CTRL_TIP             WM_USER+300
#define  PASSWORD_CTRL_MOUSE_HOVER   WM_USER+301
#define  PASSWORD_CTRL_MOUSE_LEAVE   WM_USER+302
#define  PASSWORD_CTRL_SETFOCUS           WM_USER+303
#define  PWD_CTRL_DEFAULT_TEXT       _T("请输入密码")


class COMMON_INTERFACE CRBPasswordCtrl : public CWnd
{
	DECLARE_DYNAMIC(CRBPasswordCtrl)

public:
	CRBPasswordCtrl();
	virtual ~CRBPasswordCtrl();

	/**
	 * Function		创建密码控件窗口
	 * @param		rc		密码控件大小
	 * @param		pParent	密码控件的父窗口
	 * @param		uId		该控件对应的Item ID
	 * @param		dwStyle	该控件的创建风格
	 * @return	
	 **/
	BOOL	Create(const CRect& rc, CWnd* pParentWnd, UINT uId, 
		DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP);

	/**
	 * Function		获取输入的密码明文
	 * @return		返回用户输入的密码明文
	 **/
	wstring GetPlainPassword();

	/**
	* Function		获取输入的密码的MD5
	* @return		返回用户输入的密码明文
	**/
	wstring GetPassword();

	/**
	 * Function		获取输入的密码
	 * @param		sPassword	在新的密码控件中为密码的MD5值，而不是密码明文
	 * @return 
	 **/
	void	SetPassword( wstring sPassword);

	/**
	* Function		清空密码框中所有的密码
	* @return 
	**/
	void	ClearPassword();

	/**
	* Function		设置允许输入的最大字符数目
	* @return 
	**/
	void	LimitText(LONG uNum);

//	void	HideTip() { m_ctrlTip.Hide();}


protected:
	DECLARE_MESSAGE_MAP()

	UINT		GetCharWidthInfo( wstring strText);
	int			Point2CharIndex(CPoint pt);

	/**
	 * 将给定的字符串转换为"******"格式
	 */
	wstring		ConvertToStarChar( wstring strSrc);

	/**
	 * 获取编辑框的长度
	 */
	const UINT	GetTextWidth() { return m_rectCtrl.right - m_rectCtrl.left - 1;}

	/**
	 * 从尾部索引推算获取最适合当前编辑框的字符串
	 * nEndIndex 是结束索引
	 * nBeginIndex 是推算出的起始索引
	 * 返回最适合的字符串
	 */
	wstring		GetFittableStringFromEnd( UINT nEndIndex, UINT& nBeginIndex);

	/**
	 * 从前段给定索引推算适合编辑框的字符串
	 * nBeginIndex 是起始索引
	 * nEndIndex是推算出来的结束索引
	 * 返回最适合的字符串
	 */
	wstring		GetFittableStringFromFront( UINT nBeginIdex, UINT& nEndIndex);

	/**根据新的光标位置更新光标*/
	void		UpdateCaretPos();

	/**检测光标是否位于文本的最后面，如果是末尾，则光标无法响应->键*/
	BOOL		CaretAtTextEnd();

	/**检测光标是否位于文本的最前面，如果是最前面，则不再响应<-键盘*/
	BOOL		CaretAtTextFront();

	/**检测光标是否位于控件的最右侧，如果是在最右侧，则输入字符或者向右移动的时候，需要左移编辑框中的文字*/
	BOOL		CaretAtCtrlEnd();

	/**检测光标是否位于空间的最左侧*/
	BOOL		CaretAtCtrlFront();

	void		OnProcessLeftKey();
	void		OnProcessRightKey();
	void		OnProcessChar(UINT nChar);
	void		OnProcessHome();
	void		OnProcessEnd();
	void		OnProcessDelete();
	void		OnProcessBack();

protected:

	BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg HRESULT OnMouseHover( WPARAM w, LPARAM l);
	afx_msg HRESULT OnMouseLeave( WPARAM w, LPARAM l);

	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);

private:

	CFont		m_font;					//	控件中字体的颜色
	CFont       m_defaultTextFont;      //  提示语字体

	wstring		m_strPasswordText;		//	控件中实际保存的密码文本的Base64编码值，不是明文，主要是防止搜索内存，查找密码
	wstring		m_strShowText;			//	在控件的可视区域内能够看到的文本
	wstring		m_strPasswordMD5Text;	//	密码的MD5值，对内输入和对外输出都是基于MD5

	UINT		m_nBeginPos;			//	控件中显示的文本在整个密码文本中的起始位置

	int			m_nCaretPos;			//	编辑框中光标的位置，该位置是相对于整个密码控件窗口而言，它
										//	一定位于可视区域内，它位于第几个字母之后，则它的值就是几，比如，
										//	对于 hello|，则m_nCaretPos的值为5，它的初始值为0

	BOOL		m_bCaretCreated;		//	是否已经创建了光标
	CRect		m_rectCtrl;				//	控件的窗口

	BOOL		m_bFirstClick;			//	该控件是否是失去焦点后第一次被点击
//	CTipCtrl	m_ctrlTip;
	LONG		m_nLimitText;			//	允许输入的最大字符数目
	BOOL		m_bRememberPassword;	//	该密码框中的密码是否是自动记住的
	BOOL		m_bFirstFocus;			//	是否是第一次获取焦点
	BOOL        m_bShowDefaultText;     //  是否显示默认提示语
};
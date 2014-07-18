#pragma once

//=================================================================
//            文件中定义常用的扩展Edit

#include "afxwin.h"
#include "ColorEdit.h"
#include "RBEdit.h"


class CRBLimitTextEdit;
class CRBAutoEndEdit;
class CRBMoreMemoEdit;

//=================================================================
// CRBLimitTextEdit
// 限制输入的Edit字节数目
 

class  CRBLimitTextEdit : 
	public CColorEdit
{
	DECLARE_DYNAMIC(CRBLimitTextEdit)

public:
	CRBLimitTextEdit();
	virtual ~CRBLimitTextEdit();

	void	SetLimit( UINT nLimitNum);
	const UINT	GetLimit() { return m_nLimitInBytes;}


protected:
	DECLARE_MESSAGE_MAP()

	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg LRESULT OnPaste(WPARAM w, LPARAM l);

	int		GetLengthInAnsi();
	int		GetSelLengthInAnsi();
	int		ReplaceSel(const wchar_t* szTextNew);
	void	NotifyInputExceed(int nLimitNow, int nInputLenYouWant);

private:

	UINT	m_nLimitInBytes;
};

//=================================================================
//  CRBMoreMemoEdit
//	当Edit第一次获取焦点的时候，自动定位到文字的末尾

class  CRBAutoEndEdit : 
	public CRBLimitTextEdit
{
	DECLARE_DYNAMIC(CRBAutoEndEdit)

public:
	CRBAutoEndEdit();
	virtual ~CRBAutoEndEdit();


protected:
	DECLARE_MESSAGE_MAP()

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

private:
	bool	m_bFirstClick;
};


//=================================================================
// CRBMoreMemoEdit

#define SHOW_TEXT L"(你可以在此处为好友添加如手机、Email、联系地址、生日等更多备注信息)"

class  CRBMoreMemoEdit : 
	public CRBLimitTextEdit
{
	DECLARE_DYNAMIC(CRBMoreMemoEdit)

public:
	CRBMoreMemoEdit();
	virtual ~CRBMoreMemoEdit();


protected:
	DECLARE_MESSAGE_MAP()

	afx_msg void OnSetFocus( CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
};


//=================================================================
//			CRBAutoSelectEdit
//	在个人资料页中，要求双击用户51姓名的时候自动选中

class  CRBAutoSelectEdit : 
	public CColorEdit
{
	DECLARE_DYNAMIC(CRBAutoSelectEdit)

public:
	CRBAutoSelectEdit();
	virtual ~CRBAutoSelectEdit();

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};
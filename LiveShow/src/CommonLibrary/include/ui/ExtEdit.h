#pragma once

//=================================================================
//            文件中定义常用的扩展Edit

#include "Common.h"
#include "afxwin.h"
#include "ColorEdit.h"


class CLimitTextEdit;
class CAutoEndEdit;
class CShowHintEdit;

//=================================================================
// CLimitTextEdit
// 限制输入的Edit字节数目
 

class COMMON_INTERFACE CLimitTextEdit : 
	public CColorEdit
{
	DECLARE_DYNAMIC(CLimitTextEdit)

public:
	CLimitTextEdit();
	virtual ~CLimitTextEdit();

	/**
	@fn SetLimit( UINT nLimitNum)
	@brief 显示编辑框中输入的字节数目
	@param	[in] nLimitNum	限制的字节数目
	**/
	void	SetLimit( UINT nLimitNum);

	/**
	@fn GetLimit()
	@brief 获取编辑框中允许输入的字节的最大数目
	@return	编辑框中允许输入的字节的最大数目
	**/
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
//  CShowHintEdit
//	当Edit第一次获取焦点的时候，自动定位到文字的末尾

class COMMON_INTERFACE CAutoEndEdit : 
	public CLimitTextEdit
{
	DECLARE_DYNAMIC(CAutoEndEdit)

public:
	CAutoEndEdit();
	virtual ~CAutoEndEdit();

protected:
	DECLARE_MESSAGE_MAP()

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

private:
	bool	m_bFirstClick;
};


//=================================================================
//  CShowHintEdit
//	如果编辑框中没有任何的文本，当它失去焦点后，将在编辑框中
//  显示一段提示文本，当获取焦点后，该文本被清空

class COMMON_INTERFACE CShowHintEdit : 
	public CLimitTextEdit
{
	DECLARE_DYNAMIC(CShowHintEdit)

public:
	CShowHintEdit();
	virtual ~CShowHintEdit();

	/**
	@fn SetHintText( CString strHintText)
	@brief 设置失去焦点的时候的提示文字
	@param	[in] strHintText	提示文本
	**/
	void	SetHintText( CString strHintText) { m_strHintText = strHintText;}

	/**
	@fn GetHintText
	@brief 获取失去焦点的时候的提示文字
	@param	[in] strHintText	提示文本
	**/
	CString	GetHintText(){return m_strHintText;}

protected:
	DECLARE_MESSAGE_MAP()

	afx_msg void OnSetFocus( CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);

	CString	m_strHintText;
};
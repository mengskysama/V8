#pragma once

#include "..\Resource.h"
#include "Skin_i.h"
#include "afxcmn.h"
#include "EmotionListCtrl.h"
#include "ximage.h"
// CEmotionDlg 对话框
class CRoomMidDlg;
class CEmotionDlg : public CDialog
{
	DECLARE_DYNAMIC(CEmotionDlg)

public:
	CEmotionDlg( CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CEmotionDlg();

	void SetParentWnd(CWnd * pWnd);

// 对话框数据
	enum { IDD = IDD_EMOTION_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnSkinMessage(WPARAM wParam,LPARAM lParam);

	void ShowEmotion();
	void OnClickEmotionTab(int iTab);
	void OnClickPrePage();
	void OnClickNextPage();
	void InitFontPanel();
	void SetButtonStatus(bool bBold ,bool bItalic ,bool bUnderLine ) ;
	void OnClickBtnBold(int iBold);
	void OnClickBtnUnderline(int iUnderLine);
	void OnClickBtnXieTi(int iXieTi);
	void OnClickBtnColor();

	DECLARE_MESSAGE_MAP()

public:
	void OnSelEmotion(int iEmoGroup,int iEmoIndex);
	void SetRoomMidDlg(CRoomMidDlg* pRoomMidDlg);
private:
	CComPtr<ISkinEngine> m_pSkinEngine;
	CEmotionListCtrl m_emotionListCtrl;
	CWnd* m_pParent ;
	int m_nCurSel ;
	CRoomMidDlg* m_pRoomMidDlg;
	bool m_bInitFontPannel;//是否初始化过了
public:
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnCbnSelchangeComboFontInEmotionDlg();
	afx_msg void OnCbnSelchangeComboFontsizeInEmotionDlg();
	afx_msg void OnKillFocus(CWnd* pNewWnd);
};

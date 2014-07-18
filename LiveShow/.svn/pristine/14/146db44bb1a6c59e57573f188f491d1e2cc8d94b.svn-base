#pragma once

#include "..\Resource.h"
#include "Skin_i.h"
#include "SpeakInputLogic.h"
#include "..\RoomParentDlg.h"
#include "..\iectrl\OutInputHtmlEditImpl.h"

// CWishSpeakerDlg 对话框
class CRoomParentDlg ;
class CWishSpeakerDlg : public CDialog, public IHtmlSink
{
	DECLARE_DYNAMIC(CWishSpeakerDlg)

public:
	CWishSpeakerDlg( CRoomParentDlg* pParentDlg, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CWishSpeakerDlg();

	void SetInitContent(CString strContent);
	void SetContent(CString strContent);

// 对话框数据
	enum { IDD = IDD_WISH_SPEAKER_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnSkinMessage(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnClickEmotionList(WPARAM wParam ,LPARAM lParam) ;
	afx_msg LRESULT OnClickShortCutMsg(WPARAM wParam ,LPARAM lParam);

	void OnClickClose();
	void OnClickEmotion();
	void OnClickWish();
	void OnClickColor();
	void OnClickSend();

	void CheckInputLimit();

	DECLARE_MESSAGE_MAP()
public:
	virtual bool OnInput(int nLen);

private:
	CComPtr<ISkinEngine> m_pSkinEngine;
	CRoomParentDlg* m_pParentDlg ;
	CEmotionDlg * m_pEmotionDlg ;

	COutInputHtmlEditImpl* m_pSpeakerInputHtml;			//输入
	CSpeakInputLogic m_SpeakerInputLogic;
	int m_nLeft;
	CString m_strInitContent;
	
public:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

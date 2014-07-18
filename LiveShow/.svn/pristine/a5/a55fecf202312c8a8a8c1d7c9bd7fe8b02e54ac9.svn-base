#pragma once

#include "..\Resource.h"
#include "Skin_i.h"
#include "afxcmn.h"
//#include "HtmlInputLogic.h"
#include "..\loudspeaker\SpeakInputLogic.h"
#include "..\iectrl\OutInputHtmlEditImpl.h"
//#include "..\CommonLibrary\include\ui\VDCComboBox.h"
// CUserMgrDlg 对话框

class CEmotionDlg ;
class CRoomParentDlg ;
class CUserMgrDlg : public CDialog, public IHtmlSink
{
	DECLARE_DYNAMIC(CUserMgrDlg)

public:
	CUserMgrDlg( CRoomParentDlg* pParentDlg , CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CUserMgrDlg();

	core::MessageFont GetFontInfo();
	void Refresh();
	void OnClickSave();

// 对话框数据
	enum { IDD = IDD_USER_MGR_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnSkinMessage(WPARAM wParam,LPARAM lParam);


	void InitFontPanel();	//字体大小和字体名
	void InitDlgData();
	void OnClickEmotion();
	void NotifyParentModify();
	void EnableOwnDefine(BOOL bOwn);
	void SetFontInfo(core::MessageFont stFont);

	DECLARE_MESSAGE_MAP()
private:
	CComPtr<ISkinEngine> m_pSkinEngine;
	
	CRoomParentDlg* m_pParentDlg ;
	CComboBox m_FontSizeCombo;
	CComboBox m_FontCombo;

	COutInputHtmlEditImpl* m_pInputHtml ;			//输入
	//CHtmlInputLogic m_InputLogic ;
	CSpeakInputLogic m_InputLogic ;
	CEmotionDlg *m_pEmotionDlg ;
	BOOL m_bInitDataed ;
public:
	afx_msg void OnCbnSelchangeComboWelcomeFontsize();
	afx_msg void OnCbnSelchangeComboWelcomeFont();
	afx_msg void OnBnClickedRadioUserSysWelcomeWord();
	afx_msg void OnBnClickedRadioUseDefineWelcomeWord();
	afx_msg LRESULT OnClickEmotionList( WPARAM wParam ,LPARAM lParam ) ;
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);

	void SetApplyEffect(BOOL bEffect);
	virtual bool OnInput( int nContenLen );	

protected:
	bool SaveWelcomeWords(std::wstring strWelcomeWords, core::MessageFont stFont);
	bool LoadWelcomeWords(std::wstring & strWelcomeWords, core::MessageFont & stFont);
};

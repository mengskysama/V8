#pragma once

#include "..\Resource.h"
#include "Skin_i.h"
#include "afxwin.h"
#include "SpeakInputLogic.h"
#include "..\iectrl\OutInputHtmlEditImpl.h"

// CSpeakerDlg 对话框

class CRoomParentDlg ;
class CSpeakerDlg : public CDialog, public IHtmlSink
{
	DECLARE_DYNAMIC(CSpeakerDlg)

public:
	CSpeakerDlg(CRoomParentDlg* pParentDlg , core::ENM_LoudSpeakerType enmType, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSpeakerDlg();

// 对话框数据
	enum { IDD = IDD_SPEAKER_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnSkinMessage(WPARAM wParam,LPARAM lParam);
	afx_msg void OnBnClickedRadioSmallSpeaker();
	afx_msg void OnBnClickedRadioBigSpeaker();
	afx_msg void OnBnClickedRadioSuperSpeaker();
	afx_msg LRESULT OnClickEmotionList( WPARAM wParam ,LPARAM lParam ) ;
	afx_msg LRESULT OnClickShortCutMsg(WPARAM wParam ,LPARAM lParam);

	void OnClickSend();
	void OnClickColor();
	void OnClickInsertRoom();
	void OnClickEmotion();
	void OnClickWish();
	
	void CheckInputLimit();

	DECLARE_MESSAGE_MAP()
public:
	virtual bool OnInput( int nContenLen );	
	void SetInitContent(CString strContent);
	void SetContent(CString strContent);
	void SetType(core::ENM_LoudSpeakerType enmType);

private:
	CComPtr<ISkinEngine> m_pSkinEngine;
	CButton m_radioSmallSpeaker;
	CRoomParentDlg* m_pParentDlg ;

	COutInputHtmlEditImpl* m_pSpeakerInputHtml;			//输入
	CSpeakInputLogic m_SpeakerInputLogic;

	core::ENM_LoudSpeakerType m_enmSpeakerType;
	bool m_bCanInput;
	int m_nLeft;
	uint16 m_unSpeakerID;
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};

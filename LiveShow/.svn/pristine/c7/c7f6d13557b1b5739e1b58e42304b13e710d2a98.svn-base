#pragma once

#include "..\Resource.h"
#include "Skin_i.h"
#include "afxcmn.h"

// CSelfDefSentenceDlg 对话框

class CSelfDefSentenceDlg : public CDialog
{
	DECLARE_DYNAMIC(CSelfDefSentenceDlg)

public:
	CSelfDefSentenceDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSelfDefSentenceDlg();

// 对话框数据
	enum { IDD = IDD_SELF_DEFINE_SENTENCE_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnSkinMessage(WPARAM wParam,LPARAM lParam);

	void InitSentenceList();

	DECLARE_MESSAGE_MAP()

private:
	CComPtr<ISkinEngine> m_pSkinEngine;

	CListCtrl m_SentenceListCtrl;
	std::vector<wstring> m_vShortCutMsgList ;
public:
	afx_msg void OnLvnItemchangedListSelfdefSentence(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMHoverListSelfdefSentence(NMHDR *pNMHDR, LRESULT *pResult);
};

#pragma once
#include "..\Resource.h"
#include "Skin_i.h"
#include "..\screeneffect\FlyingInputRichEdit.h"
#include "..\usermanager\useritem.h"
#include "igiftitem.h"
// CGiftWordsDlg 对话框

class CRoomParentDlg ;

class CGiftWordsDlg : public CDialog
{
	DECLARE_DYNAMIC(CGiftWordsDlg)

public:
	CGiftWordsDlg( CRoomParentDlg* pParentDlg  ,CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CGiftWordsDlg();

// 对话框数据
	enum { IDD = IDD_ROOM_GIFT_WORDS_DLG };

	void UpdateGiftWordsInfo( IGiftItem * pGiftItem , uint32 unI79ID ,uint32 unUIN , std::wstring strUserName , uint16 unCount );						   //更新礼物信息79
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnSkinMessage(WPARAM wParam, LPARAM lParam);

	void OnClickBtnSendWords();                        //点击发送按钮	

	DECLARE_MESSAGE_MAP()
	void OnInputChange() ;

private:
	CComPtr<ISkinEngine> m_pSkinEngine; 
	CFlyingInputRichEdit m_InputCtrl;
	CRoomParentDlg* m_pParentDlg ;
	IGiftItem * m_pGiftItem ; 
	uint32 m_unUIN ;
	uint16 m_unCount ;
};

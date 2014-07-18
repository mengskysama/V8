#pragma once
#include "..\Resource.h"
#include "Skin_i.h"
#include "MyGoodsListCtrl.h"


// CMyGoodsDlg 对话框

class CMyGoodsDlg : public CDialog
{
	DECLARE_DYNAMIC(CMyGoodsDlg)

public:
	CMyGoodsDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMyGoodsDlg();

// 对话框数据
	enum { IDD = IDD_MYGOODS_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

private:
	CComPtr<ISkinEngine> m_pSkinEngine;
	CMyGoodsListCtrl m_MyGoodsListCtrl;

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnInitDialog();
};

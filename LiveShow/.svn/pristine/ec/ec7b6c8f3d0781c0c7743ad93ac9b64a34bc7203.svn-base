#pragma once

#include "..\Resource.h"
#include "Skin_i.h"

// CPhotoDlg 对话框

class CPhotoDlg : public CDialog
{
	DECLARE_DYNAMIC(CPhotoDlg)

public:
	CPhotoDlg(const CString& strDirPath ,const CString& strFilePath ,CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPhotoDlg();

// 对话框数据
	enum { IDD = IDD_PHOTO_SUCCESS_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnSkinMessage(WPARAM wParam,LPARAM lParam);

	void OnClickPhotoDir();//点击照片目录
	void OnClickPreview();//点击浏览

	DECLARE_MESSAGE_MAP()
private:
	CComPtr<ISkinEngine> m_pSkinEngine;
	CString m_strDirPath ;
	CString m_strFilePath ;
public:
	afx_msg void OnNcDestroy();
};

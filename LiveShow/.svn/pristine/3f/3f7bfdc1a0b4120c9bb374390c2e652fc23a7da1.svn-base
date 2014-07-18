#pragma once

#include "resource.h"
#include "Skin_i.h"
// CHeadSettingDlg 对话框

class CPicTracker;
class CHeadSettingDlg : public CDialog
{
	DECLARE_DYNAMIC(CHeadSettingDlg)

public:
	CHeadSettingDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CHeadSettingDlg();

// 对话框数据
	enum { IDD = IDD_HEAD_SETTING_DLG };

	
	void OnEventRespHeadUpload(Event const& e);  //上传自定义头像请求返回响应函数	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnSkinMessage(WPARAM wParam, LPARAM lParam);
	void OnClose();

	void OnClickBtnUploadLocal();//点击本地上传
	void OnClickBtnConfirm();//点击确定
	void ModifyHeadImageTime(time_t imageTime); //修改头像文件时间

	void EventSendHeadUpload(); //发送上传自定义头像请求
    BOOL PopPicTracker(CString strImgPath);//弹出图片裁剪框
	BOOL SetHeaderImage(CString strImagePath);//设置头像
	CString GetHeadImagePath(); //获取用户头像路径
	int ShowOpenFileDlg2(CString& sFilePath,  //打开指定格式的图片文件选择框
		CString  sInitPath,       
		PBOOL    pbReadOnly/* = NULL*/, 
		LPCTSTR  lpszDefExt/* = NULL*/,
		LPCTSTR  lpszFileName/* = NULL*/,
		LPCTSTR  lpszFilter, // = NULL"图象文件(*.bmp;*.jpg;*.gif)|*.bmp;*.jpg;*.gif|所有文件(*.*)|*.*||"
		DWORD    dwFlags/* = OFN_ENABLESIZING*/,
		CWnd*    pParentWnd/* = NULL*/);         

	DECLARE_MESSAGE_MAP()
private:
	CComPtr<ISkinEngine> m_pSkinEngine;

    CPicTracker *m_picTracker;
	CString m_strHeadImage;//头像的存放路径
	uint32 nHeadSeqNo; //上传头像时的序列号
	bool m_upload;  //是否需要上传
};

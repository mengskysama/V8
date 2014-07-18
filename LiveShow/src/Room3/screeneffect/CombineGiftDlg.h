#pragma once
#include "..\Resource.h"
#include "ScreenEffectManager.h"


// CCombineGiftDlg 对话框

class CCombineGiftDlg : public CDialog
{
	DECLARE_DYNAMIC(CCombineGiftDlg)

public:
	CCombineGiftDlg(combinegift_info * pInfo, IGiftItem * pGiftItem, std::wstring strSrcName, std::wstring strDstName, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCombineGiftDlg();

// 对话框数据
	enum { IDD = IDD_COMBINEGIFTDLG };

	bool IsFinish() { return m_bFinish; }

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnInitDialog();

protected:
	void LoadRes();
	void Draw();

protected:
	//HINSTANCE m_hInst;
	BLENDFUNCTION m_blend;
	//typedef BOOL (WINAPI* FUpdateLayeredWindow)(HWND,HDC,POINT*,SIZE *,HDC,POINT*,COLORREF,BLENDFUNCTION* ,DWORD);
	//FUpdateLayeredWindow UpdateLayeredWindow ;
	combinegift_info * m_pInfo;						//组合礼物信息
	Image * m_pBackImage;							//背景图片
	Image * m_pCardImage;							//卡片图片
	Image * m_pUnitImage;							//单元图片
	Image * m_pFrontImage;							//前景图片
	std::wstring m_strSrcName;						//赠送用户名
	std::wstring m_strDstName;						//目标用户名
	IGiftItem * m_pGiftItem;						//道具信息
	std::vector<CPoint> m_vecBeginPos;				//坐标信息
	int m_nFrames;									//帧数
	int m_nType;									//效果类型
	int m_nPointPerCircle;							//随机点每次循环画多少点
	bool m_bFirst;
	int m_nCount;
	bool m_bFinish;									//展示完成
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

#pragma once

#include "..\Resource.h"
#include "Skin_i.h"
// CRoomActorMenuDlg 对话框
#include "..\usermanager\UserManager.h"
#include "..\..\CommonLibrary\include\ui\ImageImpl.h"

#define ROOMACTOR_MOUSE_TIMER_IDEVENT 1
#define ROOMACTOR_MOUSE_TIMER_INTERVALTIME 500
#define ROOMACTOR_GIDUE_TIMER_IDEVENT 2  
#define ROOMACTOR_GIDUE_TIMER_INTERVALTIME 60*1000

struct ActorInfo
{
	uint32 m_ActorUin;       //艺人uin
	uint32 m_VipLevel;       //身份
	uint8 m_RichLevel;       //富豪等级
	bool m_IsLine;           //是否在线	(即是否在房间)
	bool m_IsMan;            //是否为男
	std::wstring m_ActorRoleName;  //艺人昵称	
	common::ui::CImageImpl* m_HeadImage;     //原色头像
	common::ui::CImageImpl* m_HeadImageGray; //灰色头像


	ActorInfo()
	{
		m_ActorUin = 0;
		m_VipLevel = 0;
		m_RichLevel = 0;
		m_IsLine = 0;	
		m_IsMan = 0;
		m_HeadImage = NULL;
		m_HeadImageGray = NULL;

	}
};

class CRoomActor;
class CRoomParentDlg ;
class CRoomActorMenuDlg : public CDialog
{
	DECLARE_DYNAMIC(CRoomActorMenuDlg)

public:
	CRoomActorMenuDlg( CRoomParentDlg* pParentDlg ,CRoomActor* pRoomActor,CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRoomActorMenuDlg();

// 对话框数据
	enum { IDD = IDD_ROOMACTOR_MENU_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog() ;
	void OnTimer(UINT_PTR nIDEvent);

	DECLARE_MESSAGE_MAP()
public:
	void Show(ActorInfo *pActor);
	void Hide();
	void SetActorInfo(ActorInfo *pActor);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseLeave(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnSkinMessage(WPARAM wParam,LPARAM lParam) ;
	void OnClickbtnActorInfo() ;  //主播信息
	void OnClickbtnSelTalk() ;  //选中聊天对象
	void OnClickbtnSelSendGifts(); //选中送礼对象
	void OnClickbtnSeePrivateVideo();//观看私麦视频
	void OnClickbtnOneToOneVideo();//一对一视频

private:
	CComPtr<ISkinEngine> m_pSkinEngine;
	CRoomParentDlg* m_pParentDlg ;
	CRoomActor* m_RoomActor;
	ActorInfo* m_pActor;
};

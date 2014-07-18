#pragma once
#include "..\usermanager\useritem.h"
#include "..\..\CommonLibrary\include\ui\MenuEx.h"
#include <map>
using namespace std;
class CRoomDlg ;
class CRoomMidDlg ;
class CRoomParentDlg ;

class UserListMenu
{
public:
	UserListMenu(CWnd * pWnd);
	~UserListMenu(void);

	void PopupMenu(IUserItem * pMyItem, IUserItem * pDstItem);
	void PopupMenu( IUserItem * pDstItem ) ;
	void PopupGuestMenu();
	void OnCommand( UINT32 nCommand ) ;
private:
	CWnd * m_pWnd;
};

class CaiZiMenu
{
public:
	CaiZiMenu( CWnd * pWnd ) ;
	~CaiZiMenu() ;
	void PopupMenu() ;
private:
	CWnd * m_pWnd;
};
class ShortCutMsgMenu
{
public:
	//ShortCutMsgMenu( CRoomDlg* pRoomDlg ) ;
	ShortCutMsgMenu(CWnd * pWnd);
	~ShortCutMsgMenu() ;
	void PopupMenu() ;
private:
	//CRoomDlg * m_pRoomDlg ;
	CWnd * m_pWnd;
};

class AutoReplyMenu
{
public:
	AutoReplyMenu(CWnd* pWnd);
	~AutoReplyMenu();
	void PopupMenu();

	common::ui::CMenuEx *  GetAutoReplyMenu();//作为子菜单时被调用
	void DeleteReplyMenu();//作为子菜单被调用后删除工作
private:
	CWnd* m_pWnd;
	common::ui::CMenuEx * m_pMenu;
};

class MicDlgMenu
{
public:
	MicDlgMenu( CRoomParentDlg* pParentDlg ) ;
	~MicDlgMenu() ;
	void PopupMenu( IUserItem * pMyItem, int16 nIndex , IUserItem * pDstItem ) ;
	void PopupMenu( IUserItem* pMyItem , int16 nIndex ) ;
	void OnCommand( UINT32 nCommand ,UINT32 unDstUIN , int16 nIndex ) ;
	void OnCommand( UINT32 nCommand ,int16 nIndex ) ;
private:
	CRoomParentDlg * m_pParentDlg ;
};

#define FLYINT_TEXT_EFFECT_RANDOM	100

class FlyTextMenu
{
public:
	FlyTextMenu(CWnd * pWnd, int nType);
	~FlyTextMenu();

	int PopupMenu();

private:
	CWnd * m_pWnd;
	int m_nType;
};
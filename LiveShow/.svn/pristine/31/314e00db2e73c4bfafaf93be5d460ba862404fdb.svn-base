#pragma once
#include "coredefine.h"
#include "..\usermanager\useritem.h"
#include "SofaItem.h"

struct ISofaManagerSink 
{
	virtual void OnBuySofaNotify(CSofaItem * pSofaItem) = 0;				//购买沙发通知
	virtual void OnSofaInfoUpdate(std::vector<CSofaItem*> vecSofaItem) = 0;	//沙发用户信息更新
};

class CRoomParentDlg;
class CSofaManager
{
public:
	CSofaManager(uint32 unRoomID, ISofaManagerSink * pSofaManagerSink);
	~CSofaManager(void);

	void InitList(core::GetSofaListRspEvent::SofaInfo sofainfo[]);
	void ClearSofaList();
	void UpdateSofa(uint8 unIndex, uint32 unPrice, IUserItem * pUserItem);
	bool IsOnSofa(uint32 unUIN);
	void UpdateSofaHead(uint32 unUIN);
	void UpdateSofaVipLevel(uint32 unUIN, uint8 unVipLevel);
	void UpdateSofaRichLevel(uint32 unUIN, uint8 unRichLevel);
	void UpdateSofaUserInfo(IUserItem * pUserItem);
	CString GetInitString();
	CString GetOneSofaString(uint8 unIndex, bool bJustUpdate = false);
	void FindSofaIndexByUIN(uint32 unUIN, std::vector<uint8> & vecIndex);
	CSofaItem * GetSofaItemByIndex(uint8 unIndex);

protected:
	uint32 m_unRoomID;
	std::map<uint8, CSofaItem*> m_mapSofaList;
	ISofaManagerSink * m_pSofaManagerSink;
};

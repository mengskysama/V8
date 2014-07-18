#pragma once

#include "ILogicListener.h"
#include "RC.h"

enum
{
	kReason_User_Cancel   =  0x00,
	kReason_Net_Error     =  0x01
};

class AVChannel : public ILogicListener, public RCObject
{
public:

	AVChannel(UInt32 iRoomId,UInt32 iFromUin, UInt8 cLogicID,bool bA);
	virtual ~AVChannel();

	bool                      Start(UInt64 lSymbolTime);
	void                      Stop(bool bRemoveListener=true);

	void                      UserCanncel();

	bool                      Maintain();

	inline UInt32             GetRoomId()      {  return m_iRoomId;   }
	inline UInt8              GetLogicID()     {  return m_cLogicID;  }

protected:

	virtual void              onConnected();
	virtual void              onDisconnect(UInt8 cReason);
	virtual void              onStatus(UInt16 sStatus);

	virtual void              onTimer() {}      //在 maintain() 中周期执行

	virtual void              onReceive(const UInt8* pBuf, UInt32 iLen, unsigned int iSeq) = 0;
	virtual void              StartSelfAV()                             = 0;
	virtual void              StopSelfAV()                              = 0;

protected:

	bool              m_bUserCancel;
	bool              m_bNeedDestory;
	UInt32            m_iRoomId;
	UInt8             m_cLogicID;
	UInt32			  m_iFromUin;
	bool			  m_bA;
};
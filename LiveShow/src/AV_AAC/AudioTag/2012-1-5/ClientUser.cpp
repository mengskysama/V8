#include "stdafx.h"
#include "ClientUser.h"
#include "AudioOutput.h"
#include "Global.h"

std::map<UInt64, ClientUser *> ClientUser::c_qmUsers;
Mutex ClientUser::c_lUsers;


ClientUser *ClientUser::get(UInt64 uiSession) {
	MutexLocker lock(&c_lUsers);
	ClientUser *p = (c_qmUsers.find(uiSession) == c_qmUsers.end())?NULL : c_qmUsers.find(uiSession)->second;
	return p;
}

ClientUser *ClientUser::add(UInt64 uiSession) {
	MutexLocker lock(&c_lUsers);

	ClientUser *p = new ClientUser;
	p->uiSession = uiSession;
	c_qmUsers[uiSession] = p;
	return p;
}

bool ClientUser::isPause(UInt64 uiSession)
{
	if (c_qmUsers.find(uiSession) != c_qmUsers.end())
	{
		return c_qmUsers.find(uiSession)->second->bPause;
	}
	return false;
}

bool ClientUser::setPause(UInt64 uiSession,bool bIsPause)
{
	if (c_qmUsers.find(uiSession) != c_qmUsers.end())
	{
		c_qmUsers.find(uiSession)->second->bPause = bIsPause;
		return true;
	}
	return false;
}

void ClientUser::remove(UInt64 uiSession) {
	MutexLocker lock(&c_lUsers);
	ClientUser *p = (c_qmUsers.find(uiSession) == c_qmUsers.end()) ? NULL : c_qmUsers.find(uiSession)->second;
	if (p) {
		AudioOutputPtr ao = g_struct.ao;
		if (ao)
			ao->removeBuffer(p);
	}
	c_qmUsers.erase(uiSession);
}

void ClientUser::clearRoom(UInt32 iRoomId)
{
	MutexLocker lock(&c_lUsers);
	std::map<UInt64, ClientUser *>::iterator iter;
	for (iter=c_qmUsers.begin();iter!=c_qmUsers.end();)
	{	
		UInt64 lUser= iter->first;
		UInt32 iRoom = (UInt32)lUser>>32;
		if (iRoom == iRoomId)
		{
			AudioOutputPtr ao = g_struct.ao;
			if (ao)
				ao->removeBuffer(iter->second);
			
			iter = c_qmUsers.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}

void ClientUser::clearRoom()
{
	MutexLocker lock(&c_lUsers);
	std::map<UInt64, ClientUser *>::iterator iter;
	for (iter=c_qmUsers.begin();iter!=c_qmUsers.end();)
	{	
		AudioOutputPtr ao = g_struct.ao;
		if (ao)
			ao->removeBuffer(iter->second);
		iter = c_qmUsers.erase(iter);
	}
}

void ClientUser::remove(ClientUser *p) {
	remove(p->uiSession);
}

bool ClientUser::isRoomUp(UInt32 iRoomId)
{
	UInt32 iRoom = 0;
	std::map<UInt64, ClientUser *>::iterator iter;
	for(iter=c_qmUsers.begin();iter!=c_qmUsers.end();++iter)
	{
		UInt64 iSession = iter->first;
		iRoom = (UInt32)(iSession >> 32);
		if (iRoom == iRoomId)
		{
			return true;
		}
	}
	return false;
}
#include "stdafx.h"
#include "BufferManager.h"
#include "BufferEngine.h"

CBufferManager g_bufferManager;

CBufferManager::CBufferManager()
{
	m_isRelease = false;
	BufferTrace("CBufferManager::CBufferManager() begin\n");
}
CBufferManager::~CBufferManager()
{
	BufferTrace("CBufferManager::~CBufferManager() begin\n");
}

void CBufferManager::Initialize()
{
	g_bufferEngine.Initialize();
}

void CBufferManager::UnInitialize()
{
	g_bufferEngine.UnInitialize();
}

void CBufferManager::DestroyInstance()
{
	ErrTrace("CBufferManager::DestroyInstance() begin\n");
	g_bufferEngine.DestroyInstance();
	ErrTrace("CBufferManager::DestroyInstance() end\n");
}

void CBufferManager::AddInputPacket(UInt32 iRoomId,Packet* pDataPacket,UInt32 iSequence,bool bA)
{
	pDataPacket->AddReference();
	pDataPacket->resetOffset();
	g_bufferEngine.AddInputPacket(iRoomId,pDataPacket,iSequence,bA);
}

void CBufferManager::AddOutputPacket(UInt32 iRoomId,UInt32 iFromUin,Packet* pDataPacket,UInt32 iSequence,bool bA)
{
	if (IsRelease()) 
	{
		ErrTrace("CBufferManager::AddOutputPacket 正在销毁对象.....操作失败\n");
		return;
	}
	pDataPacket->AddReference();
	pDataPacket->resetOffset();
	g_bufferEngine.AddOutputPacket(iRoomId,iFromUin,pDataPacket,iSequence,bA);
}

bool CBufferManager::GetInputPacket(UInt32 iRoomId,Packet* &pDataPacket,UInt32 iSequence,bool bA)
{
	return g_bufferEngine.GetInputPacket(iRoomId,pDataPacket,iSequence,bA);
}

void CBufferManager::ClearInputPackets(UInt32 iRoomId,bool bA)
{
	g_bufferEngine.ClearInputPackets(iRoomId,bA);
}

void CBufferManager::ClearOutputPackets(UInt32 iRoomId,UInt32 iFromUin,bool bA)
{
	g_bufferEngine.ClearOutputPackets(iRoomId,iFromUin,bA);
}

void CBufferManager::SetRoomOnMic(UInt32 iRoomId,bool bEnable)
{
	g_bufferEngine.SetRoomOnMic(iRoomId,bEnable);
}

void CBufferManager::EchoAudioInputInfo(UInt32 iRoomId)
{
	g_bufferEngine.EchoAudioInputInfo(iRoomId);
}

void CBufferManager::EchoVideoInputInfo(UInt32 iRoomId)
{
	g_bufferEngine.EchoVideoInputInfo(iRoomId);
}
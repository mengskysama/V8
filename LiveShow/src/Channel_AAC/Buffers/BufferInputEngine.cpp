#include "stdafx.h"
#include "BufferInputEngine.h"

CBufferInputEngine::CBufferInputEngine(UInt32 iRoomId)
:m_iRoomId(iRoomId)
{
		BufferTrace("CBufferInputEngine::CBufferInputEngine() begin\n");
}
CBufferInputEngine::~CBufferInputEngine()
{
		BufferTrace("CBufferInputEngine::~CBufferInputEngine() begin\n");
}

void	CBufferInputEngine::AddInputPacket(Packet* pDataPacket,UInt32 iSequence,bool bA) 
{
		BufferTrace("CBufferInputEngine::AddInputPacket() begin\n");
		if (bA)
		{
			m_inputAudioEngine.AddPacket(pDataPacket,iSequence);
		}
		else
		{
			m_inputVideoEngine.AddPacket(pDataPacket,iSequence);
		}
}

bool	CBufferInputEngine::GetInputPacket(Packet* &pDataPacket,UInt32 iSequence,bool bA) 
{
		BufferTrace("CBufferInputEngine::GetInputPacket() begin\n");
		if (bA)
		{
			return m_inputAudioEngine.GetPacket(pDataPacket,iSequence);
		}
		else
		{
			return m_inputVideoEngine.GetPacket(pDataPacket,iSequence);
		}
}

void	CBufferInputEngine::ClearInputPackets(bool bA)
{
		BufferTrace("CBufferInputEngine::ClearInputPackets() begin\n");
		if (bA)
		{
			m_inputAudioEngine.ClearPackets();
		}
		else
		{
			m_inputVideoEngine.ClearPackets();
		}
}

void	CBufferInputEngine::KeepBalance()
{
		Trace("CBufferInputEngine::KeepBalance() begin \n");
		m_inputAudioEngine.KeepBalance();
		m_inputVideoEngine.KeepBalance();
		Trace("CBufferInputEngine::KeepBalance() end \n");
}

void	CBufferInputEngine::Initialize()
{
		BufferTrace("CBufferInputEngine::Initialize() begin\n");
		m_inputVideoEngine.Initialize();
		m_inputAudioEngine.Initialize();
}

void	CBufferInputEngine::UnInitialize()
{
		BufferTrace("CBufferInputEngine::UnInitialize() begin\n");
		m_inputVideoEngine.UnInitialize();
		m_inputAudioEngine.UnInitialize();
}

bool	CBufferInputEngine::GetEngineUsed()
{
		BufferTrace("CBufferInputEngine::GetEngineUsed() begin\n");
		return m_inputVideoEngine.GetEngineUsed() || m_inputAudioEngine.GetEngineUsed();
}

void	CBufferInputEngine::EchoAudioInputInfo()
{
		m_inputAudioEngine.EchoAudioInputInfo();
}

void	CBufferInputEngine::EchoVideoInputInfo()
{
		m_inputVideoEngine.EchoVideoInputInfo();
}
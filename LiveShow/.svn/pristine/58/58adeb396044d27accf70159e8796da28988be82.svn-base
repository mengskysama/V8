#include "stdafx.h"
#include "CommonPacket.h"
#include "MyAssert.h"


Packet*  BinaryPacket::makePacket(SInt32 iID, UInt16 sVersion, UInt16 sPacketType, UInt32 iBodylen)
{
	Packet* pPacket = new Packet(iID, BINARY_PACKET_HEADER_LEN+iBodylen);
	if (pPacket)
	{
		pPacket->writeUInt16(sVersion);
		pPacket->writeUInt16(sPacketType);
	}

	return pPacket;
}

Bool	BinaryPacket::parserPacket(Packet* pPacket)
{
	Bool bRet = 0;
	if (pPacket)
	{
		bRet = pPacket->checkBytesLeft(BINARY_PACKET_HEADER_LEN);
		if(bRet)
		{
			m_sVersion      =   pPacket->readUInt16();
			m_sPacketType   =   pPacket->readUInt16();
		}
		else
		{
			Assert(0);
		}
	}
	return bRet;
}

Bool    Packet::checkBytesLeft(UInt32 inLenLeft)
{
		if ((inLenLeft < 0) || (m_iPacketLen-m_iOffset< inLenLeft))
		{
			return 0;
		}
		return 1;
}

void    Packet::writeUInt8(UInt8 cValue)
{
		if(checkBytesLeft(1))
		{
			m_pPacketContent[m_iOffset++] = (UInt8)cValue;
		}
		else
		{
			Assert(0);  
		}
}

UInt8    Packet::readUInt8()
{
	UInt8 ret=0;
	if(checkBytesLeft(1))
	{
		ret=(UInt8)m_pPacketContent[m_iOffset++];
	}
	else
	{
		Assert(0);  
	}
	return ret;
}

void    Packet::writeUInt16(UInt16 value)
{
		if(checkBytesLeft(2))
		{
			UInt16* ptr=(UInt16*)(&(m_pPacketContent[m_iOffset]));
			*ptr =htons(value);
			m_iOffset += 2;
		}
		else
		{
			Assert(0);      
		}
}

UInt16  Packet::readUInt16()
{
		UInt16 ret=0;
		if(checkBytesLeft(2))
		{
			UInt16* ptr=(UInt16*)&m_pPacketContent[m_iOffset];
			ret=ntohs(*ptr);
			m_iOffset += 2;
		}
		else
		{
			Assert(0);        
		}
		return ret;
}

void    Packet::writeUInt32(UInt32 value)
{
		if(checkBytesLeft(4))
		{
			UInt32* ptr=(UInt32*)&m_pPacketContent[m_iOffset];
			*ptr=htonl(value);
			m_iOffset += 4;
		}
		else
		{
			Assert(0);     
		}
}

UInt32  Packet::readUInt32()
{
		UInt32 ret=0;
		if(checkBytesLeft(4))
		{
			UInt32* ptr=(UInt32*)&m_pPacketContent[m_iOffset];		
			ret=ntohl(*ptr);
			m_iOffset += 4;
		}
		else
		{
			Assert(0);      
		}
		return ret;
}

void	Packet::writeUInt64(UInt64 value)
{
		if(checkBytesLeft(8))
		{
			UInt32	part1=((UInt32)(value>>32))&0xFFFFFFFFL;
			UInt32	part2=((UInt32)value)&0xFFFFFFFFL;
			writeUInt32(part1);
			writeUInt32(part2);
		}
		else
		{
			Assert(0); 
		}
}

UInt64 Packet::readUInt64()
{
		UInt64 ret=0;
		if(checkBytesLeft(8))
		{
			UInt64 part1 = readUInt32();
			UInt32 part2 = readUInt32();

			ret= (part1 << 32) + (part2 & 0xFFFFFFFFL);
		}
		else
		{
			Assert(0);   
		}
		return ret;
}

void	Packet::writeByteArray(const UInt8* pData, UInt32 iDataLen)
{
	    UInt8* contPointer=&m_pPacketContent[m_iOffset];
	    if (checkBytesLeft(iDataLen))
	    {
			::memcpy(contPointer, &pData[0], iDataLen);
			m_iOffset+=iDataLen;
	    }
		else
		{
			Assert(0);
		}
}

bool  Packet::readByteArray(UInt8* pData, UInt32 iDataLen)
{
	    bool bRet = true;
	    UInt8* contPointer=&m_pPacketContent[m_iOffset];
		if(checkBytesLeft(iDataLen))
		{		
			::memcpy(pData, contPointer, iDataLen);
			m_iOffset += iDataLen;
		}
		else
		{
			bRet = false;
		}

		return bRet;
}

void  Packet::AddReference()
{
	++m_iRefCount;
}

void  Packet::RemoveReference()
{
	Assert(m_iRefCount!=0);
	--m_iRefCount;
	if (0==m_iRefCount) 
	{
		delete this;
	}
}
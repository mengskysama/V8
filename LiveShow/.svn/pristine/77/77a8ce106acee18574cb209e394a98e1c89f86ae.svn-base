#include "ProtocolBase.h"
#include "..\CoreModule.h"
#include "..\crypt\Crypt.h"
#include "..\TokenConnection.h"

uint16 ClientPacket::m_seq = 0;
std::map<uint32, uint32> ClientPacket::m_seq2AttachData;

void CLIENT_HEADER::Pack(common::net::Packet & packet)
{
	packet<<version<<cmd<<uin<<roomid<<seq<<option;
}

void SERVER_HEADER::Unpack(common::net::Packet & packet)
{
	packet>>version>>cmd>>uin>>roomid>>seq>>option;
}

//////////////////////////////////////////////////////////////////////////

ClientPacket::ClientPacket(uint32 cmd, uint32 roomid)
{
	this->m_header.cmd		=	cmd;
	this->m_header.roomid	=	roomid;
	this->m_header.uin		=	CoreModule::GetCurInfo().unUIN;
}

ClientPacket::~ClientPacket()
{
}

void ClientPacket::Pack()
{
	m_header.seq = NextSeq();

	memset(data,0,sizeof(data));

	assert(data == cursor);

	if(data != cursor)
	{
		MessageBox(NULL, L"ClientPacket::ClientPacket 可能是多次pack了！", L"出错啦！", MB_OK);
	}

	m_header.Pack(*this);
}

void ClientPacket::HeadBusiness()
{
	ClientPacket::Pack();
}

void ClientPacket::TailBusiness()
{
	ByteBuf buf(Size()-sizeof(CLIENT_HEADER), data+sizeof(CLIENT_HEADER));

	Encode(buf);

	MoveCur(buf.first+sizeof(CLIENT_HEADER), common::net::Packet::from_begin);
}

void ClientPacket::Encode(ByteBuf& buf)
{
	static char realData[MAX_PACKET_SIZE];

	int length = Auth51::CXTEA::Encrypt((char*)buf.second, buf.first, realData, 
		MAX_PACKET_SIZE, (char*)TokenConnection::GetSessionKey());

	memcpy(buf.second, realData, length);

	buf.first = length;
}

//////////////////////////////////////////////////////////////////////////

void ServerPacket::Decode(ByteBuf& buf)
{
	static char realData[MAX_PACKET_SIZE];

	int length = Auth51::CXTEA::Decrypt((char*)buf.second, buf.first, realData, 
		MAX_PACKET_SIZE, (char*)TokenConnection::GetSessionKey());

	memcpy(buf.second, realData, length);

	buf.first = length;
}

void ServerPacket::HeadBusiness()
{
	Decode(ByteBuf(dataSize-2, data));
}
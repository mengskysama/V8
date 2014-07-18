/**
* ConnectionCommon.h
* 
*
* @version 1.0 (06 ,04, 2008)
* @author reinhardt ken
*
*/
#pragma  once

#include "ipport.h"
#include <vector>

namespace netlayer
{
	enum COMM_TYPE { //通讯类型
		UDP,
		TCPCLIENT, //三字节头
		TCPSERVER, //tcp server
		HTTPPROXY,//三字节头
		SOCK5PROXY,//三字节头

		TCPCLIENT2,//两字节头
		HTTPPROXY2,//两字节头		
		SOCK5PROXY2,//两字节头

		TCPCLIENT4,//四字节头
		HTTPPROXY4,//四字节头
		SOCK5PROXY4,//四字节头
	};

	enum {COMM_FLAG = 0x00};
	enum {MAX_PACKET_LEN = 1500};
	enum COMM_MSG {
		COMM_ACCEPT,
		COMM_CONNECT,
		COMM_CLOSE,
		COMM_DATA,
		COMM_ERROR,//by ken 新加，用于tcp受到错误数据时
		COMM_CLOSE_NEED_PS,

	};

	//convert.h/////////////////
	namespace Convert
	{
		template <typename T>
		inline void ToHostOrder(T& v)
		{
			v.ToHostOrder();
		}

		template <>
		inline void ToHostOrder<char>(char& v)
		{
			return;
		}

		template <>
		inline void ToHostOrder<unsigned char>(unsigned char& v)
		{
			return;
		}

		template <>
		inline void ToHostOrder<short>(short& v)
		{
			v = ntohs(v);
		}

		template <>
		inline void ToHostOrder<unsigned short>(unsigned short& v)
		{
			v = ntohs(v);
		}

		template <>
		inline void ToHostOrder<int>(int& v)
		{
			v = ntohl(v);
		}

		template <>
		inline void ToHostOrder<unsigned int>(unsigned int& v)
		{
			v = ntohl(v);
		}

		template <>
		inline void ToHostOrder<long>(long& v)
		{
			v = ntohl(v);
		}

		template <>
		inline void ToHostOrder<unsigned long>(unsigned long& v)
		{
			v = ntohl(v);
		}

		template <typename T>
		inline void ToNetOrder(T& v)
		{
			v.ToNetOrder();
		}

		template <>
		inline void ToNetOrder<char>(char& v)
		{
			return;
		}

		template <>
		inline void ToNetOrder<unsigned char>(unsigned char& v)
		{
			return;
		}

		template <>
		inline void ToNetOrder<short>(short& v)
		{
			v = htons(v);
		}

		template <>
		inline void ToNetOrder<unsigned short>(unsigned short& v)
		{
			v = htons(v);
		}

		template <>
		inline void ToNetOrder<int>(int& v)
		{
			v = htonl(v);
		}

		template <>
		inline void ToNetOrder<unsigned int>(unsigned int& v)
		{
			v = htonl(v);
		}

		template <>
		inline void ToNetOrder<long>(long& v)
		{
			v = htonl(v);
		}

		template <>
		inline void ToNetOrder<unsigned long>(unsigned long& v)
		{
			v = htonl(v);
		}

		template <typename T>
		inline T ToHostOrderByVal(T v)
		{
			ToHostOrder(v);
			return v;
		}

		template <typename T>
		inline T ToNetOrderByVal(T v)
		{
			ToNetOrder(v);
			return v;
		}
	};
		
#pragma pack(1)

	struct Header //通讯层的包头
	{
		unsigned char ucFlag;	//flag

		union
		{
			unsigned short usShortSize;	//数据长度，不包括Header的长度
			unsigned int   unIntSize;
		};

		inline Header(): ucFlag(COMM_FLAG), unIntSize(0)
		{
		}

		inline void ToHostOrder()
		{
			Convert::ToHostOrder(usShortSize);
		}

		inline void ToNetOrder()
		{
			Convert::ToNetOrder(usShortSize);
		}

		inline void ToIntHostOrder()
		{
			Convert::ToHostOrder(unIntSize);
		}

		inline void ToIntNetOrder()
		{
			Convert::ToNetOrder(unIntSize);
		}
	};

	class Packet
	{
	public:
		enum HEADTYPE
		{
			ZERO_BYTES_HEAD   = 0,//null
			TWO_BYTES_HEAD    = 2,//short
			THREE_BYTES_HEAD  = 3,//flag + short
			FOUR_BYTES_HEAD   = 4,//int
			UNKNOWN_HEAD,
		};

	private:

		Header oHeader;
		std::vector<unsigned char> arrBuf;

	public:
		Packet():arrBuf(MAX_PACKET_LEN)
		{
		}

		void resize(int nBufferSize)
		{
			if(nBufferSize > MAX_PACKET_LEN)
			{
				arrBuf.resize(nBufferSize);
			}
		}

		inline bool SetData(const void * pBuf, unsigned int uiBytes, HEADTYPE sendheadType)
		{
			if (uiBytes+sizeof(Header) > MAX_PACKET_LEN)
			{
				arrBuf.resize(uiBytes+sizeof(Header));
			}
			
			if(sendheadType == THREE_BYTES_HEAD)
			{
				arrBuf[0] = COMM_FLAG;
				*(unsigned short*)&arrBuf[1] = uiBytes+THREE_BYTES_HEAD;
				Convert::ToNetOrder(*(unsigned short*)&arrBuf[1]);
				memcpy(&arrBuf[3], pBuf, uiBytes);
				oHeader.usShortSize = uiBytes+THREE_BYTES_HEAD;
			}
			else if(sendheadType == TWO_BYTES_HEAD)
			{
				*(unsigned short*)&arrBuf[0] = uiBytes+TWO_BYTES_HEAD;
				Convert::ToNetOrder(*(unsigned short*)&arrBuf[0]);
				memcpy(&arrBuf[2], pBuf, uiBytes);
				oHeader.usShortSize = uiBytes+TWO_BYTES_HEAD;
			}
			else
			{
				memcpy(&arrBuf[0], pBuf, uiBytes);
				oHeader.usShortSize = uiBytes;
			}

			return true;
		}
		
		inline bool Check(unsigned int uiBytes) const
		{
			return true;
		}

		inline void ToHostOrder()
		{
			oHeader.ToHostOrder();
		}

		inline void ToNetOrder() 
		{
			oHeader.ToNetOrder();
		}

		inline void ToIntHostOrder()
		{
			oHeader.ToIntHostOrder();
		}

		inline void ToIntNetOrder() 
		{
			oHeader.ToIntNetOrder();
		}	

		unsigned short GetUdpSize()
		{
			return oHeader.usShortSize-2;
		}

		char GetFlag(){return oHeader.ucFlag;}

		unsigned char * GetBufW()
		{
			//返回数据部分的buf
			return &arrBuf[0];
		}

		unsigned char const * GetBufR()
		{
			//返回数据部分的buf
			return &arrBuf[0];
		}

		int GetBufSize() {return arrBuf.size();}		

		void SetFlag(char flag)
		{
			oHeader.ucFlag = flag;
		}

		void SetDataSize(short size)
		{
			oHeader.usShortSize = size;
		}

		unsigned short GetDataSize()
		{
			//返回包括长度在内的长度
			return oHeader.usShortSize;
		}				

		void SetIntDataSize(unsigned int size)
		{
			oHeader.unIntSize = size;
		}

		unsigned int GetIntDataSize()
		{
			return oHeader.unIntSize;
		}
	};
	
	struct MsgObj
	{
		COMM_MSG enMsg;
		unsigned int uiID; //通讯的ID
		bool bTcp; //是否是TCP
		IpPort oIpPort;
		Packet oPacket;

		void resize(int nBufferSize)
		{
			oPacket.resize(nBufferSize);
		}
	};

	//cyclebuff.h///////////////
	template <typename T>
	struct CycleBufferNode
	{
		CycleBufferNode (bool IsBlank = true)
			: IsBlank_(IsBlank) {
		}

		volatile bool IsBlank_;
		T Data_;
	};

	template <typename DATA>
	class CycleBuffer 
	{
		//缺省拷贝构造可以很好的工作
	public:
		typedef DATA data_type;

		CycleBuffer(int MaxCount = 1024)
			: MaxCount_(MaxCount), Nodes_(MaxCount), WriteIndex_(0), ReadIndex_(0) {
				assert(MaxCount);
		}

		~CycleBuffer() {
		}

		bool Write(const DATA& data) {
			bool bok;
			if (Nodes_[WriteIndex_].IsBlank_) {
				Nodes_[WriteIndex_].Data_ = data;
				Nodes_[WriteIndex_].IsBlank_ = false;
				bok = true;
			}
			else bok = false;
			if (bok) {
				if (WriteIndex_ < MaxCount_-1) ++WriteIndex_;
				else WriteIndex_ = 0;
			}
			return bok;
		}

		bool Read(DATA& data) {
			bool bok;
			if (!Nodes_[ReadIndex_].IsBlank_) {
				data = Nodes_[ReadIndex_].Data_;
				Nodes_[ReadIndex_].IsBlank_ = true;
				bok = true;
			}
			else bok = false;
			if (bok) {
				if (ReadIndex_ < MaxCount_-1) ++ReadIndex_;
				else ReadIndex_ = 0;
			}
			return bok;
		}

		bool ReadWithReset(DATA& data) {
			bool bok;
			if (!Nodes_[ReadIndex_].IsBlank_) {
				data = Nodes_[ReadIndex_].Data_;
				Nodes_[ReadIndex_].Data_ = DATA();
				Nodes_[ReadIndex_].IsBlank_ = true;
				bok = true;
			}
			else bok = false;
			if (bok) {
				if (ReadIndex_ < MaxCount_-1) ++ReadIndex_;
				else ReadIndex_ = 0;
			}
			return bok;
		}

		int Size() const {
			return MaxCount_;
		}
	private:
		typedef std::vector<CycleBufferNode<DATA> > NODES;
		NODES Nodes_;
		const int MaxCount_;
		int WriteIndex_;
		int ReadIndex_;
	}; //class CycleBuffer


	typedef CycleBuffer<MsgObj *> MSG_BUF;

#pragma pack()

}; 

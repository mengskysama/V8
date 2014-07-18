/**
* packet.h
* 用于网络字节序转换
*
* @version 1.0 (06 ,10, 2008)
* @author reinhardt ken
*
*/
#pragma once

#include "..\Common.h"
#include <string>
#include <vector>
#include "..\include\utility\StringHelper.h"

namespace common
{
	namespace net
	{
		class COMMON_INTERFACE Packet
		{
		public:
			enum origin
			{
				from_cur,
				from_begin,
				from_end,
			};
		public:

			/*
			* 根据buffer构造一个packet
			*/

			Packet(ByteBuf &buf) :data(buf.second), dataSize((uint32)buf.first)
			{
				cursor = data;
			}
			
			/*
			* 根据buffer构造一个packet
			*/

			void Construct(ByteBuf &buf)
			{
				data=buf.second;
				dataSize=(uint32)buf.first;
				cursor = data;
			}

			/*
			* 返回buffer
			*/

			byte const *Data() 
			{
				return data;
			}

			/*
			* 返回游标移动长度
			*/

			uint32 Size() 
			{
				return (uint32)(cursor - data);
			}

			/*
			* 返回游标剩余移动长度
			*/

			uint32 FreeSize()
			{
				return (uint32)(data + dataSize - cursor);
			}

			/*
			* 移动游标
			* @param [in] off:现对origin_的偏移
			* @param [in] origin_:偏移起始位置
			*/

			void MoveCur(int off, origin origin_);

			/*
			* pack一个string (utf-8)
			*/

			Packet &operator <<(std::string &str) 
			{
				return operator <<(str.c_str());
			}

			/*
			* pack一个wstring
			*/

			Packet &operator <<(std::wstring &str)
			{
				std::string des = common::utility::stringhelper::UnicodeToUTF8(str);
				operator <<(des.c_str());
				return *this;
			}

			/*
			* pack一个uint8
			*/

			Packet &operator <<(uint8 b);

			/*
			* pack一个uint16
			*/

			Packet &operator <<(uint16 w);

			/*
			* pack一个uint32
			*/

			Packet &operator <<(uint32 dw);

			/*
			* pack一个uint64
			*/

			Packet &operator <<(uint64 ddw);

			/*
			* pack一个char*
			*/

			Packet &operator <<(char const *str);

			/*
			* pack一个的byte的vector
			*/

			Packet &operator<<(std::vector<byte>& vec)
			{
				*this<<(uint16)(vec.size()+1);
					for(uint32 i=0;i<vec.size();i++)
						*this<<vec[i];
					*this<<(uint8)0;
					return *this;
			}

			/*
			* pack有size大小的buffer
			*/

			void WriteData(byte const  *buf, uint32 size);

			/*
			* unpack一个uint8
			*/

			Packet &operator >>(uint8 &b);

			/*
			* unpack一个uint16
			*/

			Packet &operator >>(uint16 &w);

			/*
			* unpack一个uint32
			*/

			Packet &operator >>(uint32 &dw);

			/*
			* unpack一个uint64
			*/

			Packet &operator >>(uint64 &ddw);

			/*
			* unpack字符串
			*/

			Packet &operator >>(char const *&str);

			/*
			* unpack字符串
			*/

			Packet &operator >>(unsigned char const *&str);

			/*
			* unpack字符串
			*/

			Packet &operator >>(char *str);

			/*
			* unpack字符串到string (utf-8)
			*/

			Packet &operator >>(std::string &str);

			/*
			* unpack字符串到wstring
			*/

			Packet &operator >>(std::wstring &str) 
			{
				std::string temp;
				operator >>(temp);
				str = common::utility::stringhelper::UTF8ToUnicode(temp);
				return *this;
			}

			/*
			* unpack字符串到byte的vector
			*/

			Packet &operator >>(std::vector<byte>& vec);

			/*
			* 返回cursor
			* @param n:剩余size
			*/

			byte const* GetCursor();

			/*
			* 设置buffer大小
			*/

			void SetDataSize(uint32 size);

		protected:
			byte *data;
			byte *cursor;
			uint32 dataSize;
		};

		#define MAX_PACKET_SIZE 65535

		class COMMON_INTERFACE OutPacket : public Packet
		{
		public:
			OutPacket():Packet(ByteBuf(sizeof(realData),realData)){}

		protected:
			byte realData[MAX_PACKET_SIZE];
		};

		class COMMON_INTERFACE InPacket : public Packet
		{
		public:
			InPacket(ByteBuf &buf):Packet(buf){}
		};
	}
}
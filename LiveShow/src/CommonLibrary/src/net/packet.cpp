/**
* packet.cpp
* ÓÃÓÚÍøÂç×Ö½ÚÐò×ª»»
*
* @version 1.0 (06 ,10, 2008)
* @author reinhardt ken
*
*/

#include "assert.h"
#include "winsock2.h"
#include "..\..\include\Net\packet.h"

namespace common
{
namespace net
{
#define L16(x)  (USHORT)(x&0x0000ffff)
#define H16(x)  (USHORT)((x&0xffff0000)>>16)
#define L32(x)  (uint32)(x&0x00000000ffffffff)
#define H32(x)  (uint32)((x&0xffffffff00000000)>>32)

	void Packet::MoveCur(int off,origin origin_)
	{
		switch(origin_)
		{
		case from_begin:
			{
				cursor = data + off;
				break;
			}
		case from_end:
			{
				cursor = data + dataSize+off;
				break;
			}
		case from_cur:
			{
				cursor += off;
				break;
			}
		}
	}

	Packet &Packet::operator <<(uint8 b)
	{
		if (cursor <= data + dataSize - sizeof(b)) 
		{
			*(uint8 *) cursor = b;
			cursor += sizeof(b);
		}
		else
		{
			assert(0);
		}
		return (*this);
	}

	Packet &Packet::operator <<(uint16 w)
	{
		if (cursor <= data + dataSize - sizeof(w)) {
			*(uint16 *) cursor = htons(w);
			cursor += sizeof(w);
		}
		else
		{
			assert(0);
		}
		return (*this);
	}

	Packet &Packet::operator <<(uint32 dw)
	{
		if (cursor <= data + dataSize - sizeof(dw)) {
			*(uint32 *) cursor = htonl(dw);
			cursor += sizeof(dw);
		}
		else
		{
			assert(0);
		}
		return (*this);
	}
	Packet &Packet::operator <<(uint64 ddw)
	{
		(*this) << H32(ddw) << L32(ddw);
		return (*this);
	}
	Packet &Packet::operator <<(char const  *str)
	{
		uint16 len = strlen(str) + 1;

		if(len == 1) //¿Õ´®
		{
			operator <<(--len);
			return (*this);
		}

		if (cursor <= data + dataSize - sizeof(len) - len) 
		{
			operator <<(len);
			strncpy((char*)cursor, str,len/*-1*/);
			cursor += len;
		}
		else
		{
			assert(0);
		}
		return (*this);
	}

	void Packet::WriteData(byte const  *buf, uint32 n)
	{
		if (cursor <= data + dataSize - n) 
		{
			memcpy(cursor, buf, n);
			cursor += n;
		}
		else
		{
			assert(0);
		}
	}

	Packet &Packet::operator >>(uint8 &b)
	{
		if (cursor <= data + dataSize - sizeof(b)) 
		{
			b = *(uint8 *) cursor;
			cursor += sizeof(b);
		}
		else
		{
			b = 0;
		}
		return (*this);
	}

	Packet &Packet::operator >>(uint16 &w)
	{
		if (cursor <= data + dataSize - sizeof(w)) 
		{
			w = ntohs(*(uint16 *) cursor);
			cursor += sizeof(w);
		} 
		else
		{
			assert(0);
			w = 0;
		}
		return (*this);
	}

	Packet &Packet::operator >>(uint32 &dw)
	{
		if (cursor <= data + dataSize - sizeof(dw))
		{
			dw = ntohl(*(uint32 *) cursor);
			cursor += sizeof(dw);
		} 
		else
		{
			assert(0);
			dw = 0;
		}
		return (*this);
	}

	Packet &Packet::operator >>(uint64 &ddw)
	{
		uint32 hv,lv;
		(*this) >> hv >> lv;
		ddw = hv;
		ddw = ddw << 32;
		ddw += lv;
		return (*this);
	}

	Packet &Packet::operator >>(char const *&str)
	{
		uint16 len;
		operator >>(len);

		if(len == 0)//¿Õ´®
		{
			str = (char*)(cursor-1);
			return (*this);
		}

		bool b = (cursor <= data + dataSize - len);

		if (b && !cursor[len - 1]) 
		{
			str = (char*)cursor;
			cursor += len;
		} 
		else
		{
			assert(0);
			str = NULL;
		}
		return (*this);
	}

	Packet &Packet::operator >>(unsigned char const *&str)
	{
		uint16 len;
		operator >>(len);

		if(len == 0)//¿Õ´®
		{
			str = (unsigned char*)(cursor-1);
			return (*this);
		}

		if (cursor <= data + dataSize - len && !cursor[len - 1]) 
		{
			str = (unsigned char*)cursor;
			cursor += len;
		} 
		else
		{
			assert(0);
			str = NULL;
		}
		return (*this);
	}

	Packet &Packet::operator >>(char *str)
	{
		uint16 len;
		operator >>(len);

		if(len == 0)//¿Õ´®
		{
			str[0] = '\0';
			return (*this);
		}

		if (len && cursor <= data + dataSize - len && !cursor[len - 1]) {
			strncpy(str,(char*)cursor,len-1);
			cursor += len;
		} 
		else
		{
			assert(0);
			*str = '\0';
		}
		return (*this);
	}

	Packet &Packet::operator >>(std::string &str)
	{
		char const  *p;
		operator >>(p);
		if(p) str = p;
		return (*this);
	}

	Packet &Packet::operator>>(std::vector<byte>& vec)
	{
		uint16 size;
		*this>>size;
		size--;
		vec.resize(size);
		for(int i=0;i<vec.size();i++)
			*this>>vec[i];
		return *this;
	}

	byte const* Packet::GetCursor()
	{
		return cursor;
	}

	void Packet::SetDataSize(uint32 size)
	{
          dataSize=size;
	}
}
}
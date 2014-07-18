/* Copyright (C) 2005-2010, Thorvald Natvig <thorvald@natvig.com>

   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

   - Redistributions of source code must retain the above copyright notice,
     this list of conditions and the following disclaimer.
   - Redistributions in binary form must reproduce the above copyright notice,
     this list of conditions and the following disclaimer in the documentation
     and/or other materials provided with the distribution.
   - Neither the name of the Mumble Developers nor the names of its
     contributors may be used to endorse or promote products derived from this
     software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR
   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef _PACKETDATASTREAM_H
#define _PACKETDATASTREAM_H

#include "stdafx.h"
#include "audio_pch.h"
#include "string_helper.h"
using namespace boost;
/*
 * GCC doesn't yet do inter-object-file inlining, so unfortunately, this all has to be defined here.
 */

class PacketDataStream {
	private:
		unsigned char *data;
		unsigned int maxsize;
		unsigned int offset;
		unsigned int overshoot;
		bool ok;
	public:
		unsigned int size() const {
			return offset;
		}

		unsigned int capacity() const {
			return maxsize;
		}

		bool isValid() const {
			return ok;
		}

		unsigned int left() const {
			return maxsize - offset;
		}

		unsigned int undersize() const {
			return overshoot;
		}

		void append(const unsigned long v) {
			if (offset < maxsize)
				data[offset++] = static_cast<unsigned char>(v);
			else {
				ok = false;
				overshoot++;
			}
		};

		void append(const char *d, unsigned int len) {
			if (left() >= len) {
				memcpy(& data[offset], d, len);
				offset += len;
			} else {
				int l = left();
				memset(& data[offset], 0, l);
				offset += l;
				overshoot += len - l;
				ok = false;
			}
		}

		void skip(unsigned int len) {
			if (left() >= len)
				offset += len;
			else
				ok = false;
		}

		unsigned long next() {
			if (offset < maxsize)
				return data[offset++];
			else {
				ok = false;
				return 0;
			}
		};

		unsigned char next8() {
			if (offset < maxsize)
				return data[offset++];
			else {
				ok = false;
				return 0;
			}
		}

		void rewind() {
			offset = 0;
		}

		void truncate() {
			maxsize = offset;
		}

		const unsigned char *dataPtr() const {
			return reinterpret_cast<const unsigned char *>(& data[offset]);
		}

		const char *charPtr() const {
			return reinterpret_cast<const char *>(& data[offset]);
		}

		QByteArray dataBlock(unsigned int len) {
			if (len <= left()) {
				//QByteArray a(charPtr(), len);
				QByteArray a;
				for (int i=0; i<len; i++)
				{
					a.push_back(data[offset+i]);
				}
				
				offset +=len;
				return a;
			} else {
				ok = false;
				return QByteArray();
			}
		}

	protected:
		void setup(unsigned char *d, int msize) {
			data = d;
			offset = 0;
			overshoot = 0;
			maxsize = msize;
			ok = true;
		}
	public:
		PacketDataStream(const char *d, int msize) {
			setup(const_cast<unsigned char *>(reinterpret_cast<const unsigned char *>(d)), msize);
		};

		PacketDataStream(char *d, int msize) {
			setup(reinterpret_cast<unsigned char *>(d), msize);
		};

		PacketDataStream(unsigned char *d, int msize) {
			setup(d, msize);
		};

		PacketDataStream(const QByteArray &qba) {
			setup((unsigned char*)(&qba[0]), qba.size());
		}

		PacketDataStream(QByteArray &qba) {
			unsigned char *ptr=reinterpret_cast<unsigned char *>(&qba[0]);
			setup(ptr, qba.capacity());
		}

		PacketDataStream &operator <<(const unsigned long value) {
			unsigned long i = value;

			if ((i & 0x8000000000000000LL) && (~i < 0x100000000LL)) {
				// Signed number.
				i = ~i;
				if (i <= 0x3) {
					// Shortcase for -1 to -4
					append(0xFC | i);
					return *this;
				} else {
					append(0xF8);
				}
			}
			if (i < 0x80) {
				// Need top bit clear
				append(i);
			} else if (i < 0x4000) {
				// Need top two bits clear
				append((i >> 8) | 0x80);
				append(i & 0xFF);
			} else if (i < 0x200000) {
				// Need top three bits clear
				append((i >> 16) | 0xC0);
				append((i >> 8) & 0xFF);
				append(i & 0xFF);
			} else if (i < 0x10000000) {
				// Need top four bits clear
				append((i >> 24) | 0xE0);
				append((i >> 16) & 0xFF);
				append((i >> 8) & 0xFF);
				append(i & 0xFF);
			} else if (i < 0x100000000LL) {
				// It's a full 32-bit integer.
				append(0xF0);
				append((i >> 24) & 0xFF);
				append((i >> 16) & 0xFF);
				append((i >> 8) & 0xFF);
				append(i & 0xFF);
			} else {
				// It's a 64-bit value.
				append(0xF4);
				append((i >> 56) & 0xFF);
				append((i >> 48) & 0xFF);
				append((i >> 40) & 0xFF);
				append((i >> 32) & 0xFF);
				append((i >> 24) & 0xFF);
				append((i >> 16) & 0xFF);
				append((i >> 8) & 0xFF);
				append(i & 0xFF);
			}
			return *this;
		}

		PacketDataStream &operator >>(unsigned long &i) {
			unsigned long v = next();

			if ((v & 0x80) == 0x00) {
				i=(v & 0x7F);
			} else if ((v & 0xC0) == 0x80) {
				i=(v & 0x3F) << 8 | next();
			} else if ((v & 0xF0) == 0xF0) {
				switch (v & 0xFC) {
					case 0xF0:
						i=next() << 24 | next() << 16 | next() << 8 | next();
						break;
					case 0xF4:
						i=next() << 56 | next() << 48 | next() << 40 | next() << 32 | next() << 24 | next() << 16 | next() << 8 | next();
						break;
					case 0xF8:
						*this >> i;
						i = ~i;
						break;
					case 0xFC:
						i=v & 0x03;
						i = ~i;
						break;
					default:
						ok = false;
						i = 0;
						break;
				}
			} else if ((v & 0xF0) == 0xE0) {
				i=(v & 0x0F) << 24 | next() << 16 | next() << 8 | next();
			} else if ((v & 0xE0) == 0xC0) {
				i=(v & 0x1F) << 16 | next() << 8 | next();
			}
			return *this;
		}

		PacketDataStream &operator <<(const QByteArray &a) {
			*this << a.size();
			append((char*)&a[0], a.size());
			return *this;
		}

		PacketDataStream &operator >>(QByteArray &a) {
			unsigned int len;
			*this >> len;
			if (len > left()) {
				len = left();
				ok = false;
			}
			//a = QByteArray(reinterpret_cast<const char *>(& data[offset]), len);
			for (int i=0; i<len; i++)
			{
				a[i] = data[i+offset];
			}
			
			offset+=len;
			return *this;
		}

		PacketDataStream &operator <<(const wstring &s) {
			return *this << string_helper::UnicodeToUTF8(s).c_str();
		}

		// Using the data directly instead of through qbuff avoids a copy.
		PacketDataStream &operator >>(wstring &s) {
			unsigned int len;
			*this >> len;
			if (len > left()) {
				len = left();
				ok = false;
			}
			s =string_helper::UTF8ToUnicode(reinterpret_cast<const char *>(& data[offset]));
			offset+=len;
			return *this;
		}

		PacketDataStream &operator <<(const bool b) {
			unsigned int v = b ? 1 : 0;
			return *this << v;
		}

		PacketDataStream &operator >>(bool &b) {
			unsigned int v;
			*this >> v;
			b = v ? true : false;
			return *this;
		}

#define INTMAPOPERATOR(type) \
		PacketDataStream &operator <<(const type v) { \
			return *this << static_cast<unsigned long>(v); \
		} \
		PacketDataStream &operator >>(type &v) { \
			unsigned long vv; \
			*this >> vv; \
			v = static_cast<type>(vv); \
			return *this; \
		}


		INTMAPOPERATOR(int);
		INTMAPOPERATOR(unsigned int);
		INTMAPOPERATOR(short);
		INTMAPOPERATOR(unsigned short);
		INTMAPOPERATOR(char);
		INTMAPOPERATOR(unsigned char);

		union double64u {
			unsigned long ui;
			double d;
		};

		PacketDataStream &operator <<(const double v) {
			double64u u;
			u.d = v;
			return *this << u.ui;
		}

		PacketDataStream &operator >>(double &v) {
			double64u u;
			*this >> u.ui;
			v = u.d;
			return *this;
		}

		union float32u {
			unsigned char ui[4];
			float f;
		};

		PacketDataStream &operator <<(const float v) {
			float32u u;
			u.f = v;
			append(u.ui[0]);
			append(u.ui[1]);
			append(u.ui[2]);
			append(u.ui[3]);
			return *this;
		}

		PacketDataStream &operator >>(float &v) {
			float32u u;
			if (left() < 4) {
				ok = false;
				v = 0;
			}
			u.ui[0] = next8();
			u.ui[1] = next8();
			u.ui[2] = next8();
			u.ui[3] = next8();
			v = u.f;
			return *this;
		}

		template <typename T>
		PacketDataStream &operator <<(const std::list<T> &l) {
			*this << l.size();
			for (int i=0;i < l.size();i++)
				*this << l.at(i);
			return *this;
		}

		template <typename T>
		PacketDataStream &operator >>(std::list<T> &l) {
			l.clear();
			unsigned int len;
			*this >> len;
			if (len > left()) {
				len = left();
				ok = false;
			}
			for (unsigned int i=0;i<len;i++) {
				if (left() == 0) {
					ok = false;
					break;
				}

				T t;
				*this >> t;
				l.append(t);
			}
			return *this;
		}


		template <typename T>
		PacketDataStream &operator <<(const std::set<T> &s) {
			*this << s.size();
			for (typename set<T>::const_iterator i=s.begin();i!=s.end();++i)
				*this << *i;
			return *this;
		}

		template <typename T>
		PacketDataStream &operator >>(std::set<T> &s) {
			s.clear();
			unsigned int len;
			*this >> len;
			if (len > left()) {
				len = left();
				ok = false;
			}
			for (unsigned int i=0;i<len;i++) {
				if (left() == 0) {
					ok = false;
					break;
				}

				T t;
				*this >> t;
				s.insert(t);
			}
			return *this;
		}

		template <typename T,typename U>
		PacketDataStream &operator <<(const std::pair<T,U> &p) {
			return *this << p.first << p.second;
		}

		template <typename T,typename U>
		PacketDataStream &operator >>(std::pair<T,U> &p) {
			return *this >> p.first >> p.second;
		}

};

#else
class PacketDataStream;
#endif

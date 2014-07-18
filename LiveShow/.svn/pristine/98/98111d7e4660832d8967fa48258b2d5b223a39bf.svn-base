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

		void append8(unsigned char d)
		{
			if (left()>=1)
			{
				data[offset] = d;
				offset ++;
			}
			else
			{
				ok = false;
				overshoot++;
			}
		}

		void append16(short d)
		{
			if (left() >= 2) {
				memcpy(& data[offset], &d, 2);
				offset += 2;
			} else {
				int l = left();
				memset(& data[offset], 0, l);
				offset += l;
				overshoot += 2 - l;
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

		short next16()
		{
			short sdd = 0;
			if (offset+2 <= maxsize){
				sdd = *((short*)&data[offset]);
			    offset+=2;
				return sdd;
			}else {
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

		bool dataBlock(unsigned int len, QByteArray& outArray) {
			if (len <= left() && len>0) {
				outArray.resize(len);
				memcpy(&outArray[0], &data[offset], len);
				offset +=len;
				return true;
			} 
			else {
				ok = false;
				return false;
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
};

#else
class PacketDataStream;
#endif

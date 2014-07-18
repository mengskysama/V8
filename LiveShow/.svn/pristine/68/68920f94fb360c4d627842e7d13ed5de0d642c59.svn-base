#pragma once

#include "Mutex.h"


/*注意：非线程安全*/

template<unsigned int SIZE>
class CycleBuffer
{
public:
	CycleBuffer()
	{
		Reset();
	}

	virtual ~CycleBuffer()
	{

	}

	//写到读索引前面位置
	unsigned int WriteToHead(const unsigned char *pBuf, unsigned int nDataSize)
	{
		if (pBuf == NULL || nDataSize<=0)
		{
			return 0;
		}

		//剩余空间不足
		if(m_nDataSize + nDataSize > SIZE)
		{
			return 0;
		}

		if(m_nWriteIndex < m_nReadIndex)
		{
			if(m_nReadIndex - nDataSize < 0)
			{
				return 0;
			}

			memcpy(&m_arrBuf[m_nReadIndex - nDataSize], pBuf, nDataSize);
			m_nReadIndex -= nDataSize;
		}
		else
		{
			unsigned int nLeftDataSize = (m_nReadIndex - nDataSize < 0) ? m_nReadIndex : nDataSize;
			if(nLeftDataSize < nDataSize)
			{
				unsigned int nTailDataSize = nDataSize - nLeftDataSize;
				unsigned int nWriteIndex = SIZE - nTailDataSize;
				memcpy(&m_arrBuf[nWriteIndex], pBuf, nTailDataSize);
				memcpy(&m_arrBuf[0], pBuf + nTailDataSize, nLeftDataSize);
				m_nWriteIndex = nWriteIndex;
			}
			else
			{
				if(m_nReadIndex - nDataSize < 0)
				{
					return 0;
				}

				memcpy(&m_arrBuf[m_nReadIndex - nDataSize], pBuf, nDataSize);
				m_nReadIndex -= nDataSize;
			}
		}

		m_nDataSize += nDataSize;

		return nDataSize;
	}

	//写到写索引的尾部内存
	unsigned int Write(const unsigned char *pBuf, unsigned int nDataSize)
	{
		if (pBuf == NULL || nDataSize<=0)
		{
			return 0;
		}

		//剩余空间不足
		if(m_nDataSize + nDataSize > SIZE)
		{
			return 0;
		}

		if(m_nWriteIndex < m_nReadIndex)
		{
			memcpy(&m_arrBuf[m_nWriteIndex], pBuf, nDataSize);
			m_nWriteIndex += nDataSize;
		}
		else
		{
			unsigned int nLeftDataSize = SIZE - m_nWriteIndex;
			if(nLeftDataSize < nDataSize)
			{
				memcpy(&m_arrBuf[m_nWriteIndex], pBuf, nLeftDataSize);
				memcpy(&m_arrBuf[0], pBuf + nLeftDataSize, nDataSize - nLeftDataSize);
				m_nWriteIndex = nDataSize - nLeftDataSize;
			}
			else
			{
				memcpy(&m_arrBuf[m_nWriteIndex], pBuf, nDataSize);
				m_nWriteIndex += nDataSize;
			}
		}
		
		if(m_nWriteIndex == SIZE) m_nWriteIndex = 0;

		m_nDataSize += nDataSize;

		return nDataSize;
	}

	//读取读索引头部内存
	unsigned int Read(unsigned char *pBuf, const unsigned int nWantSize)
	{
		if (pBuf == NULL || nWantSize<=0)
		{
			return 0;
		}

		unsigned int nDataSize = ((m_nDataSize < nWantSize) ? m_nDataSize : nWantSize);
		if(m_nReadIndex < m_nWriteIndex)
		{
			memcpy(pBuf, &m_arrBuf[m_nReadIndex], nDataSize);
			m_nReadIndex += nDataSize;
		}
		else
		{
			unsigned int nLeftDataSize = SIZE - m_nReadIndex;
			if(nLeftDataSize < nDataSize)
			{
				memcpy(pBuf, &m_arrBuf[m_nReadIndex], nLeftDataSize);
				memcpy(pBuf + nLeftDataSize, &m_arrBuf[0], nDataSize - nLeftDataSize);
				m_nReadIndex = nDataSize - nLeftDataSize;
			}
			else
			{
				memcpy(pBuf, &m_arrBuf[m_nReadIndex], nDataSize);
				m_nReadIndex += nDataSize;
			}
		}
		
		if(m_nReadIndex == SIZE) m_nReadIndex = 0;

		m_nDataSize -= nDataSize;

		return nDataSize;
	}

	//读取数据但是不修改读索引
	unsigned int PeekRead(unsigned char *pBuf, const unsigned int nWantSize)
	{
		if (pBuf == NULL || nWantSize<=0)
		{
			return 0;
		}

		unsigned int nDataSize = ((m_nDataSize < nWantSize) ? m_nDataSize : nWantSize);
		if(m_nReadIndex < m_nWriteIndex)
		{
			memcpy(pBuf, &m_arrBuf[m_nReadIndex], nDataSize);
		}
		else
		{
			unsigned int nLeftDataSize = SIZE - m_nReadIndex;
			if(nLeftDataSize < nDataSize)
			{
				memcpy(pBuf, &m_arrBuf[m_nReadIndex], nLeftDataSize);
				memcpy(pBuf + nLeftDataSize, &m_arrBuf[0], nDataSize - nLeftDataSize);
			}
			else
			{
				memcpy(pBuf, &m_arrBuf[m_nReadIndex], nDataSize);
			}
		}

		return nDataSize;
	}

	void Reset()
	{
		m_nReadIndex = 0;
		m_nWriteIndex = 0;
		m_nDataSize = 0;
		memset(m_arrBuf, 0, sizeof(m_arrBuf));
	}

	unsigned int Size()
	{
		return m_nDataSize;
	}

	unsigned int Capatity()
	{
		return SIZE;
	}

	unsigned int Space()
	{
		return SIZE - m_nDataSize;
	}

protected:
	unsigned int  m_nReadIndex;
	unsigned int  m_nWriteIndex;
	unsigned int  m_nDataSize;
	unsigned char m_arrBuf[SIZE];
};



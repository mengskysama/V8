#pragma once

#include "Common.h"
#include "MyAssert.h"

template<class T, UInt32 SIZE=256>
class CircleBuffer
{
public:

	//缓冲区存放内存指针(即VDATA)个数，一经初始化，不再改变
	explicit CircleBuffer(); 

	~CircleBuffer();

	inline UInt32   size()        { return SIZE;                           }  
	inline bool     empty()       { return m_iNextReadPos==m_iNextWritePos; } 


	/*
	* 将数据放入缓冲，
	* @param p 要放入缓冲区的数据起始指针
	* @param len 要放入缓冲区的数据长度
	* @returnCode bool 
	*   true  : 放入成功
	*   false : 放入失败，缓冲区满
	*/
	bool            push_back(T pData, UInt32 iLen, UInt32 iType);

	/*
	* 将缓冲区下一个要读数据取出，
	* @param p 读出数据存放的起始指针
	* @param len p指向缓冲区长度，读出数据长度
	* @returnCode bool 
	*   true  : 读出成功
	*   false : 读出失败，p缓冲大小不够或者缓冲无数据
	*/
	bool            pop_front(T &pData, UInt32& iLen, UInt32& iType);

private:  

	typedef struct _VDATA
	{
		T        pData;
		UInt32   iDataLen;
		UInt32   iDataType;
	}VDATA;

	//不允许复制和拷贝构造
	CircleBuffer(CircleBuffer& cycleBufer);
	CircleBuffer&  operator=(CircleBuffer& cycleBufer);

private:
	UInt32          m_iNextReadPos;       //下一个要读数据位置
	UInt32          m_iNextWritePos;      //下一个要写数据位置
	VDATA           m_pVDATA[SIZE];       //记录数据指针数组
};

template<class T, UInt32 SIZE>
CircleBuffer<T, SIZE>::CircleBuffer()
                :m_iNextReadPos(0),
                 m_iNextWritePos(0)
{
}

template<class T, UInt32 SIZE>
CircleBuffer<T, SIZE>::~CircleBuffer()
{
}

template<class T, UInt32 SIZE>
bool  CircleBuffer<T, SIZE>::push_back(T pData, UInt32 iLen, UInt32 iType)
{
	if(  (m_iNextWritePos==SIZE-1 && 0==m_iNextReadPos) 
		|| (m_iNextWritePos==m_iNextReadPos-1) ) 
	{
		Assert(0);
		return false;
	}

	m_pVDATA[m_iNextWritePos].pData     = pData;
	m_pVDATA[m_iNextWritePos].iDataLen  = iLen;
	m_pVDATA[m_iNextWritePos].iDataType = iType;

	++m_iNextWritePos;
	if(m_iNextWritePos==SIZE) 
	{
		m_iNextWritePos = 0;  	
	}

	return true;
}

template<class T, UInt32 SIZE>
bool  CircleBuffer<T, SIZE>::pop_front(T& pData, UInt32& iLen, UInt32& iType)
{
	if(m_iNextWritePos==m_iNextReadPos) 
	{
		return false;
	}

	pData = m_pVDATA[m_iNextReadPos].pData;
	iLen  = m_pVDATA[m_iNextReadPos].iDataLen;
	iType = m_pVDATA[m_iNextReadPos].iDataType;

	++m_iNextReadPos;
	if (m_iNextReadPos==SIZE)
	{
		m_iNextReadPos=0;
	}

	return true;	
}
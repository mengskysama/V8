#pragma once

#include "Common.h"
#include "Mutex.h"

class  SocketIDGenerater
{
public:

	//初始化分配表的参照位图;
	static void      initialize(UInt32 iTotalSocketNum);

	//释放参照位图的资源;
	static void	     destroy();

	static SInt32    getNewID();

	//在一个Socket被释放之后;清除SocketUniqueID的分配
	static void      clearUniqueID(SInt32 iUniqueID);

protected:

	SocketIDGenerater() {}

	virtual ~SocketIDGenerater(){}

private:

	static  UInt8						 s_bitMask[8];

	static  Mutex                        s_Mutex;
	static  SInt32						 s_iSocketUniqueID;
	static  SInt32						 s_iTotalSocketsNum;
	static  UInt8**						 s_pThreadsBitMap;	 
};
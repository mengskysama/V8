#include "stdafx.h"
#include "SocketIDGenerater.h"
#include "ThreadsManager.h"

SInt32				SocketIDGenerater::s_iSocketUniqueID=-1;
Mutex				SocketIDGenerater::s_Mutex;
UInt8**				SocketIDGenerater::s_pThreadsBitMap =NULL;

SInt32				SocketIDGenerater::s_iTotalSocketsNum=0;

UInt8				SocketIDGenerater::s_bitMask[8]= { 0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01 };


//初始化分配表的参照位图;
void     SocketIDGenerater::initialize(UInt32 iTotalSocketNum)
{
	     s_iTotalSocketsNum = iTotalSocketNum;

	     if(s_pThreadsBitMap==NULL)
		 {
			s_pThreadsBitMap= new UInt8*[ThreadsManager::s_iWorkThreadsNum];
			UInt32  iBitArraySize = ThreadsManager::s_iSocketNumPerThread/8 +1;
			for(UInt32 iCount=0; iCount<ThreadsManager::s_iWorkThreadsNum; iCount++)
			{
				s_pThreadsBitMap[iCount] = new UInt8[iBitArraySize];
				::memset(s_pThreadsBitMap[iCount], 0, iBitArraySize);
			}
		 }		 
}


//释放参照位图的资源;
void	 SocketIDGenerater::destroy()
{
		if(s_pThreadsBitMap!=NULL)
		{
			for(UInt32 iCount=0; iCount<ThreadsManager::s_iWorkThreadsNum; iCount++)
			{
				if(s_pThreadsBitMap[iCount]!=NULL)
				{
					delete [] s_pThreadsBitMap[iCount];
					s_pThreadsBitMap[iCount] =NULL;
				}
			}
			delete [] s_pThreadsBitMap;
			s_pThreadsBitMap = NULL;	//for a error when refresh in the web page.
		 }
}

SInt32    SocketIDGenerater::getNewID()
{
		  SInt32  iThreadIndex	=0;
	      SInt32  iArrayIndex	=0;

		  UInt32  iBitArrayIndex = 0; 
		  UInt32  iBitMaskIndex  = 0; 

	      MutexLocker locker(&s_Mutex);

	      while(1)
	      {
		        s_iSocketUniqueID++;
				if(s_iSocketUniqueID<0 || s_iSocketUniqueID>=s_iTotalSocketsNum) s_iSocketUniqueID=0;
				iThreadIndex	= s_iSocketUniqueID%ThreadsManager::s_iWorkThreadsNum;
				iArrayIndex		= (s_iSocketUniqueID/ThreadsManager::s_iWorkThreadsNum)%ThreadsManager::s_iSocketNumPerThread;

				iBitArrayIndex  = iArrayIndex/8;
				iBitMaskIndex   = iArrayIndex%8;

				if(!(s_pThreadsBitMap[iThreadIndex][iBitArrayIndex] & s_bitMask[iBitMaskIndex]))
				{
					break;
				}
				else
				{
					Assert(0);
				}
		  }
		  s_pThreadsBitMap[iThreadIndex][iBitArrayIndex] |= s_bitMask[iBitMaskIndex];
		  return s_iSocketUniqueID;
}

//在一个Socket被释放之后;清除SocketUniqueID的分配
void        SocketIDGenerater::clearUniqueID(SInt32 iUniqueID)
{
		    SInt32  iThreadIndex = iUniqueID%ThreadsManager::s_iWorkThreadsNum;
		    SInt32  iArrayIndex  = (iUniqueID/ThreadsManager::s_iWorkThreadsNum)%ThreadsManager::s_iSocketNumPerThread;

			UInt32  iBitArrayIndex = iArrayIndex/8;
			UInt32  iBitMaskIndex  = iArrayIndex%8;

			MutexLocker locker(&s_Mutex);
			s_pThreadsBitMap[iThreadIndex][iBitArrayIndex] &= ~(s_bitMask[iBitMaskIndex]);
}
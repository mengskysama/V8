#pragma once

#include "Common.h"

class Heap;

class COMMON_OUT_CLASS HeapElem
{
public:

	HeapElem()
		: m_lValue(0), 
		  m_pCurrentHeap(NULL) 
	{}

	HeapElem(void* pEnclosingObject)
		: m_lValue(0), 
		  m_pEnclosingObject(pEnclosingObject), 
		  m_pCurrentHeap(NULL) 
	{}

	~HeapElem() {}

	//This data structure emphasizes performance over extensibility
	//If it were properly object-oriented, the compare routine would
	//be virtual. However, to avoid the use of v-functions in this data
	//structure, I am assuming that the objects are compared using a 64 bit number.
	//
	void	SetValue(SInt64 lValue)           { m_lValue = lValue;             }
	SInt64	GetValue()				          { return m_lValue;               }
	void    SetEnclosingObject(void* pObject) { m_pEnclosingObject = pObject;    }
	void*   GetEnclosingObject() 	          { return m_pEnclosingObject;      }
	Bool	IsMemberOfAnyHeap()		          { return m_pCurrentHeap != NULL; }

private:

	SInt64	              m_lValue;
	void*                 m_pEnclosingObject;
	Heap*	              m_pCurrentHeap;

	friend  class Heap;
};

class COMMON_OUT_CLASS Heap
{
public:

	enum
	{
		kDefaultStartSize = 64           //UInt32
	};

	Heap(UInt32 iStartSize = kDefaultStartSize);

	~Heap();

	void           Insert(HeapElem* pElem);

	HeapElem*      Remove(HeapElem* pElem);

	//getter/setter
	UInt32         GetCurHeapSize()    { return m_iFreeIndex-1; }
	HeapElem*      PeekMin()           { if(GetCurHeapSize()>0) return m_ppHeap[1]; return NULL; }
	HeapElem*      ExtractMin()        { return Extract(1); }      

private:

	HeapElem*	   Extract(UInt32 iIndex);

#ifdef _DEBUG
	//verifies that the heap is in fact a heap
	void		   SanityCheck(UInt32 root);
#endif

	HeapElem**         m_ppHeap;
	UInt32             m_iFreeIndex;
	UInt32             m_iArraySize;
};
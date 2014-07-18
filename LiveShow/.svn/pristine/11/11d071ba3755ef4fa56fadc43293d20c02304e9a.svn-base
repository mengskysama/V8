#include "stdafx.h"
#include "Heap.h"
#include "MyAssert.h"

Heap::Heap(UInt32 iStartSize)
     :m_ppHeap(NULL), 
      m_iFreeIndex(1),
      m_iArraySize(iStartSize)
{
	if (iStartSize<2)
	{
		m_iArraySize = 2;
	}

	m_ppHeap    = new HeapElem*[m_iArraySize];
	m_ppHeap[0] = NULL;  //0下标不使用
}

Heap::~Heap()
{
	SAFE_DELETEA(m_ppHeap);
}

void  Heap::Insert(HeapElem* pElem)
{
	if (NULL==pElem) return;

	if ((NULL==m_ppHeap)||(m_iFreeIndex==m_iArraySize))
	{
		//重新分配内存
		m_iArraySize *= 2;
		HeapElem** ppTempArray = new HeapElem*[m_iArraySize];
		Assert(ppTempArray!=NULL);
		if ((m_ppHeap!=NULL)&&(m_iFreeIndex>1))
		{
			::memcpy(ppTempArray, m_ppHeap, sizeof(HeapElem*) * m_iFreeIndex);
			delete[] m_ppHeap;
			m_ppHeap = NULL;
		}
		m_ppHeap = ppTempArray;
	}


	Assert(m_ppHeap != NULL);
	Assert(pElem->m_pCurrentHeap == NULL);
	Assert(m_iArraySize > m_iFreeIndex);

#ifdef _DEBUG
	SanityCheck(1);
#endif

	//insert the element into the last leaf of the tree
	m_ppHeap[m_iFreeIndex] = pElem;

	//bubble the new element up to its proper place in the heap

	//start at the last leaf of the tree
	UInt32 swapPos = m_iFreeIndex;
	while (swapPos > 1)
	{
		//move up the chain until we get to the root, bubbling this new element
		//to its proper place in the tree
		UInt32 nextSwapPos = swapPos >> 1;

		//if this child is greater than it's parent, we need to do the old
		//switcheroo
		if (m_ppHeap[swapPos]->m_lValue < m_ppHeap[nextSwapPos]->m_lValue)
		{
			HeapElem* temp     = m_ppHeap[swapPos];
			m_ppHeap[swapPos]     = m_ppHeap[nextSwapPos];
			m_ppHeap[nextSwapPos] = temp;
			swapPos = nextSwapPos;
		}
		else
			//if not, we are done!
			break;
	}
	pElem->m_pCurrentHeap = this;
	m_iFreeIndex++;
}

HeapElem* Heap::Remove(HeapElem* pElem)
{
	if ((m_ppHeap == NULL) || (m_iFreeIndex == 1))
		return NULL;

#ifdef _DEBUG
	SanityCheck(1);
#endif

	//first attempt to locate this element in the heap
	UInt32 theIndex = 1;
	for ( ; theIndex < m_iFreeIndex; theIndex++)
	{
		if (pElem == m_ppHeap[theIndex]) break;
	}

	//either we've found it, or this is a bogus element
	if (theIndex == m_iFreeIndex)
		return NULL;

	return Extract(theIndex);
}

HeapElem* Heap::Extract(UInt32 iIndex)
{
	if ((!m_ppHeap)||(m_iFreeIndex<=iIndex))  return NULL;

#ifdef _DEBUG
	SanityCheck(1);
#endif

	//store a reference to the element we want to extract
	HeapElem* pVictim = m_ppHeap[iIndex];
	Assert(pVictim->m_pCurrentHeap==this);
	pVictim->m_pCurrentHeap=NULL;

	//but now we need to preserve this heuristic. We do this by taking
	//the last leaf, putting it at the empty position, then heapifying that chain
	m_ppHeap[iIndex] = m_ppHeap[m_iFreeIndex - 1];
	m_iFreeIndex--;

	//The following is an implementation of the Heapify algorithm (CLR 7.1 pp 143)
	//The gist is that this new item at the top of the heap needs to be bubbled down
	//until it is bigger than its two children, therefore maintaining the heap property.

	UInt32 parent = iIndex;
	while (parent < m_iFreeIndex)
	{
		//which is bigger? parent or left child?
		UInt32 littlest = parent;
		UInt32 leftChild = parent * 2;
		if ((leftChild < m_iFreeIndex) && (m_ppHeap[leftChild]->m_lValue < m_ppHeap[parent]->m_lValue))
			littlest = leftChild;

		//which is bigger? the biggest so far or the right child?
		UInt32 rightChild = (parent * 2) + 1;
		if ((rightChild < m_iFreeIndex) && (m_ppHeap[rightChild]->m_lValue < m_ppHeap[littlest]->m_lValue))
			littlest = rightChild;

		//if the parent is in fact bigger than its two children, we have bubbled
		//this element down far enough
		if (littlest == parent)
			break;

		//parent is not bigger than at least one of its two children, so swap the parent
		//with the largest item.
		HeapElem* temp     = m_ppHeap[parent];
		m_ppHeap[parent]   = m_ppHeap[littlest];
		m_ppHeap[littlest] = temp;

		//now heapify the remaining chain
		parent = littlest;
	}

	return pVictim;
}

#ifdef _DEBUG

void Heap::SanityCheck(UInt32 root)
{
	//make sure root is greater than both its children. Do so recursively
	if (root < m_iFreeIndex)
	{
		if ((root * 2) < m_iFreeIndex)
		{
			Assert(m_ppHeap[root]->m_lValue <= m_ppHeap[root * 2]->m_lValue);
			SanityCheck(root * 2);
		}
		if (((root * 2) + 1) < m_iFreeIndex)
		{
			Assert(m_ppHeap[root]->m_lValue <= m_ppHeap[(root * 2) + 1]->m_lValue);
			SanityCheck((root * 2) + 1);
		}
	}
}
#endif
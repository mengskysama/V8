#pragma once

#include "Mutex.h"
#include "Cond.h"

class COMMON_OUT_CLASS RCObject
{
public:
	RCObject():m_iRefCount(0){}
	RCObject(const RCObject&):m_iRefCount(0){}
	RCObject& operator=(const RCObject& rhs){return *this;}

	virtual ~RCObject()=0;

	//引用计数加1
	void                 AddReference();

	//引用计数减1, 返回true表示内存可以回收或已经被释放
	void                 RemoveReference();

	//获取引用号
	unsigned int		 GetReference();

	//设置引用计数
	void				 SetReference(unsigned int iref);

	//判断对象是否被多次引用
	bool                 isShared();

	//等待被其他线程释放
	void                 WaitUnshared();

private:
	
	Mutex             m_RCMutex;     //操作对象的互斥锁
	Cond              m_Cond;

	SInt32            m_iRefCount;    //对象的引用计数
};

//智能指针
template <class T>
class RCPtr
{
public:
	RCPtr(T* realPtr = NULL):pointee(realPtr)    {   init();  }
	RCPtr(const RCPtr& rhs):pointee(rhs.pointee) {   init();  }
	RCPtr& operator = (const RCPtr& rhs)
	{
		if (pointee!=rhs.pointee)
		{
			if (pointee) 
			{
				pointee->RemoveReference();
			}

			pointee = rhs.pointee;
	
			init();
		}
		return *this;
	}

	~RCPtr()
	{
		if (pointee) 
		{
			pointee->RemoveReference();
		}
	}

	T* operator->() const  { return pointee; }
	T& operator*()  const  { return *pointee; }
	operator bool() const  { if (pointee) return true; return false;}

private:

	T* pointee;

	void init()
	{
		if (NULL==pointee ) 
		{
			return;
		}
		pointee->AddReference();
	}
};


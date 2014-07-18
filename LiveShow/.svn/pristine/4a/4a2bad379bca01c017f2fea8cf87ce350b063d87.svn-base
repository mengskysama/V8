#pragma once
#include "exportmacro.h"
#include <winbase.h>  

namespace common
{
namespace utility
{

	//请优先使用以下宏
	#define LOCK_GUARD(A,B)  common::utility::LockMgr<common::utility::CSWrapper> guard(A,B);

	class COMMON_INTERFACE CSWrapper
	{
	public:
		CSWrapper();
		~CSWrapper();
		// 
		// This function waits for ownership of the specified critical section object 
		// 
		void Enter();
		//
		// Releases ownership of the specified critical section object. 
		// 
		void Leave();
	private:
		CRITICAL_SECTION m_cs;
		long m_nSpinCount;
	};




	template <class T>
	class LockMgr  
	{
	public:
		//
		// Constructor
		//
		LockMgr(T& lockObject, BOOL bEnabled):
			m_rLockObject( lockObject ),
			m_bEnabled( bEnabled )
		{
			if ( m_bEnabled )
				m_rLockObject.Enter();
		}
		//
		// Destructor
		//
		~LockMgr()
		{
			if ( m_bEnabled )
				m_rLockObject.Leave();
		}
	private:
		T&   m_rLockObject;
		BOOL m_bEnabled;
	};




}
}

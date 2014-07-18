//---------------------------------------------------------------------------
//
// LockMgr.cpp
//
// SUBSYSTEM: 
//				Generic libraries
// MODULE:    
//              Interface declaration of CSWrapper CRITICAL_SECTION wrapper 
//
// DESCRIPTION:
//              
//
// AUTHOR:		Ivo Ivanov
//                                                                         
//---------------------------------------------------------------------------
#include "stdafx.h"
#include "utility/Lock.h"

namespace common
{
namespace utility
{

	//---------------------------------------------------------------------------
	//
	// class CSWrapper 
	//
	// CRTICIAL_SECTION user object wrapper
	//
	//---------------------------------------------------------------------------

	//---------------------------------------------------------------------------
	//
	// Constructor
	//
	//---------------------------------------------------------------------------
	CSWrapper::CSWrapper()
	{
		m_nSpinCount = 0;
		::InitializeCriticalSection( &m_cs );
	}

	//---------------------------------------------------------------------------
	//
	// Destructor
	//
	//---------------------------------------------------------------------------
	CSWrapper::~CSWrapper()
	{
		::DeleteCriticalSection( &m_cs );
	}


	//---------------------------------------------------------------------------
	// Enter 
	//
	// This function waits for ownership of the specified critical section object 
	//---------------------------------------------------------------------------
	void CSWrapper::Enter()
	{
		::EnterCriticalSection( &m_cs );
		m_nSpinCount++;
	}

	//---------------------------------------------------------------------------
	// Leave
	//
	// Releases ownership of the specified critical section object. 
	//---------------------------------------------------------------------------
	void CSWrapper::Leave()
	{
		m_nSpinCount--;
		::LeaveCriticalSection( &m_cs );
	}


}
}
//--------------------- End of the file -------------------------------------

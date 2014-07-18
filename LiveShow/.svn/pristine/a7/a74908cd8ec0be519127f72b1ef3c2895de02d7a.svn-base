#ifndef __BASETHREAD_H__
#define __BASETHREAD_H__

#include "Common.h"
#include "Mutex.h"

class COMMON_OUT_CLASS BaseThread
{
public:
	    //
		// Call before calling any other Thread function
		static void			Initialize();
		static void         Destroy();

		BaseThread();
		virtual 			~BaseThread();
		//
		// Derived classes must implement their own entry function
		virtual void 		Entry() = 0;
		void 				Start();

		static void			Sleep(UInt32 inMsec);

		void				Join();
		void				CancelStopRequest()					{	MutexLocker locker(&m_mutex); m_bStopRequested = false;}
		void				SendStopRequest()					{ 	MutexLocker locker(&m_mutex); m_bStopRequested = true; }
		bool				IsStopRequested()					{ 	MutexLocker locker(&m_mutex);  return m_bStopRequested; }
		void                SetRunningState(bool bRunning)      { 	MutexLocker locker(&m_mutex); m_bThreadRunning = bRunning; }
		bool                IsRunningState()                    { 	MutexLocker locker(&m_mutex); return m_bThreadRunning; }
		void				StopAndWaitForThread();

		void*				GetThreadData()						{ return m_pThreadData; }
		void				SetThreadData(void* inThreadData)	{ m_pThreadData = inThreadData; }

		static void*		GetMainThreadData()					{ return s_pMainThreadData; }
		static void			SetMainThreadData(void* inData)		{ s_pMainThreadData = inData; }


		inline				void	SetSeqNo(UInt32 dwSeqNo)			{ m_dwThreadSeqNo = dwSeqNo; }
		inline				UInt32  GetSeqNo()							{ return m_dwThreadSeqNo; }

#ifdef DEBUG
		UInt32				GetNumLocksHeld()					{ return 0; }
		void				IncrementLocksHeld() {}
		void				DecrementLocksHeld() {}
#endif


	    static int			GetErrno();
	    static DWORD		GetCurrentThreadID()			{ return ::GetCurrentThreadId(); }



		HANDLE				GetThreadID()			
		{ 
//			MutexLocker locker(&m_mutex); 
			return m_nThreadID; 
		}

		static  BaseThread*		GetCurrent();


protected:

    static DWORD			s_nThreadStorageIndex;
	UInt32					m_dwThreadSeqNo;

	static void*			s_pMainThreadData;

	mutable bool			m_bStopRequested;
	bool					m_bJoined;
	void*					m_pThreadData;
	bool                    m_bThreadRunning;
	HANDLE					m_nThreadID;           //¾ä±ú
	Mutex					m_mutex;

	static unsigned int WINAPI _Entry(LPVOID inThread);
	inline void				Init();

};

class ThreadDataSetter
{
	public:

		ThreadDataSetter(void* inInitialValue, void* inFinalValue) : m_pFinalValue(inFinalValue)
			{ BaseThread::GetCurrent()->SetThreadData(inInitialValue); }

		~ThreadDataSetter() { BaseThread::GetCurrent()->SetThreadData(m_pFinalValue); }

	private:

		void*				m_pFinalValue;
};

#endif //__BASETHREAD_H__

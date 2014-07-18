#ifndef _TIMEUTILS_H_
#define _TIMEUTILS_H_

#include "Common.h"

#include <string>

class COMMON_OUT_CLASS TimeUtils
{
	public:
	
		//call this before calling anything else
		static void	Initialize();

		static SInt32 Min(SInt32 a, SInt32 b) { if (a < b) return a; return b; }
		
		//
		// Milliseconds always returns milliseconds since Jan 1, 1970 GMT.
		// This basically makes it the same as a POSIX time_t value, except
		// in msec, not seconds. To convert to a time_t, divide by 1000.
		static SInt64 	Milliseconds();

		static SInt64	Microseconds();
							
		static SInt64	TimeMilli_To_Fixed64Secs(SInt64 inMilliseconds)
							{ return (SInt64)(.5 + inMilliseconds * 4294967.296L); }
		
		static SInt64	TimeMilli_To_1900Fixed64Secs(SInt64 inMilliseconds)
							{ return TimeMilli_To_Fixed64Secs(sMsecSince1900 + inMilliseconds); }

		static SInt64	TimeMilli_To_UnixTimeMilli(SInt64 inMilliseconds)
							{ return inMilliseconds; }

		static time_t	TimeMilli_To_UnixTimeSecs(SInt64 inMilliseconds)
							{ return (time_t)  ( (SInt64) TimeMilli_To_UnixTimeMilli(inMilliseconds) / (SInt64) 1000); }
		
		static time_t 	UnixTime_Secs(void) // Seconds since 1970
							{ return TimeMilli_To_UnixTimeSecs(Milliseconds()); }
							
		// Returns the offset in hours between local time and GMT (or UTC) time.
		static SInt32	GetGMTOffset();

		static std::string GetCurTime();
	private:
		static SInt64 sMsecSince1900;
		static SInt64 sMsecSince1970;
		static SInt64 sInitialMsec;
		static SInt64 sWrapTime;
		static SInt64 sCompareWrap;
		static SInt64 sLastTimeMilli;
};


#endif //_TIMEUTILS_H_



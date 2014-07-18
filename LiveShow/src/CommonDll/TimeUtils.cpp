#include "stdafx.h"
#include "TimeUtils.h"

#include "Mutex.h"

static Mutex	 sLastMillisMutex;

SInt64 TimeUtils::sMsecSince1970 = 0;
SInt64 TimeUtils::sMsecSince1900 = 0;
SInt64 TimeUtils::sInitialMsec = 0;
SInt64 TimeUtils::sWrapTime = 0;
SInt64 TimeUtils::sCompareWrap = 0;
SInt64 TimeUtils::sLastTimeMilli = 0;

void TimeUtils::Initialize()
{
	Assert(sInitialMsec == 0);  // do only once
	if (sInitialMsec != 0)
		return;

	//setup t0 value for msec since 1900

	//t.tv_sec is number of seconds since Jan 1, 1970. Convert to seconds since 1900	
	SInt64 the1900Msec = 24 * 60 * 60;
	the1900Msec *= (70 * 365) + 17;
	sMsecSince1900 = the1900Msec;

	sWrapTime = (SInt64)0x00000001 << 32;
	sCompareWrap = (SInt64)0xffffffff << 32;
	sLastTimeMilli = 0;

	sInitialMsec = TimeUtils::Milliseconds(); //Milliseconds uses sInitialMsec so this assignment is valid only once.

	sMsecSince1970 = ::time(NULL); 	// POSIX time always returns seconds since 1970
	sMsecSince1970 *= 1000;			// Convert to msec
}

SInt64 TimeUtils::Milliseconds()
{
	MutexLocker locker(&sLastMillisMutex);
	// curTimeMilli = timeGetTime() + ((sLastTimeMilli/ 2^32) * 2^32)
	// using binary & to reduce it to one operation from two
	// sCompareWrap and sWrapTime are constants that are never changed
	// sLastTimeMilli is updated with the curTimeMilli after each call to this function
	SInt64 curTimeMilli = (UInt32)::timeGetTime() + (sLastTimeMilli& sCompareWrap);
	if ((curTimeMilli - sLastTimeMilli) < 0)
	{
		curTimeMilli += sWrapTime;
	}
	sLastTimeMilli = curTimeMilli;

	return (curTimeMilli - sInitialMsec) + sMsecSince1970; // convert to application time
}

SInt64 TimeUtils::Microseconds()
{
	SInt64 curTime = (SInt64)::timeGetTime(); // unsigned long system time in milliseconds
	curTime -= sInitialMsec; // convert to application time
	curTime *= 1000; // convert to microseconds 				  
	return curTime;
}

SInt32 TimeUtils::GetGMTOffset()
{
	TIME_ZONE_INFORMATION tzInfo;
	DWORD theErr = ::GetTimeZoneInformation(&tzInfo);
	if (theErr == TIME_ZONE_ID_INVALID)
		return 0;

	return ((tzInfo.Bias / 60) * -1);
}

std::string TimeUtils::GetCurTime()
{
	std::string retStr;
	char s[50];
	struct tm curr;

	time_t	iCurTime;

	time(&iCurTime);

    localtime_s(&curr, &iCurTime);

	if (curr.tm_year > 50)
	{
		sprintf_s(s, 50, "%04d-%02d-%02d %02d:%02d:%02d", 
			curr.tm_year+1900, curr.tm_mon+1, curr.tm_mday,
			curr.tm_hour, curr.tm_min, curr.tm_sec);
	}
	else
	{
		sprintf_s(s, 50, "%04d-%02d-%02d %02d:%02d:%02d", 
			curr.tm_year+2000, curr.tm_mon+1, curr.tm_mday,
			curr.tm_hour, curr.tm_min, curr.tm_sec);
	}
	retStr =s;
	return retStr;
}



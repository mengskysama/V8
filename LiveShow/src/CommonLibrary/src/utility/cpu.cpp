#include "stdafx.h"
#include "utility/cpu.h"

namespace common
{
namespace utility
{
namespace systemhelper
{
	/// 时间转换  
	static __int64 file_time_2_utc(const FILETIME* ftime)  
	{
		LARGE_INTEGER li;

		assert(ftime);  
		li.LowPart = ftime->dwLowDateTime;  
		li.HighPart = ftime->dwHighDateTime;  
		return li.QuadPart;  
	}

	/// 获得CPU的核数  
	int get_processor_number()  
	{
		//cpu数量  
		static int processor_count_ = -1;  

		if(processor_count_ == -1)  
		{
			SYSTEM_INFO info;  
			GetSystemInfo(&info);  
			processor_count_ = (int)info.dwNumberOfProcessors;
		}

		return processor_count_;  
	}

	float get_cpu_usage()  
	{
		//cpu数量  
		static int processor_count_ = get_processor_number();  
		//上一次的时间  
		static __int64 last_time_ = 0;  
		static __int64 last_system_time_ = 0;  

		FILETIME now;  
		FILETIME creation_time;  
		FILETIME exit_time;  
		FILETIME kernel_time;  
		FILETIME user_time;  
		__int64 system_time;  
		__int64 time;  
		__int64 system_time_delta;  
		__int64 time_delta;  

		float cpu = -1;

		GetSystemTimeAsFileTime(&now);  

		if (!GetProcessTimes(GetCurrentProcess(), &creation_time, &exit_time,  
			&kernel_time, &user_time))  
		{
			return -1;
		}  
		system_time = (file_time_2_utc(&kernel_time) + file_time_2_utc(&user_time)) /  processor_count_;  
		time = file_time_2_utc(&now);  

		if ((last_system_time_ == 0) || (last_time_ == 0))  
		{
			// First call, just set the last values.  
			last_system_time_ = system_time;  
			last_time_ = time;  
			return -1;  
		}  

		system_time_delta = system_time - last_system_time_;  
		time_delta = time - last_time_;  

		assert(time_delta != 0);  

		if (time_delta == 0)  
			return -1;

		// We add time_delta / 2 so the result is rounded.  
		cpu = (((float)system_time_delta * 100 + (float)time_delta / 2) / time_delta);  
		last_system_time_ = system_time;  
		last_time_ = time;  
		return cpu;
	}
}
}
}
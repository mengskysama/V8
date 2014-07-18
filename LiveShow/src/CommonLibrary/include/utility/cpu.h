#pragma once

#include "../Common.h"

namespace common
{
namespace utility
{
namespace systemhelper
{
	//获取当前进程的cpu占用率
	COMMON_INTERFACE float get_cpu_usage();

	//获取cpu核数
	COMMON_INTERFACE int get_processor_number();
};

}
}
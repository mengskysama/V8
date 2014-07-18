#pragma once
//boost
#include <boost/shared_ptr.hpp>
using namespace boost;

//speex
#include <speex/speex.h>
#include <speex/speex_header.h>
#include <speex/speex_jitter.h>
#include <speex/speex_preprocess.h>
#include <speex/speex_echo.h>
#include <speex/speex_resampler.h>

//celt
#include <celt.h>
#include <celt_header.h>

//stl
#include <list>
#include <string>
#include <map>
#include <set>
#include <deque>
#include <vector>
using namespace std;

//math
#include <math.h>


//#defines
#define M_PI 3.14159265358979323846
#define STACKVAR(type, varname, count) type *varname=reinterpret_cast<type *>(_alloca(sizeof(type) * (count)))

#define iroundf(x) ( static_cast<int>(x) )
#define lroundf(x) ( static_cast<int>( (x) + ((x) >= 0 ? 0.5 : -0.5) ) )

typedef unsigned char byte;
typedef std::vector<byte> QByteArray;

//windows
//#include <WinSock.h>
//#include <Windows.h>
#include <process.h>
#define  RESTRICT
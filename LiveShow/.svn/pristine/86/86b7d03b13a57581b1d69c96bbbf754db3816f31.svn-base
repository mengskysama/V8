#pragma once

#include <stdio.h>
#include "Common.h"
#include <assert.h>

#define __LOG__       1

class COMMON_OUT_CLASS AssertLogger
{
public:
	  
	  AssertLogger();

	  ~AssertLogger();

	  // An interface can write a message
	  void  LogAssert(const char* format, ... );
	  void  LogNone(const char* format, ... ){}

#if __LOG__

private:
	  FILE*		m_pFd;
	  char* GetCurrPath(char* buf,int len);
#endif

};

// class CSynLogManager;
// extern CSynLogManager g_synlogmanager;

#if __LOG__

#define Assert(condition)	 {								     \
	if (!(condition)) 										     \
	{														     \
	    g_Logger.LogAssert("_Assert: %s, %d \n", __FILE__, __LINE__); 	 \
	}}


#define AssertV(condition, errNo) 	{									         \
	if (!(condition)) 													         \
    { 																	         \
	    g_Logger.LogAssert("_AssertV: %s, %d (%d)\n", __FILE__, __LINE__, (errNo));         \
    }}

#define AssertS(condition, msg){									              \
	if(!(condition)){															  \
		g_Logger.LogAssert("_AssertS: %s, %d (%s)\n", __FILE__, __LINE__, (msg)); \
	}}

#define AssertVS(condition,no, msg){									              \
	if(!(condition)){															  \
	g_Logger.LogAssert("_AssertVS: %s, %d (%d:%s)\n", __FILE__, __LINE__,(no),(msg)); \
	}}	

#define AssertVSS(condition,no,no1, msg){												\
	if(!(condition)){																	\
	g_Logger.LogAssert("_AssertVS: %s, %d, (%d:%d:%s)\n", __FILE__, __LINE__,(no),(no1),(msg)); \
	}}	

#define Warn(condition) {										\
	if (!(condition)) 										    \
    {                                                           \
	    g_Logger.LogAssert("_Warn: %s, %d\n", __FILE__, __LINE__);        \
	}}

#define WarnV(condition, msg)	{          \
	if (!(condition))                                                             \
	{                                                                             \
		g_Logger.LogAssert("_WarnV: %s, %d (%s) \n", __FILE__, __LINE__, (msg) ); \
	}}

#else                     

#define Assert(condition)                  ((void) 0)
#define AssertV(condition, errNo)          ((void) 0)
#define Warn(condition)                    ((void) 0)
#define WarnV(condition, msg)              ((void) 0)

#endif
extern COMMON_OUT_CLASS AssertLogger  g_Logger;   

//#define Trace               g_Logger.LogAssert
#define Trace                 g_Logger.LogNone
#define BufferTrace           g_Logger.LogNone
#define LostTrace             g_Logger.LogAssert
#define ErrTrace			  g_Logger.LogAssert
#define CocTrace			  g_Logger.LogAssert





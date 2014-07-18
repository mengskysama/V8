/**
* ModuleManager.h
* this file define the ModuleManager class
*
* @version 1.0 (2011-09-30)
* @author plushu
*
*/

#pragma once

#include <utility>

typedef signed __int64 int64;
typedef signed int int32;
typedef signed short int16;
typedef signed char int8;

typedef unsigned __int64 uint64;
typedef unsigned int uint32;
typedef unsigned short uint16;
typedef unsigned char uint8;
typedef unsigned char byte;
typedef unsigned long       ulong;

typedef uint32 uintptr;
typedef uint32 EventValue;
typedef uint32 MessageValue;
typedef uint32 ServiceValue;
typedef uint32 uin;
typedef uint32 tid;
typedef uint32 param;

typedef uint16 ModuleIdentity;
typedef uint32 Cookie;
typedef uint32 Value;

typedef std::pair<std::size_t, byte*> ByteBuf;
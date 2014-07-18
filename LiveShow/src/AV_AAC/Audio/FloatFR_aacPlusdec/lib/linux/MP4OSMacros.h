/*
This software module was originally developed by Apple Computer, Inc.
in the course of development of MPEG-4. 
*/

#ifndef INCLUDED_MP4OSMACROS_H
#define INCLUDED_MP4OSMACROS_H

/* includes not used by the library, but harmless & used by programs using the library - dws */
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <stdint.h>

#ifndef TEST_RETURN
#define TEST_RETURN(err)
/*#define TEST_RETURN(err) assert((err)==0)*/
#endif

#define MP4_EXTERN(v) extern v

typedef long long u64;
typedef long long s64;

typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;
typedef int s32;
typedef short s16;
typedef char s8;

#define MP4_FOUR_CHAR_CODE( a, b, c, d ) (((a)<<24)|((b)<<16)|((c)<<8)|(d))

#endif

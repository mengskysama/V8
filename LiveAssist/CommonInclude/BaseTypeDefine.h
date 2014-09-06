/********************************************************************
 * File name: BaseTypeDefine.h
 *
 * Author:    DragonZ
 *
 * Version:   1.0
 * 
 * Date:      2014/08/27
 * 
 * Description: 基本数据类型定义
********************************************************************/

#pragma once

#ifndef BASETYPEDEFINE_H
#define BASETYPEDEFINE_H


#ifndef ALREADY_DEFINE_DATA_TYPE

#if defined(WIN32) || defined(_WIN32)

typedef char						int8_t;
typedef unsigned char				uint8_t;
typedef short						int16_t;
typedef unsigned short				uint16_t;
typedef int							int32_t;
typedef unsigned int				uint32_t;
typedef long long					int64_t;
typedef unsigned long long			uint64_t;

#elif defined(_WIN64)

typedef char						int8_t;
typedef unsigned char				uint8_t;
typedef short						int16_t;
typedef unsigned short				uint16_t;
typedef int							int32_t;
typedef unsigned int				uint32_t;
typedef long 						int64_t;
typedef unsigned long				uint64_t;

#endif

#endif  //ALREADY_DEFINE_DATA_TYPE


typedef uint32_t EventValue;
typedef uint32_t MessageValue;
typedef uint32_t ServiceValue;
typedef uint16_t ModuleId;
typedef uint32_t ModuleParam;


#endif // BASETYPEDEFINE_H
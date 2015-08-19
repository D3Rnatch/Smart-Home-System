#ifndef __TYPES_H__
#define __TYPES_H__
#pragma once

typedef signed char s8;
typedef unsigned char u8;

typedef signed short s16;
typedef unsigned short u16;

#ifdef __linux__
    typedef signed int   s32;
    typedef unsigned int u32;

    typedef signed long s64;
    typedef unsigned long u64;
#else
    typedef signed long s32;
    typedef unsigned long u32;

    typedef signed __int64 s64;
    typedef unsigned __int64 u64;
#endif

typedef float f32;
typedef double f64;

#endif

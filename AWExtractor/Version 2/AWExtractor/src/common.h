#pragma once
#include <string>

typedef std::string string;

typedef unsigned char u8;

typedef unsigned short u16;

typedef unsigned long u32;
typedef long s32;

typedef unsigned long long u64;
typedef long long s64;

typedef u64 rid;

#define MAKEFOURCC(ch0, ch1, ch2, ch3)                              \
                ((u32)(u8)(ch0) | ((u32)(u8)(ch1) << 8) |       \
                ((u32)(u8)(ch2) << 16) | ((u32)(u8)(ch3) << 24))
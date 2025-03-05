#pragma once
#ifndef TOOLS_H
#define TOOLS_H
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <graphics.h>

// Same as windows api and C++ std::max and std::min
#ifndef max
#define max(a,b) (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif

// HEX String to COLORREF Tool
COLORREF hexToColorref(const char* hex_str);
bool checkHexColor(const char* hex_str);

// Darken Color Tool
color_t darkenColor(color_t color, float factor);

// MurmurHash3算法 也是G++的哈希算法 用来实现密码加密
// 参考自其他实现
uint32_t hash_murmur3(const void* data, size_t length);

// KMP算法
int kmpSearchWchar(const wchar_t* text, const wchar_t* pattern);

#endif // TOOLS_H

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

#endif // TOOLS_H

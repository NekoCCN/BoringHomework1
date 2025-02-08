#pragma once
#ifndef TOOLS_H
#define TOOLS_H
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <graphics.h>

// HEX String to COLORREF Tool
COLORREF hexToColorref(const char* hex_str);
bool checkHexColor(const char* hex_str);



#endif // TOOLS_H

#pragma once
#ifndef TOOLS_H
#define TOOLS_H
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <graphics.h>

// MD Color
#define MD_LIGHT_GREY   hexToColorref("#EEEEEE")
#define MD_DARK_GREY    hexToColorref("#BDBDBD")
#define MD_TEXT_PRIMARY hexToColorref("#212121")
#define MD_TEXT_SECONDARY hexToColorref("#757575")
#define MD_BLUE         hexToColorref("#2196F3")
#define MD_SCROLLBAR_BG hexToColorref("#C0C0C0")
#define MD_SCROLLBAR_THUMB hexToColorref("#808080")
#define MD_BUTTON_NORMAL hexToColorref("#e0e0e0")
#define MD_BUTTON_HOVER hexToColorref("#bdbdbd")
#define MD_BUTTON_ACTIVE hexToColorref("#4fc3f7")

// HEX String to COLORREF Tool
COLORREF hexToColorref(const char* hex_str);
bool checkHexColor(const char* hex_str);



#endif // TOOLS_H

#pragma once
#ifndef MATERIALINPUT_H
#define MATERIALINPUT_H
#include <graphics.h>
#include <windows.h>
#include <wchar.h>
#include "../../Tools/Tools.h"

// Windows API 自带的坐标系也太难用了 弃用
typedef struct MaterialInputStruct
{
    int x_;
    int y_;
    int width_;
    int height_;

    wchar_t* text_;

    wchar_t* visible_text_buffer_;

    size_t buffer_size_;

    int cursor_position_;
    int scroll_offset_;

    bool has_focus_;
    bool is_hovered_;

    // 颜色
    color_t background_color_;
    color_t text_color_;
    color_t cursor_color_;
    color_t focus_border_color_;
    color_t unfocus_border_color_;
    color_t hover_border_color_;
    color_t placeholder_text_color_;

    // Windows API 仍然是用于截断 和MaterialList一样
    HDC hdc;
    HRGN hRgn;
    HWND hWnd;

    wchar_t* placeholder_;

} MaterialInput;

MaterialInput* MATERIALINPUT_createGrayInput(int x, int y, int width, int height, const wchar_t* placeholder);
MaterialInput* MATERIALINPUT_createBlueInput(int x, int y, int width, int height, const wchar_t* placeholder);
MaterialInput* MATERIALINPUT_create(int x, int y, int width, int height, const wchar_t* placeholder,
    color_t background_color, color_t text_color,
    color_t cursor_color, color_t focus_border_color, color_t unfocus_border_color,
    color_t hover_border_color,
    color_t placeholder_text_color);

void MATERIALINPUT_destroy(MaterialInput* input);

void MATERIALINPUT_draw(MaterialInput* input);

void MATERIALINPUT_handleKeyDown(MaterialInput* input, key_msg msg);

void MATERIALINPUT_handleChar(MaterialInput* input, key_msg msg);

void MATERIALINPUT_handleMouseDown(MaterialInput* input, int x, int y);

void MATERIALINPUT_handleMouseMove(MaterialInput* input, int x, int y);

void MATERIALINPUT_setFocus(MaterialInput* input, bool focus);

const wchar_t* MATERIALINPUT_getText(MaterialInput* input);

void MATERIALINPUT_setText(MaterialInput* input, const wchar_t* text);

void MATERIALINPUT_resizeBuffer(MaterialInput* input);

void MATERIALINPUT_handleInput(MaterialInput* input, key_msg msg);

int MATERIALINPUT_calculateCursorX(MaterialInput* input);

void MATERIALINPUT_adjustScrollOffset(MaterialInput* input);

#endif // MATERIALINPUT_H
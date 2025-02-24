#pragma once
#ifndef LISTBUTTON_H
#define LISTBUTTON_H
#include <graphics.h>
#include "../../Tools/Tools.h"

typedef struct ListButton
{
    RECT rect_;

    COLORREF normal_color_;
    COLORREF hover_color_;
    COLORREF actived_color_;
    COLORREF current_color_;

    COLORREF font_color_;

    bool have_frame_;
    COLORREF frame_color_;

    int radius_;

    const char* text_;

    bool is_mouse_inside_;
    bool is_active_;

    int padding;
} ListButton;

ListButton* LISTBUTTON_createGrayButton(int left, int top, int right, int bottom, const char* text, int radius);

void LISTBUTTON_draw(ListButton* self);

void LISTBUTTON_handleMouseMove(ListButton* self, int x, int y);

void LISTBUTTON_handleButtonDown(ListButton* self, int x, int y);

bool LISTBUTTON_isActived(ListButton* self);

void LISTBUTTON_SetStatus(ListButton* self, bool status);

void LISTBUTTON_destroy(ListButton* self);

ListButton* LISTBUTTON_create(int left, int top, int right, int bottom, const char* text,
    COLORREF normal_color, COLORREF hover_color, COLORREF actived_color,
    COLORREF font_color, bool have_frame, COLORREF frame_color, int radius, int padding); // 添加 padding 参数

#endif
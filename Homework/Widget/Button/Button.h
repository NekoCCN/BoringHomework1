#pragma once
#ifndef BUTTON_H
#define BUTTON_H
#include <graphics.h>
#include <wchar.h>
#include "../../Tools/Tools.h"

typedef struct ButtonStruct
{
    int x_;
    int y_;
    int width_;
    int height_;
    wchar_t* text_;


    color_t background_color_;
    color_t text_color_;
    color_t hover_color_;
    color_t click_color_;


    bool is_hovered_;
    bool is_pressed_;

    bool is_clicked_;
} Button;

Button* BUTTON_create(int x, int y, int width, int height, const wchar_t* text,
    color_t background_color, color_t text_color,
    color_t hover_color, color_t click_color);

Button* BUTTON_createGrayButton(int x, int y, int width, int height, const wchar_t* text);
Button* BUTTON_createBlueButton(int x, int y, int width, int height, const wchar_t* text);

void BUTTON_destroy(Button* button);
void BUTTON_draw(Button* button);
void BUTTON_handleMouseMove(Button* button, int x, int y);
void BUTTON_handleMouseDown(Button* button, int x, int y);
void BUTTON_handleMouseUp(Button* button, int x, int y);
bool BUTTON_isClicked(Button* button);

#endif
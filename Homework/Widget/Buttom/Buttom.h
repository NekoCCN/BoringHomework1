#pragma once
#ifndef BUTTOM_H
#define BUTTOM_H
#include <graphics.h>

typedef struct Button
{
    RECT rect_;

    COLORREF normal_color_;
    COLORREF hover_color_;
    COLORREF click_color_;
    COLORREF current_color_;

    int radius_;

    const char* text_;

    int steps_;
    int delayTime_;

    bool is_mouse_inside_;

    bool is_clicked_;

    HWND hwnd_;
} Button;

Button* Button_createCyanButton(int left, int top, int right, int bottom, const char* text, int radius);

void Button_draw(Button* self);

void Button_animateColor(Button* self, COLORREF startColor, COLORREF endColor);

void Button_handleMouseMove(Button* self, int x, int y);

void Button_handleButtonDown(Button* self, int x, int y);

void Button_destroy(Button* self);

Button* Button_create(int left, int top, int right, int bottom, const char* text,
    COLORREF normal_color, COLORREF hover_color, COLORREF click_color, int radius);

#endif
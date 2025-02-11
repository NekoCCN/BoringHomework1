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

    COLORREF font_color_;

    bool have_frame_;
    COLORREF frame_color_;

    int radius_;

    const char* text_;

    int steps_;
    int delayTime_;

    bool is_mouse_inside_;

    bool is_clicked_;
} Button;

Button* BUTTON_createCyanButton(int left, int top, int right, int bottom, const char* text, int radius);

void BUTTON_draw(Button* self);

void BUTTON_animateColor(Button* self, COLORREF startColor, COLORREF endColor);

void BUTTON_handleMouseMove(Button* self, int x, int y);

void BUTTON_handleButtonDown(Button* self, int x, int y);

bool button_isClicked(Button* self);

void BUTTON_destroy(Button* self);

Button* BUTTON_create(int left, int top, int right, int bottom, const char* text,
    COLORREF normal_color, COLORREF hover_color, COLORREF click_color,
    COLORREF font_color, bool have_frame, COLORREF frame_color, int radius);

#endif
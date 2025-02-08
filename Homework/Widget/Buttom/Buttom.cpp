#include "Buttom.h"

Button* Button_createCyanButton(int left, int top, int right, int bottom, const char* text, int radius)
{
    return Button_create(left, top, right, bottom, text, RGB(0, 191, 255), RGB(0, 255, 255),
        RGB(0, 139, 139), radius);
}

void Button_draw(Button* self)
{
    setfillcolor(self->current_color_);
    fillroundrect(self->rect_.left, self->rect_.top, self->rect_.right, self->rect_.bottom, 10, 10);
    setcolor(WHITE);
    roundrect(self->rect_.left, self->rect_.top, self->rect_.right, self->rect_.bottom, 10, 10);

    int text_width = textwidth(self->text_);
    int text_height = textheight(self->text_);

    // 取按钮中间
    int text_x = self->rect_.left + (self->rect_.right - self->rect_.left - text_width) / 2;
    int text_y = self->rect_.top + (self->rect_.bottom - self->rect_.top - text_height) / 2;

    setcolor(WHITE);

    // 更改透明混合模式 否则字体周围会有黑色边框
    setbkmode(TRANSPARENT);

    outtextxy(text_x, text_y, self->text_);
}

void Button_animateColor(Button* self, COLORREF startColor, COLORREF endColor)
{
    float rStart = GetRValue(startColor);
    float gStart = GetGValue(startColor);
    float bStart = GetBValue(startColor);

    float rEnd = GetRValue(endColor);
    float gEnd = GetGValue(endColor);
    float bEnd = GetBValue(endColor);

    for (int i = 0; i <= self->steps_; i++)
    {
        float t = (float)i / (float)self->steps_;

        // 由step决定 逐步简单线性插值到 endColor
        int r = (int)(rStart + (rEnd - rStart) * t);
        int g = (int)(gStart + (gEnd - gStart) * t);
        int b = (int)(bStart + (bEnd - bStart) * t);

        self->current_color_ = RGB(r, g, b);

        Button_draw(self);
        Sleep(self->delayTime_);
    }
}

void Button_handleMouseMove(Button* self, int x, int y)
{
    POINT pt = { x, y };

    if (PtInRect(&self->rect_, pt))
    {
        if (!self->is_mouse_inside_)
        {
            self->is_mouse_inside_ = true;
            Button_animateColor(self, self->normal_color_, self->hover_color_);
        }
    }
    else
    {
        if (self->is_mouse_inside_)
        {
            self->is_mouse_inside_ = false;
            Button_animateColor(self, self->hover_color_, self->normal_color_);
        }
    }
}

void Button_handleButtonDown(Button* self, int x, int y)
{
    POINT pt = { x, y };

    if (PtInRect(&self->rect_, pt))
    {
        self->current_color_ = self->click_color_;

        GetCursorPos(&pt);
        ScreenToClient(self->hwnd_, &pt);
        if (PtInRect(&self->rect_, pt))
        {
            Button_animateColor(self, self->click_color_, self->hover_color_);
        }
        else
        {
            Button_animateColor(self, self->click_color_, self->normal_color_);
        }
    }
    Sleep(self->delayTime_);
}

void Button_destroy(Button* self)
{
    free(self);
}

Button* Button_create(int left, int top, int right, int bottom, const char* text,
    COLORREF normal_color, COLORREF hover_color, COLORREF click_color, int radius)
{
    Button* button = (Button*)malloc(sizeof(Button));

    // 纯C语言没有异常机制
    if (button == NULL)
    {
        return NULL;
    }

    button->rect_.left = left;
    button->rect_.top = top;
    button->rect_.right = right;
    button->rect_.bottom = bottom;

    button->normal_color_ = normal_color;
    button->hover_color_ = hover_color;
    button->click_color_ = click_color;
    button->current_color_ = button->normal_color_;

    button->radius_ = radius;

    button->text_ = text;

    button->steps_ = 60;
    button->delayTime_ = 5;

    button->is_mouse_inside_ = false;
    button->is_clicked_ = false;

    button->hwnd_ = getHWnd();

    return button;
}
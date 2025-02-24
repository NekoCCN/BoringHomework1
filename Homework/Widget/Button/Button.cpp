#include "Button.h"

Button* BUTTON_createGrayButton(int x, int y, int width, int height, const wchar_t* text)
{
    color_t background_color = EGERGB(0xF5, 0xF5, 0xF5);
    color_t text_color = EGERGB(0x00, 0x00, 0x00);

    color_t hover_color = darkenColor(background_color, 0.9f);
    color_t click_color = darkenColor(background_color, 0.8f);

    return BUTTON_create(x, y, width, height, text, background_color, text_color,
        hover_color, click_color);
}

Button* BUTTON_createBlueButton(int x, int y, int width, int height, const wchar_t* text)
{    
    color_t background_color = EGERGB(0x21, 0x96, 0xF3);
    color_t text_color = EGERGB(0xFF, 0xFF, 0xFF);

    color_t hover_color = darkenColor(background_color, 0.9f);
    color_t click_color = darkenColor(background_color, 0.8f);

    return BUTTON_create(x, y, width, height, text, background_color, text_color,
        hover_color, click_color);
}

Button* BUTTON_create(int x, int y, int width, int height, const wchar_t* text,    color_t background_color, color_t text_color,
    color_t hover_color, color_t click_color)
{
    Button* button = (Button*)malloc(sizeof(Button));
    if (!button)
    {
        return NULL;
    }

    button->x_ = x;
    button->y_ = y;
    button->width_ = width;
    button->height_ = height;

    size_t text_len = wcslen(text);
    button->text_ = (wchar_t*)malloc((text_len + 1) * sizeof(wchar_t));
    if (!button->text_)
    {
        free(button);
        return NULL;
    }
    wcscpy(button->text_, text);

    button->background_color_ = background_color;
    button->text_color_ = text_color;
    button->hover_color_ = hover_color;
    button->click_color_ = click_color;

    button->is_hovered_ = false;
    button->is_pressed_ = false;

    button->is_clicked_ = false;

    return button;
}

void BUTTON_destroy(Button* button)
{
    if (button)
    {
        free(button->text_);
        free(button);
    }
}

void BUTTON_draw(Button* button)
{    
    color_t current_bg_color;
    if (button->is_pressed_)
    {
        current_bg_color = button->click_color_;
    }
    else if (button->is_hovered_)
    {
        current_bg_color = button->hover_color_;
    }
    else
    {
        current_bg_color = button->background_color_;
    }

    setfillcolor(current_bg_color);
    fillroundrect(button->x_, button->y_, button->x_ + button->width_, button->y_ + button->height_, 10, 10);
    setcolor(current_bg_color);
    roundrect(button->x_, button->y_, button->x_ + button->width_, button->y_ + button->height_, 10, 10);

    setcolor(button->text_color_);
    setbkmode(TRANSPARENT);
    int text_x = button->x_ + (button->width_ - textwidth(button->text_)) / 2;
    int text_y = button->y_ + (button->height_ - textheight(button->text_)) / 2;
    outtextxy(text_x, text_y, button->text_);
}

void BUTTON_handleMouseMove(Button* button, int x, int y)
{
    button->is_hovered_ = (x >= button->x_ && x <= button->x_ + button->width_ &&
        y >= button->y_ && y <= button->y_ + button->height_);
}

void BUTTON_handleMouseDown(Button* button, int x, int y)
{    
    if (button->is_hovered_)
    {
        button->is_pressed_ = true;

        if (button->is_clicked_ == false)
        {
            button->is_clicked_ = true;
        }
    }
}

void BUTTON_handleMouseUp(Button* button, int x, int y)
{
    button->is_pressed_ = false;
}

bool BUTTON_isClicked(Button* button)
{
    if (button->is_clicked_ == true)
    {
        button->is_clicked_ = false;
        return true;
    }

    return false;
}
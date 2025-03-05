#include "ListButton.h"

ListButton* LISTBUTTON_createGrayButton(int left, int top, int right, int bottom, const char* text, int radius)
{
    return LISTBUTTON_create(left, top, right, bottom, text,
        hexToColorref("#e0e0e0"), hexToColorref("#bdbdbd"),
        hexToColorref("#4fc3f7"), BLACK,
        false, WHITE, radius, 10); // 默认内边距为 10
}

ListButton* LISTBUTTON_createGrayButton_XYWH(int x, int y, int width, int height, const char* text, int radius)
{
    int left = x;
    int top = y;
    int right = x + width;
    int bottom = y + height;
    return LISTBUTTON_createGrayButton(left, top, right, bottom, text, radius);
}

void LISTBUTTON_draw(ListButton* self)
{
    // 渲染填充色
    setfillcolor(self->current_color_);
    fillroundrect(self->rect_.left, self->rect_.top, self->rect_.right, self->rect_.bottom, self->radius_, self->radius_);

    // 渲染边框
    if (self->have_frame_)
    {
        setcolor(self->frame_color_);
        roundrect(self->rect_.left, self->rect_.top, self->rect_.right, self->rect_.bottom, self->radius_, self->radius_);
    }
    else
    {
        setcolor(self->current_color_);
        roundrect(self->rect_.left, self->rect_.top, self->rect_.right, self->rect_.bottom, self->radius_, self->radius_);
    }

    int text_width = textwidth(self->text_);
    int text_height = textheight(self->text_);
    int text_x = self->rect_.left + self->padding;
    int text_y = self->rect_.top + (self->rect_.bottom - self->rect_.top - text_height) / 2;

    setcolor(self->font_color_);

    setbkmode(TRANSPARENT);

    outtextxy(text_x, text_y, self->text_);
}

void LISTBUTTON_handleMouseMove(ListButton* self, int x, int y)
{
    if (self->is_active_)
    {
        return;
    }

    POINT pt = { x, y };

    if (PtInRect(&self->rect_, pt))
    {
        if (!self->is_mouse_inside_)
        {
            self->is_mouse_inside_ = true;
            self->current_color_ = self->hover_color_;
        }
    }
    else
    {
        if (self->is_mouse_inside_)
        {
            self->is_mouse_inside_ = false;
            self->current_color_ = self->normal_color_;
        }
    }
}
bool LISTBUTTON_handleButtonDown(ListButton* self, int x, int y)
{
    POINT pt = { x, y };
    if (PtInRect(&self->rect_, pt))
    {
        self->is_active_ = true;
        self->current_color_ = self->actived_color_;
        return true;
    }

    return false;
}

bool LISTBUTTON_isActived(ListButton* self)
{
    return self->is_active_;
}

void LISTBUTTON_SetStatus(ListButton* self, bool status)
{
    self->is_active_ = status;
    self->current_color_ = status ? self->actived_color_ : self->normal_color_;
}

void LISTBUTTON_destroy(ListButton* self)
{
    free(self);
}

ListButton* LISTBUTTON_create(int left, int top, int right, int bottom, const char* text, COLORREF normal_color,
    COLORREF hover_color, COLORREF actived_color, COLORREF font_color, bool have_frame, COLORREF frame_color,
    int radius, int padding) // 添加 padding 参数
{
    ListButton* button = (ListButton*)malloc(sizeof(ListButton));
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

    button->actived_color_ = actived_color;
    button->current_color_ = button->normal_color_;
    button->font_color_ = font_color;

    button->have_frame_ = have_frame;
    button->frame_color_ = frame_color;

    button->radius_ = radius;

    button->text_ = text;

    button->is_mouse_inside_ = false;
    button->is_active_ = false;

    button->padding = padding;

    return button;
}
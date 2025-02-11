#include "Sidebar.h"

Sidebar* SIDEBAR_create(int left, int top, int right, int bottom, COLORREF color, COLORREF font_color,
    COLORREF button_hover_color, COLORREF button_active_color, int radius)
{
    Sidebar* self = (Sidebar*)malloc(sizeof(Sidebar));
    if (!self)
    {
        return NULL;
    }

    self->rect_ = { left, top, right, bottom };
    self->color_ = color;
    self->status_ = SIDEBAR_BUTTON_QUERY;

    int buttonHeight = 30;
    int spacing = 10;
    int padding = 10;
    int currentTop = top + spacing;

    int buttonWidth = right - left - 2 * padding;

    int buttonLeft = left + (right - left - buttonWidth) / 2;

    self->button_insert_ = LISTBUTTON_create(buttonLeft, currentTop, buttonLeft + buttonWidth, currentTop + buttonHeight, "Insert",
        color, button_hover_color, button_active_color, font_color,
        false, WHITE, radius, padding);
    if (!self->button_insert_)
    {
        free(self);
        return NULL;
    }
    currentTop += buttonHeight + spacing;

    self->button_query_ = LISTBUTTON_create(buttonLeft, currentTop, buttonLeft + buttonWidth, currentTop + buttonHeight, "Query",
        color, button_hover_color, button_active_color, font_color,
        false, WHITE, radius, padding);
    if (!self->button_query_)
    {
        free(self->button_insert_);
        free(self);
        return NULL;
    }
    currentTop += buttonHeight + spacing;

    self->button_statistics_ = LISTBUTTON_create(buttonLeft, currentTop, buttonLeft + buttonWidth, currentTop + buttonHeight, "Statistics",
        color, button_hover_color, button_active_color, font_color,
        false, WHITE, radius, padding);
    if (!self->button_statistics_)
    {
        free(self->button_query_);
        free(self->button_insert_);
        free(self);
        return NULL;
    }

    int exitButtonTop = bottom - spacing - buttonHeight;
    self->button_exit_ = LISTBUTTON_create(buttonLeft, exitButtonTop, buttonLeft + buttonWidth, exitButtonTop + buttonHeight, "Exit",
        color, button_hover_color, button_active_color, font_color,
        false, WHITE, radius, padding);
    if (!self->button_exit_)
    {
        free(self->button_statistics_);
        free(self->button_query_);
        free(self->button_insert_);
        free(self);
        return NULL;
    }

    self->is_exit_ = false;

    LISTBUTTON_SetStatus(self->button_query_, true); // 设置初始激活状态 (Query)

    return self;
}
Sidebar* SIDEBAR_createGraySidebar(int left, int top, int right, int bottom)
{
    return SIDEBAR_create(left, top, right, bottom,
        hexToColorref("#e0e0e0"),
        BLACK, hexToColorref("#bdbdbd"),
        hexToColorref("#4fc3f7"), 5);
}

void SIDEBAR_draw(Sidebar* self)
{
    setfillcolor(self->color_);
    fillrect(self->rect_.left, self->rect_.top, self->rect_.right / 2, self->rect_.bottom);
    setcolor(self->color_);
    rectangle(self->rect_.left, self->rect_.top, self->rect_.right / 2, self->rect_.bottom);

    setfillcolor(self->color_);
    fillroundrect(self->rect_.left, self->rect_.top, self->rect_.right, self->rect_.bottom, 10, 10);
    setcolor(self->color_);
    roundrect(self->rect_.left, self->rect_.top, self->rect_.right, self->rect_.bottom, 10, 10);

    LISTBUTTON_draw(self->button_insert_);
    LISTBUTTON_draw(self->button_query_);
    LISTBUTTON_draw(self->button_statistics_);
    LISTBUTTON_draw(self->button_exit_);
}

void SIDEBAR_handleMouseMove(Sidebar* self, int x, int y)
{
    LISTBUTTON_handleMouseMove(self->button_insert_, x, y);
    LISTBUTTON_handleMouseMove(self->button_query_, x, y);
    LISTBUTTON_handleMouseMove(self->button_statistics_, x, y);
    LISTBUTTON_handleMouseMove(self->button_exit_, x, y);
}

void SIDEBAR_handleButtonDown(Sidebar* self, int x, int y)
{
    LISTBUTTON_handleButtonDown(self->button_exit_, x, y);
    if (LISTBUTTON_isActived(self->button_exit_))
    {
        self->is_exit_ = true;
    }

    if (self->status_ == SIDEBAR_BUTTON_INSERT)
    {
        LISTBUTTON_handleButtonDown(self->button_query_, x, y);
        LISTBUTTON_handleButtonDown(self->button_statistics_, x, y);

        if (LISTBUTTON_isActived(self->button_query_) || LISTBUTTON_isActived(self->button_statistics_))
        {
            LISTBUTTON_SetStatus(self->button_insert_, false);
            self->status_ = LISTBUTTON_isActived(self->button_query_) ? SIDEBAR_BUTTON_QUERY : SIDEBAR_BUTTON_STATISTICS;
        }
    }
    else if (self->status_ == SIDEBAR_BUTTON_QUERY)
    {
        LISTBUTTON_handleButtonDown(self->button_insert_, x, y);
        LISTBUTTON_handleButtonDown(self->button_statistics_, x, y);

        if (LISTBUTTON_isActived(self->button_insert_) || LISTBUTTON_isActived(self->button_statistics_))
        {
            LISTBUTTON_SetStatus(self->button_query_, false);
            self->status_ = LISTBUTTON_isActived(self->button_insert_) ? SIDEBAR_BUTTON_INSERT : SIDEBAR_BUTTON_STATISTICS;
        }
    }
    else if (self->status_ == SIDEBAR_BUTTON_STATISTICS)
    {
        LISTBUTTON_handleButtonDown(self->button_insert_, x, y);
        LISTBUTTON_handleButtonDown(self->button_query_, x, y);

        if (LISTBUTTON_isActived(self->button_insert_) || LISTBUTTON_isActived(self->button_query_))
        {
            LISTBUTTON_SetStatus(self->button_statistics_, false);
            self->status_ = LISTBUTTON_isActived(self->button_insert_) ? SIDEBAR_BUTTON_INSERT : SIDEBAR_BUTTON_QUERY;
        }
    }
}

SidebarButtonType SIDEBAR_getStatus(Sidebar* self)
{
    return self->status_;
}

bool SIDEBAR_isExit(Sidebar* self)
{
    return self->is_exit_;
}

void SIDEBAR_destroy(Sidebar* self)
{
    LISTBUTTON_destroy(self->button_insert_);
    LISTBUTTON_destroy(self->button_query_);
    LISTBUTTON_destroy(self->button_statistics_);

    free(self);
}
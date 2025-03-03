#include "Sidebar.h"

static ListButton* SIDEBAR_getStatusButton(Sidebar* self)
{
    switch (self->status_)
    {
    case SIDEBAR_BUTTON_INSERTBOOK:
        return self->button_insert_book_;
    case SIDEBAR_BUTTON_INSERTUSER:
        return self->button_insert_user_;
    case SIDEBAR_BUTTON_QUERYBOOK:
        return self->button_query_book_;
    case SIDEBAR_BUTTON_QUERYUSER:
        return self->button_query_user_;
    case SIDEBAR_BUTTON_ACCOUNT:
        return self->button_account_;
    case SIDEBAR_BUTTON_STATISTICS:
        return self->button_statistics_;
    default:
        return NULL;
    }
}
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
    self->status_ = SIDEBAR_BUTTON_QUERYBOOK;

    // 由于不考虑窗口大小变化 硬编码大小参数
    int buttonHeight = 30;
    int spacing = 10;
    int padding = 10;
    int currentTop = top + spacing;

    int buttonWidth = right - left - 2 * padding;

    int buttonLeft = left + (right - left - buttonWidth) / 2;

    self->button_insert_book_ = LISTBUTTON_create(buttonLeft, currentTop, buttonLeft + buttonWidth, currentTop + buttonHeight, "添加图书",
        color, button_hover_color, button_active_color, font_color,
        false, WHITE, radius, padding);
    if (!self->button_insert_book_)
    {
        free(self);
        return NULL;
    }
    currentTop += buttonHeight + spacing;

    self->button_query_book_ = LISTBUTTON_create(buttonLeft, currentTop, buttonLeft + buttonWidth, currentTop + buttonHeight, "查询图书",
        color, button_hover_color, button_active_color, font_color,
        false, WHITE, radius, padding);
    if (!self->button_query_book_)
    {
        free(self->button_insert_book_);
        free(self);
        return NULL;
    }
    currentTop += buttonHeight + spacing;

    self->button_insert_user_ = LISTBUTTON_create(buttonLeft, currentTop, buttonLeft + buttonWidth, currentTop + buttonHeight, "添加借阅",
        color, button_hover_color, button_active_color, font_color,
        false, WHITE, radius, padding);
    if (!self->button_insert_user_)
    {
        free(self->button_insert_book_);
        free(self->button_query_book_);
        free(self);
        return NULL;
    }
    currentTop += buttonHeight + spacing;

    self->button_query_user_ = LISTBUTTON_create(buttonLeft, currentTop, buttonLeft + buttonWidth, currentTop + buttonHeight, "查询借阅",
        color, button_hover_color, button_active_color, font_color,
        false, WHITE, radius, padding);
    if (!self->button_query_user_)
    {
        free(self->button_insert_book_);
        free(self->button_query_book_);
        free(self->button_insert_user_);
        free(self);
        return NULL;
    }
    currentTop += buttonHeight + spacing;

    self->button_account_ = LISTBUTTON_create(buttonLeft, currentTop, buttonLeft + buttonWidth, currentTop + buttonHeight, "账户管理",
        color, button_hover_color, button_active_color, font_color,
        false, WHITE, radius, padding);
    if (!self->button_account_)
    {
        free(self->button_insert_book_);
        free(self->button_query_book_);
        free(self->button_insert_user_);
        free(self->button_query_user_);
        free(self);
        return NULL;
    }
    currentTop += buttonHeight + spacing;

    //self->button_statistics_ = LISTBUTTON_create(buttonLeft, currentTop, buttonLeft + buttonWidth, currentTop + buttonHeight, "统计",
    //    color, button_hover_color, button_active_color, font_color,
    //    false, WHITE, radius, padding);
    //if (!self->button_statistics_)
    //{
    //    free(self->button_insert_book_);
    //    free(self->button_query_book_);
    //    free(self->button_insert_user_);
    //    free(self->button_query_user_);
    //    free(self->button_account_);
    //    free(self);
    //    return NULL;
    //}
    //currentTop += buttonHeight + spacing;

    int exitButtonTop = bottom - spacing - buttonHeight;
    self->button_exit_ = LISTBUTTON_create(buttonLeft, exitButtonTop, buttonLeft + buttonWidth, exitButtonTop + buttonHeight, "退出",
        color, button_hover_color, button_active_color, font_color,
        false, WHITE, radius, padding);
    if (!self->button_exit_)
    {
        free(self->button_insert_book_);
        free(self->button_query_book_);
        free(self->button_insert_user_);
        free(self->button_query_user_);
        free(self->button_statistics_);
        free(self);
        return NULL;
    }

    self->is_exit_ = false;

    LISTBUTTON_SetStatus(self->button_insert_book_, true); // 设置初始激活状态 (Query)

    self->status_ = SIDEBAR_BUTTON_INSERTBOOK;

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
    if (self->is_active_)
    {
        // 下面的目的是为了实现一边平一边圆角的半圆角矩形
        // 一个小的矩形
        setfillcolor(self->color_);
        fillrect(self->rect_.left, self->rect_.top, self->rect_.right / 2, self->rect_.bottom);
        setcolor(self->color_);
        rectangle(self->rect_.left, self->rect_.top, self->rect_.right / 2, self->rect_.bottom);

        // 一个小的圆角矩形
        setfillcolor(self->color_);
        fillroundrect(self->rect_.left, self->rect_.top, self->rect_.right, self->rect_.bottom, 10, 10);
        setcolor(self->color_);
        roundrect(self->rect_.left, self->rect_.top, self->rect_.right, self->rect_.bottom, 10, 10);

        LISTBUTTON_draw(self->button_insert_book_);
        LISTBUTTON_draw(self->button_query_book_);
        LISTBUTTON_draw(self->button_insert_user_);
        LISTBUTTON_draw(self->button_query_user_);

        LISTBUTTON_draw(self->button_account_);

        //LISTBUTTON_draw(self->button_statistics_);
        LISTBUTTON_draw(self->button_exit_);
    }
}

void SIDEBAR_handleMouseMove(Sidebar* self, int x, int y)
{
    if (self->is_active_)
    {
        LISTBUTTON_handleMouseMove(self->button_insert_book_, x, y);
        LISTBUTTON_handleMouseMove(self->button_query_book_, x, y);
        LISTBUTTON_handleMouseMove(self->button_insert_user_, x, y);
        LISTBUTTON_handleMouseMove(self->button_query_user_, x, y);

        LISTBUTTON_handleMouseMove(self->button_account_, x, y);

        //LISTBUTTON_handleMouseMove(self->button_statistics_, x, y);

        LISTBUTTON_handleMouseMove(self->button_exit_, x, y);
    }
}

void SIDEBAR_handleButtonDown(Sidebar* self, int x, int y)
{
    if (self->is_active_)
    {
        SidebarButtonType status = self->status_;

        LISTBUTTON_handleButtonDown(self->button_insert_book_, x, y);
        if (LISTBUTTON_isActived(self->button_insert_book_) && self->status_ != SIDEBAR_BUTTON_INSERTBOOK)
        {
            status = SIDEBAR_BUTTON_INSERTBOOK;
        }

        LISTBUTTON_handleButtonDown(self->button_query_book_, x, y);
        if (LISTBUTTON_isActived(self->button_query_book_) && self->status_ != SIDEBAR_BUTTON_QUERYBOOK)
        {
            status = SIDEBAR_BUTTON_QUERYBOOK;
        }

        LISTBUTTON_handleButtonDown(self->button_insert_user_, x, y);
        if (LISTBUTTON_isActived(self->button_insert_user_) && self->status_ != SIDEBAR_BUTTON_INSERTUSER)
        {
            status = SIDEBAR_BUTTON_INSERTUSER;
        }

        LISTBUTTON_handleButtonDown(self->button_query_user_, x, y);
        if (LISTBUTTON_isActived(self->button_query_user_) && self->status_ != SIDEBAR_BUTTON_QUERYUSER)
        {
            status = SIDEBAR_BUTTON_QUERYUSER;
        }

        LISTBUTTON_handleButtonDown(self->button_account_, x, y);
        if (LISTBUTTON_isActived(self->button_account_) && self->status_ != SIDEBAR_BUTTON_ACCOUNT)
        {
            status = SIDEBAR_BUTTON_ACCOUNT;
        }

        //LISTBUTTON_handleButtonDown(self->button_statistics_, x, y);
        //if (LISTBUTTON_isActived(self->button_statistics_) && self->status_ != SIDEBAR_BUTTON_STATISTICS)
        //{
        //    status = SIDEBAR_BUTTON_STATISTICS;
        //}

        LISTBUTTON_handleButtonDown(self->button_exit_, x, y);
        if (LISTBUTTON_isActived(self->button_exit_))
        {
            self->is_exit_ = true;
        }

        if (status != self->status_)
        {
            LISTBUTTON_SetStatus(SIDEBAR_getStatusButton(self), false);
            self->status_ = status;
        }
    }
}

SidebarButtonType SIDEBAR_getStatus(Sidebar* self)
{
    return self->status_;
}

bool SIDEBAR_isActive(Sidebar* self)
{
    return self->is_active_;
}

void SIDEBAR_setActive(Sidebar* self, bool active)
{
    self->is_active_ = active;
}

void SIDEBAR_setStatus(Sidebar* self, SidebarButtonType status)
{
    LISTBUTTON_SetStatus(self->button_insert_book_, status == SIDEBAR_BUTTON_INSERTBOOK);
    LISTBUTTON_SetStatus(self->button_insert_user_, status == SIDEBAR_BUTTON_INSERTUSER);
    LISTBUTTON_SetStatus(self->button_query_book_, status == SIDEBAR_BUTTON_QUERYBOOK);
    LISTBUTTON_SetStatus(self->button_query_user_, status == SIDEBAR_BUTTON_QUERYUSER);
    LISTBUTTON_SetStatus(self->button_account_, status == SIDEBAR_BUTTON_ACCOUNT);
    //LISTBUTTON_SetStatus(self->button_statistics_, status == SIDEBAR_BUTTON_STATISTICS);
    self->status_ = status;
}

bool SIDEBAR_isExit(Sidebar* self)
{
    return self->is_exit_;
}

void SIDEBAR_destroy(Sidebar* self)
{
    LISTBUTTON_destroy(self->button_insert_book_);
    LISTBUTTON_destroy(self->button_insert_user_);
    LISTBUTTON_destroy(self->button_query_book_);
    LISTBUTTON_destroy(self->button_query_user_);

    LISTBUTTON_destroy(self->button_account_);

    //LISTBUTTON_destroy(self->button_statistics_);

    LISTBUTTON_destroy(self->button_exit_);

    free(self);
}
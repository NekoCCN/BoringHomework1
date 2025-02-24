#pragma once
#ifndef SIDEBAR_H
#define SIDEBAR_H
#include "../../Widget/ListButton/ListButton.h"
#include "../../Tools/Tools.h"

typedef enum Sidebar_Button_Type
{
    SIDEBAR_BUTTON_QUERYBOOK,
    SIDEBAR_BUTTON_INSERTBOOK,
    SIDEBAR_BUTTON_INSERTUSER,
    SIDEBAR_BUTTON_QUERYUSER,
    SIDEBAR_BUTTON_STATISTICS,
} SidebarButtonType;

typedef struct Sidebar
{
    // 这里的大小参数是Windows API风格的 即上下左右 实际上是不如统一为x, y与长宽的 不过它作为一个UI组成而不是控件（widget）问题不大
    RECT rect_;

    COLORREF color_;

    SidebarButtonType status_;

    ListButton* button_insert_book_;
    ListButton* button_query_book_;
    ListButton* button_insert_user_;
    ListButton* button_query_user_;

    ListButton* button_statistics_;

    ListButton* button_exit_;

    bool is_exit_;
} Sidebar;

Sidebar* SIDEBAR_create(int left, int top, int right, int bottom, COLORREF color,
    COLORREF font_color, COLORREF button_color, int radius);

Sidebar* SIDEBAR_createGraySidebar(int left, int top, int right, int bottom);

void SIDEBAR_draw(Sidebar* self);

void SIDEBAR_handleMouseMove(Sidebar* self, int x, int y);

void SIDEBAR_handleButtonDown(Sidebar* self, int x, int y);

SidebarButtonType SIDEBAR_getStatus(Sidebar* self);

void SIDEBAR_setStatus(Sidebar* self, SidebarButtonType status);

bool SIDEBAR_isExit(Sidebar* self);

void SIDEBAR_destroy(Sidebar* self);

#endif

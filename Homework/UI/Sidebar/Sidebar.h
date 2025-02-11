#pragma once
#ifndef SIDEBAR_H
#define SIDEBAR_H
#include "../../Widget/ListButtom/ListButtom.h"
#include "../../Tools/Tools.h"

typedef enum Sidebar_Button_Type
{
    SIDEBAR_BUTTON_QUERY,
    SIDEBAR_BUTTON_INSERT,
    SIDEBAR_BUTTON_STATISTICS
} SidebarButtonType;

typedef struct Sidebar
{
    RECT rect_;

    COLORREF color_;

    SidebarButtonType status_;

    ListButton* button_query_;
    ListButton* button_insert_;
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

bool SIDEBAR_isExit(Sidebar* self);

void SIDEBAR_destroy(Sidebar* self);

#endif

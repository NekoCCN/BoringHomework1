#pragma once
#ifndef MATERIALLIST_H
#define MATERIALLIST_H
#include <graphics.h>
#include <windows.h>
#include <stdbool.h>
#include "../../Data/StringSkipList/StringSkipList.h"
#include "../../Tools/Tools.h"

typedef struct MaterialListStruct
{
    int x;
    int y;
    int width;
    int height;
    int itemHeight;

    int scrollY;

    int hoverIndex;

    SearchResult* items;
    int itemCount;
    int visibleItems;
    int padding;
    bool isWheel;

    int selected_index_;

    HDC hdc;
    HRGN hRgn;
    HWND hWnd;

    // 颜色
    color_t backgroundColor;
    color_t itemBackgroundColor;
    color_t itemHoverColor;
    color_t selected_item_color_;
    color_t scrollBarColor;
    color_t textColor;
} MaterialList;

MaterialList* MATERIALLIST_createGrayMaterialList(int x, int y, int width, int height, int itemHeight, int padding);

MaterialList* MATERIALLIST_createBlueMaterialList(int x, int y, int width, int height, int itemHeight, int padding);

MaterialList* MATERIALLIST_create(int x, int y, int width, int height, int itemHeight, int padding,
    color_t backgroundColor, color_t itemBackgroundColor,
    color_t itemHoverColor, color_t selected_item_color, color_t scrollBarColor, color_t textColor);


void MATERIALLIST_destroy(MaterialList* list);

void MATERIALLIST_setData(MaterialList* list, SearchResult* items, int itemCount);

void MATERIALLIST_draw(MaterialList* list);

void MATERIALLIST_handleMouseMove(MaterialList* list, int mouseX, int mouseY);

void MATERIALLIST_handleMouseWheel(MaterialList* list, int wheel);

void MATERIALLIST_handleMouseDown(MaterialList* list, int mouseX, int mouseY);

void MATERIALLIST_clearData(MaterialList* list);

bool MATERIALLIST_isSelected(MaterialList* list);

SearchResult MATERIALLIST_getSelectedItem(MaterialList* list);

#endif // MATERIALLIST_H

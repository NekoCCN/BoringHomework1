#pragma once
#ifndef MATERIALLIST_H
#define MATERIALLIST_H
#include <graphics.h>
#include <windows.h>
#include <stdbool.h>
#include "../../Data/StringSkipList/StringSkipList.h"
#include "../../Tools/Tools.h"


typedef struct MaterialListDataStruct
{
    uint32_t UUID_;  // 预留做唯一标识符
    wchar_t* key_;

    char* first_line_;
    char* second_line_;

    color_t first_line_color_;
    color_t second_line_color_;
} MaterialListData;

typedef struct MaterialListStruct
{
    int x;
    int y;
    int width;
    int height;
    int item_height_;

    int scrollY_;

    int hover_index_;

    MaterialListData* items_;
    int item_count_;
    int visible_items_;
    int padding_;
    bool is_wheel_;

    int selected_index_;

    // 实际的说 引入Windows 原生API 裁剪区是一个不好的做法 但是为了解决列表项目的偏移问题 似乎是唯一解
    HDC hdc;
    HRGN hRgn;
    HWND hWnd;

    // 颜色
    color_t background_color_;
    color_t item_background_color_;
    color_t item_hover_color_;
    color_t selected_item_color_;
    color_t scroll_bar_color_;
} MaterialList;

MaterialList* MATERIALLIST_createGrayMaterialList(int x, int y, int width, int height, int itemHeight, int padding);

MaterialList* MATERIALLIST_createBlueMaterialList(int x, int y, int width, int height, int itemHeight, int padding);

MaterialList* MATERIALLIST_create(int x, int y, int width, int height, int itemHeight, int padding,
    color_t background_color, color_t item_background_color,
    color_t item_hover_color, color_t selected_item_color, color_t scroll_bar_color);


void MATERIALLIST_destroy(MaterialList* list);

void MATERIALLIST_setData(MaterialList* list, MaterialListData* items, int itemCount);

void MATERIALLIST_draw(MaterialList* list);

void MATERIALLIST_handleMouseMove(MaterialList* list, int mouseX, int mouseY);

void MATERIALLIST_handleMouseWheel(MaterialList* list, int wheel);

void MATERIALLIST_handleMouseDown(MaterialList* list, int mouseX, int mouseY);

void MATERIALLIST_clearData(MaterialList* list);

bool MATERIALLIST_isSelected(MaterialList* list);

MaterialListData MATERIALLIST_getSelectedItem(MaterialList* list);

#endif // MATERIALLIST_H

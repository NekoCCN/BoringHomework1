#include "MaterialList.h"
#include <stdio.h>
#include <algorithm>

MaterialList* MATERIALLIST_create(int x, int y, int width, int height, int item_height_, int padding,
    color_t background_color, color_t item_background_color,
    color_t item_hover_color, color_t selected_item_color, color_t scroll_bar_color)
{
    MaterialList* list = (MaterialList*)malloc(sizeof(MaterialList));
    if (list == NULL)
    {
        return NULL;
    }

    list->x = x;
    list->y = y;
    list->width = width;
    list->height = height;

    list->item_height_ = item_height_;

    list->padding_ = padding;

    list->scrollY_ = 0;

    list->hover_index_ = -1;
    list->selected_index_ = -1;

    list->items_ = NULL;
    list->item_count_ = 0;

    list->is_wheel_ = false;
    list->visible_items_ = height / item_height_;

    list->hdc = getHDC();
    list->hWnd = getHWnd();
    list->hRgn = CreateRectRgn(list->x, list->y, list->x + list->width, list->y + list->height);

    list->background_color_ = background_color;
    list->item_background_color_ = item_background_color;
    list->item_hover_color_ = item_hover_color;
    list->selected_item_color_ = selected_item_color;
    list->scroll_bar_color_ = scroll_bar_color;

    return list;
}
MaterialList* MATERIALLIST_createGrayMaterialList(int x, int y, int width, int height, int item_height_, int padding)
{
    return MATERIALLIST_create(x, y, width, height, item_height_, padding,
        hexToColorref("#F5F5F5"), hexToColorref("#F5F5F5"),
        hexToColorref("#E0E0E0"), hexToColorref("#C0C0C0"), hexToColorref("#808080"));
}

MaterialList* MATERIALLIST_createBlueMaterialList(int x, int y, int width, int height, int item_height_, int padding)
{
    return MATERIALLIST_create(x, y, width, height, item_height_, padding,
        hexToColorref("#2196F3"), hexToColorref("#FFFFFF"),
        hexToColorref("#1976D2"), hexToColorref("#90CAF9"), hexToColorref("#64B5F6"));
}
void MATERIALLIST_destroy(MaterialList* list)
{
    if (list)
    {
        MATERIALLIST_clearData(list);

        SelectClipRgn(list->hdc, NULL);
        DeleteObject(list->hRgn);
        ReleaseDC(list->hWnd, list->hdc);

        free(list);
    }
}

void MATERIALLIST_clearData(MaterialList* list)
{
    if (list->items_ != NULL)
    {
        for (int i = 0; i < list->item_count_; i++)
        {
            if (list->items_[i].first_line_ != NULL)
            {
                free(list->items_[i].first_line_);
            }
            if (list->items_[i].second_line_ != NULL)
            {
                free(list->items_[i].second_line_);
            }

            if (list->items_[i].key_ != NULL)
            {
                free(list->items_[i].key_);
            }
        }
        free(list->items_);
        list->items_ = NULL;
        list->item_count_ = 0;
    }
}

// 需要注意：此函数会移交内存所有权 不应该再继续使用items
void MATERIALLIST_setData(MaterialList* list, MaterialListData* items, int item_count)
{
    MATERIALLIST_clearData(list);
    list->items_ = items;
    list->item_count_ = item_count;
    list->visible_items_ = list->height / list->item_height_;
    list->scrollY_ = 0;

    list->selected_index_ = -1;
    if (list->hRgn)
    {
        DeleteObject(list->hRgn);
    }
    list->hRgn = CreateRectRgn(list->x, list->y, list->x + list->width, list->y + list->height);
}

static void MATERIALLIST_drawItem(MaterialList* list, int index, int y, bool isHovered)
{
    color_t bgColor;
    if (index == list->selected_index_)
    {
        bgColor = list->selected_item_color_;
    }
    else if (isHovered)
    {
        bgColor = list->item_hover_color_;
    }
    else
    {
        bgColor = list->item_background_color_;
    }

    setfillcolor(bgColor);
    int x = list->x + list->padding_;
    int itemWidth = list->width - 2 * list->padding_;

    fillroundrect(x, y, x + itemWidth, y + list->item_height_, 10, 10);
    setcolor(bgColor);
    roundrect(x, y, x + itemWidth, y + list->item_height_, 10, 10);

    setcolor(list->items_[index].first_line_color_);
    outtextxy(x + list->padding_, y + list->padding_, list->items_[index].first_line_);

    setcolor(list->items_[index].second_line_color_);
    outtextxy(x + list->padding_, y + list->padding_ + 20, list->items_[index].second_line_);
}

void MATERIALLIST_draw(MaterialList* list)
{
    setfillcolor(list->background_color_);
    fillroundrect(list->x, list->y, list->x + list->width, list->y + list->height, 10, 10); // 10 is a example of CORNER_RADIUS

    setcolor(list->background_color_);
    roundrect(list->x, list->y, list->x + list->width, list->y + list->height, 10, 10);

    SelectClipRgn(list->hdc, list->hRgn);

    int startIndex = list->scrollY_ / list->item_height_;
    int endIndex = startIndex + list->visible_items_;
    endIndex = min(endIndex, list->item_count_);

    for (int i = startIndex; i < endIndex; i++)
    {
        int y = list->y + (i - startIndex) * list->item_height_ - (list->scrollY_ % list->item_height_) + list->padding_;
        bool isHovered = (i == list->hover_index_);
        MATERIALLIST_drawItem(list, i, y, isHovered);
    }

    if (list->item_count_ > list->visible_items_)
    {
        int scrollBarHeight = (list->visible_items_ * list->height) / list->item_count_;
        int scrollBarY = list->y + (list->scrollY_ * list->height) / (list->item_count_ * list->item_height_);

        setfillcolor(list->scroll_bar_color_);
        fillroundrect(list->x + list->width - 8, scrollBarY + 4, list->x + list->width - 4, scrollBarY + scrollBarHeight - 4, 4, 4);
    }
    SelectClipRgn(list->hdc, NULL);
}
void MATERIALLIST_handleMouseMove(MaterialList* list, int mouseX, int mouseY)
{
    if (mouseX >= list->x && mouseX < list->x + list->width && mouseY >= list->y && mouseY < list->y + list->height)
    {
        int itemIndex = (mouseY - list->y + list->scrollY_) / list->item_height_;
        if (itemIndex >= 0 && itemIndex < list->item_count_)
        {
            list->hover_index_ = itemIndex;
        }
        else
        {
            list->hover_index_ = -1;
        }
    }
    else
    {
        list->hover_index_ = -1;
    }
}

void MATERIALLIST_handleMouseWheel(MaterialList* list, int wheel)
{
    list->scrollY_ -= wheel / 10;
    list->scrollY_ = max(list->scrollY_, 0);
    int maxScrollY = (list->item_count_ - list->visible_items_) * list->item_height_;
    maxScrollY = max(maxScrollY, 0);
    list->scrollY_ = min(list->scrollY_, maxScrollY);

    list->is_wheel_ = (wheel != 0);
}

void MATERIALLIST_handleMouseDown(MaterialList* list, int mouseX, int mouseY)
{
    if (list->hover_index_ != -1)
    {
        list->selected_index_ = list->hover_index_;
    }
}

bool MATERIALLIST_isSelected(MaterialList* list)
{
    if (list->selected_index_ == -1)
    {
        return false;
    }
    return true;
}

MaterialListData MATERIALLIST_getSelectedItem(MaterialList* list)
{
    return list->items_[list->selected_index_];
}
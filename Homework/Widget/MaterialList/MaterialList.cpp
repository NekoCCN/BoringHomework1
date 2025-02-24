#include "MaterialList.h"
#include <stdio.h>
#include <algorithm>

MaterialList* MATERIALLIST_create(int x, int y, int width, int height, int itemHeight, int padding,
    color_t backgroundColor, color_t itemBackgroundColor,
    color_t itemHoverColor, color_t selected_item_color, color_t scrollBarColor, color_t textColor)
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

    list->itemHeight = itemHeight;

    list->padding = padding;

    list->scrollY = 0;

    list->hoverIndex = -1;
    list->selected_index_ = -1;

    list->items = NULL;
    list->itemCount = 0;

    list->isWheel = false;
    list->visibleItems = height / itemHeight;

    list->hdc = getHDC();
    list->hWnd = getHWnd();
    list->hRgn = CreateRectRgn(list->x, list->y, list->x + list->width, list->y + list->height);

    list->backgroundColor = backgroundColor;
    list->itemBackgroundColor = itemBackgroundColor;
    list->itemHoverColor = itemHoverColor;
    list->selected_item_color_ = selected_item_color;
    list->scrollBarColor = scrollBarColor;
    list->textColor = textColor;

    return list;
}
MaterialList* MATERIALLIST_createGrayMaterialList(int x, int y, int width, int height, int itemHeight, int padding)
{
    return MATERIALLIST_create(x, y, width, height, itemHeight, padding,
        hexToColorref("#F5F5F5"), hexToColorref("#F5F5F5"),
        hexToColorref("#E0E0E0"), hexToColorref("#C0C0C0"), hexToColorref("#808080"), hexToColorref("#000000"));
}

MaterialList* MATERIALLIST_createBlueMaterialList(int x, int y, int width, int height, int itemHeight, int padding)
{
    return MATERIALLIST_create(x, y, width, height, itemHeight, padding,
        hexToColorref("#2196F3"), hexToColorref("#FFFFFF"),
        hexToColorref("#1976D2"), hexToColorref("#90CAF9"), hexToColorref("#64B5F6"), hexToColorref("#000000"));
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
    if (list->items != NULL)
    {
        for (int i = 0; i < list->itemCount; i++)
        {
            free(list->items[i].key);
        }
        free(list->items);
        list->items = NULL;
        list->itemCount = 0;
    }
}

void MATERIALLIST_setData(MaterialList* list, SearchResult* items, int itemCount)
{
    MATERIALLIST_clearData(list);
    list->items = items;
    list->itemCount = itemCount;
    list->visibleItems = list->height / list->itemHeight;
    list->scrollY = 0;

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
        bgColor = list->itemHoverColor;
    }
    else
    {
        bgColor = list->itemBackgroundColor;
    }

    setfillcolor(bgColor);
    int x = list->x + list->padding;
    int itemWidth = list->width - 2 * list->padding;

    fillroundrect(x, y, x + itemWidth, y + list->itemHeight, 10, 10);
    setcolor(bgColor);
    roundrect(x, y, x + itemWidth, y + list->itemHeight, 10, 10);

    setcolor(list->textColor);
    setfont(16, 0, "Arial");

    char buffer[256];
    SearchResult* item = &(list->items[index]);

    sprintf(buffer, "ID: %u, Name: %ls", item->value.id, item->key);
    outtextxy(x + list->padding, y + list->padding, buffer);

    sprintf(buffer, "Course: %ls, Daily: %lld, Final: %lld, DailyScoreRatio: %.5f, Total: %lld",
        item->value.course, item->value.daily_score, item->value.final_score, item->value.daily_score_ratio, item->value.final_score);
    outtextxy(x + list->padding, y + list->padding + 20, buffer);
}

void MATERIALLIST_draw(MaterialList* list)
{
    setfillcolor(list->backgroundColor);
    fillroundrect(list->x, list->y, list->x + list->width, list->y + list->height, 10, 10); // 10 is a example of CORNER_RADIUS

    setcolor(list->backgroundColor);
    roundrect(list->x, list->y, list->x + list->width, list->y + list->height, 10, 10);

    SelectClipRgn(list->hdc, list->hRgn);

    int startIndex = list->scrollY / list->itemHeight;
    int endIndex = startIndex + list->visibleItems;
    endIndex = min(endIndex, list->itemCount);

    for (int i = startIndex; i < endIndex; i++)
    {
        int y = list->y + (i - startIndex) * list->itemHeight - (list->scrollY % list->itemHeight) + list->padding;
        bool isHovered = (i == list->hoverIndex);
        MATERIALLIST_drawItem(list, i, y, isHovered);
    }

    if (list->itemCount > list->visibleItems)
    {
        int scrollBarHeight = (list->visibleItems * list->height) / list->itemCount;
        int scrollBarY = list->y + (list->scrollY * list->height) / (list->itemCount * list->itemHeight);

        setfillcolor(list->scrollBarColor);
        fillroundrect(list->x + list->width - 8, scrollBarY + 4, list->x + list->width - 4, scrollBarY + scrollBarHeight - 4, 4, 4);
    }
    SelectClipRgn(list->hdc, NULL);
}
void MATERIALLIST_handleMouseMove(MaterialList* list, int mouseX, int mouseY)
{
    if (mouseX >= list->x && mouseX < list->x + list->width && mouseY >= list->y && mouseY < list->y + list->height)
    {
        int itemIndex = (mouseY - list->y + list->scrollY) / list->itemHeight;
        if (itemIndex >= 0 && itemIndex < list->itemCount)
        {
            list->hoverIndex = itemIndex;
        }
        else
        {
            list->hoverIndex = -1;
        }
    }
    else
    {
        list->hoverIndex = -1;
    }
}

void MATERIALLIST_handleMouseWheel(MaterialList* list, int wheel)
{
    list->scrollY -= wheel / 10;
    list->scrollY = max(list->scrollY, 0);
    int maxScrollY = (list->itemCount - list->visibleItems) * list->itemHeight;
    maxScrollY = max(maxScrollY, 0);
    list->scrollY = min(list->scrollY, maxScrollY);

    list->isWheel = (wheel != 0);
}

void MATERIALLIST_handleMouseDown(MaterialList* list, int mouseX, int mouseY)
{
    if (list->hoverIndex != -1)
    {
        list->selected_index_ = list->hoverIndex;
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

SearchResult MATERIALLIST_getSelectedItem(MaterialList* list)
{
    return list->items[list->selected_index_];
}
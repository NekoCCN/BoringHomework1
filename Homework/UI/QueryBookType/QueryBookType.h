#pragma once
#ifndef QUERYTYPE_H
#define QUERYTYPE_H
#include "../../Widget/ListButton/ListButton.h"
#include "../../Tools/Tools.h"

typedef enum QueryType_Button_Type
{
    QUERYBOOK_TYPE_BUTTON_PREFIX,
    QUERYBOOK_TYPE_BUTTON_ENTIRE,

    QUERYBOOK_TYPE_BUTTON_ISBN,
    QUERYBOOK_TYPE_BUTTON_FUZZY,
} QueryTypeButtonType;

typedef struct QueryTypeStruct
{
    int x_;
    int y_;
    int width_;
    int height_;

    COLORREF color_;

    ListButton* button_prefix_;
    ListButton* button_entire_;

    ListButton* button_isbn_;
    ListButton* button_fuzzy_;

    QueryTypeButtonType status_;
} QueryType;

QueryType* QUERYTYPE_create(int x, int y, int width, int radius);

void QUERYTYPE_draw(QueryType* self);

void QUERYTYPE_handleMouseMove(QueryType* self, int x, int y);

void QUERYTYPE_handleButtonDown(QueryType* self, int x, int y);

QueryTypeButtonType QUERYTYPE_getStatus(QueryType* self);

void QUERYTYPE_destroy(QueryType* self);

#endif
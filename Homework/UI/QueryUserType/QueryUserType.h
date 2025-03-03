#pragma once
#ifndef QUERYUSERTYPE_H
#define QUERYUSERTYPE_H
#include "../../Widget/ListButton/ListButton.h"
#include "../../Tools/Tools.h"

typedef enum QueryUserType_Button_Type
{
    QUERYUSER_TYPE_BUTTON_ENTIRE,
    QUERYUSER_TYPE_BUTTON_ID,
    QUERYUSER_TYPE_BUTTON_BOOKNAME_ENTIRE,
    QUERYUSER_TYPE_TIME,
    QUERYUSER_TYPE_REGEX
} QueryUserTypeButtonType;

typedef struct QueryUserTypeStruct
{
    int x_;
    int y_;
    int width_;
    int height_;

    COLORREF color_;

    ListButton* button_bookname_entire_;
    ListButton* button_entire_;

    ListButton* button_id_;
    ListButton* button_time_;

    ListButton* button_regex_;

    QueryUserTypeButtonType status_;
} QueryUserType;

QueryUserType* QUERYUSERTYPE_create(int x, int y, int width, int radius);

void QUERYUSERTYPE_draw(QueryUserType* self);

void QUERYUSERTYPE_handleMouseMove(QueryUserType* self, int x, int y);

void QUERYUSERTYPE_handleButtonDown(QueryUserType* self, int x, int y);

QueryUserTypeButtonType QUERYUSERTYPE_getStatus(QueryUserType* self);

void QUERYUSERTYPE_destroy(QueryUserType* self);

#endif
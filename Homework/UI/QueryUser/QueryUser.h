#pragma once
#ifndef QUERYUSER_H
#define QUERYUSER_H
#include <stdbool.h>
#include "../../Widget/MaterialInput/MaterialInput.h"
#include "../../Widget/MaterialList/MaterialList.h"
#include "../../Widget/Button/Button.h"
#include "../../Data/StringSkipList/StringSkipList.h"
#include "../QueryUserType/QueryUserType.h"
#include "../../Data/UDVector/UDVector.h"
#include "../../UI/InsertUser/InsertUser.h"
#include "../../Tools/Regex.hpp"
#include "../../Tools/Tools.h"
#include "../../UI/Sidebar/Sidebar.h"
#include "../../UI/Login/Login.h"
#include "../../Tools/Date.h"
#include <wchar.h>
#include <wctype.h>

typedef struct QueryUserStruct
{
    MaterialInput* input_;
    Button* search_;
    MaterialList* list_;

    list* data_;

    // 避嫌C++关键字delete
    Button* del_;
    Button* update_;

    QueryUserType* type_;

    StringSkipList* book_data_;

    UDVector* buf_;

    LoginStatus status_;

    bool is_active_;


} QueryUser;


QueryUser* QUERYUSER_createGrey(int x, int y, int width, int height, list* data, StringSkipList* book_data, LoginStatus status);

void QUERYUSER_draw(QueryUser* self);

void QUERYUSER_handleMouseMove(QueryUser* self, int x, int y);

void QUERYUSER_handleButtonDown(QueryUser* self, int x, int y, InsertUser* insert, Sidebar* sidebar);

void QUERYUSER_handleMouseUp(QueryUser* self, int x, int y);

bool QUERYUSER_isActived(QueryUser* self);

void QUERYUSER_SetStatus(QueryUser* self, bool status);

void QUERYUSER_setLoginStatus(QueryUser* self, LoginStatus status);

void QUERYUSER_destroy(QueryUser* self);

void QUERYUSER_handleMouseWheel(QueryUser* self, int wheel);

void QUERYUSER_handleInput(QueryUser* self, key_msg msg);

#endif // QUERYUSER_H

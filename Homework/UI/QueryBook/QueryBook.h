#pragma once
#ifndef QUERYBOOK_H
#define QUERYBOOK_H
#include <stdbool.h>
#include "../../Widget/MaterialInput/MaterialInput.h"
#include "../../Widget/MaterialList/MaterialList.h"
#include "../../Widget/Button/Button.h"
#include "../../Data/StringSkipList/StringSkipList.h"
#include "../QueryBookType/QueryBookType.h"
#include "../../Data/SRVector/SRVector.h"
#include "../../UI/InsertBook/InsertBook.h"
#include "../../UI/InsertUser/InsertUser.h"
#include "../../UI/Sidebar/Sidebar.h"
#include "../../UI/Login/Login.h"
#include <wchar.h>
#include <wctype.h>

typedef struct QueryStruct
{
    MaterialInput* input_;
    Button* search_;
    MaterialList* list_;

    StringSkipList* data_;

    // 避嫌C++关键字delete
    Button* del_;
    Button* update_;

    Button* set_can_rent_;
    Button* rent_;

    QueryType* type_;

    SRVector* buf_;

    LoginStatus status_;

    bool is_active_;


} Query;


// 仍然抛弃window api的坐标系表示
Query* QUERYBOOK_createGrey(int x, int y, int width, int height, StringSkipList* data_, LoginStatus status);

void QUERYBOOK_draw(Query* self);

void QUERYBOOK_handleMouseMove(Query* self, int x, int y);

void QUERYBOOK_handleButtonDown(Query* self, int x, int y, InsertBook* insert, Sidebar* sidebar, InsertUser* insert_user);

void QUERYBOOK_handleMouseUp(Query* self, int x, int y);

bool QUERYBOOK_isActived(Query* self);

void QUERYBOOK_SetStatus(Query* self, bool status);

void QUERYBOOK_setLoginStatus(Query* self, LoginStatus status);

void QUERYBOOK_destroy(Query* self);

void QUERYBOOK_handleMouseWheel(Query* self, int wheel);

void QUERYBOOK_handleInput(Query* self, key_msg msg);

#endif // QUERYBOOK_H

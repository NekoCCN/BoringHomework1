#pragma once
#ifndef INSERTUSER_H
#define INSERTUSER_H
#include "../../Widget/MaterialInput/MaterialInput.h"
#include "../../Data/UserList/UserList.h"
#include "../../Widget/Button/Button.h"
#include "../../Data/StringSkipList/StringSkipList.h"
#include "../../Tools/Date.h"
#include "../../UI/Login/Login.h"

typedef struct InsertUserStruct
{
	int x;
	int y;
	int width;
	int height;

    MaterialInput* input_name_;
    MaterialInput* input_id_;
    MaterialInput* input_book_name_;

	MaterialInput* input_day_time_;

	Button* button_insert_;

	list* data_;

	StringSkipList* book_data_;

	LoginStatus status_;

	bool is_active_;
} InsertUser;

InsertUser* INSERTUSER_createGrey(int x, int y, int width, int height, list* data_, StringSkipList* book_data);

void INSERTUSER_draw(InsertUser* self);

void INSERTUSER_handleMouseMove(InsertUser* self, int x, int y);

void INSERTUSER_handleMouseDown(InsertUser* self, int x, int y);

void INSERTUSER_handleMouseUp(InsertUser* self, int x, int y);

bool INSERTUSER_isActived(InsertUser* self);

void INSERTUSER_setLoginStatus(InsertUser* self, LoginStatus status);

void INSERTUSER_SetStatus(InsertUser* self, bool status);

void INSERTUSER_destroy(InsertUser* self);

void INSERTUSER_handleInput(InsertUser* self, key_msg msg);

#endif // INSERTUSER_H

#pragma once
#ifndef INSERTBOOK_H
#define INSERTBOOK_H
#include "../../Widget/MaterialInput/MaterialInput.h"
#include "../../Data/StringSkipList/StringSkipList.h"
#include "../../Widget/Button/Button.h"
#include "../../UI/Login/Login.h"

typedef struct InsertBookStruct
{
	int x;
	int y;
	int width;
	int height;

    MaterialInput* input_name_;
    MaterialInput* input_ISBN_;
    MaterialInput* input_catagory_;

	MaterialInput* input_publisher_;
    MaterialInput* input_num_;

	MaterialInput* input_image_path_;

	Button* button_insert_;

	StringSkipList* data_;

	LoginStatus status_;

	bool is_rental_;

	bool is_update_;
	int ori_diff_;
	wchar_t* update_key_;

	bool is_active_;
} InsertBook;

InsertBook* INSERTBOOK_createGrey(int x, int y, int width, int height, StringSkipList* data_);

void INSERTBOOK_draw(InsertBook* self);

void INSERTBOOK_handleMouseMove(InsertBook* self, int x, int y);

void INSERTBOOK_handleMouseDown(InsertBook* self, int x, int y);

void INSERTBOOK_handleMouseUp(InsertBook* self, int x, int y);

bool INSERTBOOK_isActived(InsertBook* self);

void INSERTBOOK_SetStatus(InsertBook* self, bool status);

void INSERTBOOK_setLoginStatus(InsertBook* self, LoginStatus status);

void INSERTBOOK_destroy(InsertBook* self);

void INSERTBOOK_handleInput(InsertBook* self, key_msg msg);

#endif // INSERTBOOK_H

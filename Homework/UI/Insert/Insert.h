#pragma once
#ifndef INSERT_H
#define INSERT_H
#include "../../Widget/MaterialInput/MaterialInput.h"
#include "../../Data/StringSkipList/StringSkipList.h"
#include "../../Widget/Button/Button.h"

typedef struct InsertStruct
{
	int x;
	int y;
	int width;
	int height;

    MaterialInput* input_name_;
    MaterialInput* input_id_;
    MaterialInput* input_course_;

	MaterialInput* input_daily_score_ratio_;
    MaterialInput* input_daily_score_;
    MaterialInput* input_final_score_;

	Button* button_insert_;

	StringSkipList* data_;

	bool is_active_;
} Insert;

Insert* INSERT_createGrey(int x, int y, int width, int height, StringSkipList* data_);

void INSERT_draw(Insert* self);

void INSERT_handleMouseMove(Insert* self, int x, int y);

void INSERT_handleMouseDown(Insert* self, int x, int y);

void INSERT_handleMouseUp(Insert* self, int x, int y);

bool INSERT_isActived(Insert* self);

void INSERT_SetStatus(Insert* self, bool status);

void INSERT_destroy(Insert* self);

void INSERT_handleInput(Insert* self, key_msg msg);

#endif // INSERT_H

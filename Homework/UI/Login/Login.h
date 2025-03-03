#pragma once
#ifndef LOGIN_H
#define LOGIN_H
#include <stdbool.h>
#include "../../Widget/MaterialInput/MaterialInput.h"
#include "../../Widget/Button/Button.h"
#include "../../DataStream/DataWStream/DataWStream.h"
#include "../../Tools/Tools.h"

enum LoginStatus
{
	LOGIN_STATUS_GUEST,
	LOGIN_STATUS_ADMIN,
	LOGIN_STATUS_ERROR,
};
typedef struct LoginStruct
{
	int x_;
	int y_;
	int width_;
	int height_;

	User* user_;
	uint32_t user_num_;

	MaterialInput* username_;
	MaterialInput* password_;

	Button* login_;
	Button* guest_;

	Button* exit_;

	LoginStatus login_status_;

	bool is_done_;

	bool will_exit_;

	bool is_active_;
} Login;

// 这里会移交user所有权 但不会处理 比如删除
Login* LOGIN_createGrey(int x, int y, int width, int height, User* user, uint32_t user_num);

void LOGIN_draw(Login* self);

bool LOGIN_isExit(Login* self);

void LOGIN_handleMouseMove(Login* self, int x, int y);

void LOGIN_handleButtonDown(Login* self, int x, int y);

void LOGIN_handleMouseUp(Login* self, int x, int y);

bool LOGIN_isActived(Login* self);

void LOGIN_setActivedStatus(Login* self, bool status);

LoginStatus LOGIN_getLoginStatus(Login* self);

bool LOGIN_isDone(Login* self);

void LOGIN_destroy(Login* self);

void LOGIN_handleInput(Login* self, key_msg msg);

#endif // LOGIN_H

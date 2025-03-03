#pragma once
#ifndef ADMINUSER_H
#define ADMINUSER_H
#include "../../DataStream/DataWStream/DataWStream.h"
#include "../../Widget/Button/Button.h"
#include "../../Widget/MaterialList/MaterialList.h"
#include "../../Widget/MaterialInput/MaterialInput.h"
#include "../../UI/Login/Login.h"

typedef struct AdminUserStruct
{
	Button* update_;
	Button* del_;
	Button* add_;

	MaterialInput* input_username_;
	MaterialInput* input_password_;

	MaterialList* list_;

	uint32_t user_num_;
	User* user_;

	bool is_active_;

	LoginStatus status_;
} AdminUser;

AdminUser* ADMINUSER_createGrey(int x, int y, int width, int height, User* user, uint32_t num, LoginStatus status);

void ADMINUSER_draw(AdminUser* self);

void ADMINUSER_handleMouseMove(AdminUser* self, int x, int y);

void ADMINUSER_handleMouseDown(AdminUser* self, int x, int y);

void ADMINUSER_handleMouseUp(AdminUser* self, int x, int y);

bool ADMINUSER_isActived(AdminUser* self);

void ADMINUSER_SetStatus(AdminUser* self, bool status);

void ADMINUSER_setLoginStatus(AdminUser* self, LoginStatus status);

void ADMINUSER_destroy(AdminUser* self);

void ADMINUSER_handleInput(AdminUser* self, key_msg msg);

void ADMINUSER_handleMouseWheel(AdminUser* self, int wheel);

#endif
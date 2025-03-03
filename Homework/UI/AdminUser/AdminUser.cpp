#include "AdminUser.h"

static MaterialListData* adminUserToMaterialInputData(User* user, uint32_t num)
{
    MaterialListData* newdata = (MaterialListData*)malloc(sizeof(MaterialListData) * num);
    char buffer[512];

    uint64_t index = 0;

    for (int i = 0; i < num; ++i)
    {
        newdata[i].UUID_ = index;
        ++index;

        sprintf(buffer, "用户名: %ls", user[i].name);

        // 可以使用宽字符串函数wcscup精简
        newdata[i].first_line_ = (char*)malloc(strlen(buffer) + 1);
        strcpy(newdata[i].first_line_, buffer);

        newdata[i].UUID_ = i;

        newdata[i].key_ = NULL;

        sprintf(buffer, "密码Hash：%d", user[i].password_hash);
        newdata[i].second_line_ = (char*)malloc(strlen(buffer) + 1);
        strcpy(newdata[i].second_line_, buffer);

        newdata[i].first_line_color_ = BLACK;
        newdata[i].second_line_color_ = BLACK;
    }

    return newdata;
}
// 关于User 不会进行复制！！！
AdminUser* ADMINUSER_createGrey(int x, int y, int width, int height, User* user, uint32_t num, LoginStatus status)
{
    AdminUser* self = (AdminUser*)malloc(sizeof(AdminUser));

    self->input_username_ = MATERIALINPUT_createGrayInput(x + 30, y + height / 16, width / 1.4f, height / 14, L"用户名");
    self->input_password_ = MATERIALINPUT_createGrayInput(x + 30, y + height / 16 + height / 14 + 10, width / 1.4f, height / 14, L"密码");
    
    self->list_ = MATERIALLIST_createGrayMaterialList(x + 30, y + height / 6 + height / 14 + 10, width - 50,
        height - height / 5 - height / 14 - 10, 60, 16);

    self->add_ = BUTTON_createBlueButton((x + 30 + width / 2) + 220, y + height / 10 - 24, width / 10, height / 16, L"添加");

    self->update_ = BUTTON_createBlueButton(x + 30 + width / 2 + 330, y + height / 10 - 24, width / 10, height / 16, L"更新");
    self->del_ = BUTTON_createBlueButton(x + 30 + width / 2 + 330, y + height / 10 + 30, width / 10, height / 16, L"删除");

    self->user_ = user;
    self->user_num_ = num;

    self->is_active_ = false;

    int size = 0;
    MATERIALLIST_setData(self->list_, adminUserToMaterialInputData(user, num), num);

    return self;
}

void ADMINUSER_draw(AdminUser* self)
{
    if (self->is_active_)
    {
        MATERIALINPUT_draw(self->input_username_);
        MATERIALINPUT_draw(self->input_password_);

        if (self->status_ == LOGIN_STATUS_ADMIN)
        {
            MATERIALLIST_draw(self->list_);
            BUTTON_draw(self->add_);
            BUTTON_draw(self->update_);
            BUTTON_draw(self->del_);
        }
    }
}

void ADMINUSER_handleMouseMove(AdminUser* self, int x, int y)
{
    if (self->is_active_)
    {
        MATERIALINPUT_handleMouseMove(self->input_username_, x, y);
        MATERIALINPUT_handleMouseMove(self->input_password_, x, y);

        if (self->status_ == LOGIN_STATUS_ADMIN)
        {
            MATERIALLIST_handleMouseMove(self->list_, x, y);
            BUTTON_handleMouseMove(self->add_, x, y);
            BUTTON_handleMouseMove(self->update_, x, y);
            BUTTON_handleMouseMove(self->del_, x, y);
        }
    }
}

void ADMINUSER_handleMouseDown(AdminUser* self, int x, int y)
{
    if (self->is_active_)
    {
        MATERIALINPUT_handleMouseDown(self->input_username_, x, y);
        MATERIALINPUT_handleMouseDown(self->input_password_, x, y);

        if (self->status_ == LOGIN_STATUS_ADMIN)
        {
            MATERIALLIST_handleMouseDown(self->list_, x, y);
            BUTTON_handleMouseDown(self->add_, x, y);
            BUTTON_handleMouseDown(self->update_, x, y);
            BUTTON_handleMouseDown(self->del_, x, y);
        }

        if (BUTTON_isClicked(self->add_))
        {
            int index = -1;
            for (int i = 0; i < self->user_num_; ++i)
            {
                if (wcscmp(self->user_[i].name, MATERIALINPUT_getText(self->input_username_)) == 0)
                {
                    MATERIALINPUT_setText(self->input_username_, L"用户名重复, 如果你要覆盖请点击update");
                    return;
                }
            }
            if (wcslen(MATERIALINPUT_getText(self->input_password_)) >= 50)
            {
                MATERIALINPUT_setText(self->input_password_, L"用户名过长(>50)");
                return;
            }
            if (wcslen(MATERIALINPUT_getText(self->input_username_)) == 0)
            {
                MATERIALINPUT_setText(self->input_password_, L"用户名不能为空！");
            }
            if (wcslen(MATERIALINPUT_getText(self->input_password_)) == 0)
            {
                MATERIALINPUT_setText(self->input_password_, L"密码不能为空！");
                return;
            }

            self->user_ = (User*)realloc(self->user_, sizeof(User) * (self->user_num_ + 1));
            ++self->user_num_;
            wcscpy(self->user_[self->user_num_ - 1].name, MATERIALINPUT_getText(self->input_username_));
            self->user_[self->user_num_ - 1].password_hash = hash_murmur3(MATERIALINPUT_getText(self->input_password_)
                , sizeof(wchar_t) * wcslen(MATERIALINPUT_getText(self->input_password_)));

            MATERIALINPUT_setText(self->input_password_, L"");
            MATERIALINPUT_setText(self->input_username_, L"");

            MATERIALLIST_setData(self->list_, adminUserToMaterialInputData(self->user_, self->user_num_), self->user_num_);
        }
        else if (BUTTON_isClicked(self->del_))
        {
            if (MATERIALLIST_isSelected(self->list_))
            {
                if (self->user_num_ == 1)
                {
                    MATERIALINPUT_setText(self->input_username_, L"必须要有一个用户名！");
                    return;
                }

                uint32_t index = MATERIALLIST_getSelectedItem(self->list_).UUID_;

                User* new_user = (User*)malloc(sizeof(User) * (self->user_num_ - 1));

                int indexbuf = 0;
                for (uint32_t i = 0; i < self->user_num_; ++i)
                {
                    if (i != index)
                    {
                        new_user[indexbuf].password_hash = self->user_[i].password_hash;
                        wcscpy(new_user[indexbuf].name, self->user_[i].name);
                        ++indexbuf;
                    }
                }

                --self->user_num_;

                free(self->user_);

                self->user_ = new_user;

                MATERIALLIST_setData(self->list_, adminUserToMaterialInputData(self->user_, self->user_num_), self->user_num_);
            }
        }
        else if (BUTTON_isClicked(self->update_))
        {
            if (wcslen(MATERIALINPUT_getText(self->input_username_)))
            {
                if (wcslen(MATERIALINPUT_getText(self->input_password_)) >= 50)
                {
                    MATERIALINPUT_setText(self->input_password_, L"用户名过长(>50)");
                    return;
                }
                if (wcslen(MATERIALINPUT_getText(self->input_password_)) == 0)
                {
                    MATERIALINPUT_setText(self->input_password_, L"密码不能为空！");
                    return;
                }

                int index = -1;
                for (int i = 0; i < self->user_num_; ++i)
                {
                    if (wcscmp(MATERIALINPUT_getText(self->input_username_), self->user_[i].name) == 0)
                    {
                        index = i;
                        break;
                    }
                }

                if (index == -1)
                {
                    MATERIALINPUT_setText(self->input_password_, L"找不到用户名");
                    return;
                }

                self->user_[index].password_hash = hash_murmur3(MATERIALINPUT_getText(self->input_password_),
                    wcslen(MATERIALINPUT_getText(self->input_password_)) * sizeof(wchar_t));

                MATERIALINPUT_setText(self->input_password_, L"");
                MATERIALINPUT_setText(self->input_username_, L"");

                MATERIALLIST_setData(self->list_, adminUserToMaterialInputData(self->user_, self->user_num_), self->user_num_);
            }
            else if (MATERIALLIST_isSelected(self->list_))
            {
                User* ptr = self->user_ + MATERIALLIST_getSelectedItem(self->list_).UUID_;
                MATERIALINPUT_setText(self->input_username_, ptr->name);
                MATERIALINPUT_setText(self->input_password_, L"");
            }
        }
    }
}

void ADMINUSER_handleMouseUp(AdminUser* self, int x, int y)
{
    if (self->is_active_)
    {
        if (self->status_ == LOGIN_STATUS_ADMIN)
        {
            BUTTON_handleMouseUp(self->add_, x, y);
            BUTTON_handleMouseUp(self->del_, x, y);
            BUTTON_handleMouseUp(self->update_, x, y);
        }
    }
}

bool ADMINUSER_isActived(AdminUser* self)
{
    return self->is_active_;
}

void ADMINUSER_SetStatus(AdminUser* self, bool status)
{
    self->is_active_ = status;
}

void ADMINUSER_setLoginStatus(AdminUser* self, LoginStatus status)
{
    self->status_ = status;
}

void ADMINUSER_destroy(AdminUser* self)
{
    BUTTON_destroy(self->add_);
    BUTTON_destroy(self->del_);
    BUTTON_destroy(self->update_);
    MATERIALINPUT_destroy(self->input_password_);
    MATERIALINPUT_destroy(self->input_username_);
    MATERIALLIST_destroy(self->list_);
}

void ADMINUSER_handleInput(AdminUser* self, key_msg msg)
{
    if (self->is_active_)
    {
        MATERIALINPUT_handleInput(self->input_username_, msg);
        MATERIALINPUT_handleInput(self->input_password_, msg);
    }
}

void ADMINUSER_handleMouseWheel(AdminUser* self, int wheel)
{
    MATERIALLIST_handleMouseWheel(self->list_, wheel);
}


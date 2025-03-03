#include "Login.h"

// 这里会移交user所有权 但不会处理 比如删除
Login* LOGIN_createGrey(int x, int y, int width, int height, User* user, uint32_t user_num)
{
    Login* self = (Login*)malloc(sizeof(Login));
    if (!self)
    {
        return NULL;
    }

    self->x_ = x;
    self->y_ = y;
    self->width_ = width;
    self->height_ = height;

    self->user_ = user;
    self->user_num_ = user_num;

    int vertical_spacing = height / 20;

    int input_width = width * 2 / 4;
    int button_width = width / 7;

    // 用户名输入框
    self->username_ = MATERIALINPUT_createGrayInput(
        x + (width - input_width) / 2,
        y + height / 3,
        input_width,
        height / 16,
        L"用户名"
    );
    if (!self->username_)
    {
        free(self);
        return NULL;
    }

    self->password_ = MATERIALINPUT_createGrayInput(
        x + (width - input_width) / 2,  // 水平居中
        y + height / 3 + height / 16 + vertical_spacing, // 在用户名下方，加上间距
        input_width,
        height / 16,
        L"密码"
    );
    if (!self->password_)
    {
        MATERIALINPUT_destroy(self->username_);
        free(self);
        return NULL;
    }
    self->login_ = BUTTON_createBlueButton(
        x + (width - input_width) / 2,
        y + height / 3 + 2 * (height / 16) + 2 * vertical_spacing,
        button_width,
        height / 16,
        L"登录"
    );
    if (!self->login_)
    {
        MATERIALINPUT_destroy(self->password_);
        MATERIALINPUT_destroy(self->username_);
        free(self);
        return NULL;
    }

    self->guest_ = BUTTON_createBlueButton(
        x + (width - input_width) / 2 + button_width + 20, // 水平居中
        y + height / 3 + 2 * (height / 16) + 2 * vertical_spacing, // 在登录下方，加上间距
        button_width,
        height / 16,
        L"游客"
    );
    if (!self->guest_)
    {
        BUTTON_destroy(self->login_);
        MATERIALINPUT_destroy(self->password_);
        MATERIALINPUT_destroy(self->username_);
        free(self);
        return NULL;
    }

    self->exit_ = BUTTON_createBlueButton(
        x + (width - input_width) / 2 + 2 * button_width + 40,
        y + height / 3 + 2 * (height / 16) + 2 * vertical_spacing,
        button_width,
        height / 16,
        L"退出"
    );

    self->is_done_ = false;
    self->is_active_ = false;
    self->will_exit_ = false;
    self->login_status_ = LOGIN_STATUS_GUEST;
    return self;
}

void LOGIN_draw(Login* self)
{
    if (self->is_active_)
    {
        setcolor(BLACK);
        setfont(50, 0, "微软雅黑");
        outtextxy(self->x_ + (self->width_ - self->width_ * 1 / 2) / 2,  // 水平居中
            self->y_ + self->height_ / 16 + self->height_ / 7, L"图书借阅管理系统");
        setfont(20, 0, "微软雅黑");

        MATERIALINPUT_draw(self->username_);
        MATERIALINPUT_draw(self->password_);

        BUTTON_draw(self->login_);
        BUTTON_draw(self->guest_);
        BUTTON_draw(self->exit_);
    }
}

void LOGIN_handleMouseMove(Login* self, int x, int y)
{
    if (self->is_active_)
    {
        MATERIALINPUT_handleMouseMove(self->username_, x, y);
        MATERIALINPUT_handleMouseMove(self->password_, x, y);
        BUTTON_handleMouseMove(self->login_, x, y);
        BUTTON_handleMouseMove(self->guest_, x, y);
        BUTTON_handleMouseMove(self->exit_, x, y);
    }
}

bool LOGIN_isExit(Login* self)
{
    return self->will_exit_;
}

void LOGIN_handleButtonDown(Login* self, int x, int y)
{
    if (self->is_active_)
    {
        MATERIALINPUT_handleMouseDown(self->username_, x, y);
        MATERIALINPUT_handleMouseDown(self->password_, x, y);
        BUTTON_handleMouseDown(self->login_, x, y);
        BUTTON_handleMouseDown(self->guest_, x, y);
        BUTTON_handleMouseDown(self->exit_, x, y);

        if (BUTTON_isClicked(self->login_))
        {
            for (int i = 0; i < self->user_num_; ++i)
            {
                if (wcscmp(self->user_[i].name, MATERIALINPUT_getText(self->username_)) == 0 &&
                    self->user_[i].password_hash == hash_murmur3(MATERIALINPUT_getText(self->password_), sizeof(wchar_t) * wcslen(MATERIALINPUT_getText(self->password_))))
                {
                    self->login_status_ = LOGIN_STATUS_ADMIN;
                    self->is_done_ = true;
                    return;
                }
            }
            self->login_status_ = LOGIN_STATUS_ERROR;
            self->is_done_ = false;

            MATERIALINPUT_setText(self->username_, L"账号或密码错误");
            MATERIALINPUT_setText(self->password_, L"");

            return;
        }
        if (BUTTON_isClicked(self->guest_))
        {
            self->login_status_ = LOGIN_STATUS_GUEST;
            self->is_done_ = true;
            return;
        }
        if (BUTTON_isClicked(self->exit_))
        {
            self->will_exit_ = true;
        }
    }
}

void LOGIN_handleMouseUp(Login* self, int x, int y)
{
    if (self->is_active_)
    {
        BUTTON_handleMouseUp(self->login_, x, y);
        BUTTON_handleMouseUp(self->guest_, x, y);
        BUTTON_handleMouseUp(self->exit_, x, y);
    }
}

bool LOGIN_isActived(Login* self)
{
    return self->is_active_;
}

void LOGIN_setActivedStatus(Login* self, bool status)
{
    self->is_active_ = status;
}

LoginStatus LOGIN_getLoginStatus(Login* self)
{
    return self->login_status_;
}

bool LOGIN_isDone(Login* self)
{
    return self->is_done_;
}

void LOGIN_destroy(Login* self)
{
    BUTTON_destroy(self->login_);
    BUTTON_destroy(self->guest_);
    MATERIALINPUT_destroy(self->username_);
    MATERIALINPUT_destroy(self->password_);
}

void LOGIN_handleInput(Login* self, key_msg msg)
{
    if (self->is_active_)
    {
        MATERIALINPUT_handleInput(self->username_, msg);
        MATERIALINPUT_handleInput(self->password_, msg);
    }
}

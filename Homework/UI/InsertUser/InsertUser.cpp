#include "InsertUser.h"

// 这里从WEB前端引用了一个很好的排布方式 可以预留窗口大小变化的实现 但是如果要修改之前的话 那绝对会是个大工程吧
InsertUser* INSERTUSER_createGrey(int container_x, int container_y, int container_width, int container_height, list* data_, StringSkipList* book_data)
{
    InsertUser* self = (InsertUser*)malloc(sizeof(InsertUser));
    self->data_ = data_;
    self->book_data_ = book_data;

    float margin_ratio = 0.06;
    int margin = container_height * margin_ratio;

    int column_width = (container_width - 3 * margin) / 2;
    int input_height = (container_height - 4 * margin) / 12;

    // 第一列
    self->input_name_ = MATERIALINPUT_createGrayInput(
        container_x + margin,
        container_y + margin,
        column_width,
        input_height,
        L"名称");

    self->input_id_ = MATERIALINPUT_createGrayInput(
        container_x + margin,
        container_y + margin + input_height + margin,
        column_width,
        input_height,
        L"ID");


    // 第二列
    self->input_book_name_ = MATERIALINPUT_createGrayInput(
        container_x + 2 * margin + column_width,
        container_y + margin,
        column_width,
        input_height,
        L"书名");

    self->input_day_time_ = MATERIALINPUT_createGrayInput(
        container_x + 2 * margin + column_width,
        container_y + margin + input_height + margin,
        column_width,
        input_height,
        L"借阅天数");

    self->button_insert_ = BUTTON_createBlueButton(
        container_x + margin,
        container_y + margin + 2 * (input_height + margin),
        column_width,
        input_height,
        L"插入");

    self->is_active_ = false;

    return self;
}

bool INSERTUSER_isStringNum(const wchar_t* str)
{
    if (str[0] == L'\0')
    {
        return false;
    }

    size_t len = wcslen(str);
    bool valid_number = true;
    for (size_t i = 0; i < len; i++)
    {
        if (!iswdigit(str[i]))
        {
            valid_number = false;
            break;
        }
    }

    if (!valid_number)
    {
        return false;
    }

    return true;
}

void INSERTUSER_draw(InsertUser* self)
{
    if (self->is_active_)
    {
        MATERIALINPUT_draw(self->input_name_);
        MATERIALINPUT_draw(self->input_book_name_);
        MATERIALINPUT_draw(self->input_id_);
        MATERIALINPUT_draw(self->input_day_time_);

        if (self->status_ == LOGIN_STATUS_ADMIN)
        {
            BUTTON_draw(self->button_insert_);
        }
    }
}

void INSERTUSER_handleMouseMove(InsertUser* self, int x, int y)
{
    if (self->is_active_)
    {
        MATERIALINPUT_handleMouseMove(self->input_name_, x, y);
        MATERIALINPUT_handleMouseMove(self->input_book_name_, x, y);
        MATERIALINPUT_handleMouseMove(self->input_id_, x, y);
        MATERIALINPUT_handleMouseMove(self->input_day_time_, x, y);

        if (self->status_ == LOGIN_STATUS_ADMIN)
        {
            BUTTON_handleMouseMove(self->button_insert_, x, y);
        }
    }
}

void INSERTUSER_handleMouseDown(InsertUser* self, int x, int y)
{
    if (self->is_active_)
    {
        MATERIALINPUT_handleMouseDown(self->input_name_, x, y);
        MATERIALINPUT_handleMouseDown(self->input_book_name_, x, y);
        MATERIALINPUT_handleMouseDown(self->input_id_, x, y);
        MATERIALINPUT_handleMouseDown(self->input_day_time_, x, y);

        if (self->status_ == LOGIN_STATUS_ADMIN)
        {
            BUTTON_handleMouseDown(self->button_insert_, x, y);
        }
    }

    if (BUTTON_isClicked(self->button_insert_))
    {
        if (MATERIALINPUT_getText(self->input_book_name_) == NULL || MATERIALINPUT_getText(self->input_book_name_) == NULL
            || MATERIALINPUT_getText(self->input_id_) == NULL || MATERIALINPUT_getText(self->input_day_time_) == NULL)
        {
            return;
        }

        const wchar_t* id_s = MATERIALINPUT_getText(self->input_id_);
        if (!INSERTUSER_isStringNum(id_s))
        {
            MATERIALINPUT_setText(self->input_id_, L"这可不是一个合法的数字喔");
            return;
        }
        const wchar_t* day_time_s = MATERIALINPUT_getText(self->input_day_time_);
        if (!INSERTUSER_isStringNum(day_time_s))
        {
            MATERIALINPUT_setText(self->input_day_time_, L"这可不是一个合法的数字喔");
            return;
        }

        const wchar_t* name = MATERIALINPUT_getText(self->input_name_);
        const wchar_t* book_name = MATERIALINPUT_getText(self->input_book_name_);

        uint64_t id = wcstoull(id_s, NULL, 10);
        uint32_t day_time = wcstoul(day_time_s, NULL, 10);

        SearchResult res;
        bool st = STRINGSKIPLIST_searchW(self->book_data_, book_name, &res);
        if (!st)
        {
            MATERIALINPUT_setText(self->input_book_name_, L"不存在的图书名");
            return;
        }
        if (res.value.num_ == 0)
        {
            MATERIALINPUT_setText(self->input_book_name_, L"剩余图书数量不足");
            return;
        }
        if (res.value.can_rent_ == false)
        {
            MATERIALINPUT_setText(self->input_book_name_, L"这本书是不可租借的");
            return;
        }

        res.value.num_ -= 1;
        
        STRINGSKIPLIST_deleteW(self->book_data_, res.key);
        STRINGSKIPLIST_insertW(self->book_data_, res.key, &res.value);

        if (st)
        {
            free(res.key);
        }

        BookData data = {};

        Date now = Date_today();

        Date target = Date_addDays(now, day_time);

        LIST_PushBack(self->data_, name, book_name, target.year, target.month, target.day, id);

        MATERIALINPUT_setText(self->input_book_name_, L"");
        MATERIALINPUT_setText(self->input_name_, L"");
        MATERIALINPUT_setText(self->input_id_, L"");
        MATERIALINPUT_setText(self->input_day_time_, L"");
    }
}

void INSERTUSER_handleMouseUp(InsertUser* self, int x, int y)
{
    if (self->is_active_)
    {
        BUTTON_handleMouseUp(self->button_insert_, x, y);
    }
}

bool INSERTUSER_isActived(InsertUser* self)
{
    return self->is_active_;
}

void INSERTUSER_setLoginStatus(InsertUser* self, LoginStatus status)
{
    self->status_ = status;
}

void INSERTUSER_SetStatus(InsertUser* self, bool status)
{
    self->is_active_ = status;
}

void INSERTUSER_destroy(InsertUser* self)
{
    MATERIALINPUT_destroy(self->input_name_);
    MATERIALINPUT_destroy(self->input_book_name_);
    MATERIALINPUT_destroy(self->input_id_);
    MATERIALINPUT_destroy(self->input_day_time_);

    BUTTON_destroy(self->button_insert_);
}

void INSERTUSER_handleInput(InsertUser* self, key_msg msg)
{
    if (self->is_active_)
    {
        MATERIALINPUT_handleInput(self->input_name_, msg);
        MATERIALINPUT_handleInput(self->input_book_name_, msg);
        MATERIALINPUT_handleInput(self->input_id_, msg);
        MATERIALINPUT_handleInput(self->input_day_time_, msg);
    }
}

#include "InsertBook.h"

// 这里从WEB前端引用了一个很好的排布方式 可以预留窗口大小变化的实现 但是如果要修改之前的话 那绝对会是个大工程吧
InsertBook* INSERTBOOK_createGrey(int container_x, int container_y, int container_width, int container_height, StringSkipList* data_)
{
    InsertBook* self = (InsertBook*)malloc(sizeof(InsertBook));
    self->data_ = data_;

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
        L"书名");

    self->input_catagory_ = MATERIALINPUT_createGrayInput(
        container_x + margin,
        container_y + margin + input_height + margin,
        column_width,
        input_height,
        L"类别");

    self->input_num_ = MATERIALINPUT_createGrayInput(
        container_x + margin,
        container_y + margin + 2 * (input_height + margin),
        column_width,
        input_height,
        L"数量");

    // 第二列
    self->input_ISBN_ = MATERIALINPUT_createGrayInput(
        container_x + 2 * margin + column_width,
        container_y + margin,
        column_width,
        input_height,
        L"ISBN");

    self->input_publisher_ = MATERIALINPUT_createGrayInput(
        container_x + 2 * margin + column_width,
        container_y + margin + input_height + margin,
        column_width,
        input_height,
        L"出版社");

    self->button_insert_ = BUTTON_createBlueButton(
        container_x + 2 * margin + column_width,
        container_y + margin + 2 * (input_height + margin),
        column_width,
        input_height,
        L"插入");

    self->is_active_ = false;

    self->is_update_ = false;
    self->update_key_ = NULL;
    self->ori_diff_ = 0;

    return self;
}

bool INSERTBOOK_isStringNum(const wchar_t* str)
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

void INSERTBOOK_draw(InsertBook* self)
{
    if (self->is_active_)
    {
        MATERIALINPUT_draw(self->input_name_);
        MATERIALINPUT_draw(self->input_catagory_);
        MATERIALINPUT_draw(self->input_num_);
        MATERIALINPUT_draw(self->input_publisher_);
        MATERIALINPUT_draw(self->input_ISBN_);
        if (self->status_ == LOGIN_STATUS_ADMIN)
        {
            BUTTON_draw(self->button_insert_);
        }
    }
}

void INSERTBOOK_handleMouseMove(InsertBook* self, int x, int y)
{
    if (self->is_active_)
    {
        MATERIALINPUT_handleMouseMove(self->input_name_, x, y);
        MATERIALINPUT_handleMouseMove(self->input_ISBN_, x, y);
        MATERIALINPUT_handleMouseMove(self->input_catagory_, x, y);
        MATERIALINPUT_handleMouseMove(self->input_num_, x, y);
        MATERIALINPUT_handleMouseMove(self->input_publisher_, x, y);

        if (self->status_ == LOGIN_STATUS_ADMIN)
        {
            BUTTON_handleMouseMove(self->button_insert_, x, y);
        }
    }
}

void INSERTBOOK_handleMouseDown(InsertBook* self, int x, int y)
{
    if (self->is_active_)
    {
        MATERIALINPUT_handleMouseDown(self->input_name_, x, y);
        MATERIALINPUT_handleMouseDown(self->input_ISBN_, x, y);
        MATERIALINPUT_handleMouseDown(self->input_catagory_, x, y);
        MATERIALINPUT_handleMouseDown(self->input_num_, x, y);
        MATERIALINPUT_handleMouseDown(self->input_publisher_, x, y);

        if (self->status_ == LOGIN_STATUS_ADMIN)
        {
            BUTTON_handleMouseDown(self->button_insert_, x, y);
        }
    }

    if (BUTTON_isClicked(self->button_insert_))
    {
        if (MATERIALINPUT_getText(self->input_name_) == NULL || MATERIALINPUT_getText(self->input_ISBN_) == NULL 
            || MATERIALINPUT_getText(self->input_catagory_) == NULL || MATERIALINPUT_getText(self->input_num_) == NULL 
            || MATERIALINPUT_getText(self->input_publisher_) == NULL)
        {
            return;
        }

        const wchar_t* ISBN_s = MATERIALINPUT_getText(self->input_ISBN_);
        if (!INSERTBOOK_isStringNum(ISBN_s))
        {
            MATERIALINPUT_setText(self->input_ISBN_, L"这可不是一个合法的数字喔");
            return;
        }
        const wchar_t* num_s = MATERIALINPUT_getText(self->input_num_);
        if (!INSERTBOOK_isStringNum(num_s))
        {
            MATERIALINPUT_setText(self->input_num_, L"这可不是一个合法的数字喔");
            return;
        }
        const wchar_t* catagory_s = MATERIALINPUT_getText(self->input_catagory_);
        if (wcslen(catagory_s) >= 30)
        {
            MATERIALINPUT_setText(self->input_catagory_, L"类别过长(>30)");
            return;
        }

        const wchar_t* publisher_s = MATERIALINPUT_getText(self->input_publisher_);
        if (wcslen(publisher_s) >= 50)
        {
            MATERIALINPUT_setText(self->input_publisher_, L"出版社过长(>50)");
            return;
        }

        const wchar_t* name = MATERIALINPUT_getText(self->input_name_);
        const wchar_t* catagory = MATERIALINPUT_getText(self->input_catagory_);
        const wchar_t* publisher = MATERIALINPUT_getText(self->input_publisher_);

        uint64_t ISBN = wcstoull(ISBN_s, NULL, 10);
        uint32_t num = wcstoul(num_s, NULL, 10);

        SearchResult res;
        bool st = STRINGSKIPLIST_searchW(self->data_, name, &res);
        if (st)
        {
            MATERIALINPUT_setText(self->input_name_, L"重复的图书名");
            return;
        }
        if (st)
        {
            free(res.key);
        }

        BookData data = {};

        data.can_rent_ = true;

        wcscpy(data.catagory_, catagory);
        wcscpy(data.publisher_, publisher);
        data.isbn_ = ISBN;
        data.image_toc_index_ = 0;
        data.total_num_ = num;

        if (self->is_update_ == true && self->update_key_ != NULL)
        {
            if (wcscmp(name, self->update_key_) == 0)
            {
                data.num_ = num - self->ori_diff_;
                self->is_update_ = false;
                free(self->update_key_);
                self->update_key_ = NULL;
                self->ori_diff_ = 0;
            }
            else
            {
                data.num_ = num;
            }
        }
        else
        {
            data.num_ = num;
        }

        STRINGSKIPLIST_insertW(self->data_, name, &data);

        MATERIALINPUT_setText(self->input_catagory_, L"");
        MATERIALINPUT_setText(self->input_name_, L"");
        MATERIALINPUT_setText(self->input_ISBN_, L"");
        MATERIALINPUT_setText(self->input_publisher_, L"");
        MATERIALINPUT_setText(self->input_num_, L"");
    }
}

void INSERTBOOK_handleMouseUp(InsertBook* self, int x, int y)
{
    if (self->status_ == LOGIN_STATUS_ADMIN)
    {
        BUTTON_handleMouseUp(self->button_insert_, x, y);
    }
}

bool INSERTBOOK_isActived(InsertBook* self)
{
    return self->is_active_;
}

void INSERTBOOK_SetStatus(InsertBook* self, bool status)
{
    self->is_active_ = status;
}

void INSERTBOOK_setLoginStatus(InsertBook* self, LoginStatus status)
{
    self->status_ = status;
}

void INSERTBOOK_destroy(InsertBook* self)
{
    MATERIALINPUT_destroy(self->input_name_);
    MATERIALINPUT_destroy(self->input_catagory_);
    MATERIALINPUT_destroy(self->input_num_);
    MATERIALINPUT_destroy(self->input_publisher_);
    MATERIALINPUT_destroy(self->input_ISBN_);


    BUTTON_destroy(self->button_insert_);
}

void INSERTBOOK_handleInput(InsertBook* self, key_msg msg)
{
    if (self->is_active_)
    {
        MATERIALINPUT_handleInput(self->input_catagory_, msg);
        MATERIALINPUT_handleInput(self->input_name_, msg);
        MATERIALINPUT_handleInput(self->input_publisher_, msg);
        MATERIALINPUT_handleInput(self->input_ISBN_, msg);
        MATERIALINPUT_handleInput(self->input_num_, msg);
    }
}

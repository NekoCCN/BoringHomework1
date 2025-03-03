#include "QueryUser.h"

static void UDToUDVector(QueryUser* self)
{
    UDVECTOR_clear(self->buf_);

    UserListIterator it = LIST_iteratorBegin(self->data_); // 初始化迭代器

    while (!LIST_iteratorEnd(&it))
    {
        UDVECTOR_pushBack(self->buf_, LIST_iteratorGetData(&it));
        LIST_iteratorNext(&it);
    }
}
static MaterialListData* userToMaterialInputData(const UserData* data, uint64_t count)
{
    MaterialListData* newdata = (MaterialListData*)malloc(sizeof(MaterialListData) * count);
    char buffer[512];

    uint64_t index = 0;

    for (int i = 0; i < count; ++i)
    {
        newdata[i].UUID_ = index;
        ++index;

        sprintf(buffer, "%ls  书名:%ls", data[i].user_name_, data[i].book_name_);
        newdata[i].first_line_ = (char*)malloc(strlen(buffer) + 1);
        strcpy(newdata[i].first_line_, buffer);

        Date today = Date_today();
        Date pre = Date_create(data[i].year_, data[i].month_, data[i].day_);

        sprintf(buffer, "截止日期: %d, %d, %d, 剩余天数：%d",
            data[i].year_, data[i].month_, data[i].day_, Date_diff(pre, today));
        newdata[i].second_line_ = (char*)malloc(strlen(buffer) + 1);
        strcpy(newdata[i].second_line_, buffer);

        newdata[i].key_ = wcsdup(data[i].book_name_);

        if (Date_diff(pre, today) < 0)
        {
            newdata[i].first_line_color_ = RED;
            newdata[i].second_line_color_ = RED;
        }
        else if (Date_diff(pre, today) <= 2)
        {
            newdata[i].first_line_color_ = hexToColorref("#FF7F00");
            newdata[i].second_line_color_ = hexToColorref("#FF7F00");
        }
        else
        {
            newdata[i].first_line_color_ = BLACK;
            newdata[i].second_line_color_ = BLACK;
        }
    }

    return newdata;
}
QueryUser* QUERYUSER_createGrey(int x, int y, int width, int height, list* data, StringSkipList* book_data, LoginStatus status)
{
    QueryUser* self = (QueryUser*)malloc(sizeof(QueryUser));

    self->input_ = MATERIALINPUT_createGrayInput(x + 30, y + height / 10 , width / 2, height / 14, L"输入一些东西吧！");
    self->list_ = MATERIALLIST_createGrayMaterialList(x + 30, y + height / 5, width - 50,
        height - height / 5 - 20, 60, 16);

    self->search_ = BUTTON_createBlueButton((x + 30 + width / 2) + 20, y + height / 10 + 5, width / 10, height / 14 - 10, L"搜索");

    self->data_ = data;

    self->update_ = BUTTON_createBlueButton(x + 30 + width / 2 + 220, y + height / 10 + 5, width / 10, height / 14 - 10, L"更新");
    self->del_ = BUTTON_createBlueButton(x + 30 + width / 2 + 330, y + height / 10 + 5, width / 10, height / 14 - 10, L"删除");

    self->buf_ = UDVECTOR_construct();

    self->book_data_ = book_data;
    
    UDToUDVector(self);
    
    int size = 0;
    UserData* dataw =  UDVECTOR_copiedData(self->buf_, &size);
    MATERIALLIST_setData(self->list_, userToMaterialInputData(dataw, size), size);

    self->type_ = QUERYUSERTYPE_create(x, y + 17, width, 10);

    return self;
}

void QUERYUSER_draw(QueryUser* self)
{
    if (self->is_active_)
    {
        MATERIALINPUT_draw(self->input_);
        MATERIALLIST_draw(self->list_);
        BUTTON_draw(self->search_);

        if (self->status_ == LOGIN_STATUS_ADMIN)
        {
            BUTTON_draw(self->update_);
            BUTTON_draw(self->del_);
        }

        QUERYUSERTYPE_draw(self->type_);
    }
}

void QUERYUSER_handleMouseMove(QueryUser* self, int x, int y)
{
    if (self->is_active_)
    {
        BUTTON_handleMouseMove(self->search_, x, y);
        if (self->status_ == LOGIN_STATUS_ADMIN)
        {
            BUTTON_handleMouseMove(self->update_, x, y);
            BUTTON_handleMouseMove(self->del_, x, y);
        }

        MATERIALINPUT_handleMouseMove(self->input_, x, y);
        MATERIALLIST_handleMouseMove(self->list_, x, y);

        QUERYUSERTYPE_handleMouseMove(self->type_, x, y);
    }
}

void QUERYUSER_handleButtonDown(QueryUser* self, int x, int y, InsertUser* insert, Sidebar* sidebar)
{
    if (self->is_active_)
    {
        BUTTON_handleMouseDown(self->search_, x, y);

        if (self->status_ == LOGIN_STATUS_ADMIN)
        {
            BUTTON_handleMouseDown(self->update_, x, y);
            BUTTON_handleMouseDown(self->del_, x, y);
        }

        MATERIALINPUT_handleMouseDown(self->input_, x, y);
        MATERIALLIST_handleMouseDown(self->list_, x, y);

        QUERYUSERTYPE_handleButtonDown(self->type_, x, y);

        if (BUTTON_isClicked(self->search_))
        {
            switch (self->type_->status_)
            {
            case QUERYUSER_TYPE_BUTTON_ENTIRE:
            {
                int count = 0;
                const wchar_t* text = MATERIALINPUT_getText(self->input_);
                if (wcslen(text) == 0)
                {
                    UDToUDVector(self);

                    int size = 0;
                    UserData* data = UDVECTOR_copiedData(self->buf_, &size);
                    MATERIALLIST_setData(self->list_, userToMaterialInputData(data, size), size);
                }
                else
                {
                    UDVECTOR_clear(self->buf_);

                    UserListIterator it = LIST_iteratorBegin(self->data_);

                    while (!LIST_iteratorEnd(&it))
                    {
                        UserData data = LIST_iteratorGetData(&it);
                        if (wcscmp(data.user_name_, text) == 0)
                        {
                            UDVECTOR_pushBack(self->buf_, data);
                            LIST_iteratorNext(&it);
                        }
                    }
                }
            }
            break;
            case QUERYUSER_TYPE_BUTTON_BOOKNAME_ENTIRE:
            {
                int count = 0;
                const wchar_t* text = MATERIALINPUT_getText(self->input_);
                if (wcslen(text) == 0)
                {
                    UDToUDVector(self);

                    int size = 0;
                    UserData* data = UDVECTOR_copiedData(self->buf_, &size);
                    MATERIALLIST_setData(self->list_, userToMaterialInputData(data, size), size);
                }
                else
                {
                    UDVECTOR_clear(self->buf_);

                    UserListIterator it = LIST_iteratorBegin(self->data_);

                    while (!LIST_iteratorEnd(&it))
                    {
                        UserData data = LIST_iteratorGetData(&it);
                        if (wcscmp(data.book_name_, text) == 0)
                        {
                            UDVECTOR_pushBack(self->buf_, data);
                            LIST_iteratorNext(&it);
                        }
                    }

                    int size = 0;
                    UserData* data = UDVECTOR_copiedData(self->buf_, &size);
                    MATERIALLIST_setData(self->list_, userToMaterialInputData(data, size), size);
                }
            }
            break;
            case QUERYUSER_TYPE_BUTTON_ID:
            {
                int count = 0;
                UDVECTOR_clear(self->buf_);

                const wchar_t* str = MATERIALINPUT_getText(self->input_);
                if (str[0] == L'\0' || str[0] == L'0')
                {
                    UDToUDVector(self);

                    int size = 0;
                    UserData* data = UDVECTOR_copiedData(self->buf_, &size);
                    MATERIALLIST_setData(self->list_, userToMaterialInputData(data, size), size);
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
                    UDToUDVector(self);

                    int size = 0;
                    UserData* data = UDVECTOR_copiedData(self->buf_, &size);
                    MATERIALLIST_setData(self->list_, userToMaterialInputData(data, size), size);
                    MATERIALINPUT_setText(self->input_, L"这可不是一个合法的数字喔");
                    break;
                }

                unsigned long long num = wcstoull(str, NULL, 10);
                UDVECTOR_clear(self->buf_);

                UserListIterator it = LIST_iteratorBegin(self->data_);

                while (!LIST_iteratorEnd(&it))
                {
                    UserData data = LIST_iteratorGetData(&it);
                    if (data.id_ == num)
                    {
                        UDVECTOR_pushBack(self->buf_, data);
                        LIST_iteratorNext(&it);
                    }
                }

                int size = 0;
                UserData* data = UDVECTOR_copiedData(self->buf_, &size);
                MATERIALLIST_setData(self->list_, userToMaterialInputData(data, size), size);
                break;
            }
            case QUERYUSER_TYPE_TIME:
            {
                MATERIALINPUT_setText(self->input_, L"");

                UserListIterator it = LIST_iteratorBegin(self->data_);

                Date today = Date_today();

                while (!LIST_iteratorEnd(&it))
                {
                    UserData data = LIST_iteratorGetData(&it);

                    Date tg = Date_create(data.year_, data.month_, data.day_);
                    int32_t day = Date_diff(tg, today);

                    if (day < 0)
                    {
                        UDVECTOR_pushBack(self->buf_, data);
                        LIST_iteratorNext(&it);
                    }
                }

                int size = 0;
                UserData* data = UDVECTOR_copiedData(self->buf_, &size);
                MATERIALLIST_setData(self->list_, userToMaterialInputData(data, size), size);
                break;
            }
            case QUERYUSER_TYPE_REGEX:
            {
                int count = 0;
                const wchar_t* text = MATERIALINPUT_getText(self->input_);
                if (wcslen(text) == 0)
                {
                    UDToUDVector(self);

                    int size = 0;
                    UserData* data = UDVECTOR_copiedData(self->buf_, &size);
                    MATERIALLIST_setData(self->list_, userToMaterialInputData(data, size), size);
                }
                else
                {
                    UDVECTOR_clear(self->buf_);

                    RegexMatcher* matcher = RegexMatcher::of(MATERIALINPUT_getText(self->input_));

                    if (matcher == NULL)
                    {
                        UDToUDVector(self);

                        int size = 0;
                        UserData* data = UDVECTOR_copiedData(self->buf_, &size);
                        MATERIALLIST_setData(self->list_, userToMaterialInputData(data, size), size);
                        MATERIALINPUT_setText(self->input_, L"这可不是一个合法的正则表达式哦");
                        break;
                    }

                    UserListIterator it = LIST_iteratorBegin(self->data_);

                    while (!LIST_iteratorEnd(&it))
                    {
                        UserData data = LIST_iteratorGetData(&it);
                        if (matcher->isMatch(data.user_name_))
                        {
                            UDVECTOR_pushBack(self->buf_, data);
                            LIST_iteratorNext(&it);
                        }
                    }

                    delete matcher;
                }

                break;
            }
            }
        }

        if (BUTTON_isClicked(self->del_))
        {
            if (MATERIALLIST_isSelected(self->list_) == true)
            {
                LIST_Delete(self->data_, MATERIALLIST_getSelectedItem(self->list_).UUID_);

                SearchResult res;
                if (STRINGSKIPLIST_searchW(self->book_data_, MATERIALLIST_getSelectedItem(self->list_).key_, &res))
                {
                    res.value.num_ += 1;
                    STRINGSKIPLIST_deleteW(self->book_data_, MATERIALLIST_getSelectedItem(self->list_).key_);
                    STRINGSKIPLIST_insertW(self->book_data_, MATERIALLIST_getSelectedItem(self->list_).key_, &res.value);
                    free(res.key);
                }

                // 把上面的搬下来
                int count = 0;
                const wchar_t* text = MATERIALINPUT_getText(self->input_);

                UDToUDVector(self);

                int size = 0;
                UserData* data = UDVECTOR_copiedData(self->buf_, &size);
                MATERIALLIST_setData(self->list_, userToMaterialInputData(data, size), size);
            }
        }
        else if (BUTTON_isClicked(self->update_))
        {
            if (MATERIALLIST_isSelected(self->list_) == true)
            {
                UserData data = LIST_Get(self->data_, MATERIALLIST_getSelectedItem(self->list_).UUID_);

                wchar_t buf[512];
                MATERIALINPUT_setText(insert->input_name_, data.user_name_);
                MATERIALINPUT_setText(insert->input_book_name_, data.book_name_);

                Date today = Date_today();
                Date tg = Date_create(data.year_, data.month_, data.day_);

                swprintf(buf, 512, L"%lld", Date_diff(today, tg));
                MATERIALINPUT_setText(insert->input_day_time_, buf);
                swprintf(buf, 512, L"%lld", data.id_);
                MATERIALINPUT_setText(insert->input_id_, buf);

                LIST_Delete(self->data_, MATERIALLIST_getSelectedItem(self->list_).UUID_);

                // 把上面的搬下来
                int count = 0;
                const wchar_t* text = MATERIALINPUT_getText(self->input_);

                UDToUDVector(self);

                int size = 0;
                UserData* dataw = UDVECTOR_copiedData(self->buf_, &size);
                MATERIALLIST_setData(self->list_, userToMaterialInputData(dataw, size), size);

                SIDEBAR_setStatus(sidebar, SIDEBAR_BUTTON_INSERTUSER);
            }
        }
    }
}

void QUERYUSER_handleMouseUp(QueryUser* self, int x, int y)
{
    BUTTON_handleMouseUp(self->search_, x, y);
    BUTTON_handleMouseUp(self->update_, x, y);
    BUTTON_handleMouseUp(self->del_, x, y);
}

bool QUERYUSER_isActived(QueryUser* self)
{
    return self->is_active_;
}

void QUERYUSER_SetStatus(QueryUser* self, bool status)
{
    self->is_active_ = status;
}

void QUERYUSER_setLoginStatus(QueryUser* self, LoginStatus status)
{
    self->status_ = status;
}

void QUERYUSER_destroy(QueryUser* self)
{
    MATERIALLIST_destroy(self->list_);
    MATERIALINPUT_destroy(self->input_);
    BUTTON_destroy(self->search_);
    BUTTON_destroy(self->update_);
    BUTTON_destroy(self->del_);
    QUERYUSERTYPE_destroy(self->type_);
}

void QUERYUSER_handleMouseWheel(QueryUser* self, int wheel)
{
    MATERIALLIST_handleMouseWheel(self->list_, wheel);
}

void QUERYUSER_handleInput(QueryUser* self, key_msg msg)
{
    MATERIALINPUT_handleInput(self->input_, msg);
}

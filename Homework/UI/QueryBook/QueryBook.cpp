#include "QueryBook.h"

static MaterialListData* searchResultToMaterialInputData(SearchResult* data, uint64_t count)
{
    MaterialListData* newdata = (MaterialListData*)malloc(sizeof(MaterialListData) * count);
    char buffer[512];

    for (int i = 0; i < count; ++i)
    {
        newdata[i].key_ = (wchar_t*)malloc(sizeof(wchar_t) * (wcslen(data[i].key) + 1));
        memcpy(newdata[i].key_, data[i].key, sizeof(wchar_t) * (wcslen(data[i].key) + 1));

        sprintf(buffer, "%ls  ISBN:%llu", data[i].key, data[i].value.isbn_);
        newdata[i].first_line_ = (char*)malloc(strlen(buffer) + 1);
        strcpy(newdata[i].first_line_, buffer);

        sprintf(buffer, "类别: %ls, 出版社: %ls, 剩余数量: %u, 总数量: %u， %ls",
            data[i].value.catagory_, data[i].value.publisher_, data[i].value.num_, data[i].value.total_num_
        , data[i].value.can_rent_ ? L"可租借" : L"不可租借");
        newdata[i].second_line_ = (char*)malloc(strlen(buffer) + 1);
        strcpy(newdata[i].second_line_, buffer);

        if (data[i].value.num_ == 0 || data[i].value.can_rent_ == false)
        {
            newdata[i].first_line_color_ = RED;
            newdata[i].second_line_color_ = RED;
        }
        else
        {
            newdata[i].first_line_color_ = BLACK;
            newdata[i].second_line_color_ = BLACK;
        }
    }

    return newdata;
}
Query* QUERYBOOK_createGrey(int x, int y, int width, int height, StringSkipList* data, LoginStatus status)
{
	Query* self = (Query*)malloc(sizeof(Query));

    self->input_ = MATERIALINPUT_createGrayInput(x + 30, y + height / 10 , width / 2, height / 14, L"输入一些东西吧！");
    self->list_ = MATERIALLIST_createGrayMaterialList(x + 30, y + height / 5, width - 50,
        height - height / 5 - 20, 60, 16);

    self->search_ = BUTTON_createBlueButton((x + 30 + width / 2) + 20, y + height / 10 + 5, width / 10, height / 14 - 10, L"搜索");

    self->data_ = data;

    self->update_ = BUTTON_createBlueButton(x + 30 + width / 2 + 220, y + height / 10 + 5, width / 10, height / 14 - 10, L"更新");
    self->del_ = BUTTON_createBlueButton(x + 30 + width / 2 + 330, y + height / 10 + 5, width / 10, height / 14 - 10, L"删除");

    self->set_can_rent_ = BUTTON_createBlueButton(x + 30 + width / 2 + 220, y + height / 10 - 40, width / 10, height / 14 - 10, L"设置可租借");
    self->rent_ = BUTTON_createBlueButton(x + 30 + width / 2 + 330, y + height / 10 - 40, width / 10, height / 14 - 10, L"租借");

    int num = 0;
    SearchResult* res = STRINGSKIPLIST_getAll(self->data_, &num);
    MATERIALLIST_setData(self->list_, searchResultToMaterialInputData(res, num), num);

    self->type_ = QUERYTYPE_create(x, y + 17, width, 10);

    self->buf_ = SRVECTOR_construct();

    return self;
}

void QUERYBOOK_draw(Query* self)
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

            BUTTON_draw(self->set_can_rent_);
            BUTTON_draw(self->rent_);
        }

        QUERYTYPE_draw(self->type_);
    }
}

void QUERYBOOK_handleMouseMove(Query* self, int x, int y)
{
    if (self->is_active_)
    {
        BUTTON_handleMouseMove(self->search_, x, y);

        if (self->status_ == LOGIN_STATUS_ADMIN)
        {
            BUTTON_handleMouseMove(self->update_, x, y);
            BUTTON_handleMouseMove(self->del_, x, y);

            BUTTON_handleMouseMove(self->set_can_rent_, x, y);
            BUTTON_handleMouseMove(self->rent_, x, y);
        }

        MATERIALINPUT_handleMouseMove(self->input_, x, y);
        MATERIALLIST_handleMouseMove(self->list_, x, y);

        QUERYTYPE_handleMouseMove(self->type_, x, y);
    }
}

void QUERYBOOK_handleButtonDown(Query* self, int x, int y, InsertBook* insert, Sidebar* sidebar, InsertUser* insert_user)
{
    if (self->is_active_)
    {
        BUTTON_handleMouseDown(self->search_, x, y);

        if (self->status_ == LOGIN_STATUS_ADMIN)
        {
            BUTTON_handleMouseDown(self->update_, x, y);
            BUTTON_handleMouseDown(self->del_, x, y);

            BUTTON_handleMouseDown(self->set_can_rent_, x, y);
            BUTTON_handleMouseDown(self->rent_, x, y);
        }

        MATERIALINPUT_handleMouseDown(self->input_, x, y);
        MATERIALLIST_handleMouseDown(self->list_, x, y);

        QUERYTYPE_handleButtonDown(self->type_, x, y);

        if (BUTTON_isClicked(self->search_))
        {
            switch (self->type_->status_)
            {
            case QUERYBOOK_TYPE_BUTTON_PREFIX:
            {
                int count = 0;
                const wchar_t* text = MATERIALINPUT_getText(self->input_);
                if (wcslen(text) == 0)
                {
                    SearchResult* res = STRINGSKIPLIST_getAll(self->data_, &count);
                    MATERIALLIST_setData(self->list_, searchResultToMaterialInputData(res, count), count);
                }
                else
                {
                    SearchResult* res = STRINGSKIPLIST_prefixSearchW(self->data_, text, &count);
                    MATERIALLIST_setData(self->list_, searchResultToMaterialInputData(res, count), count);
                }
            }
            break;
            case QUERYBOOK_TYPE_BUTTON_ENTIRE:
            {
                int count = 0;

                const wchar_t* text = MATERIALINPUT_getText(self->input_);
                if (wcslen(text) == 0)
                {
                    SearchResult* res = STRINGSKIPLIST_getAll(self->data_, &count);
                    MATERIALLIST_setData(self->list_, searchResultToMaterialInputData(res, count), count);
                }
                else
                {
                    SearchResult data;
                    if (STRINGSKIPLIST_searchW(self->data_, text, &data) == true)
                    {
                        SRVECTOR_clear(self->buf_);
                        SRVECTOR_pushBack(self->buf_, data);
                        SearchResult* res = SRVECTOR_data(self->buf_, &count);
                        MATERIALLIST_setData(self->list_, searchResultToMaterialInputData(res, 1), 1);
                    }
                    else
                    {
                        SRVECTOR_clear(self->buf_);
                        SearchResult* res = SRVECTOR_data(self->buf_, &count);
                        MATERIALLIST_setData(self->list_, searchResultToMaterialInputData(res, 0), 0);
                    }
                }
            }
            break;
            case QUERYBOOK_TYPE_BUTTON_ISBN:
            {
                int count = 0;
                SRVECTOR_clear(self->buf_);

                const wchar_t* str = MATERIALINPUT_getText(self->input_);
                if (str[0] == L'\0' || str[0] == L'0')
                {
                    SearchResult* res = STRINGSKIPLIST_getAll(self->data_, &count);
                    MATERIALLIST_setData(self->list_, searchResultToMaterialInputData(res, count), count);
                    break;
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
                    SRVECTOR_clear(self->buf_);
                    SearchResult* res = SRVECTOR_copiedData(self->buf_, &count);
                    MATERIALLIST_setData(self->list_, searchResultToMaterialInputData(res, count), 0);
                    MATERIALINPUT_setText(self->input_, L"这可不是一个合法的数字喔");
                    break;
                }

                unsigned long long num = wcstoull(str, NULL, 10);

                StringSkipListIterator it = STRINGSKIPLIST_iteratorBegin(self->data_); // 初始化迭代器

                while (!STRINGSKIPLIST_iteratorEnd(&it))
                {
                    SearchResult data = STRINGSKIPLIST_iteratorGetResult(&it);
                    if (data.value.isbn_ == num)
                    {
                        SRVECTOR_pushBack(self->buf_, data);
                    }
                    STRINGSKIPLIST_iteratorNext(&it);
                }
                SearchResult* res = SRVECTOR_copiedData(self->buf_, &count);
                MATERIALLIST_setData(self->list_, searchResultToMaterialInputData(res, count), count);
                break;
            }
            // 搬上面的
            case QUERYBOOK_TYPE_BUTTON_REGEX:
            {
                int32_t count = 0;

                RegexMatcher* matcher = RegexMatcher::of(MATERIALINPUT_getText(self->input_));

                if (matcher == NULL)
                {
                    SRVECTOR_clear(self->buf_);
                    SearchResult* res = SRVECTOR_copiedData(self->buf_, &count);
                    MATERIALLIST_setData(self->list_, searchResultToMaterialInputData(res, count), 0);
                    MATERIALINPUT_setText(self->input_, L"这可不是一个合法的正则表达式哦");
                    break;
                }

                SRVECTOR_clear(self->buf_);
                StringSkipListIterator it = STRINGSKIPLIST_iteratorBegin(self->data_); // 初始化迭代器

                while (!STRINGSKIPLIST_iteratorEnd(&it))
                {
                    SearchResult data = STRINGSKIPLIST_iteratorGetResult(&it);
                    if (matcher->isMatch(data.key))
                    {
                        SRVECTOR_pushBack(self->buf_, data);
                    }
                    STRINGSKIPLIST_iteratorNext(&it);
                }
                SearchResult* res = SRVECTOR_copiedData(self->buf_, &count);
                MATERIALLIST_setData(self->list_, searchResultToMaterialInputData(res, count), count);
                break;
            }
            }
        }

        if (BUTTON_isClicked(self->del_))
        {
            if (MATERIALLIST_isSelected(self->list_) == true)
            {
                STRINGSKIPLIST_deleteW(self->data_, MATERIALLIST_getSelectedItem(self->list_).key_);
                // 把上面的搬下来
                int count = 0;
                const wchar_t* text = MATERIALINPUT_getText(self->input_);
                if (wcslen(text) == 0)
                {
                    SearchResult* res = STRINGSKIPLIST_getAll(self->data_, &count);
                    MATERIALLIST_setData(self->list_, searchResultToMaterialInputData(res, count), count);
                }
                else
                {
                    SearchResult* res = STRINGSKIPLIST_prefixSearchW(self->data_, text, &count);
                    MATERIALLIST_setData(self->list_, searchResultToMaterialInputData(res, count), count);
                }
            }
        }
        else if (BUTTON_isClicked(self->update_))
        {
            if (MATERIALLIST_isSelected(self->list_) == true)
            {
                SearchResult res;
                STRINGSKIPLIST_searchW(self->data_, MATERIALLIST_getSelectedItem(self->list_).key_, &res);
                
                wchar_t buf[512];
                MATERIALINPUT_setText(insert->input_name_, res.key);
                MATERIALINPUT_setText(insert->input_catagory_, res.value.catagory_);
                MATERIALINPUT_setText(insert->input_publisher_, res.value.publisher_);

                swprintf(buf, 512, L"%lld", res.value.isbn_);
                MATERIALINPUT_setText(insert->input_ISBN_, buf);

                insert->is_update_ = true;
                if (insert->update_key_ != NULL)
                {
                    free(insert->update_key_);
                }
                // 复制字符串
                insert->update_key_ = wcsdup(res.key);
                insert->ori_diff_ = res.value.total_num_ - res.value.num_;
                swprintf(buf, 512, L"%lld", res.value.num_);
                MATERIALINPUT_setText(insert->input_num_, buf);

                STRINGSKIPLIST_deleteW(self->data_, MATERIALLIST_getSelectedItem(self->list_).key_);

                // 把上面的搬下来
                int count = 0;
                const wchar_t* text = MATERIALINPUT_getText(self->input_);
                if (wcslen(text) == 0)
                {
                    SearchResult* res = STRINGSKIPLIST_getAll(self->data_, &count);
                    MATERIALLIST_setData(self->list_, searchResultToMaterialInputData(res, count), count);
                }
                else
                {
                    SearchResult* res = STRINGSKIPLIST_prefixSearchW(self->data_, text, &count);
                    MATERIALLIST_setData(self->list_, searchResultToMaterialInputData(res, count), count);
                }

                SIDEBAR_setStatus(sidebar, SIDEBAR_BUTTON_INSERTBOOK);
            }
        }
        else if (BUTTON_isClicked(self->set_can_rent_))
        {
            if (MATERIALLIST_isSelected(self->list_) == true)
            {
                SearchResult res;
                STRINGSKIPLIST_searchW(self->data_, MATERIALLIST_getSelectedItem(self->list_).key_, &res);
                STRINGSKIPLIST_deleteW(self->data_, res.key);
                res.value.can_rent_ = !res.value.can_rent_;
                STRINGSKIPLIST_insertW(self->data_, res.key, &res.value);
                free(res.key);

                // 把上面的搬下来
                int count = 0;
                const wchar_t* text = MATERIALINPUT_getText(self->input_);
                if (wcslen(text) == 0)
                {
                    SearchResult* res = STRINGSKIPLIST_getAll(self->data_, &count);
                    MATERIALLIST_setData(self->list_, searchResultToMaterialInputData(res, count), count);
                }
                else
                {
                    SearchResult* res = STRINGSKIPLIST_prefixSearchW(self->data_, text, &count);
                    MATERIALLIST_setData(self->list_, searchResultToMaterialInputData(res, count), count);
                }
            }
        }
        else if (BUTTON_isClicked(self->rent_))
        {
            SearchResult res;
            if (STRINGSKIPLIST_searchW(self->data_, MATERIALLIST_getSelectedItem(self->list_).key_, &res))
            {

                if (res.value.can_rent_ == false || res.value.num_ == 0)
                {
                    MATERIALINPUT_setText(self->input_, L"选中的这本书是不可租借的");
                    free(res.key);
                    return;
                }
                else
                {
                    MATERIALINPUT_setText(insert_user->input_book_name_, res.key);
                    free(res.key);

                    SIDEBAR_setStatus(sidebar, SIDEBAR_BUTTON_INSERTUSER);
                }
            }
        }
    }
}

void QUERYBOOK_handleMouseUp(Query* self, int x, int y)
{
    BUTTON_handleMouseUp(self->search_, x, y);
    if (self->status_ == LOGIN_STATUS_ADMIN)
    {
        BUTTON_handleMouseUp(self->update_, x, y);
        BUTTON_handleMouseUp(self->del_, x, y);


        BUTTON_handleMouseUp(self->set_can_rent_, x, y);
        BUTTON_handleMouseUp(self->rent_, x, y);
    }
}

bool QUERYBOOK_isActived(Query* self)
{
    return self->is_active_;
}

void QUERYBOOK_SetStatus(Query* self, bool status)
{
    self->is_active_ = status;
}

void QUERYBOOK_setLoginStatus(Query* self, LoginStatus status)
{
    self->status_ = status;
}

void QUERYBOOK_destroy(Query* self)
{
    MATERIALINPUT_destroy(self->input_);
    MATERIALLIST_destroy(self->list_);
    BUTTON_destroy(self->search_);
    BUTTON_destroy(self->update_);
    BUTTON_destroy(self->del_);
    QUERYTYPE_destroy(self->type_);
}

void QUERYBOOK_handleMouseWheel(Query* self, int wheel)
{
    MATERIALLIST_handleMouseWheel(self->list_, wheel);
}

void QUERYBOOK_handleInput(Query* self, key_msg msg)
{
    MATERIALINPUT_handleInput(self->input_, msg);
}

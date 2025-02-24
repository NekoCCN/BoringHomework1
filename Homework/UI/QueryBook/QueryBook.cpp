#include "QueryBook.h"

Query* QUERYBOOK_createGrey(int x, int y, int width, int height, StringSkipList* data)
{
	Query* self = (Query*)malloc(sizeof(Query));

    self->input_ = MATERIALINPUT_createGrayInput(x + 30, y + height / 10 , width / 2, height / 14, L"输入一些东西吧！");
    self->list_ = MATERIALLIST_createGrayMaterialList(x + 30, y + height / 5, width - 50,
        height - height / 5 - 20, 60, 16);

    self->search_ = BUTTON_createBlueButton((x + 30 + width / 2) + 20, y + height / 10 + 5, width / 10, height / 14 - 10, L"Search");

    self->data_ = data;

    self->update_ = BUTTON_createBlueButton(x + 30 + width / 2 + 220, y + height / 10 + 5, width / 10, height / 14 - 10, L"Update");
    self->del_ = BUTTON_createBlueButton(x + 30 + width / 2 + 330, y + height / 10 + 5, width / 10, height / 14 - 10, L"Delete");

    MATERIALLIST_setData(self->list_, STRINGSKIPLIST_getAll(self->data_, &self->list_->itemCount), self->list_->itemCount);

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

        BUTTON_draw(self->update_);
        BUTTON_draw(self->del_);

        QUERYTYPE_draw(self->type_);
    }
}

void QUERYBOOK_handleMouseMove(Query* self, int x, int y)
{
    if (self->is_active_)
    {
        BUTTON_handleMouseMove(self->search_, x, y);
        BUTTON_handleMouseMove(self->update_, x, y);
        BUTTON_handleMouseMove(self->del_, x, y);

        MATERIALINPUT_handleMouseMove(self->input_, x, y);
        MATERIALLIST_handleMouseMove(self->list_, x, y);

        QUERYTYPE_handleMouseMove(self->type_, x, y);
    }
}

void QUERYBOOK_handleButtonDown(Query* self, int x, int y, Insert* insert, Sidebar* sidebar)
{
    if (self->is_active_)
    {
        BUTTON_handleMouseDown(self->search_, x, y);

        BUTTON_handleMouseDown(self->update_, x, y);
        BUTTON_handleMouseDown(self->del_, x, y);

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
                    MATERIALLIST_setData(self->list_, STRINGSKIPLIST_getAll(self->data_, &count), count);
                }
                else
                {
                    MATERIALLIST_setData(self->list_, STRINGSKIPLIST_prefixSearchW(self->data_, text, &count), count);
                }
            }
            break;
            case QUERYBOOK_TYPE_BUTTON_ENTIRE:
            {
                size_t count = 0;

                const wchar_t* text = MATERIALINPUT_getText(self->input_);
                if (wcslen(text) == 0)
                {
                    int counts = 0;
                    MATERIALLIST_setData(self->list_, STRINGSKIPLIST_getAll(self->data_, &counts), counts);
                }
                else
                {
                    SearchResult data;
                    if (STRINGSKIPLIST_searchW(self->data_, text, &data) == true)
                    {
                        SRVECTOR_clear(self->buf_);
                        SRVECTOR_pushBack(self->buf_, data);
                        MATERIALLIST_setData(self->list_, SRVECTOR_data(self->buf_, &count), 1);
                    }
                    else
                    {
                        SRVECTOR_clear(self->buf_);
                        MATERIALLIST_setData(self->list_, SRVECTOR_data(self->buf_, &count), 0);
                    }
                }
            }
            break;
            case QUERYBOOK_TYPE_BUTTON_SCORE:
            {
                size_t count = 0;
                SRVECTOR_clear(self->buf_);

                const wchar_t* str = MATERIALINPUT_getText(self->input_);
                if (str[0] == L'\0' || str[0] == L'0')
                {
                    int count = 0;
                    MATERIALLIST_setData(self->list_, STRINGSKIPLIST_getAll(self->data_, &count), count);
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
                    MATERIALLIST_setData(self->list_, SRVECTOR_copiedData(self->buf_, &count), 0);
                    MATERIALINPUT_setText(self->input_, L"这可不是一个合法的数字喔");
                    break;
                }

                long long num = wcstoll(str, NULL, 10);

                StringSkipListIterator it = STRINGSKIPLIST_iteratorBegin(self->data_); // 初始化迭代器

                while (!STRINGSKIPLIST_iteratorEnd(&it))
                {
                    SearchResult data = STRINGSKIPLIST_iteratorGetResult(&it);
                    if (data.value.score == num)
                    {
                        SRVECTOR_pushBack(self->buf_, data);
                    }
                    STRINGSKIPLIST_iteratorNext(&it);
                }
                MATERIALLIST_setData(self->list_, SRVECTOR_copiedData(self->buf_, &count), count);
                break;
            }
            // 搬上面的
            case QUERYBOOK_TYPE_BUTTON_TOTAL:
            {
                size_t count = 0;
                SRVECTOR_clear(self->buf_);

                const wchar_t* str = MATERIALINPUT_getText(self->input_);
                if (str[0] == L'\0' || str[0] == L'0')
                {
                    int count = 0;
                    MATERIALLIST_setData(self->list_, STRINGSKIPLIST_getAll(self->data_, &count), count);
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
                    MATERIALLIST_setData(self->list_, SRVECTOR_copiedData(self->buf_, &count), 0);
                    MATERIALINPUT_setText(self->input_, L"这可不是一个合法的数字喔");
                    break;
                }

                long long num = wcstoll(str, NULL, 10);

                StringSkipListIterator it = STRINGSKIPLIST_iteratorBegin(self->data_); // 初始化迭代器

                while (!STRINGSKIPLIST_iteratorEnd(&it))
                {
                    SearchResult data = STRINGSKIPLIST_iteratorGetResult(&it);
                    if (data.value.final_score == num)
                    {
                        SRVECTOR_pushBack(self->buf_, data);
                    }
                    STRINGSKIPLIST_iteratorNext(&it);
                }
                MATERIALLIST_setData(self->list_, SRVECTOR_copiedData(self->buf_, &count), count);
                break;
            }
            case QUERYBOOK_TYPE_BUTTON_ID:
            {
                size_t count = 0;
                SRVECTOR_clear(self->buf_);

                const wchar_t* str = MATERIALINPUT_getText(self->input_);
                if (str[0] == L'\0' || str[0] == L'0')
                {
                    int count = 0;
                    MATERIALLIST_setData(self->list_, STRINGSKIPLIST_getAll(self->data_, &count), count);
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
                    MATERIALLIST_setData(self->list_, SRVECTOR_copiedData(self->buf_, &count), 0);
                    MATERIALINPUT_setText(self->input_, L"这可不是一个合法的数字喔");
                    break;
                }

                long long num = wcstoll(str, NULL, 10);

                StringSkipListIterator it = STRINGSKIPLIST_iteratorBegin(self->data_); // 初始化迭代器

                while (!STRINGSKIPLIST_iteratorEnd(&it))
                {
                    SearchResult data = STRINGSKIPLIST_iteratorGetResult(&it);
                    if (data.value.id == num)
                    {
                        SRVECTOR_pushBack(self->buf_, data);
                    }
                    STRINGSKIPLIST_iteratorNext(&it);
                }
                MATERIALLIST_setData(self->list_, SRVECTOR_copiedData(self->buf_, &count), count);
                break;
            }
            }
        }

        if (BUTTON_isClicked(self->del_))
        {
            if (MATERIALLIST_isSelected(self->list_) == true)
            {
                STRINGSKIPLIST_deleteW(self->data_, MATERIALLIST_getSelectedItem(self->list_).key);
                // 把上面的搬下来
                int count = 0;
                const wchar_t* text = MATERIALINPUT_getText(self->input_);
                if (wcslen(text) == 0)
                {
                    MATERIALLIST_setData(self->list_, STRINGSKIPLIST_getAll(self->data_, &self->list_->itemCount), self->list_->itemCount);
                }
                else
                {
                    MATERIALLIST_setData(self->list_, STRINGSKIPLIST_prefixSearchW(self->data_, text, &count), count);
                }
            }
        }
        else if (BUTTON_isClicked(self->update_))
        {
            if (MATERIALLIST_isSelected(self->list_) == true)
            {
                STRINGSKIPLIST_deleteW(self->data_, MATERIALLIST_getSelectedItem(self->list_).key);

                wchar_t buf[512];
                MATERIALINPUT_setText(insert->input_name_, MATERIALLIST_getSelectedItem(self->list_).key);
                swprintf(buf, 512, L"%lld", MATERIALLIST_getSelectedItem(self->list_).value.id);
                MATERIALINPUT_setText(insert->input_id_, buf);
                swprintf(buf, 512, L"%lld", MATERIALLIST_getSelectedItem(self->list_).value.final_score);
                MATERIALINPUT_setText(insert->input_final_score_, buf);
                swprintf(buf, 512, L"%lld", MATERIALLIST_getSelectedItem(self->list_).value.daily_score);
                MATERIALINPUT_setText(insert->input_daily_score_, buf);
                swprintf(buf, 512, L"%lf", MATERIALLIST_getSelectedItem(self->list_).value.daily_score_ratio);
                MATERIALINPUT_setText(insert->input_daily_score_ratio_, buf);
                swprintf(buf, 512, MATERIALLIST_getSelectedItem(self->list_).value.course);
                MATERIALINPUT_setText(insert->input_course_, buf);

                // 把上面的搬下来
                int count = 0;
                const wchar_t* text = MATERIALINPUT_getText(self->input_);
                if (wcslen(text) == 0)
                {
                    MATERIALLIST_setData(self->list_, STRINGSKIPLIST_getAll(self->data_, &self->list_->itemCount), self->list_->itemCount);
                }
                else
                {
                    MATERIALLIST_setData(self->list_, STRINGSKIPLIST_prefixSearchW(self->data_, text, &count), count);
                }

                SIDEBAR_setStatus(sidebar, SIDEBAR_BUTTON_INSERTBOOK);
            }
        }
    }
}

void QUERYBOOK_handleMouseUp(Query* self, int x, int y)
{
    BUTTON_handleMouseUp(self->search_, x, y);
    BUTTON_handleMouseUp(self->update_, x, y);
    BUTTON_handleMouseUp(self->del_, x, y);
}

bool QUERYBOOK_isActived(Query* self)
{
    return self->is_active_;
}

void QUERYBOOK_SetStatus(Query* self, bool status)
{
    self->is_active_ = status;
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

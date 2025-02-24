#include "Insert.h"

// 这里从WEB前端引用了一个很好的排布方式 可以预留窗口大小变化的实现 但是如果要修改之前的话 那绝对会是个大工程吧
Insert* INSERT_createGrey(int container_x, int container_y, int container_width, int container_height, StringSkipList* data_)
{
    Insert* self = (Insert*)malloc(sizeof(Insert));
    self->data_ = data_;

    float margin_ratio = 0.06;
    int margin = container_height * margin_ratio;

    int column_width = (container_width - 3 * margin) / 2;
    int input_height = (container_height - 4 * margin) / 12;

    // 第一列
    self->input_id_ = MATERIALINPUT_createGrayInput(
        container_x + margin,
        container_y + margin,
        column_width,
        input_height,
        L"ID");

    self->input_name_ = MATERIALINPUT_createGrayInput(
        container_x + margin,
        container_y + margin + input_height + margin,
        column_width,
        input_height,
        L"Name");

    self->input_course_ = MATERIALINPUT_createGrayInput(
        container_x + margin,
        container_y + margin + 2 * (input_height + margin),
        column_width,
        input_height,
        L"Course");

    // 第二列
    self->input_daily_score_ = MATERIALINPUT_createGrayInput(
        container_x + 2 * margin + column_width,
        container_y + margin,
        column_width,
        input_height,
        L"Daily Score");

    self->input_daily_score_ratio_ = MATERIALINPUT_createGrayInput(
        container_x + 2 * margin + column_width,
        container_y + margin + input_height + margin,
        column_width,
        input_height,
        L"Daily Score Ratio");

    self->input_final_score_ = MATERIALINPUT_createGrayInput(
        container_x + 2 * margin + column_width,
        container_y + margin + 2 * (input_height + margin),
        column_width,
        input_height,
        L"Final Score");

    self->button_insert_ = BUTTON_createBlueButton(
        container_x + 2 * margin + column_width,
        container_y + margin + 3 * (input_height + margin),
        column_width,
        input_height,
        L"Insert");

    self->is_active_ = false;

    return self;
}

bool INSERT_isStringNum(const wchar_t* str)
{
    if (str[0] == L'\0')
    {
        return false;
    }

    size_t len = wcslen(str);
    bool valid_number = true;
    for (size_t i = 0; i < len; i++)
    {
        if (!iswdigit(str[i]) && str[i] != L'.')
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

void INSERT_draw(Insert* self)
{
    if (self->is_active_)
    {
        MATERIALINPUT_draw(self->input_id_);
        MATERIALINPUT_draw(self->input_name_);
        MATERIALINPUT_draw(self->input_course_);
        MATERIALINPUT_draw(self->input_daily_score_);
        MATERIALINPUT_draw(self->input_daily_score_ratio_);
        MATERIALINPUT_draw(self->input_final_score_);

        BUTTON_draw(self->button_insert_);
    }
}

void INSERT_handleMouseMove(Insert* self, int x, int y)
{
    if (self->is_active_)
    {
        MATERIALINPUT_handleMouseMove(self->input_id_, x, y);
        MATERIALINPUT_handleMouseMove(self->input_name_, x, y);
        MATERIALINPUT_handleMouseMove(self->input_course_, x, y);
        MATERIALINPUT_handleMouseMove(self->input_daily_score_, x, y);
        MATERIALINPUT_handleMouseMove(self->input_daily_score_ratio_, x, y);
        MATERIALINPUT_handleMouseMove(self->input_final_score_, x, y);

        BUTTON_handleMouseMove(self->button_insert_, x, y);
    }
}

void INSERT_handleMouseDown(Insert* self, int x, int y)
{
    if (self->is_active_)
    {
        BUTTON_handleMouseDown(self->button_insert_, x, y);

        MATERIALINPUT_handleMouseDown(self->input_id_, x, y);
        MATERIALINPUT_handleMouseDown(self->input_name_, x, y);
        MATERIALINPUT_handleMouseDown(self->input_course_, x, y);
        MATERIALINPUT_handleMouseDown(self->input_daily_score_, x, y);
        MATERIALINPUT_handleMouseDown(self->input_daily_score_ratio_, x, y);
        MATERIALINPUT_handleMouseDown(self->input_final_score_, x, y);
    }

    if (BUTTON_isClicked(self->button_insert_))
    {
        if (MATERIALINPUT_getText(self->input_id_) == NULL || MATERIALINPUT_getText(self->input_name_) == NULL ||
            MATERIALINPUT_getText(self->input_course_) == NULL || MATERIALINPUT_getText(self->input_daily_score_) == NULL ||
            MATERIALINPUT_getText(self->input_daily_score_ratio_) == NULL || MATERIALINPUT_getText(self->input_final_score_) == NULL)
        {
            return;
        }

        const wchar_t* id_s = MATERIALINPUT_getText(self->input_id_);
        if (!INSERT_isStringNum(id_s))
        {
            MATERIALINPUT_setText(self->input_id_, L"这可不是一个合法的数字喔");
            return;
        }
        const wchar_t* daily_score_s = MATERIALINPUT_getText(self->input_daily_score_);
        if (!INSERT_isStringNum(daily_score_s))
        {
            MATERIALINPUT_setText(self->input_daily_score_, L"这可不是一个合法的数字喔");
            return;
        }
        const wchar_t* daily_score_ratio_s = MATERIALINPUT_getText(self->input_daily_score_ratio_);
        if (!INSERT_isStringNum(daily_score_ratio_s))
        {
            MATERIALINPUT_setText(self->input_daily_score_ratio_, L"这可不是一个合法的小数喔（不支持E表示）");
            return;
        }
        const wchar_t* final_score_s = MATERIALINPUT_getText(self->input_final_score_);
        if (!INSERT_isStringNum(final_score_s))
        {
            MATERIALINPUT_setText(self->input_final_score_, L"这可不是一个合法的数字喔");
            return;
        }

        const wchar_t* name = MATERIALINPUT_getText(self->input_name_);
        const wchar_t* course = MATERIALINPUT_getText(self->input_course_);

        long long id = wcstoll(id_s, NULL, 10);
        long long daily_score = wcstoll(daily_score_s, NULL, 10);
        double daily_score_ratio = wcstod(daily_score_ratio_s, NULL);
        long long final_score = wcstoll(final_score_s, NULL, 10);

        SearchResult res;
        bool st = STRINGSKIPLIST_searchW(self->data_, name, &res);
        if (st)
        {
            MATERIALINPUT_setText(self->input_name_, L"系统内有重复名称 尝试给它编号 如123(1)与123(2)或123_1与123_2");
            return;
        }
        if (st)
        {
            free(res.key);
        }

        long long score = daily_score * daily_score_ratio + final_score;

        if (wcslen(course) > 32)
        {
            MATERIALINPUT_setText(self->input_course_, L"课程名太长(>32)");
            return;
        }

        BaseData data = {};

        data.id = id;
        data.daily_score = daily_score;
        data.daily_score_ratio = daily_score_ratio;
        data.final_score = final_score;
        data.score = score;
        wcscpy(data.course, course);

        STRINGSKIPLIST_insertW(self->data_, name, &data);

        MATERIALINPUT_setText(self->input_id_, L"");
        MATERIALINPUT_setText(self->input_name_, L"");
        MATERIALINPUT_setText(self->input_course_, L"");
        MATERIALINPUT_setText(self->input_daily_score_, L"");
        MATERIALINPUT_setText(self->input_daily_score_ratio_, L"");
        MATERIALINPUT_setText(self->input_final_score_, L"");
    }
}

void INSERT_handleMouseUp(Insert* self, int x, int y)
{
    if (self->is_active_)
    {
        BUTTON_handleMouseUp(self->button_insert_, x, y);
    }
}

bool INSERT_isActived(Insert* self)
{
    return self->is_active_;
}

void INSERT_SetStatus(Insert* self, bool status)
{
    self->is_active_ = status;
}

void INSERT_destroy(Insert* self)
{
    MATERIALINPUT_destroy(self->input_id_);
    MATERIALINPUT_destroy(self->input_name_);
    MATERIALINPUT_destroy(self->input_course_);
    MATERIALINPUT_destroy(self->input_daily_score_);
    MATERIALINPUT_destroy(self->input_daily_score_ratio_);
    MATERIALINPUT_destroy(self->input_final_score_);

    BUTTON_destroy(self->button_insert_);
}

void INSERT_handleInput(Insert* self, key_msg msg)
{
    if (self->is_active_)
    {
        MATERIALINPUT_handleInput(self->input_id_, msg);
        MATERIALINPUT_handleInput(self->input_name_, msg);
        MATERIALINPUT_handleInput(self->input_course_, msg);
        MATERIALINPUT_handleInput(self->input_daily_score_, msg);
        MATERIALINPUT_handleInput(self->input_daily_score_ratio_, msg);
        MATERIALINPUT_handleInput(self->input_final_score_, msg);
    }
}

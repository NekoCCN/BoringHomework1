#include "QueryUserType.h"

static ListButton* QUERYUSERTYPE_getStatusButton(QueryUserType* self)
{
    switch (self->status_)
    {
        case QUERYUSER_TYPE_BUTTON_ENTIRE:
            return self->button_entire_;
        case QUERYUSER_TYPE_BUTTON_ID:
            return self->button_id_;
        case QUERYUSER_TYPE_BUTTON_BOOKNAME_ENTIRE:
            return self->button_bookname_entire_;
        case QUERYUSER_TYPE_TIME:
            return self->button_time_;
    }
}
QueryUserType* QUERYUSERTYPE_create(int x, int y, int width, int radius)
{
    color_t background_color = EGERGB(0xF5, 0xF5, 0xF5);
    color_t text_color = EGERGB(0x00, 0x00, 0x00);

    color_t hover_color = darkenColor(background_color, 0.9f);
    color_t active_color = darkenColor(background_color, 0.8f);

    QueryUserType* self = (QueryUserType*)malloc(sizeof(QueryUserType));
    if (!self)
    {
        return NULL;
    }

    self->color_ = EGERGB(0xF5, 0xF5, 0xF5);
    self->status_ = QUERYUSER_TYPE_BUTTON_ENTIRE;

    int button_height = 30;
    int small_spacing = 10;
    int big_spacing = 60;
    int padding = 10;

    int button_width = width / 10;

    int button_left = x + 30;

    self->button_entire_ = LISTBUTTON_create(button_left, y, button_left + button_width, y + button_height
        , "完整名称", background_color, hover_color, active_color, text_color,
        false, WHITE, radius, padding);
    if (!self->button_entire_)
    {
        free(self);
        return NULL;
    }

    self->button_bookname_entire_ = LISTBUTTON_create(button_left + button_width + small_spacing, y, button_left + button_width + small_spacing + button_width,
        y + button_height, "完整书名",
        background_color, hover_color, active_color, text_color,
        false, WHITE, radius, padding);
    if (!self->button_bookname_entire_)
    {
        free(self->button_entire_);
        free(self);
        return NULL;
    }

    self->button_regex_ = LISTBUTTON_create(button_left + button_width * 2 + small_spacing + big_spacing, y, button_left + button_width * 2 + small_spacing * 1 + button_width + big_spacing,
        y + button_height, "正则（模糊）",
        background_color, hover_color, active_color, text_color,
        false, WHITE, radius, padding);
    if (!self->button_regex_)
    {
        free(self->button_bookname_entire_);
        free(self->button_entire_);
        free(self);
        return NULL;
    }

    self->button_time_ = LISTBUTTON_create(button_left + button_width * 3 + small_spacing * 2 + big_spacing, y, button_left + button_width * 3 + small_spacing * 2 + button_width + big_spacing,
        y + button_height, "逾期未还",
        background_color, hover_color, active_color, text_color,
        false, WHITE, radius, padding);
    if (!self->button_time_)
    {
        free(self->button_regex_);
        free(self->button_entire_);
        free(self->button_bookname_entire_);
        free(self);
        return NULL;
    }

    self->button_id_ = LISTBUTTON_create(button_left + button_width * 4 + small_spacing * 3 + big_spacing, y, button_left + button_width * 4 + small_spacing * 3 + button_width + big_spacing,
        y + button_height, "ID",
        background_color, hover_color, active_color, text_color,
        false, WHITE, radius, padding);

    LISTBUTTON_SetStatus(self->button_entire_, true);

    return self;
}

void QUERYUSERTYPE_draw(QueryUserType* self)
{
    int small_spacing = 10;
    int big_spacing = 30;
    int button_height = 30;
    int button_width = self->width_ / 10;
    int button_left = self->x_ + 30;

    setfillcolor(hexToColorref("#F5F5F5"));
    fillroundrect(button_left + button_width * 2 + small_spacing + big_spacing - 10,
        self->y_ - 3,
        button_left + button_width * 4 + small_spacing * 3 + big_spacing + 10,
        self->y_ + self->height_ + 3, 10);
    setcolor(hexToColorref("#F5F5F5"));
    roundrect(button_left + button_width * 2 + small_spacing + big_spacing - 10,
        self->y_ - 3,
        button_left + button_width * 4 + small_spacing * 3 + big_spacing + 10,
        self->y_ + self->height_ + 3, 10);

    LISTBUTTON_draw(self->button_bookname_entire_);
    LISTBUTTON_draw(self->button_entire_);
    LISTBUTTON_draw(self->button_id_);
    LISTBUTTON_draw(self->button_time_);
    LISTBUTTON_draw(self->button_regex_);
}

void QUERYUSERTYPE_handleMouseMove(QueryUserType* self, int x, int y)
{
    LISTBUTTON_handleMouseMove(self->button_bookname_entire_, x, y);
    LISTBUTTON_handleMouseMove(self->button_entire_, x, y);
    LISTBUTTON_handleMouseMove(self->button_id_, x, y);
    LISTBUTTON_handleMouseMove(self->button_time_, x, y);
    LISTBUTTON_handleMouseMove(self->button_regex_, x, y);
}

void QUERYUSERTYPE_handleButtonDown(QueryUserType* self, int x, int y)
{
    QueryUserTypeButtonType status = self->status_;

    LISTBUTTON_handleButtonDown(self->button_entire_, x, y);
    if (LISTBUTTON_isActived(self->button_entire_) && self->status_ != QUERYUSER_TYPE_BUTTON_ENTIRE)
    {
        status = QUERYUSER_TYPE_BUTTON_ENTIRE;
    }

    LISTBUTTON_handleButtonDown(self->button_bookname_entire_, x, y);
    if (LISTBUTTON_isActived(self->button_bookname_entire_) && self->status_ != QUERYUSER_TYPE_BUTTON_BOOKNAME_ENTIRE)
    {
        status = QUERYUSER_TYPE_BUTTON_BOOKNAME_ENTIRE;
    }

    LISTBUTTON_handleButtonDown(self->button_id_, x, y);
    if (LISTBUTTON_isActived(self->button_id_) && self->status_ != QUERYUSER_TYPE_BUTTON_ID)
    {
        status = QUERYUSER_TYPE_BUTTON_ID;
    }

    LISTBUTTON_handleButtonDown(self->button_time_, x, y);
    if (LISTBUTTON_isActived(self->button_time_) && self->status_ != QUERYUSER_TYPE_TIME)
    {
        status = QUERYUSER_TYPE_TIME;
    }

    LISTBUTTON_handleButtonDown(self->button_regex_, x, y);
    if (LISTBUTTON_isActived(self->button_regex_) && self->status_ != QUERYUSER_TYPE_REGEX)
    {
        status = QUERYUSER_TYPE_REGEX;
    }

    if (status != self->status_)
    {
        LISTBUTTON_SetStatus(QUERYUSERTYPE_getStatusButton(self), false);
        self->status_ = status;
    }
}

QueryUserTypeButtonType QUERYUSERTYPE_getStatus(QueryUserType* self)
{
    return self->status_;
}

void QUERYUSERTYPE_destroy(QueryUserType* self)
{
    LISTBUTTON_destroy(self->button_bookname_entire_);
    LISTBUTTON_destroy(self->button_entire_);
    LISTBUTTON_destroy(self->button_id_);
    LISTBUTTON_destroy(self->button_time_);
    free(self);
}
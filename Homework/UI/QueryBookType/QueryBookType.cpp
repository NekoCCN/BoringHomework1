#include "QueryBookType.h"

static ListButton* QUERYTYPE_getStatusButton(QueryType* self)
{
    switch (self->status_)
    {
    case QUERYBOOK_TYPE_BUTTON_ENTIRE:
        return self->button_entire_;
    case QUERYBOOK_TYPE_BUTTON_ID:
        return self->button_id_;
    case QUERYBOOK_TYPE_BUTTON_PREFIX:
        return self->button_prefix_;
    case QUERYBOOK_TYPE_BUTTON_SCORE:
        return self->button_score_;
    case QUERYBOOK_TYPE_BUTTON_TOTAL:
        return self->button_total_;
    default:
        return NULL;
    }
}
QueryType* QUERYTYPE_create(int x, int y, int width, int radius)
{
    color_t background_color = EGERGB(0xF5, 0xF5, 0xF5);
    color_t text_color = EGERGB(0x00, 0x00, 0x00);

    color_t hover_color = darkenColor(background_color, 0.9f);
    color_t active_color = darkenColor(background_color, 0.8f);

    QueryType* self = (QueryType*)malloc(sizeof(QueryType));
    if (!self)
    {
        return NULL;
    }

    self->color_ = EGERGB(0xF5, 0xF5, 0xF5);
    self->status_ = QUERYBOOK_TYPE_BUTTON_PREFIX;

    int button_height = 30;
    int small_spacing = 10;
    int big_spacing = 60;
    int padding = 10;

    int button_width = width / 10;

    int button_left = x + 30;

    self->button_prefix_ = LISTBUTTON_create(button_left, y, button_left + button_width, y + button_height, "前缀（较快）",
        background_color, hover_color, active_color, text_color,
        false, WHITE, radius, padding);
    if (!self->button_prefix_)
    {
        free(self->button_prefix_);
        free(self);
        return NULL;
    }

    self->button_entire_ = LISTBUTTON_create(button_left + button_width + small_spacing, y, button_left + button_width + small_spacing + button_width,
        y + button_height, "完整（较快）",
        background_color, hover_color, active_color, text_color,
        false, WHITE, radius, padding);
    if (!self->button_entire_)
    {
        free(self->button_prefix_);
        free(self->button_entire_);
        free(self);
        return NULL;
    }

    self->button_score_ = LISTBUTTON_create(button_left + button_width * 2 + small_spacing + big_spacing, y, button_left + button_width * 2 + small_spacing * 1 + button_width + big_spacing,
        y + button_height, "单项分数",
        background_color, hover_color, active_color, text_color,
        false, WHITE, radius, padding);
    if (!self->button_score_)
    {
        free(self->button_prefix_);
        free(self->button_entire_);
        free(self->button_score_);
        free(self);
        return NULL;
    }

    self->button_total_ = LISTBUTTON_create(button_left + button_width * 3 + small_spacing * 2 + big_spacing, y, button_left + button_width * 3 + small_spacing * 2 + button_width + big_spacing,
        y + button_height, "总分",
        background_color, hover_color, active_color, text_color,
        false, WHITE, radius, padding);
    if (!self->button_total_)
    {
        free(self->button_prefix_);
        free(self->button_entire_);
        free(self->button_score_);
        free(self->button_total_);
        free(self);
        return NULL;
    }

    self->button_id_ = LISTBUTTON_create(button_left + button_width * 4 + small_spacing * 3 + big_spacing, y, button_left + button_width * 4 + small_spacing * 3 + button_width + big_spacing,
        y + button_height, "ID",
        background_color, hover_color, active_color, text_color,
        false, WHITE, radius, padding);
    if (!self->button_id_)
    {
        free(self->button_prefix_);
        free(self->button_entire_);
        free(self->button_score_);
        free(self->button_total_);
        free(self->button_id_);
        free(self);
        return NULL;
    }

    LISTBUTTON_SetStatus(self->button_prefix_, true);

    return self;
}

void QUERYTYPE_draw(QueryType* self)
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

    LISTBUTTON_draw(self->button_prefix_);
    LISTBUTTON_draw(self->button_entire_);
    LISTBUTTON_draw(self->button_score_);
    LISTBUTTON_draw(self->button_total_);
    LISTBUTTON_draw(self->button_id_);
}

void QUERYTYPE_handleMouseMove(QueryType* self, int x, int y)
{
    LISTBUTTON_handleMouseMove(self->button_prefix_, x, y);
    LISTBUTTON_handleMouseMove(self->button_entire_, x, y);
    LISTBUTTON_handleMouseMove(self->button_score_, x, y);
    LISTBUTTON_handleMouseMove(self->button_total_, x, y);
    LISTBUTTON_handleMouseMove(self->button_id_, x, y);
}

void QUERYTYPE_handleButtonDown(QueryType* self, int x, int y)
{
    QueryTypeButtonType status = self->status_;

    LISTBUTTON_handleButtonDown(self->button_prefix_, x, y);
    if (LISTBUTTON_isActived(self->button_prefix_) && self->status_ != QUERYBOOK_TYPE_BUTTON_PREFIX)
    {
        status = QUERYBOOK_TYPE_BUTTON_PREFIX;
    }

    LISTBUTTON_handleButtonDown(self->button_entire_, x, y);
    if (LISTBUTTON_isActived(self->button_entire_) && self->status_ != QUERYBOOK_TYPE_BUTTON_ENTIRE)
    {
        status = QUERYBOOK_TYPE_BUTTON_ENTIRE;
    }

    LISTBUTTON_handleButtonDown(self->button_score_, x, y);
    if (LISTBUTTON_isActived(self->button_score_) && self->status_ != QUERYBOOK_TYPE_BUTTON_SCORE)
    {
        status = QUERYBOOK_TYPE_BUTTON_SCORE;
    }

    LISTBUTTON_handleButtonDown(self->button_total_, x, y);
    if (LISTBUTTON_isActived(self->button_total_) && self->status_ != QUERYBOOK_TYPE_BUTTON_TOTAL)
    {
        status = QUERYBOOK_TYPE_BUTTON_TOTAL;
    }

    LISTBUTTON_handleButtonDown(self->button_id_, x, y);
    if (LISTBUTTON_isActived(self->button_id_) && self->status_ != QUERYBOOK_TYPE_BUTTON_ID)
    {
        status = QUERYBOOK_TYPE_BUTTON_ID;
    }

    if (status != self->status_)
    {
        LISTBUTTON_SetStatus(QUERYTYPE_getStatusButton(self), false);
        self->status_ = status;
    }
}

QueryTypeButtonType QUERYTYPE_getStatus(QueryType* self)
{
    return self->status_;
}

void QUERYTYPE_destroy(QueryType* self)
{
    LISTBUTTON_destroy(self->button_prefix_);
    LISTBUTTON_destroy(self->button_entire_);
    LISTBUTTON_destroy(self->button_score_);
    LISTBUTTON_destroy(self->button_total_);
    LISTBUTTON_destroy(self->button_id_);
    free(self);
}
#include "MaterialInput.h"

// 获取起始到光标的文本宽度
int MATERIALINPUT_getTextWidthUpToCursor(MaterialInput* input, int position)
{
    if (position <= 0)
    {
        return 0;
    }

    wchar_t* temp_text = (wchar_t*)malloc((position + 1) * sizeof(wchar_t));
    if (!temp_text)
    {
        return 0;
    }
    wcsncpy(temp_text, input->text_, position);

    temp_text[position] = L'\0';

    int width = textwidth(temp_text);

    free(temp_text);
    return width;
}

int MATERIALINPUT_calculateCursorX(MaterialInput* input)
{
    int text_width_up_to_cursor = MATERIALINPUT_getTextWidthUpToCursor(input, input->cursor_position_);

    int scrolled_text_width = MATERIALINPUT_getTextWidthUpToCursor(input, input->scroll_offset_);

    // 提供一个恰到好处的偏移 比如5 perfect
    return input->x_ + 5 + (text_width_up_to_cursor - scrolled_text_width);
}

void MATERIALINPUT_adjustScrollOffset(MaterialInput* input)
{
    int cursor_x = MATERIALINPUT_calculateCursorX(input);
    int padding = 5;
    int visible_left = input->x_ + padding;
    int visible_right = input->x_ + input->width_ - padding;

    if (cursor_x > visible_right)
    {
        while (MATERIALINPUT_calculateCursorX(input) > visible_right)
        {
            if (input->scroll_offset_ < wcslen(input->text_))
            {
                input->scroll_offset_++;
            }
            // 这是一个循环承诺
            else
            {
                break;
            }
        }
    }
    else if (cursor_x < visible_left)
    {
        while (MATERIALINPUT_calculateCursorX(input) < visible_left)
        {
            if (input->scroll_offset_ > 0)
            {
                input->scroll_offset_--;
            }
            else
            {
                break;
            }
        }
    }

    //// 这些也是承诺 理论上如果做的好可以不需要的
    //input->scroll_offset_ = max(input->scroll_offset_, 0);
    //if (input->scroll_offset_ > wcslen(input->text_))
    //{
    //    input->scroll_offset_ = wcslen(input->text_);
    //}
}

MaterialInput* MATERIALINPUT_createGrayInput(int x, int y, int width, int height, const wchar_t* placeholder)
{
    return MATERIALINPUT_create(x, y, width, height, placeholder,
        hexToColorref("#F5F5F5"),
        hexToColorref("#000000"),
        hexToColorref("#000000"),
        hexToColorref("#808080"),
        hexToColorref("#E0E0E0"),
        hexToColorref("#A0A0A0"),
        hexToColorref("#808080")
    );
}

MaterialInput* MATERIALINPUT_createBlueInput(int x, int y, int width, int height, const wchar_t* placeholder)
{
    return MATERIALINPUT_create(x, y, width, height, placeholder,
        hexToColorref("#FFFFFF"),
        hexToColorref("#000000"),
        hexToColorref("#2196F3"),
        hexToColorref("#2196F3"),
        hexToColorref("#1976D2"),
        hexToColorref("#64B5F6"),
        hexToColorref("#64B5F6")
    );
}

MaterialInput* MATERIALINPUT_create(int x, int y, int width, int height, const wchar_t* placeholder,
    color_t background_color, color_t text_color,
    color_t cursor_color, color_t focus_border_color, color_t unfocus_border_color,
    color_t hover_border_color,
    color_t placeholder_text_color)
{
    MaterialInput* input = (MaterialInput*)malloc(sizeof(MaterialInput));
    if (!input)
    {
        return NULL;
    }

    input->x_ = x;
    input->y_ = y;
    input->width_ = width;
    input->height_ = height;

    input->buffer_size_ = 512;

    input->text_ = (wchar_t*)malloc(input->buffer_size_ * sizeof(wchar_t));
    if (!input->text_)
    {
        free(input);
        return NULL;
    }
    input->visible_text_buffer_ = (wchar_t*)malloc(input->buffer_size_ * sizeof(wchar_t));
    if (!input->visible_text_buffer_)
    {
        free(input->text_);
        free(input);
        return NULL;
    }

    input->text_[0] = L'\0';

    input->cursor_position_ = 0;
    input->scroll_offset_ = 0;

    input->has_focus_ = false;
    input->is_hovered_ = false;

    input->background_color_ = background_color;
    input->text_color_ = text_color;
    input->cursor_color_ = cursor_color;
    input->focus_border_color_ = focus_border_color;
    input->unfocus_border_color_ = unfocus_border_color;
    input->hover_border_color_ = hover_border_color;

    input->placeholder_text_color_ = placeholder_text_color;
    input->placeholder_ = (wchar_t*)malloc((wcslen(placeholder) + 1) * sizeof(wchar_t));
    if (!input->placeholder_)
    {
        free(input->text_);
        free(input);
        return NULL;
    }
    wcscpy(input->placeholder_, placeholder);

    input->hWnd = getHWnd();
    // 不可以使用Windows API来获取HDC喔 不然和ege的API调用不是一个HDC 窗口的HDC和HWnd不一样 不是唯一的
    // input->hdc = GetDC(input->hWnd);
    input->hdc = getHDC();

    // 要和下面的圆角矩形匹配喔 不过这里我不考虑做复用 所以不提供圆角率的自定义
    input->hRgn = CreateRoundRectRgn(input->x_, input->y_, input->x_ + input->width_ - 5, input->y_ + input->height_, 10, 10);

    return input;
}

void MATERIALINPUT_destroy(MaterialInput* input)
{
    if (input)
    {
        free(input->text_);
        free(input->placeholder_);
        free(input);
    }
}

void MATERIALINPUT_resizeBuffer(MaterialInput* input)
{
    input->buffer_size_ *= 2; // 加倍缓冲区大小
    wchar_t* new_buffer = (wchar_t*)realloc(input->text_, input->buffer_size_ * sizeof(wchar_t));
    if (new_buffer)
    {
        input->text_ = new_buffer;
    }
}

void MATERIALINPUT_draw(MaterialInput* input)
{
    color_t border_color;
    if (input->has_focus_)
    {
        border_color = input->focus_border_color_;
    }
    else if (input->is_hovered_)
    {
        border_color = input->hover_border_color_;
    }
    else
    {
        border_color = input->unfocus_border_color_;
    }

    setfillcolor(border_color);
    fillroundrect(input->x_, input->y_, input->x_ + input->width_, input->y_ + input->height_, 10, 10);
    setcolor(border_color);
    roundrect(input->x_, input->y_, input->x_ + input->width_, input->y_ + input->height_, 10, 10);

    setfillcolor(input->background_color_);
    fillroundrect(input->x_ + 2, input->y_ + 2, input->x_ + input->width_ - 2, input->y_ + input->height_ - 2, 8, 8);
    setcolor(input->background_color_);
    roundrect(input->x_ + 2, input->y_ + 2, input->x_ + input->width_ - 2, input->y_ + input->height_ - 2, 8, 8);

    setcolor(input->text_color_);
    setbkmode(TRANSPARENT);

    int textX = input->x_ + 5;
    // 垂直居中
    int textY = input->y_ + (input->height_ - textheight(L"A")) / 2;

    // 截取可见区域的文本 出于字体的搞不懂的复杂性 比如Kerning会导致多个字符可能具有不确认的宽度 所以我们只考虑左边的字符 右边利用Windows API截断
    SelectClipRgn(input->hdc, input->hRgn);

    wcscpy(input->visible_text_buffer_, &input->text_[input->scroll_offset_]);
    const wchar_t* drawText = input->visible_text_buffer_;
    if (input->text_[0] == L'\0')
    {
        drawText = input->placeholder_;
        setcolor(input->placeholder_text_color_);
    }
    outtextxy(textX, textY, drawText);

    // 记得重置 不然爆炸了
    SelectClipRgn(input->hdc, NULL);

    // 绘制光标
    if (input->has_focus_)
    {
        int cursorX = MATERIALINPUT_calculateCursorX(input);
        // 一个小小的坐标系变换 有点抽象
        int cursorY1 = input->y_ + (input->height_ - textheight(L"A")) / 2;
        int cursorY2 = cursorY1 + textheight(L"A");

        if (GetTickCount() / 500 % 2 == 0)
        {
            setcolor(input->cursor_color_);
            line(cursorX, cursorY1, cursorX, cursorY2);
        }
    }
}

void MATERIALINPUT_handleKeyDown(MaterialInput* input, key_msg msg)
{
    if (!input->has_focus_) return;

    switch (msg.key)
    {
    case key_left:
        if (input->cursor_position_ > 0)
        {
            input->cursor_position_--;
            MATERIALINPUT_adjustScrollOffset(input);
        }
        break;
    case key_right:
        if (input->cursor_position_ < wcslen(input->text_))
        {
            input->cursor_position_++;
            MATERIALINPUT_adjustScrollOffset(input);
        }
        break;
    case key_back:
        if (input->cursor_position_ > 0)
        {
            size_t text_len = wcslen(input->text_);
            // 低效的 剔除字符的方法 但是没什么办法呢
            wmemmove(&input->text_[input->cursor_position_ - 1],
                &input->text_[input->cursor_position_],
                text_len - input->cursor_position_ + 1);
            input->cursor_position_--;
            MATERIALINPUT_adjustScrollOffset(input);
        }
        break;
    case key_delete:
        if (input->cursor_position_ < wcslen(input->text_))
        {
            size_t text_len = wcslen(input->text_);
            // 低效的 剔除字符的方法 但是没什么办法呢 但是不改变光标 和上面的基本一致
            wmemmove(&input->text_[input->cursor_position_],
                &input->text_[input->cursor_position_ + 1],
                text_len - input->cursor_position_ + 1);
            MATERIALINPUT_adjustScrollOffset(input);
        }
        break;
    // 本来不太想管其他键呢 但是这两个键很简单呐
    case key_home:
        input->cursor_position_ = 0;
        MATERIALINPUT_adjustScrollOffset(input);
        break;
    case key_end:
        input->cursor_position_ = wcslen(input->text_);
        MATERIALINPUT_adjustScrollOffset(input);
        break;
    default:
        break;
    }
}

void MATERIALINPUT_handleChar(MaterialInput* input, key_msg msg)
{
    if (!input->has_focus_) return;

    // ASCII可见字符 不处理额外键 不然会出现一堆不好看的小方框 而且缓冲区会爆炸
    if (msg.key >= 32)
    {
        if (wcslen(input->text_) + 2 >= input->buffer_size_)
        {
            MATERIALINPUT_resizeBuffer(input);
        }

        // 将字符插入到光标位置
        wmemmove(&input->text_[input->cursor_position_ + 1],
            &input->text_[input->cursor_position_],
            wcslen(input->text_) - input->cursor_position_ + 1);
        input->text_[input->cursor_position_] = (wchar_t)msg.key;
        input->cursor_position_++;

        MATERIALINPUT_adjustScrollOffset(input); // 调整滚动偏移
    }
}

//鼠标点击事件
void MATERIALINPUT_handleMouseDown(MaterialInput* input, int x, int y)
{
    if (x >= input->x_ && x <= input->x_ + input->width_ &&
        y >= input->y_ && y <= input->y_ + input->height_)
    {
        input->has_focus_ = true;

        int clickedPos = 0;
        int scrolled_text_width = MATERIALINPUT_getTextWidthUpToCursor(input, input->scroll_offset_);
        // 要按内间距哦
        int relative_mouse_x = x - input->x_ - 5 + scrolled_text_width;

        for (int i = 0; i < wcslen(input->text_); ++i)
        {
            wchar_t temp[2] = { input->text_[i], L'\0' };
            if (MATERIALINPUT_getTextWidthUpToCursor(input, i) + textwidth(temp) > relative_mouse_x)
            {
                clickedPos = i;
                break;
            }
            clickedPos = i + 1;
        }

        input->cursor_position_ = clickedPos;
        MATERIALINPUT_adjustScrollOffset(input);
    }
    else
    {
        MATERIALINPUT_setFocus(input, false);
    }
}

void MATERIALINPUT_handleMouseMove(MaterialInput* input, int mouseX, int mouseY)
{
    input->is_hovered_ = mouseX >= input->x_ && mouseX <= input->x_ + input->width_ &&
	    mouseY >= input->y_ && mouseY <= input->y_ + input->height_;
}

void MATERIALINPUT_setFocus(MaterialInput* input, bool focus)
{
    if (input->has_focus_ != focus)
    {
        input->has_focus_ = focus;
        if (!focus)
        {
            input->is_hovered_ = false;
        }
    }
}

const wchar_t* MATERIALINPUT_getText(MaterialInput* input)
{
    return input->text_;
}

// 可能有用的接口 比如在输错的时候写一个有趣的错误提示
void MATERIALINPUT_setText(MaterialInput* input, const wchar_t* text)
{
    input->text_[0] = L'\0';
    input->cursor_position_ = 0;

    size_t len = wcslen(text);
    if (len >= input->buffer_size_)
    {
        MATERIALINPUT_resizeBuffer(input);
    }
    wcscpy(input->text_, text);
    input->cursor_position_ = len;
    input->scroll_offset_ = 0;
    MATERIALINPUT_adjustScrollOffset(input);
}

void MATERIALINPUT_handleInput(MaterialInput* input, key_msg msg)
{
    if (!input)
    {
        return;
    }
    switch (msg.msg)
    {
    case key_msg_down:
        MATERIALINPUT_handleKeyDown(input, msg);
        break;
    case key_msg_up:
        break;
    case key_msg_char:
        MATERIALINPUT_handleChar(input, msg);
        break;
    }
}
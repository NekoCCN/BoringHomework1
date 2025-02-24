#include "Tools.h"

bool checkHexColor(const char* hex_str)
{
    // Cyan:#00FFFF
    if (hex_str == NULL || hex_str[0] != '#' || strlen(hex_str) != 7)
    {
        return false;
    }
    for (int i = 1; i < 7; i++)
    {
        if (!isxdigit(hex_str[i])) 
        {
            return false;
        }
    }
    return true;
}

color_t darkenColor(color_t color, float factor)
{
    int r = (int)(GetRValue(color) * factor);
    int g = (int)(GetGValue(color) * factor);
    int b = (int)(GetBValue(color) * factor);

    r = (r < 0) ? 0 : ((r > 255) ? 255 : r);
    g = (g < 0) ? 0 : ((g > 255) ? 255 : g);
    b = (b < 0) ? 0 : ((b > 255) ? 255 : b);

    return EGERGB(r, g, b);
}

// 将十六进制 RGB 字符串转换为 COLORREF
COLORREF hexToColorref(const char* hex_str)
{
    // C语言没有异常处理还是有点难受
    if (!checkHexColor(hex_str))
    {
        return BLACK;
    }

    // 使用 strtol 将字符串转换为长整型
    char* endptr;
    long rgb = strtol(hex_str + 1, &endptr, 16); // +1 跳过 '#'

    if (*endptr != '\0')
    {
        return BLACK;
    }

    // 分离RGB分量 RGB每一个分量各占8位
    int r = (rgb >> 16) & 0xFF;
    int g = (rgb >> 8) & 0xFF;
    int b = rgb & 0xFF;

    return EGERGB(r, g, b);
}
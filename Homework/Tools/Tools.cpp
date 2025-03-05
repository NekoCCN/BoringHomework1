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

// MurmurHash3算法 也是G++的哈希算法 用来实现密码加密
// 参考自其他实现
uint32_t hash_murmur3(const void* data, size_t length)
{
    const uint8_t* bytes = (const uint8_t*)data;
    const uint32_t seed = 0;
    const uint32_t c1 = 0xcc9e2d51;
    const uint32_t c2 = 0x1b873593;

    uint32_t h1 = seed;

    const uint32_t* blocks = (const uint32_t*)(bytes - ((size_t)bytes & 3));
    size_t blockwords = length >> 2;

    for (size_t i = 0; i < blockwords; i++) {
        uint32_t k1 = blocks[i];

        k1 *= c1;
        k1 = (k1 << 15) | (k1 >> 17);
        k1 *= c2;

        h1 ^= k1;
        h1 = (h1 << 13) | (h1 >> 19);
        h1 = h1 * 5 + 0xe6546b64;
    }

    const uint8_t* tail = bytes + (blockwords << 2);
    uint32_t k1 = 0;
    switch (length & 3)
    {
    case 3:
        k1 ^= tail[2] << 16;
        /* fall through */
    case 2:
        k1 ^= tail[1] << 8;
        /* fall through */
    case 1:
        k1 ^= tail[0];
        k1 *= c1;
        k1 = (k1 << 15) | (k1 >> 17);
        k1 *= c2;
        h1 ^= k1;
    }

    h1 ^= length;
    h1 ^= h1 >> 16;
    h1 *= 0x85ebca6b;
    h1 ^= h1 >> 13;
    h1 *= 0xc2b2ae35;
    h1 ^= h1 >> 16;

    return h1;
}

// 静态的 用于构建KMP的最长公共前后缀数组 实现下面的KMP算法
static void computeNextArrayWchar(const wchar_t* pattern, int m, int* next)
{
    next[0] = 0;
    int length = 0;
    for (int i = 1; i < m; i++)
    {
        while (length > 0 && pattern[i] != pattern[length])
        {
            length = next[length - 1];
        }
        if (pattern[i] == pattern[length])
        {
            length++;
        }
        next[i] = length;
    }
}

// KMP算法
int kmpSearchWchar(const wchar_t* text, const wchar_t* pattern)
{
    int n = wcslen(text);
    int m = wcslen(pattern);

    if (m == 0) return 0;
    if (n == 0) return -1;

    int* next = (int*)malloc(sizeof(int) * m);
    if (!next)
    {
        perror("malloc failed");
        exit(1);
    }
    computeNextArrayWchar(pattern, m, next);

    int i = 0, j = 0;
    while (i < n)
    {
        if (text[i] == pattern[j])
        {
            i++; j++;
        }
        if (j == m)
        {
            free(next);
            return i - j;
        }
        else if (i < n && text[i] != pattern[j])
        {
            if (j != 0)
            {
                j = next[j - 1];
            }
            else
            {
                i++;
            }
        }
    }
    free(next);
    return -1;
}
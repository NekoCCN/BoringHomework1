#include "Date.h"
#include <time.h>
#include <stdbool.h>
#include <stdio.h>

// 创建一个 Date 对象 (不检查日期的有效性)
Date Date_create(uint32_t year, uint32_t month, uint32_t day)
{
    Date date = { year, month, day };
    return date;
}

bool Date_isValid(Date date)
{
    if (date.year == 0 || date.month == 0 || date.month > 12 || date.day == 0)
    {
        return false;
    }

    uint32_t daysInMonth[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

    // 闰年检查
    if ((date.year % 4 == 0 && date.year % 100 != 0) || date.year % 400 == 0)
    {
        daysInMonth[2] = 29;
    }

    if (date.day > daysInMonth[date.month])
    {
        return false;
    }

    return true;
}

Date Date_createValid(uint32_t year, uint32_t month, uint32_t day)
{
    Date date = { year, month, day };
    if (!Date_isValid(date))
    {
        return Date_create(0, 0, 0);
    }
    return date;
}

Date Date_today(void)
{
    time_t t = time(NULL);
    struct tm* now = localtime(&t);

    // 参考时间戳定义
    // 1970-01-01 00:00:00 UTC
    Date today = {
        (uint32_t)(now->tm_year + 1900),
        (uint32_t)(now->tm_mon + 1),
        (uint32_t)(now->tm_mday)
    };

    return today;
}

int32_t Date_diff(Date date1, Date date2)
{
    // 将日期转换为自公元元年以来的天数 (非常简化的计算)
    int32_t days1 = date1.year * 365 + date1.day;
    int32_t days2 = date2.year * 365 + date2.day;

    uint32_t daysInMonth[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    for (int i = 1; i < date1.month; i++)
    {
        if ((date1.year % 4 == 0 && date1.year % 100 != 0) || date1.year % 400 == 0) {
            if (i == 2)
            {
                days1 += 29;
                continue;
            }
        }
        days1 += daysInMonth[i];
    }

    for (int i = 1; i < date2.month; i++)
    {
        // 闰年检查
        if ((date2.year % 4 == 0 && date2.year % 100 != 0) || date2.year % 400 == 0) {
            if (i == 2)
            {
                days2 += 29;
                continue;
            }
        }
        days2 += daysInMonth[i];
    }

    //加上闰年天数
    days1 += (date1.year / 4 - date1.year / 100 + date1.year / 400);
    days2 += (date2.year / 4 - date2.year / 100 + date2.year / 400);
    return days1 - days2;

}

void Date_format(Date date, char* buffer, size_t buffer_size)
{
    if (buffer && buffer_size > 0)
    {
        snprintf(buffer, buffer_size, "%04u-%02u-%02u", date.year, date.month, date.day);
    }
}

int Date_compare(Date date1, Date date2)
{
    if (date1.year < date2.year)
    {
        return -1;
    }
    if (date1.year > date2.year)
    {
        return 1;
    }

    if (date1.month < date2.month)
    {
        return -1;
    }
    if (date1.month > date2.month)
    {
        return 1;
    }

    if (date1.day < date2.day)
    {
        return -1;
    }
    if (date1.day > date2.day)
    {
        return 1;
    }

    return 0;
}

Date Date_addDays(Date date, int32_t days)
{
    if (!Date_isValid(date))
    {
        return Date_create(0, 0, 0);
    }

    Date result = date;

    uint32_t daysInMonth[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

    while (days > 0)
    {
        bool isLeapYear = ((result.year % 4 == 0 && result.year % 100 != 0) || result.year % 400 == 0);
        uint32_t febDays = isLeapYear ? 29 : 28;
        daysInMonth[2] = febDays;

        uint32_t daysLeftInMonth = daysInMonth[result.month] - result.day + 1;

        if (days < daysLeftInMonth)
        {
            result.day += days;
            break;
        }

        days -= daysLeftInMonth;
        result.month++;
        result.day = 1;

        if (result.month > 12)
        {
            result.year++;
            result.month = 1;
        }
    }

    while (days < 0)
    {
        if (result.day > -days)
        {
            result.day += days;
            break;
        }

        days += result.day;

        if (result.month > 1)
        {
            result.month--;
        }
        else
        {
            result.year--;
            result.month = 12;
        }

        bool isLeapYear = ((result.year % 4 == 0 && result.year % 100 != 0) || result.year % 400 == 0);
        uint32_t febDays = isLeapYear ? 29 : 28;
        daysInMonth[2] = febDays;

        result.day = daysInMonth[result.month];
    }

    return result;
}
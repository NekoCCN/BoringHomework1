#pragma once
#ifndef DATE_H
#define DATE_H
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

typedef struct DateStruct
{
    uint32_t year;
    uint32_t month;
    uint32_t day;
} Date;

Date Date_create(uint32_t year, uint32_t month, uint32_t day);

Date Date_createValid(uint32_t year, uint32_t month, uint32_t day);

bool Date_isValid(Date date);

Date Date_today(void);

int32_t Date_diff(Date date1, Date date2);

Date Date_addDays(Date date, int32_t days);

void Date_format(Date date, char* buffer, size_t buffer_size);

int Date_compare(Date date1, Date date2);

#endif // DATE_H

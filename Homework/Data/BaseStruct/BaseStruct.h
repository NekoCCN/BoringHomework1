#pragma once
#ifndef BASESTRUCT_H
#define BASESTRUCT_H
#include <stdint.h>

typedef struct BaseDataStruct
{
	int64_t id;

	wchar_t course[32];

	int64_t daily_score;
    int64_t final_score;

	double daily_score_ratio;

	int64_t score;
} BaseData;
#endif // BASESTRUCT_H

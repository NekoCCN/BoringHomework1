#include <stdint.h>

typedef struct BaseDataStruct
{
	uint32_t id;

	char course[32];

	uint32_t daily_score;
    uint32_t final_score;

	double daily_score_ratio;
} BaseData;
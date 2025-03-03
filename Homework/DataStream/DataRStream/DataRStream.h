#pragma once
#ifndef DATA_RSTREAM_H
#define DATA_RSTREAM_H
#include "../../DataStream/DataWStream/DataWStream.h"
#include "../../Data/StringSkipList/StringSkipList.h"
#include "../../Data/UserList/UserList.h"
#include "../../Tools/Tools.h"
#include <wchar.h>

enum DataRStreamResult
{
	DATARSTREAM_ERROR_FILE_IO = -1,
	DATARSTREAM_ERROR_MEMORY_ALLOCATION = -2,
	DATARSTREAM_ERROR_INVALID_INPUT = -3,
	DATARSTREAM_ERROR_DESERIALIZATION_FAILED = -4,
	DATARSTREAM_SUCCESS = 0
};

typedef struct DataRStreamStruct
{
	FILE* file_ptr_;

	StringSkipList* data_;

	list* user_list_;

	FileHeader header_;

	User* user_;
	uint64_t user_num_;
} DataRStream;

DataRStream* DATARSTREAM_create(const char* filename);

void DATARSTREAM_destory(DataRStream* self);

#endif
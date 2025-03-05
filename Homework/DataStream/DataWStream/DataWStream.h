#pragma once
#ifndef DATAWSTREAM_H
#define DATAWSTREAM_H
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "../../Data/StringSkipList/StringSkipList.h"
#include "../../Data/UserList/UserList.h"
#include <stdbool.h>

typedef struct UserStruct
{
    wchar_t name[50];
    uint32_t password_hash;
} User;

typedef struct FileHeaderStruct
{
    uint32_t magic_number;

    uint64_t user_num;
} FileHeader;

typedef struct DataWStreamStruct
{
    FileHeader file_header_;
    bool is_append_mode_;

    FILE* admin_file_ptr_;
    FILE* book_file_ptr_;
    FILE* user_file_ptr_;
} DataWStream;

DataWStream* DATAWSTREAM_create(const char* admin_filename, const char* book_filename, const char* user_filename, bool is_append_mode);

bool DATAWSTREAM_start(DataWStream* self, User* user, uint32_t user_num, StringSkipList* data, list* user_list);

void DATAWSTREAM_destory(DataWStream* self);

#endif // DATAWSTREAM_H
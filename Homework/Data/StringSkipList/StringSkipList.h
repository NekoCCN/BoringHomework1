#pragma once
#ifndef STRINGSKIPLIST_H
#define STRINGSKIPLIST_H
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <locale.h>
#include <time.h>
#include "../BaseStruct/BaseStruct.h"

// 仿C++面向对象的C风格跳表容器
// 应该满足作业的C语言和链表这两个要求...吧。

#define FIBONACCI_NUMBERS_MAX_LEVEL 32
#define FIBONACCI_NUMBERS 0.61803398875

typedef enum
{
    STRINGSKIPLIST_SUCCESS = 0,
    STRINGSKIPLIST_ERROR_MEMORY_ALLOCATION = -1,
    STRINGSKIPLIST_ERROR_INVALID_INPUT = -2,
    STRINGSKIPLIST_ERROR_DESERIALIZATION_FAILED = -3,
    STRINGSKIPLIST_ERROR_FILE_IO = -4,
    STRINGSKIPLIST_ERROR_NOT_FOUND = -5,
} StringSkipListResult;

typedef struct SearchResultStruct
{
    wchar_t* key;
    BaseData value;
} SearchResult;

typedef struct SkipListNodeStruct
{
    wchar_t* key;
    BaseData value;
    struct SkipListNodeStruct** forward;
    int level;
} SkipListNode;

typedef struct StringSkipListStruct
{
    int max_level_;
    float probability_;
    SkipListNode* header;
    int current_level_;
} StringSkipList;

typedef struct StringSkipListIteratorStruct
{
    SkipListNode* current;
} StringSkipListIterator;

// 迭代器相关函数
StringSkipListIterator STRINGSKIPLIST_IteratorBegin(StringSkipList* self);

SkipListNode* STRINGSKIPLIST_IteratorNext(StringSkipListIterator* iterator);

bool STRINGSKIPLIST_IteratorEnd(const StringSkipListIterator* iterator);

const wchar_t* STRINGSKIPLIST_IteratorGetKey(const StringSkipListIterator* iterator);

const BaseData* STRINGSKIPLIST_IteratorGetValue(const StringSkipListIterator* iterator);

// 跳表操作函数
StringSkipList* STRINGSKIPLIST_Construct(int max_level, float probability);

void STRINGSKIPLIST_Destruct(StringSkipList* self);

StringSkipListResult STRINGSKIPLIST_Insert(StringSkipList* self, const char* str, const BaseData* value);

StringSkipListResult STRINGSKIPLIST_InsertW(StringSkipList* self, const wchar_t* wstr, const BaseData* value);

bool STRINGSKIPLIST_Search(StringSkipList* self, const char* str, BaseData* value);

bool STRINGSKIPLIST_SearchW(StringSkipList* self, const wchar_t* wstr, BaseData* value);

SearchResult* STRINGSKIPLIST_PrefixSearch(StringSkipList* self, const char* prefix, int* count);

SearchResult* STRINGSKIPLIST_PrefixSearchW(StringSkipList* self, const wchar_t* prefix, int* count);

void STRINGSKIPLIST_PrintList(StringSkipList* self);

void STRINGSKIPLIST_SerializeSimple(StringSkipList* self, const char* filename);

StringSkipList* STRINGSKIPLIST_DeserializeSimple(const char* filename);

StringSkipListResult STRINGSKIPLIST_DeleteW(StringSkipList* self, const wchar_t* wstr);

StringSkipListResult STRINGSKIPLIST_Delete(StringSkipList* self, const char* str);

int STRINGSKIPLIST_DeletePrefixW(StringSkipList* self, const wchar_t* prefix);

int STRINGSKIPLIST_DeletePrefix(StringSkipList* self, const char* prefix);

#endif // STRINGSKIPLIST_H
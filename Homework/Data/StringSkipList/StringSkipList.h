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

StringSkipListIterator STRINGSKIPLIST_iteratorBegin(StringSkipList* self);

SkipListNode* STRINGSKIPLIST_iteratorNext(StringSkipListIterator* iterator);

bool STRINGSKIPLIST_iteratorEnd(const StringSkipListIterator* iterator);

wchar_t* STRINGSKIPLIST_iteratorGetKey(const StringSkipListIterator* iterator);

const BaseData* STRINGSKIPLIST_iteratorGetValue(const StringSkipListIterator* iterator);


StringSkipList* STRINGSKIPLIST_construct(int max_level, float probability);

void STRINGSKIPLIST_destruct(StringSkipList* self);

StringSkipListResult STRINGSKIPLIST_insert(StringSkipList* self, const char* str, BaseData* value);

StringSkipListResult STRINGSKIPLIST_insertW(StringSkipList* self, const wchar_t* wstr, BaseData* value);

bool STRINGSKIPLIST_search(StringSkipList* self, const char* str, SearchResult* value);

bool STRINGSKIPLIST_searchW(StringSkipList* self, const wchar_t* wstr, SearchResult* value);

SearchResult* STRINGSKIPLIST_prefixSearch(StringSkipList* self, const char* prefix, int* count);

SearchResult* STRINGSKIPLIST_prefixSearchW(StringSkipList* self, const wchar_t* prefix, int* count);

void STRINGSKIPLIST_printList(StringSkipList* self);

void STRINGSKIPLIST_serializeSimple(StringSkipList* self, const char* filename);

StringSkipList* STRINGSKIPLIST_deserializeSimple(const char* filename);

StringSkipListResult STRINGSKIPLIST_deleteW(StringSkipList* self, const wchar_t* wstr);

StringSkipListResult STRINGSKIPLIST_delete(StringSkipList* self, const char* str);

int STRINGSKIPLIST_deletePrefixW(StringSkipList* self, const wchar_t* prefix);

int STRINGSKIPLIST_deletePrefix(StringSkipList* self, const char* prefix);

SearchResult* STRINGSKIPLIST_getAll(StringSkipList* self, int* count);

SearchResult STRINGSKIPLIST_iteratorGetResult(const StringSkipListIterator* iterator);

#endif // STRINGSKIPLIST_H
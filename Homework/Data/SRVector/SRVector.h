#pragma once
#ifndef SRVECTOR_H
#define SRVECTOR_H
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <memory.h>
#include "../StringSkipList/StringSkipList.h"
#define SRVECTOR_DEFAULT_CAPACITY 5000

/*
 * 一个以前写的 C++风格vector类的C语言简单实现
*/

typedef SearchResult SRVectorType;

typedef struct SRVectorStruct
{
    unsigned long long size;
    unsigned long long capacity;
    SRVectorType* data;
} SRVector;

void SRVECTOR_PRIVATE_enlarge(SRVector* context);

SRVector* SRVECTOR_construct();

SRVector* SRVECTOR_pushBack(SRVector* context, SRVectorType value);

SRVector* SRVECTOR_popBack(SRVector* context);

SRVectorType SRVECTOR_get(SRVector* context, unsigned long long index);

void SRVECTOR_set(SRVector* context, unsigned long long index, SRVectorType value);

void SRVECTOR_clear(SRVector* context);

void SRVECTOR_destruct(SRVector* context);

SRVectorType* SRVECTOR_data(SRVector* context, size_t* size);

SRVectorType* SRVECTOR_copiedData(SRVector* context, size_t* size);

#endif // SRVECTOR_H

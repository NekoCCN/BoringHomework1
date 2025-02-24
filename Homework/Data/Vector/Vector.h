#pragma once
#ifndef VECTOR_H
#define VECTOR_H
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <memory.h>
#define VECTOR_DEFAULT_CAPACITY 5000

/*
 * 一个以前写的 C++风格vector类的C语言简单实现
*/

typedef int VectorType;

typedef struct VectorStruct
{
    unsigned long long size;
    unsigned long long capacity;
    VectorType* data;
} vector;

void VECTOR_PRIVATE_enlarge(vector* context);

vector* VECTOR_construct();

vector* VECTOR_pushBack(vector* context, VectorType value);

vector* VECTOR_popBack(vector* context);

bool VECTOR_find(vector* context, VectorType value);

VectorType VECTOR_get(vector* context, unsigned long long index);

void VECTOR_set(vector* context, unsigned long long index, VectorType value);

void VECTOR_destruct(vector* context);

VectorType* VECTOR_data(vector* context, size_t* size);

#endif // VECTOR_H

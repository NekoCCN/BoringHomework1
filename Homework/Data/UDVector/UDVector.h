#pragma once
#ifndef USERDATAVECTOR_H
#define USERDATAVECTOR_H
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <memory.h>
#include <wchar.h>
#include <stdint.h>
#include "../UserList/UserList.h"

#define UDVECTOR_DEFAULT_CAPACITY 5000

typedef struct UserDataVectorStruct
{
    unsigned long long size;
    unsigned long long capacity;
    UserData* data;
} UDVector;

void UDVECTOR_PRIVATE_enlarge(UDVector* context);

UDVector* UDVECTOR_construct();

UDVector* UDVECTOR_pushBack(UDVector* context, UserData value);

UDVector* UDVECTOR_popBack(UDVector* context);

UserData UDVECTOR_get(UDVector* context, unsigned long long index);

uint64_t UDVECTOR_size(UDVector* context);

void UDVECTOR_set(UDVector* context, unsigned long long index, UserData value);

void UDVECTOR_clear(UDVector* context);

void UDVECTOR_destruct(UDVector* context);

UserData* UDVECTOR_data(UDVector* context, int* size);

UserData* UDVECTOR_copiedData(UDVector* context, int* size);

#endif // USERDATAVECTOR_H
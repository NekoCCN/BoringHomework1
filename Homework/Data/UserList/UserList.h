#pragma once
#ifndef USERLIST_H
#define USERLIST_H

#include <stdint.h>
#include <stdlib.h>
#include <wchar.h>
#include <stdio.h>
#include <string.h>

typedef struct UserDataStruct
{
    wchar_t* user_name_;
    wchar_t* book_name_;

    uint32_t year_;
    uint32_t month_;
    uint32_t day_;

    uint64_t id_;
} UserData;

typedef struct UserNodeStruct
{
    UserData data;
    struct UserNodeStruct* next;
    struct UserNodeStruct* prev;
} UserNode;

typedef struct ListContext
{
    unsigned size;
    UserNode* head;
    UserNode* tail;
} list;

typedef struct UserListIteratorStruct
{
    list* list_;
    UserNode* current;
} UserListIterator;

list* LIST_Construct();

list* LIST_PushBack(list* context, const wchar_t* user_name, const wchar_t* book_name, uint32_t year, uint32_t month, uint32_t day, uint64_t id);

list* LIST_PushFront(list* context, const wchar_t* user_name, const wchar_t* book_name, uint32_t year, uint32_t month, uint32_t day, uint64_t id);

UserNode* LIST_PRIVATE_GetNode(list* context, unsigned index);

UserData LIST_Get(list* context, unsigned index);

list* LIST_Insert(list* context, unsigned index, const wchar_t* user_name, const wchar_t* book_name, uint32_t year, uint32_t month, uint32_t day, uint64_t id);

list* LIST_Delete(list* context, unsigned index);

void LIST_Destruct(list* context);

UserListIterator LIST_iteratorBegin(list* self);

UserNode* LIST_iteratorNext(UserListIterator* iterator);

bool LIST_iteratorEnd(const UserListIterator* iterator);

UserData LIST_iteratorGetData(const UserListIterator* iterator);

void LIST_iteratorDestruct(UserListIterator* iterator);

bool LIST_serialize(list* self, char** buffer, size_t* buffer_size);

list* LIST_deserializeFile(FILE* file);

#endif // USERLIST_H
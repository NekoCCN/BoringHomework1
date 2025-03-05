#include "UDVector.h"

void UDVECTOR_PRIVATE_enlarge(UDVector* context)
{
    context->capacity *= 2;
    context->data = (UserData*)realloc(context->data, sizeof(UserData) * context->capacity);
}

UDVector* UDVECTOR_construct()
{
    UDVector* new_vector = (UDVector*)malloc(sizeof(UDVector));
    new_vector->size = 0;
    new_vector->capacity = UDVECTOR_DEFAULT_CAPACITY;
    new_vector->data = (UserData*)malloc(sizeof(UserData) * new_vector->capacity);
    return new_vector;
}

UDVector* UDVECTOR_pushBack(UDVector* context, UserData value)
{
    if (context->size >= context->capacity)
    {
        UDVECTOR_PRIVATE_enlarge(context);
    }

    // 创建新元素并复制数据
    context->data[context->size] = value;

    // 深拷贝用户名
    if (value.user_name_)
    {
        context->data[context->size].user_name_ = (wchar_t*)malloc(sizeof(wchar_t) * (wcslen(value.user_name_) + 1));
        wcscpy(context->data[context->size].user_name_, value.user_name_);
    }
    else
    {
        context->data[context->size].user_name_ = NULL;
    }

    // 深拷贝书名
    if (value.book_name_)
    {
        context->data[context->size].book_name_ = (wchar_t*)malloc(sizeof(wchar_t) * (wcslen(value.book_name_) + 1));
        wcscpy(context->data[context->size].book_name_, value.book_name_);
    }
    else
    {
        context->data[context->size].book_name_ = NULL;
    }

    context->size++;
    return context;
}

UDVector* UDVECTOR_popBack(UDVector* context)
{
    if (context->size > 0)
    {
        // 释放最后一个元素的动态内存
        if (context->data[context->size - 1].user_name_)
        {
            free(context->data[context->size - 1].user_name_);
        }

        if (context->data[context->size - 1].book_name_)
        {
            free(context->data[context->size - 1].book_name_);
        }

        context->size--;
    }
    return context;
}

UserData UDVECTOR_get(UDVector* context, unsigned long long index)
{
    if (index < context->size)
    {
        return context->data[index];
    }
    else
    {
        // 返回一个空的UserData作为错误标识
        UserData empty = { NULL, NULL, 0, 0, 0, 0 };
        return empty;
    }
}

uint64_t UDVECTOR_size(UDVector* context)
{
    return context->size;
}

void UDVECTOR_set(UDVector* context, unsigned long long index, UserData value)
{
    if (index < context->size)
    {
        // 释放原来的内存
        if (context->data[index].user_name_)
        {
            free(context->data[index].user_name_);
        }

        if (context->data[index].book_name_)
        {
            free(context->data[index].book_name_);
        }

        // 设置新值
        context->data[index] = value;

        // 深拷贝用户名
        if (value.user_name_)
        {
            context->data[index].user_name_ = (wchar_t*)malloc(sizeof(wchar_t) * (wcslen(value.user_name_) + 1));
            wcscpy(context->data[index].user_name_, value.user_name_);
        }
        else
        {
            context->data[index].user_name_ = NULL;
        }

        // 深拷贝书名
        if (value.book_name_)
        {
            context->data[index].book_name_ = (wchar_t*)malloc(sizeof(wchar_t) * (wcslen(value.book_name_) + 1));
            wcscpy(context->data[index].book_name_, value.book_name_);
        }
        else
        {
            context->data[index].book_name_ = NULL;
        }
    }
}

void UDVECTOR_clear(UDVector* context)
{
    // 清空数组
    memset(context->data, 0, sizeof(UserData) * context->size);
    context->size = 0;
}

void UDVECTOR_destruct(UDVector* context)
{
    // 先清理所有元素的动态内存
    UDVECTOR_clear(context);

    // 释放数组和结构体
    free(context->data);
    free(context);
}

UserData* UDVECTOR_data(UDVector* context, int* size)
{
    *size = context->size;
    return context->data;
}

UserData* UDVECTOR_copiedData(UDVector* context, int* size)
{
    UserData* buf;
    *size = context->size;
    buf = (UserData*)malloc(sizeof(UserData) * context->size);

    for (int i = 0; i < context->size; i++)
    {
        // 复制基本类型
        buf[i].year_ = context->data[i].year_;
        buf[i].month_ = context->data[i].month_;
        buf[i].day_ = context->data[i].day_;
        buf[i].id_ = context->data[i].id_;
        buf[i].rent_status_ = context->data[i].rent_status_;

        // 深拷贝用户名
        if (context->data[i].user_name_)
        {
            buf[i].user_name_ = (wchar_t*)malloc(sizeof(wchar_t) * (wcslen(context->data[i].user_name_) + 1));
            wcscpy(buf[i].user_name_, context->data[i].user_name_);
        }
        else
        {
            buf[i].user_name_ = NULL;
        }

        // 深拷贝书名
        if (context->data[i].book_name_)
        {
            buf[i].book_name_ = (wchar_t*)malloc(sizeof(wchar_t) * (wcslen(context->data[i].book_name_) + 1));
            wcscpy(buf[i].book_name_, context->data[i].book_name_);
        }
        else
        {
            buf[i].book_name_ = NULL;
        }
    }

    return buf;
}
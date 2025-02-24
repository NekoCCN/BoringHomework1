#include "SRVector.h"

void SRVECTOR_PRIVATE_enlarge(SRVector* context)
{
    context->capacity *= 2;
    context->data = (SRVectorType*)realloc(context->data, sizeof(SRVectorType) * context->capacity);
}

SRVector* SRVECTOR_construct()
{
    SRVector* new_vector = (SRVector*)malloc(sizeof(SRVector));
    new_vector->size = 0;
    new_vector->capacity = SRVECTOR_DEFAULT_CAPACITY;

    new_vector->data = (SRVectorType*)malloc(sizeof(SRVectorType) * new_vector->capacity);

    return new_vector;
}

SRVector* SRVECTOR_pushBack(SRVector* context, SRVectorType value)
{
    if (context->size >= context->capacity)
    {
        SRVECTOR_PRIVATE_enlarge(context);
    }

    context->data[context->size] = value;
    context->data[context->size].key = (wchar_t*)malloc(sizeof(wchar_t) * (wcslen(value.key) + 1));
    wcscpy(context->data[context->size].key, value.key);

    context->size++;
    return context;
}

SRVector* SRVECTOR_popBack(SRVector* context)
{
    context->data[context->size] = {};
    context->size--;
    return context;
}

SRVectorType SRVECTOR_get(SRVector* context, unsigned long long index)
{
    return context->data[index];
}

void SRVECTOR_set(SRVector* context, unsigned long long index, SRVectorType value)
{
    context->data[index] = value;
}

void SRVECTOR_clear(SRVector* context)
{
    for (int i = 0; i < context->size; ++i)
    {
        if (context->data[i].key)
        {
            free(context->data[i].key);
        }
    }
    memset(context->data, 0, sizeof(SRVectorType) * context->size);
    context->size = 0;
}

void SRVECTOR_destruct(SRVector* context)
{
    free(context->data);
    free(context);
}

SRVectorType* SRVECTOR_data(SRVector* context, size_t* size)
{
    *size = context->size;
    return context->data;
}

SRVectorType* SRVECTOR_copiedData(SRVector* context, size_t* size)
{
    SRVectorType* buf;
    *size = context->size;

    buf = (SRVectorType*)malloc(sizeof(SRVectorType) * context->size);

    for (int i = 0; i < context->size; i++)
    {
        buf[i] = SRVECTOR_get(context, i);
        buf[i].key = (wchar_t*)malloc(sizeof(wchar_t) * (wcslen(context->data[i].key) + 1));
        wcscpy(buf[i].key, context->data[i].key);
    }

    return buf;
}
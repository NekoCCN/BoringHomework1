#include "Vector.h"

void VECTOR_PRIVATE_enlarge(vector* context)
{
    context->capacity *= 2;
    context->data = (VectorType*)realloc(context->data, sizeof(VectorType) * context->capacity);
}

vector* VECTOR_construct()
{
    vector* new_vector = (vector*)malloc(sizeof(vector));
    new_vector->size = 0;
    new_vector->capacity = VECTOR_DEFAULT_CAPACITY;

    new_vector->data = (VectorType*)malloc(sizeof(VectorType) * new_vector->capacity);

    return new_vector;
}

vector* VECTOR_pushBack(vector* context, VectorType value)
{
    if (context->size >= context->capacity)
    {
        VECTOR_PRIVATE_enlarge(context);
    }
    context->data[context->size] = value;
    context->size++;
    return context;
}

vector* VECTOR_popBack(vector* context)
{
    context->data[context->size] = 0;
    context->size--;
    return context;
}

bool VECTOR_find(vector* context, VectorType value)
{
    for (unsigned long long i = 0; i < context->size; i++)
    {
        if (context->data[i] == value)
        {
            return true;
        }
    }
    return false;
}

VectorType VECTOR_get(vector* context, unsigned long long index)
{
    return context->data[index];
}

void VECTOR_set(vector* context, unsigned long long index, VectorType value)
{
    context->data[index] = value;
}

void VECTOR_destruct(vector* context)
{
    free(context->data);
    free(context);
}

VectorType* VECTOR_data(vector* context, size_t* size)
{
    *size = context->size;
    return context->data;
}
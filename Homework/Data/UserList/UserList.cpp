#include "UserList.h"

list* LIST_Construct()
{
    list* context = (list*)malloc(sizeof(list));
    if (!context)
    {
        return NULL;
    }

    context->head = (UserNode*)malloc(sizeof(UserNode));
    if (!context->head)
    {
        free(context);
        return NULL;
    }
    context->head->data.year_ = 0;
    context->head->data.day_ = 0;
    context->head->data.month_ = 0;
    context->head->data.book_name_ = NULL;
    context->head->data.user_name_ = NULL;
    context->head->data.id_ = 0;
    context->head->prev = NULL;

    context->tail = (UserNode*)malloc(sizeof(UserNode));
    if (!context->tail)
    {
        free(context->head);
        free(context);
        return NULL;
    }
    context->tail->data.year_ = 0;
    context->tail->data.day_ = 0;
    context->tail->data.month_ = 0;
    context->tail->data.book_name_ = NULL;
    context->tail->data.user_name_ = NULL;
    context->head->data.id_ = 0;
    context->tail->next = NULL;

    context->head->next = context->tail;
    context->tail->prev = context->head;

    context->size = 0;
    return context;
}

list* LIST_PushBack(list* context, const wchar_t* user_name, const wchar_t* book_name, uint32_t year, uint32_t month, uint32_t day, uint64_t id)
{
    if (!context || !user_name || !book_name)
    {
        return NULL;
    }

    UserNode* newNode = (UserNode*)malloc(sizeof(UserNode));
    if (!newNode)
    {
        return NULL;
    }

    newNode->data.year_ = year;
    newNode->data.day_ = day;
    newNode->data.month_ = month;
    newNode->data.id_ = id;

    newNode->data.book_name_ = (wchar_t*)malloc(sizeof(wchar_t) * (wcslen(book_name) + 1));
    if (!newNode->data.book_name_)
    {
        free(newNode);
        return NULL;
    }
    wcscpy(newNode->data.book_name_, book_name);

    newNode->data.user_name_ = (wchar_t*)malloc(sizeof(wchar_t) * (wcslen(user_name) + 1));
    if (!newNode->data.user_name_)
    {
        free(newNode->data.book_name_);
        free(newNode);
        return NULL;
    }
    wcscpy(newNode->data.user_name_, user_name);

    newNode->next = context->tail;
    newNode->prev = context->tail->prev;

    UserNode* prev = context->tail->prev;
    if (prev)
    {
        prev->next = newNode;
    }
    context->tail->prev = newNode;

    context->size++;
    return context;
}

list* LIST_PushFront(list* context, const wchar_t* user_name, const wchar_t* book_name, uint32_t year, uint32_t month, uint32_t day, uint64_t id)
{
    if (!context || !user_name || !book_name)
    {
        return NULL;
    }

    UserNode* newNode = (UserNode*)malloc(sizeof(UserNode));
    if (!newNode)
    {
        return NULL;
    }

    newNode->data.year_ = year;
    newNode->data.day_ = day;
    newNode->data.month_ = month;
    newNode->data.id_ = id;

    newNode->data.book_name_ = (wchar_t*)malloc(sizeof(wchar_t) * (wcslen(book_name) + 1));
    if (!newNode->data.book_name_)
    {
        free(newNode);
        return NULL;
    }
    wcscpy(newNode->data.book_name_, book_name);

    newNode->data.user_name_ = (wchar_t*)malloc(sizeof(wchar_t) * (wcslen(user_name) + 1));
    if (!newNode->data.user_name_)
    {
        free(newNode->data.book_name_);
        free(newNode);
        return NULL;
    }
    wcscpy(newNode->data.user_name_, user_name);

    newNode->prev = context->head;
    newNode->next = context->head->next;

    UserNode* next = context->head->next;
    if (next)
    {
        next->prev = newNode;
    }
    context->head->next = newNode;

    context->size++;
    return context;
}

UserNode* LIST_PRIVATE_GetNode(list* context, unsigned index)
{
    if (!context || context->size == 0 || index >= context->size)
    {
        return NULL;
    }

    // 择优遍历
    if (index < context->size / 2)
    {
        UserNode* buf = context->head->next;
        for (unsigned int i = 0; i < index; i++)
        {
            if (!buf)
            {
                return NULL;
            }
            buf = buf->next;
        }
        return buf;
    }
    else
    {
        UserNode* buf = context->tail->prev;
        for (unsigned int i = 0; i < context->size - index - 1; ++i)
        {
            if (!buf)
            {
                return NULL;
            }
            buf = buf->prev;
        }
        return buf;
    }
}

UserData LIST_Get(list* context, unsigned index)
{
    UserNode* buf = LIST_PRIVATE_GetNode(context, index);
    if (!buf)
    {
        UserData emptyData = { NULL, NULL, 0, 0, 0, 0 };
        return emptyData;
    }
    return buf->data;
}

list* LIST_Insert(list* context, unsigned index, const wchar_t* user_name, const wchar_t* book_name, uint32_t year, uint32_t month, uint32_t day, uint64_t id)
{
    if (!context || !user_name || !book_name || index > context->size)
    {
        return NULL;
    }

    UserNode* buf = (index == context->size) ? context->tail : LIST_PRIVATE_GetNode(context, index);
    if (!buf)
    {
        return NULL;
    }

    UserNode* newNode = (UserNode*)malloc(sizeof(UserNode));
    if (!newNode)
    {
        return NULL;
    }

    newNode->data.year_ = year;
    newNode->data.day_ = day;
    newNode->data.month_ = month;
    newNode->data.id_ = id;

    newNode->data.book_name_ = (wchar_t*)malloc(sizeof(wchar_t) * (wcslen(book_name) + 1));
    if (!newNode->data.book_name_)
    {
        free(newNode);
        return NULL;
    }
    wcscpy(newNode->data.book_name_, book_name);

    newNode->data.user_name_ = (wchar_t*)malloc(sizeof(wchar_t) * (wcslen(user_name) + 1));
    if (!newNode->data.user_name_)
    {
        free(newNode->data.book_name_);
        free(newNode);
        return NULL;
    }
    wcscpy(newNode->data.user_name_, user_name);

    newNode->next = buf;
    newNode->prev = buf->prev;

    UserNode* tmp = buf->prev;
    if (tmp)
    {
        tmp->next = newNode;
    }
    buf->prev = newNode;

    context->size++;
    return context;
}

list* LIST_Delete(list* context, unsigned index)
{
    if (!context || index >= context->size) return NULL;

    UserNode* buf = LIST_PRIVATE_GetNode(context, index);
    if (!buf)
    {
        return NULL;
    }

    UserNode* prev = buf->prev;
    UserNode* next = buf->next;

    if (prev)
    {
        prev->next = next;
    }
    if (next)
    {
        next->prev = prev;
    }

    context->size--;
    free(buf->data.book_name_);
    free(buf->data.user_name_);
    free(buf);

    return context;
}

void LIST_Destruct(list* context)
{
    if (!context)
    {
        return;
    }

    UserNode* buf = context->head->next;
    while (buf != context->tail)
    {
        UserNode* tmp = buf;
        buf = buf->next;
        free(tmp->data.book_name_);
        free(tmp->data.user_name_);
        free(tmp);
    }
    free(context->head);
    free(context->tail);
    free(context);
}

UserListIterator LIST_iteratorBegin(list* self)
{
    UserListIterator iter;
    iter.list_ = self;
    iter.current = self->head->next;
    return iter;
}

UserNode* LIST_iteratorNext(UserListIterator* iterator)
{
    if (iterator && iterator->current && iterator->current != iterator->list_->tail)
    {
        iterator->current = iterator->current->next;
        return iterator->current;
    }
    return NULL;
}

bool LIST_iteratorEnd(const UserListIterator* iterator)
{
    return (iterator && iterator->current == iterator->list_->tail);
}

UserData LIST_iteratorGetData(const UserListIterator* iterator)
{
    UserData emptyData = { NULL, NULL, 0, 0, 0 };
    if (!iterator || !iterator->current)
    {
        return emptyData;
    }
    return iterator->current->data;
}

void LIST_iteratorDestruct(UserListIterator* iterator)
{
    if (iterator)
    {
        iterator->current = NULL;
        iterator->list_ = NULL;
    }
}

bool LIST_serialize(list* self, char** buffer, size_t* buffer_size)
{
    if (!self || !buffer || !buffer_size)
    {
        return false;
    }

    size_t required_size = sizeof(uint32_t);
    required_size += sizeof(unsigned);

    UserListIterator iter = LIST_iteratorBegin(self);

    while (!LIST_iteratorEnd(&iter))
    {
        UserData data = LIST_iteratorGetData(&iter);
        required_size += sizeof(uint32_t) * 4;

        size_t user_name_utf8_len = wcstombs(NULL, data.user_name_, 0);
        if (user_name_utf8_len == (size_t)-1)
        {
            LIST_iteratorDestruct(&iter);
            return false;
        }
        required_size += sizeof(size_t);
        required_size += user_name_utf8_len;

        size_t book_name_utf8_len = wcstombs(NULL, data.book_name_, 0);
        if (book_name_utf8_len == (size_t)-1)
        {
            LIST_iteratorDestruct(&iter);
            return false;
        }
        required_size += sizeof(size_t);
        required_size += book_name_utf8_len;

        LIST_iteratorNext(&iter);
    }
    LIST_iteratorDestruct(&iter);

    *buffer = (char*)malloc(required_size);
    if (!*buffer)
    {
        return false;
    }
    *buffer_size = required_size;

    char* current_pos = *buffer;

    uint32_t magic_number = 0x2333F;
    memcpy(current_pos, &magic_number, sizeof(uint32_t));
    current_pos += sizeof(uint32_t);

    memcpy(current_pos, &self->size, sizeof(unsigned));
    current_pos += sizeof(uint32_t);

    iter = LIST_iteratorBegin(self);
    while (!LIST_iteratorEnd(&iter))
    {
        UserData data = LIST_iteratorGetData(&iter);

        memcpy(current_pos, &data.year_, sizeof(uint32_t));
        current_pos += sizeof(uint32_t);
        memcpy(current_pos, &data.month_, sizeof(uint32_t));
        current_pos += sizeof(uint32_t);
        memcpy(current_pos, &data.day_, sizeof(uint32_t));
        current_pos += sizeof(uint32_t);
        memcpy(current_pos, &data.id_, sizeof(uint64_t));
        current_pos += sizeof(uint32_t);

        // 将 user_name_ 转换为 UTF-8 并写入
        size_t user_name_utf8_len = wcstombs(NULL, data.user_name_, 0);
        char* user_name_utf8 = (char*)malloc(user_name_utf8_len + 1);
        if (!user_name_utf8)
        {
            free(*buffer); // 释放已分配的缓冲区
            LIST_iteratorDestruct(&iter);
            return false;
        }
        wcstombs(user_name_utf8, data.user_name_, user_name_utf8_len + 1);
        memcpy(current_pos, &user_name_utf8_len, sizeof(size_t));
        current_pos += sizeof(size_t);
        memcpy(current_pos, user_name_utf8, user_name_utf8_len);
        current_pos += user_name_utf8_len;
        free(user_name_utf8);


        // 上面抄下来
        size_t book_name_utf8_len = wcstombs(NULL, data.book_name_, 0);
        char* book_name_utf8 = (char*)malloc(book_name_utf8_len + 1);
        if (!book_name_utf8)
        {
            free(*buffer);
            LIST_iteratorDestruct(&iter);
            return false;
        }
        wcstombs(book_name_utf8, data.book_name_, book_name_utf8_len + 1);
        memcpy(current_pos, &book_name_utf8_len, sizeof(size_t));
        current_pos += sizeof(size_t);
        memcpy(current_pos, book_name_utf8, book_name_utf8_len);
        current_pos += book_name_utf8_len;
        free(book_name_utf8);

        LIST_iteratorNext(&iter);
    }
    LIST_iteratorDestruct(&iter);

    return true;
}

list* LIST_deserializeFile(FILE* file)
{
    if (!file)
    {
        return NULL;
    }

    // 读取魔数
    uint32_t magic_number;
    if (fread(&magic_number, sizeof(uint32_t), 1, file) != 1)
    {
        return NULL;
    }
    if (magic_number != 0x2333F)
    {
        return NULL;
    }

    unsigned size;
    if (fread(&size, sizeof(unsigned), 1, file) != 1)
    {
        return NULL;
    }


    list* context = LIST_Construct();
    if (!context) return NULL;

    for (unsigned i = 0; i < size; ++i)
    {
        uint32_t year, month, day, id;
        size_t user_name_utf8_len, book_name_utf8_len;
        char* user_name_utf8 = NULL;
        char* book_name_utf8 = NULL;
        wchar_t* user_name = NULL;
        wchar_t* book_name = NULL;

        // 读取 year, month, day
        fread(&year, sizeof(uint32_t), 1, file);
        fread(&month, sizeof(uint32_t), 1, file);
        fread(&day, sizeof(uint32_t), 1, file);
        fread(&id, sizeof(uint32_t), 1, file);

        fread(&user_name_utf8_len, sizeof(size_t), 1, file);

        // 读取 user_name_ 的 UTF-8 数据
        user_name_utf8 = (char*)malloc(user_name_utf8_len + 1);
        if (!user_name_utf8)
        {
            if (user_name_utf8)
            {
                free(user_name_utf8);
            }
            if (book_name_utf8)
            {
                free(book_name_utf8);
            }
            if (user_name)
            {
                free(user_name);
            }
            if (book_name)
            {
                free(book_name);
            }
            LIST_Destruct(context);
            return NULL;
        }
        fread(user_name_utf8, 1, user_name_utf8_len, file);
        user_name_utf8[user_name_utf8_len] = '\0'; // 避免出错

        // 将 UTF-8 转换为宽字符
        size_t user_name_len = mbstowcs(NULL, user_name_utf8, 0);

        user_name = (wchar_t*)malloc((user_name_len + 1) * sizeof(wchar_t));
        if (!user_name)
        {
            if (user_name_utf8)
            {
                free(user_name_utf8);
            }
            if (book_name_utf8)
            {
                free(book_name_utf8);
            }
            if (user_name)
            {
                free(user_name);
            }
            if (book_name)
            {
                free(book_name);
            }
            LIST_Destruct(context);
            return NULL;
        }
        mbstowcs(user_name, user_name_utf8, user_name_len + 1);
        free(user_name_utf8);  // 释放 UTF-8 缓冲区
        user_name_utf8 = NULL;

        fread(&book_name_utf8_len, sizeof(size_t), 1, file) != 1;
        book_name_utf8 = (char*)malloc(book_name_utf8_len + 1);
        if (!book_name_utf8)
        {
            if (user_name_utf8)
            {
                free(user_name_utf8);
            }
            if (book_name_utf8)
            {
                free(book_name_utf8);
            }
            if (user_name)
            {
                free(user_name);
            }
            if (book_name)
            {
                free(book_name);
            }
            LIST_Destruct(context);
            return NULL;
        }
        fread(book_name_utf8, 1, book_name_utf8_len, file);
        book_name_utf8[book_name_utf8_len] = '\0';

        size_t book_name_len = mbstowcs(NULL, book_name_utf8, 0);

        book_name = (wchar_t*)malloc((book_name_len + 1) * sizeof(wchar_t));
        if (!book_name)
        {
            if (user_name_utf8)
            {
                free(user_name_utf8);
            }
            if (book_name_utf8)
            {
                free(book_name_utf8);
            }
            if (user_name)
            {
                free(user_name);
            }
            if (book_name)
            {
                free(book_name);
            }
            LIST_Destruct(context);
            return NULL;
        }
        mbstowcs(book_name, book_name_utf8, book_name_len + 1);
        free(book_name_utf8);
        book_name_utf8 = NULL;

        if (!LIST_PushBack(context, user_name, book_name, year, month, day, id))
        {
            if (user_name_utf8)
            {
                free(user_name_utf8);
            }
            if (book_name_utf8)
            {
                free(book_name_utf8);
            }
            if (user_name)
            {
                free(user_name);
            }
            if (book_name)
            {
                free(book_name);
            }
            LIST_Destruct(context);
            return NULL;
        }
        free(user_name);
        free(book_name);
        user_name = NULL;
        book_name = NULL;
    }

    return context;
}
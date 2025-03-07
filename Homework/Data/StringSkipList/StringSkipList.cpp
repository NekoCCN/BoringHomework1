#include "StringSkipList.h"
#include <stdint.h>

static int STRINGSKIPLIST_private_randomLevel(StringSkipList* self)
{
    int level = 1;
    while ((float)rand() / RAND_MAX < self->probability_ && level < self->max_level_)
    {
        level++;
    }
    return level;
}

static SkipListNode* STRINGSKIPLIST_private_createNode(const wchar_t* key, const BookData* value, int level)
{
    SkipListNode* newNode = (SkipListNode*)malloc(sizeof(SkipListNode));
    if (newNode == NULL)
    {
        return NULL;
    }

    newNode->key = (wchar_t*)malloc(sizeof(wchar_t) * (wcslen(key) + 1));
    if (newNode->key == NULL)
    {
        free(newNode);
        return NULL;
    }
    wcscpy(newNode->key, key);

    newNode->forward = (SkipListNode**)malloc(sizeof(SkipListNode*) * level);
    if (newNode->forward == NULL)
    {
        free(newNode->key);
        free(newNode);
        return NULL;
    }

    for (int i = 0; i < level; i++)
    {
        newNode->forward[i] = NULL;
    }

    newNode->level = level;

    if (value != NULL)
    {
        wcscpy(newNode->value.catagory_, value->catagory_);
        wcscpy(newNode->value.publisher_, value->publisher_);
        newNode->value.isbn_ = value->isbn_;
        newNode->value.num_ = value->num_;
        newNode->value.total_num_ = value->total_num_;
        newNode->value.image_toc_index_ = value->image_toc_index_;
        newNode->value.can_rent_ = value->can_rent_;
    }
    else
    {
        memset(&newNode->value, 0, sizeof(BookData));
    }

    return newNode;
}

static void STRINGSKIPLIST_private_freeNode(SkipListNode* node)
{
    if (node)
    {
        free(node->key);
        free(node->forward);
        free(node);
    }
}

StringSkipList* STRINGSKIPLIST_construct(int max_level, float probability)
{
    StringSkipList* self = (StringSkipList*)malloc(sizeof(StringSkipList));
    if (self == NULL)
    {
        return NULL;
    }

    self->max_level_ = max_level;
    self->probability_ = probability;
    self->current_level_ = 1;
    self->header = STRINGSKIPLIST_private_createNode(L"", NULL, max_level);
    if (self->header == NULL)
    {
        free(self);
        return NULL;
    }
    return self;
}

void STRINGSKIPLIST_destruct(StringSkipList* self)
{
    if (!self)
    {
        return;
    }
    SkipListNode* current = self->header;
    SkipListNode* next;

    while (current)
    {
        next = current->forward[0];
        STRINGSKIPLIST_private_freeNode(current);
        current = next;
    }
    free(self);
}

StringSkipListResult STRINGSKIPLIST_insertW(StringSkipList* self, const wchar_t* wstr, BookData* value)
{
    if (self == NULL || wstr == NULL)
    {
        return STRINGSKIPLIST_ERROR_INVALID_INPUT;
    }

    SkipListNode* current = self->header;
    SkipListNode** update = (SkipListNode**)malloc(sizeof(SkipListNode*) * self->max_level_);
    if (update == NULL)
    {
        return STRINGSKIPLIST_ERROR_MEMORY_ALLOCATION;
    }

    for (int i = 0; i < self->max_level_; ++i)
    {
        update[i] = NULL;
    }

    for (int i = self->current_level_ - 1; i >= 0; i--)
    {
        while (current->forward[i] && wcscmp(current->forward[i]->key, wstr) < 0)
        {
            current = current->forward[i];
        }
        update[i] = current;
    }

    current = current->forward[0];

    if (current == NULL || wcscmp(current->key, wstr) != 0)
    {
        int newLevel = STRINGSKIPLIST_private_randomLevel(self);

        if (newLevel > self->current_level_)
        {
            for (int i = self->current_level_; i < newLevel; i++)
            {
                update[i] = self->header;
            }
            self->current_level_ = newLevel;
        }

        SkipListNode* newNode = STRINGSKIPLIST_private_createNode(wstr, value, newLevel);
        if (newNode == NULL)
        {
            free(update);
            return STRINGSKIPLIST_ERROR_MEMORY_ALLOCATION;
        }

        for (int i = 0; i < newLevel; i++)
        {
            newNode->forward[i] = update[i]->forward[i];
            update[i]->forward[i] = newNode;
        }
    }
    free(update);
    return STRINGSKIPLIST_SUCCESS;
}

StringSkipListResult STRINGSKIPLIST_insert(StringSkipList* self, const char* str, BookData* value)
{
    if (self == NULL || str == NULL)
    {
        return STRINGSKIPLIST_ERROR_INVALID_INPUT;
    }
    size_t wide_size = mbstowcs(NULL, str, 0);
    if (wide_size == (size_t)-1)
    {
        return STRINGSKIPLIST_ERROR_INVALID_INPUT;
    }

    wchar_t* wstr = (wchar_t*)malloc((wide_size + 1) * sizeof(wchar_t));
    if (wstr == NULL)
    {
        return STRINGSKIPLIST_ERROR_MEMORY_ALLOCATION;
    }

    mbstowcs(wstr, str, wide_size + 1);
    StringSkipListResult result = STRINGSKIPLIST_insertW(self, wstr, value);
    free(wstr);
    return result;
}

bool STRINGSKIPLIST_searchW(StringSkipList* self, const wchar_t* wstr, SearchResult* value)
{
    if (self == NULL || wstr == NULL || value == NULL)
    {
        return false;
    }

    SkipListNode* current = self->header;

    for (int i = self->current_level_ - 1; i >= 0; i--)
    {
        while (current->forward[i] && wcscmp(current->forward[i]->key, wstr) < 0)
        {
            current = current->forward[i];
        }
    }

    current = current->forward[0];

    if (current && wcscmp(current->key, wstr) == 0)
    {
        value->value = current->value;
        value->key = (wchar_t*)malloc(sizeof(wchar_t) * (wcslen(current->key) + 1));
        wcscpy(value->key, current->key);
        return true;
    }

    return false;
}

bool STRINGSKIPLIST_search(StringSkipList* self, const char* str, SearchResult* value)
{
    if (self == NULL || str == NULL || value == NULL)
    {
        return false;
    }
    size_t wide_size = mbstowcs(NULL, str, 0);
    if (wide_size == (size_t)-1)
    {
        return false;
    }
    wchar_t* wstr = (wchar_t*)malloc(sizeof(wchar_t) * (wide_size + 1));
    if (wstr == NULL)
    {
        return false;
    }
    mbstowcs(wstr, str, wide_size + 1);
    bool result = STRINGSKIPLIST_searchW(self, wstr, value);
    free(wstr);
    return result;
}

SearchResult* STRINGSKIPLIST_prefixSearchW(StringSkipList* self, const wchar_t* prefix, int* count)
{
    if (self == NULL || prefix == NULL || count == NULL)
    {
        return NULL;
    }

    SkipListNode* current = self->header;
    SearchResult* results = NULL;
    *count = 0;

    for (int i = self->current_level_ - 1; i >= 0; i--)
    {
        while (current->forward[i] && wcscmp(current->forward[i]->key, prefix) < 0)
        {
            current = current->forward[i];
        }
    }

    current = current->forward[0];

    while (current && wcsncmp(current->key, prefix, wcslen(prefix)) == 0)
    {
        (*count)++;
        SearchResult* temp = (SearchResult*)realloc(results, sizeof(SearchResult) * (*count));
        if (temp == NULL)
        {
            for (int i = 0; i < *count - 1; i++)
            {
                free(results[i].key);
            }
            free(results);
            *count = 0;
            return NULL;
        }
        results = temp;

        results[*count - 1].key = (wchar_t*)malloc(sizeof(wchar_t) * (wcslen(current->key) + 1));
        if (results[*count - 1].key == NULL)
        {
            for (int i = 0; i < *count - 1; i++)
            {
                free(results[i].key);
            }
            free(results);
            *count = 0;
            return NULL;
        }
        wcscpy(results[*count - 1].key, current->key);
        results[*count - 1].value = current->value;

        current = current->forward[0];
    }

    return results;
}

SearchResult* STRINGSKIPLIST_prefixSearch(StringSkipList* self, const char* prefix, int* count)
{
    if (self == NULL || prefix == NULL || count == NULL)
    {
        return NULL;
    }
    size_t wide_size = mbstowcs(NULL, prefix, 0);
    if (wide_size == (size_t)-1)
    {
        return NULL;
    }

    wchar_t* wprefix = (wchar_t*)malloc((wide_size + 1) * sizeof(wchar_t));
    if (wprefix == NULL)
    {
        return NULL;
    }

    mbstowcs(wprefix, prefix, wide_size + 1);
    SearchResult* result = STRINGSKIPLIST_prefixSearchW(self, wprefix, count);
    free(wprefix);
    return result;
}

void STRINGSKIPLIST_printList(StringSkipList* self)
{
    if (!self) return;

    for (int i = self->current_level_ - 1; i >= 0; i--)
    {
        SkipListNode* current = self->header->forward[i];
        wprintf(L"Level %d: ", i);
        while (current)
        {
            wprintf(L"%ls ", current->key);
            current = current->forward[i];
        }
        wprintf(L"\n");
    }
    fflush(stdout);
}
// 必须给一个空指针 意味着控制权移交
bool STRINGSKIPLIST_serializeSimple(StringSkipList* self, char** buffer, int* buffer_size)
{
    if (self == NULL || buffer == NULL || buffer_size == NULL)
    {
        return false;
    }

    uint32_t total_size = sizeof(uint32_t) + sizeof(int) + sizeof(float) + sizeof(int);

    int nodeCount = 0;
    SkipListNode* current = self->header->forward[0];
    while (current)
    {
        nodeCount++;
        // 转换为UTF-8 即单位char再做序列化 和直接使用sizeof(wchar_t) * wcslen(current->key)类似但更稳定
        size_t utf8KeyLength = wcstombs(NULL, current->key, 0);
        if (utf8KeyLength == (size_t)-1)
        {
            return false;
        }
        total_size += sizeof(uint32_t);  // keyLength
        total_size += utf8KeyLength;    // key data
        total_size += sizeof(BookData);  // value
        current = current->forward[0];
    }

    char* buf = (char*)malloc(total_size);
    if (buf == NULL)
    {
        return false;
    }

    char* ptr = buf;
    uint32_t magic_number = 0x2333F;

    memcpy(ptr, &magic_number, sizeof(uint32_t));
    ptr += sizeof(uint32_t);
    memcpy(ptr, &self->max_level_, sizeof(int));
    ptr += sizeof(int);
    memcpy(ptr, &self->probability_, sizeof(float));
    ptr += sizeof(float);
    memcpy(ptr, &nodeCount, sizeof(int));
    ptr += sizeof(int);


    current = self->header->forward[0];
    while (current)
    {
        char* utf8Key = NULL;
        size_t utf8KeyLength = 0;

        utf8KeyLength = wcstombs(NULL, current->key, 0);
        if (utf8KeyLength == (size_t)-1)
        {
            free(buf);
            return false;
        }
        utf8Key = (char*)malloc(utf8KeyLength + 1);
        if (utf8Key == NULL)
        {
            free(buf);
            return false;
        }
        wcstombs(utf8Key, current->key, utf8KeyLength + 1);

        uint32_t keyLength = (uint32_t)utf8KeyLength;

        memcpy(ptr, &keyLength, sizeof(uint32_t));
        ptr += sizeof(uint32_t);
        memcpy(ptr, utf8Key, keyLength);
        ptr += keyLength;
        memcpy(ptr, &current->value, sizeof(BookData));
        ptr += sizeof(BookData);

        free(utf8Key);
        current = current->forward[0];
    }

    *buffer = buf;
    *buffer_size = total_size;
    return true;
}

// 从内存反序列化
StringSkipList* STRINGSKIPLIST_deserializeSimple(const char* buffer, int buffer_size)
{
    if (!buffer || buffer_size <= 0)
    {
        return NULL;
    }

    const char* ptr = buffer;

    uint32_t magic_number;
    memcpy(&magic_number, ptr, sizeof(uint32_t));
    ptr += sizeof(uint32_t);
    if (magic_number != 0x2333F)
    {
        return NULL;
    }

    int max_level_;
    memcpy(&max_level_, ptr, sizeof(int));
    ptr += sizeof(int);

    float probability_;
    memcpy(&probability_, ptr, sizeof(float));
    ptr += sizeof(float);

    int nodeCount;
    memcpy(&nodeCount, ptr, sizeof(int));
    ptr += sizeof(int);

    StringSkipList* self = STRINGSKIPLIST_construct(max_level_, probability_);
    if (!self)
    {
        return NULL;
    }

    for (int i = 0; i < nodeCount; i++)
    {
        uint32_t keyLength;
        memcpy(&keyLength, ptr, sizeof(uint32_t));
        ptr += sizeof(uint32_t);

        char* utf8Key = (char*)malloc(keyLength + 1);
        if (utf8Key == NULL)
        {
            STRINGSKIPLIST_destruct(self);
            return NULL;
        }
        memcpy(utf8Key, ptr, keyLength);
        ptr += keyLength;
        utf8Key[keyLength] = '\0';

        BookData value;
        memcpy(&value, ptr, sizeof(BookData));
        ptr += sizeof(BookData);

        wchar_t* wstr = NULL;
        size_t wide_size = mbstowcs(NULL, utf8Key, 0);
        if (wide_size == (size_t)-1)
        {
            free(utf8Key);
            STRINGSKIPLIST_destruct(self);
            return NULL;
        }
        wstr = (wchar_t*)malloc(sizeof(wchar_t) * (wide_size + 1));
        if (wstr == NULL)
        {
            free(utf8Key);
            STRINGSKIPLIST_destruct(self);
            return NULL;
        }
        mbstowcs(wstr, utf8Key, wide_size + 1);

        if (STRINGSKIPLIST_insertW(self, wstr, &value) != STRINGSKIPLIST_SUCCESS)
        {
            free(wstr);
            free(utf8Key);
            STRINGSKIPLIST_destruct(self);
            return NULL;
        }
        free(utf8Key);
        free(wstr);
    }

    return self;
}

// 从文件指针反序列化
StringSkipList* STRINGSKIPLIST_deserializeSimpleFile(FILE* file)
{
    if (file == NULL)
    {
        return NULL;
    }

    uint32_t magic_number;
    fread(&magic_number, 1, sizeof(uint32_t), file);
    if (magic_number != 0x2333F)
    {
        return NULL;
    }

    int max_level_;
    fread(&max_level_, 1, sizeof(int), file);

    float probability_;
    fread(&probability_, 1, sizeof(float), file);

    int nodeCount;
    fread(&nodeCount, 1, sizeof(int), file);

    StringSkipList* self = STRINGSKIPLIST_construct(max_level_, probability_);
    if (!self)
    {
        return NULL;
    }

    for (int i = 0; i < nodeCount; i++)
    {
        uint32_t keyLength;
        fread(&keyLength, 1, sizeof(uint32_t), file);

        char* utf8Key = (char*)malloc(keyLength + 1);
        if (utf8Key == NULL)
        {
            STRINGSKIPLIST_destruct(self);
            return NULL;
        }
        fread(utf8Key, keyLength, 1, file);
        utf8Key[keyLength] = '\0';

        BookData value;
        fread(&value, sizeof(BookData), 1, file);

        wchar_t* wstr = NULL;
        size_t wide_size = mbstowcs(NULL, utf8Key, 0);
        if (wide_size == (size_t)-1)
        {
            free(utf8Key);
            STRINGSKIPLIST_destruct(self);
            return NULL;
        }
        wstr = (wchar_t*)malloc(sizeof(wchar_t) * (wide_size + 1));
        if (wstr == NULL)
        {
            free(utf8Key);
            STRINGSKIPLIST_destruct(self);
            return NULL;
        }
        mbstowcs(wstr, utf8Key, wide_size + 1);

        if (STRINGSKIPLIST_insertW(self, wstr, &value) != STRINGSKIPLIST_SUCCESS)
        {
            free(wstr);
            free(utf8Key);
            STRINGSKIPLIST_destruct(self);
            return NULL;
        }
        free(utf8Key);
        free(wstr);
    }

    return self;
}

StringSkipListResult STRINGSKIPLIST_deleteW(StringSkipList* self, const wchar_t* wstr)
{
    if (self == NULL || wstr == NULL)
    {
        return STRINGSKIPLIST_ERROR_INVALID_INPUT;
    }

    SkipListNode* current = self->header;
    SkipListNode** update = (SkipListNode**)malloc(sizeof(SkipListNode*) * self->max_level_);
    if (update == NULL)
    {
        return STRINGSKIPLIST_ERROR_MEMORY_ALLOCATION;
    }

    for (int i = 0; i < self->max_level_; i++)
    {
        update[i] = NULL;
    }

    for (int i = self->current_level_ - 1; i >= 0; i--)
    {
        while (current->forward[i] && wcscmp(current->forward[i]->key, wstr) < 0)
        {
            current = current->forward[i];
        }
        update[i] = current;
    }

    current = current->forward[0];

    if (current && wcscmp(current->key, wstr) == 0)
    {
        for (int i = 0; i < current->level; i++)
        {
            if (update[i]->forward[i] != current)
            {
                break;
            }
            update[i]->forward[i] = current->forward[i];
        }

        STRINGSKIPLIST_private_freeNode(current);

        while (self->current_level_ > 1 && self->header->forward[self->current_level_ - 1] == NULL)
        {
            self->current_level_--;
        }

        free(update);
        return STRINGSKIPLIST_SUCCESS;
    }

    free(update);
    return STRINGSKIPLIST_ERROR_NOT_FOUND;
}

StringSkipListResult STRINGSKIPLIST_delete(StringSkipList* self, const char* str)
{
    if (self == NULL || str == NULL)
    {
        return STRINGSKIPLIST_ERROR_INVALID_INPUT;
    }

    size_t wide_size = mbstowcs(NULL, str, 0);
    if (wide_size == (size_t)-1)
    {
        return STRINGSKIPLIST_ERROR_INVALID_INPUT;
    }

    wchar_t* wstr = (wchar_t*)malloc(sizeof(wchar_t) * (wide_size + 1));
    if (wstr == NULL)
    {
        return STRINGSKIPLIST_ERROR_MEMORY_ALLOCATION;
    }

    mbstowcs(wstr, str, wide_size + 1);
    StringSkipListResult result = STRINGSKIPLIST_deleteW(self, wstr);
    free(wstr);
    return result;
}

int STRINGSKIPLIST_deletePrefixW(StringSkipList* self, const wchar_t* prefix)
{
    if (self == NULL || prefix == NULL)
    {
        return 0;
    }

    SkipListNode* current = self->header;
    SkipListNode** update = (SkipListNode**)malloc(sizeof(SkipListNode*) * self->max_level_);
    if (update == NULL)
    {
        return 0;
    }

    for (int i = 0; i < self->max_level_; i++) {
        update[i] = NULL;
    }

    for (int i = self->current_level_ - 1; i >= 0; i--)
    {
        while (current->forward[i] && wcscmp(current->forward[i]->key, prefix) < 0)
        {
            current = current->forward[i];
        }
        update[i] = current;
    }
    current = current->forward[0];

    int deletedCount = 0;
    while (current && wcsncmp(current->key, prefix, wcslen(prefix)) == 0)
    {
        SkipListNode* toDelete = current;
        for (int i = 0; i < toDelete->level; i++)
        {
            if (update[i]->forward[i] != toDelete)
            {
                break;
            }
            update[i]->forward[i] = toDelete->forward[i];
        }

        current = current->forward[0];
        STRINGSKIPLIST_private_freeNode(toDelete);
        deletedCount++;

        while (self->current_level_ > 1 && self->header->forward[self->current_level_ - 1] == NULL)
        {
            self->current_level_--;
        }
    }

    free(update);
    return deletedCount;
}

int STRINGSKIPLIST_deletePrefix(StringSkipList* self, const char* prefix)
{
    if (self == NULL || prefix == NULL)
    {
        return 0;
    }
    size_t wide_size = mbstowcs(NULL, prefix, 0);
    if (wide_size == (size_t)-1)
    {
        return 0;
    }

    wchar_t* wprefix = (wchar_t*)malloc(sizeof(wchar_t) * (wide_size + 1));
    if (wprefix == NULL)
    {
        return 0;
    }

    mbstowcs(wprefix, prefix, wide_size + 1);
    int count = STRINGSKIPLIST_deletePrefixW(self, wprefix);
    free(wprefix);
    return count;
}

StringSkipListIterator STRINGSKIPLIST_iteratorBegin(StringSkipList* self)
{
    StringSkipListIterator iterator;
    if (self)
    {
        iterator.current = self->header->forward[0];
    }
    else
    {
        iterator.current = NULL;
    }
    return iterator;
}

SkipListNode* STRINGSKIPLIST_iteratorNext(StringSkipListIterator* iterator)
{
    if (iterator && iterator->current)
    {
        iterator->current = iterator->current->forward[0];
    }
    return iterator->current;
}

bool STRINGSKIPLIST_iteratorEnd(const StringSkipListIterator* iterator)
{
    return (iterator == NULL || iterator->current == NULL);
}

wchar_t* STRINGSKIPLIST_iteratorGetKey(const StringSkipListIterator* iterator)
{
    if (iterator && iterator->current)
    {
        return iterator->current->key;
    }
    return NULL;
}

const BookData* STRINGSKIPLIST_iteratorGetValue(const StringSkipListIterator* iterator)
{
    if (iterator && iterator->current)
    {
        return &(iterator->current->value);
    }
    return NULL;
}

SearchResult STRINGSKIPLIST_iteratorGetResult(const StringSkipListIterator* iterator)
{
    return { iterator->current->key, iterator->current->value };
}

SearchResult* STRINGSKIPLIST_getAll(StringSkipList* self, int* count)
{
    if (self == NULL || count == NULL)
    {
        return NULL;
    }

    *count = 0;
    SearchResult* results = NULL;
    SkipListNode* current = self->header->forward[0]; // 从第一个实际数据节点开始

    while (current != NULL)
    {
        (*count)++;
        SearchResult* temp = (SearchResult*)realloc(results, sizeof(SearchResult) * (*count));
        if (temp == NULL) 
        {
            if (results != NULL)
            {
                for (int i = 0; i < *count - 1; i++)
                {
                    free(results[i].key);
                }
                free(results);
            }
            *count = 0;
            return NULL;
        }
        results = temp;

        results[*count - 1].key = (wchar_t*)malloc(sizeof(wchar_t) * (wcslen(current->key) + 1));
        if (results[*count - 1].key == NULL)
        {
            for (int i = 0; i < *count - 1; i++)
            {
                free(results[i].key);
            }
            free(results);
            *count = 0;
            return NULL;
        }
        wcscpy(results[*count - 1].key, current->key);

        // 复制 value
        wcscpy(results[*count - 1].value.catagory_, current->value.catagory_);
        results[*count - 1].value.image_toc_index_ = current->value.image_toc_index_;
        results[*count - 1].value.isbn_ = current->value.isbn_;
        results[*count - 1].value.num_ = current->value.num_;
        wcscpy(results[*count - 1].value.publisher_, current->value.publisher_);
        results[*count - 1].value.total_num_ = current->value.total_num_;
        results[*count - 1].value.can_rent_ = current->value.can_rent_;

        current = current->forward[0]; // 移动到下一个节点
    }

    return results;
}
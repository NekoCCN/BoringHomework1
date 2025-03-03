#include "DataWStream.h"
#include <string.h>

DataWStream* DATAWSTREAM_create(const char* file_name, bool is_append_mode)
{
    DataWStream* self = (DataWStream*)malloc(sizeof(DataWStream));
    if (!self)
    {
        return NULL;
    }

    self->file_ptr_ = fopen(file_name, "wb");

    if (!self->file_ptr_)
    {
        free(self);
        return NULL;
    }

    self->file_header_.magic_number = 0x2333F;
    self->file_header_.user_num = 0;

    return self;
}

bool DATAWSTREAM_start(DataWStream* self, User* user, uint32_t user_num, StringSkipList* data, list* user_list)
{
    if (!self || !data || !user_list)
    {
        return false;
    }

    char* skiplist_buffer = NULL;
    size_t skiplist_buffer_size = 0;
    if (!STRINGSKIPLIST_serializeSimple(data, &skiplist_buffer, (int*)&skiplist_buffer_size))
    {
        return false;
    }

    char* userlist_buffer = NULL;
    size_t userlist_buffer_size = 0;
    if (!LIST_serialize(user_list, &userlist_buffer, &userlist_buffer_size))
    {
        free(skiplist_buffer);
        return false;
    }

    self->file_header_.user_num = user_num;
    if (fwrite(&self->file_header_, sizeof(FileHeader), 1, self->file_ptr_) != 1)
    {
        free(skiplist_buffer);
        free(userlist_buffer);
        return false;
    }

    if (fwrite(user, sizeof(User), user_num, self->file_ptr_) != user_num)
    {
        free(skiplist_buffer);
        free(userlist_buffer);
        return false;
    }

    if (fwrite(skiplist_buffer, 1, skiplist_buffer_size, self->file_ptr_) != skiplist_buffer_size)
    {
        free(skiplist_buffer);
        free(userlist_buffer);
        return false;
    }

    if (fwrite(userlist_buffer, 1, userlist_buffer_size, self->file_ptr_) != userlist_buffer_size)
    {
        free(skiplist_buffer);
        free(userlist_buffer);
        return false;
    }

    free(skiplist_buffer);
    free(userlist_buffer);

    fclose(self->file_ptr_);

    return true;
}


void DATAWSTREAM_destory(DataWStream* self)
{
    if (self)
    {
        free(self);
    }
}
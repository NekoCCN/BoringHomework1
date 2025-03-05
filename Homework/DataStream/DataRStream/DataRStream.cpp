#include "DataRStream.h"

DataRStream* DATARSTREAM_create(const char* admin_filename, const char* book_filename, const char* user_filename)
{
    DataRStream* self = (DataRStream*)malloc(sizeof(DataRStream));

    self->user_list_ = LIST_Construct();
    self->data_ = STRINGSKIPLIST_construct(FIBONACCI_NUMBERS_MAX_LEVEL, FIBONACCI_NUMBERS);
    self->user_ = (User*)malloc(sizeof(User));
    wchar_t default_user_name[] = L"admin";
    wcscpy(self->user_[0].name, default_user_name);
    self->user_[0].password_hash = hash_murmur3((void*)default_user_name, sizeof(wchar_t) * wcslen(default_user_name));
    self->user_num_ = 1;

    self->admin_file_ptr_ = fopen(admin_filename, "rb");
    self->book_file_ptr_ = fopen(book_filename, "rb");
    self->user_file_ptr_ = fopen(user_filename, "rb");

    if (self->admin_file_ptr_ == NULL || self->book_file_ptr_ == NULL || self->user_file_ptr_ == NULL)
    {
        return self;
    }

    if (fread(&self->header_, sizeof(FileHeader), 1, self->admin_file_ptr_) != 1)
    {
        fclose(self->admin_file_ptr_);
        fclose(self->book_file_ptr_);
        fclose(self->user_file_ptr_);
        return self;
    }
    if (self->header_.magic_number != 0x2333F)
    {
        return self;
    }
    self->user_ = (User*)realloc(self->user_, sizeof(User) * self->header_.user_num);
    if (self->user_ == NULL)
    {
        fclose(self->admin_file_ptr_);
        fclose(self->book_file_ptr_);
        fclose(self->user_file_ptr_);
        return self;
    }

    if (fread(self->user_, sizeof(User), self->header_.user_num, self->admin_file_ptr_) != self->header_.user_num)
    {
        fclose(self->admin_file_ptr_);
        fclose(self->book_file_ptr_);
        fclose(self->user_file_ptr_);
        return self;
    }

    self->user_num_ = self->header_.user_num;

    STRINGSKIPLIST_destruct(self->data_);
    self->data_ = STRINGSKIPLIST_deserializeSimpleFile(self->book_file_ptr_);

    if (self->data_ == NULL)
    {
        fclose(self->admin_file_ptr_);
        fclose(self->book_file_ptr_);
        fclose(self->user_file_ptr_);
        return self;
    }

    LIST_Destruct(self->user_list_);
    self->user_list_ = LIST_deserializeFile(self->user_file_ptr_);
    if (self->user_list_ == NULL)
    {
        fclose(self->admin_file_ptr_);
        fclose(self->book_file_ptr_);
        fclose(self->user_file_ptr_);
        return self;
    }

    fclose(self->admin_file_ptr_);
    fclose(self->book_file_ptr_);
    fclose(self->user_file_ptr_);

    return self;
}

void DATARSTREAM_destory(DataRStream* self)
{
    LIST_Destruct(self->user_list_);
    STRINGSKIPLIST_destruct(self->data_);
    //free(self->user_);
}

#include "DataRStream.h"

DataRStream* DATARSTREAM_create(const char* filename)
{
	DataRStream* self = (DataRStream*)malloc(sizeof(DataRStream));

    // 二进制模式
    // fread 返回对应读取的元素个数 而不是字节数 有点抽象
	self->file_ptr_ = fopen(filename, "rb");
    if (self->file_ptr_ == NULL)
    {
        self->user_list_ = LIST_Construct();
        self->data_ = STRINGSKIPLIST_construct(FIBONACCI_NUMBERS_MAX_LEVEL, FIBONACCI_NUMBERS);

        self->user_ = (User*)malloc(sizeof(User));
        wchar_t default_user_name[] = L"admin";
        wcscpy(self->user_[0].name, default_user_name);
        self->user_[0].password_hash = hash_murmur3((void*)default_user_name, sizeof(wchar_t) * wcslen(default_user_name));
        self->user_num_ = 1;

        return self;
    }
    else
    {
        if (fread(&self->header_, sizeof(FileHeader), 1, self->file_ptr_) != 1)
        {
            self->user_list_ = LIST_Construct();
            self->data_ = STRINGSKIPLIST_construct(FIBONACCI_NUMBERS_MAX_LEVEL, FIBONACCI_NUMBERS);
           
            self->user_ = (User*)malloc(sizeof(User));
            wchar_t default_user_name[] = L"admin";
            wcscpy(self->user_[0].name, default_user_name);
            self->user_[0].password_hash = hash_murmur3((void*)default_user_name, sizeof(wchar_t) * wcslen(default_user_name));
            self->user_num_ = 1;

            fclose(self->file_ptr_);
            return self;
        }
        if (self->header_.magic_number != 0x2333F)
        {
            self->user_list_ = LIST_Construct();
            self->data_ = STRINGSKIPLIST_construct(FIBONACCI_NUMBERS_MAX_LEVEL, FIBONACCI_NUMBERS);

            self->user_ = (User*)malloc(sizeof(User));
            wchar_t default_user_name[] = L"admin";
            wcscpy(self->user_[0].name, default_user_name);
            self->user_[0].password_hash = hash_murmur3((void*)default_user_name, sizeof(wchar_t) * wcslen(default_user_name));
            self->user_num_ = 1;

            return self;
        }
        self->user_ = (User*)malloc(sizeof(User) * self->header_.user_num);
        if (fread(self->user_, sizeof(User), self->header_.user_num, self->file_ptr_) != self->header_.user_num)
        {
            self->user_list_ = LIST_Construct();
            self->data_ = STRINGSKIPLIST_construct(FIBONACCI_NUMBERS_MAX_LEVEL, FIBONACCI_NUMBERS);
            self->user_num_ = 1;
            
            self->user_ = (User*)malloc(sizeof(User));
            wchar_t default_user_name[] = L"admin";
            wcscpy(self->user_[0].name, default_user_name);
            self->user_[0].password_hash = hash_murmur3((void*)default_user_name, sizeof(wchar_t) * wcslen(default_user_name));
            self->user_num_ = 1;

            fclose(self->file_ptr_);
            return self;
        }
        
        self->user_num_ = self->header_.user_num;

        self->data_ = STRINGSKIPLIST_deserializeSimpleFile(self->file_ptr_);
        if (self->data_ == NULL)
        {
            self->user_list_ = LIST_Construct();
            self->data_ = STRINGSKIPLIST_construct(FIBONACCI_NUMBERS_MAX_LEVEL, FIBONACCI_NUMBERS);
           
            wchar_t default_user_name[] = L"admin";
            wcscpy(self->user_[0].name, default_user_name);
            self->user_[0].password_hash = hash_murmur3((void*)default_user_name, sizeof(wchar_t) * wcslen(default_user_name));
            self->user_num_ = 1;

            fclose(self->file_ptr_);
            return self;
        }
        self->user_list_ = LIST_deserializeFile(self->file_ptr_);
    }

    if (self->file_ptr_ != NULL)
    {
        fclose(self->file_ptr_);
    }

    return self;
}

void DATARSTREAM_destory(DataRStream* self)
{
    LIST_Destruct(self->user_list_);
    STRINGSKIPLIST_destruct(self->data_);
    //free(self->user_);
}

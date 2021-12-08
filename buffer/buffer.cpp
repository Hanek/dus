#include <fstream>

#include "buffer.hpp"

using namespace dus;


buffer::buffer()
    : buffer_(nullptr)
    , pos_(nullptr)
    , size_(0)
{
    size_   = default_buffer_size_;

    buffer_ = new(std::nothrow) char[default_buffer_size_];
    if (!buffer_)
    {
        report_allocation_failure(__PRETTY_FUNCTION__);
        return;
    }

    pos_ = buffer_;
    memset(buffer_, 0x00, size_);
}


buffer::~buffer()
{
    if (buffer_)
    {
        delete[] buffer_;
        buffer_ = nullptr;
    }
}


buffer::buffer(const buffer& src)
    : size_(src.size())
    , size_expected_(src.get_expected())
{
    buffer_ = new(std::nothrow) char[size_];
    if (!buffer_)
    {
        report_allocation_failure(__PRETTY_FUNCTION__);
        return;
    }

    memcpy(buffer_, src.get(), size_);
    pos_ = buffer_ + src.length();
}


buffer::buffer(buffer&& src)
    : buffer_(src.get())
    , pos_(src.get_position())
    , size_(src.size())
    , size_expected_(src.get_expected())
{
    src.buffer_ = nullptr;
    src.pos_    = nullptr;
    src.size_          = 0;
    src.size_expected_ = 0;
}


buffer& buffer::operator = (const buffer& rhs)
{
    delete this->buffer_;

    buffer_ = new(std::nothrow) char[rhs.size()];
    if (!buffer_)
    {
        report_allocation_failure(__PRETTY_FUNCTION__);
        return *this;
    }

    size_          = rhs.size();
    size_expected_ = rhs.get_expected();
    memcpy(buffer_, rhs.get(), size_);
    pos_ = buffer_ + rhs.length();

    return *this;
}


buffer& buffer::operator = (buffer&& rhs)
{
    if (this != &rhs)
    {

        delete this->buffer_;
        buffer_        = rhs.buffer_;
        size_          = rhs.size();
        size_expected_ = rhs.get_expected();
        pos_           = rhs.get_position();

        rhs.buffer_ = nullptr;
        rhs.pos_    = nullptr;
        rhs.size_          = 0;
        rhs.size_expected_ = 0;
    }

    return *this;
}


void buffer::dump(const std::string& path) const
{
    std::ofstream file(path, std::ios::out | std::ios::binary);
    file.write(buffer_, size_);
    file.close();
}


void buffer::expect(uint32_t size)
{
    size_expected_ = size;
    if (size > size_)
    {
        size_ = size;
        char* buffer_new;
        buffer_new = new(std::nothrow) char[size_];
        if (!buffer_new)
        {
            report_allocation_failure(__PRETTY_FUNCTION__);
            return;
        }

        memset(buffer_new, 0x00, size_);
        delete[] buffer_;

        buffer_ = buffer_new;
        pos_    = buffer_;
    }
}


void buffer::advance(uint32_t size)
{
    pos_ += size;
    size_expected_ -= size;
}


void buffer::clear()
{
    pos_ = buffer_;
    memset(buffer_, 0x00, size_);
}


void buffer::serialize(const std::string& str)
{
    serialize(str.c_str());
}


void buffer::serialize(const char* str)
{
    if (!str) { return; }

    while (strlen(str) >= get_space_left())
    { realloc(); }

    memcpy(pos_, str, strlen(str));
    pos_ += strlen(str);
    *pos_ = 0x00;
    pos_ += 1;
}


void buffer::serialize(const void* p, uint32_t length)
{
    if (!p || !length) { return; }

    while (length >= get_space_left())
    { realloc(); }

    memcpy(pos_, p, length);
    pos_ += length;
}


void buffer::deserialize(char* str)
{
    memcpy(str, pos_, strlen(pos_));
    pos_ += strlen(pos_) + 1;
}


void buffer::deserialize(std::string& str)
{
    str = std::string(pos_);
    pos_ += strlen(pos_) + 1;
}


void buffer::deserialize(void* p, uint32_t length)
{
    if (!p || !length) { return; }

    memcpy(p, pos_, length);
    pos_ += length;
}


void buffer::realloc()
{
    uint32_t size_old = size_;
    uint32_t pos_old  = pos_ - buffer_;

    /* double the buffer */
    size_ *= 2;

    char* buffer_new;
    buffer_new = new(std::nothrow) char[size_];
    if (!buffer_new)
    {
        report_allocation_failure(__PRETTY_FUNCTION__);
        return;
    }

    memset(buffer_new, 0x00, size_);
    memcpy(buffer_new, buffer_, size_old);
    delete[] buffer_;

    buffer_ = buffer_new;
    pos_    = buffer_ + pos_old;
}


void buffer::report_allocation_failure(const char* function)
{
    std::cerr << function << "::memory allocation failed" << std::endl;
}


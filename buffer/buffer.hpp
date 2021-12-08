#include <iostream>
#include <cstdio>
#include <cstring>

#ifndef BUFFER_H_
#define BUFFER_H_


namespace dus
{

/* Simple buffer to hold POD, c-string and binary binary data */
class buffer
{
public:

    /* Constructs empty buffer of default size */ 
    buffer();

    ~buffer();
    
    buffer(const buffer&);

    buffer(buffer&&);

    buffer& operator = (const buffer& rhs);

    buffer& operator = (buffer&& rhs);

    /* Returns current length in bytes as the distance between 
     * current position and buffer initial point */  
    uint32_t length() const { return pos_ - buffer_; }

    /* Returns buffer initial point */
    char* get() const { return buffer_; }
    
    /* Returns buffer current position */
    char* get_position() const { return pos_; }

    /* Number of bytes in memory allocated */
    uint32_t size() const { return size_; }

    /* Returns remaining of bytes expected with regards to advance() */
    uint32_t get_expected() const { return size_expected_; }
    
    /* Make the binary dump of the entire buffer */
    void dump(const std::string&) const;

    uint32_t default_buffer_size() const { return default_buffer_size_; }

    /* Used to set expected number of bytes when data being read sequentially.
     * Read available data, advance buffer by amount of bytes supplied.  
     * Consider the following use case:
     *   
     *   length = read()
     *   buffer.expect(length)
     *
     *   while(buffer.get_expected())
     *     count = read(buffer.get_position())
     *     buffer.advance(count) */
    void expect(uint32_t);

    /* Advance position by requested amount of bytes */
    void advance(uint32_t size);

    /* Sets current position to the beginning of the buffer,
     * zero out the buffer's memory */
    void clear();

    /* Sets current position to the beginning of the buffer */
    void reset() { pos_ = buffer_; }

    void serialize(const std::string& str);

    /* Serializes c-string preserving null termination */
    void serialize(const char* str);
    
    void serialize(const void* p, uint32_t length);

    template <class T> void serialize(const T var)
    {
        if (!std::is_trivial<T>::value)
        {
            /* skipping non_trivial type */
            std::cerr << __PRETTY_FUNCTION__ << "::type is not supported" << std::endl;
            return;
        }

        while (sizeof(T) >= get_space_left())
        { realloc(); }

        memcpy(pos_, &var, sizeof(T));
        pos_ += sizeof(T);
    }

    /* De-serializes c-string considering null termination */
    void deserialize(char* str);

    void deserialize(std::string& str);

    void deserialize(void* p, uint32_t length);

    template <class T> void deserialize(T& var)
    {
        memcpy(&var, pos_, sizeof(T));
        pos_ += sizeof(T);
    }


private:
    char* buffer_;

    /* Current position, every serialize/deserialize call advances it by
     * the data type size, or by the memory length in case of strings and binary data */  
    char* pos_;

    /* Number of bytes allocated and pointed to by buffer_ */
    uint32_t size_;

    /* Indicates how much bytes remains until the buffer is filled while using expect/advance methods */ 
    uint32_t size_expected_;

    void report_allocation_failure(const char*);
    
    static constexpr uint32_t default_buffer_size_ = 1024;

    /* Being constructed with default_buffer_size_, the buffer grows up as serialization require */   
    void realloc();

    constexpr size_t get_space_left() const { return static_cast<size_t>(size_ - (pos_ - buffer_) - 1); }

};

}




#endif
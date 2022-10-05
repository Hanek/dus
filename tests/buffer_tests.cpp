#include <vector>
#include <random>
#include <gtest/gtest.h>

#include "../buffer/buffer.hpp"



TEST(ConstructionTest, Ctor)
{
    dus::buffer b;

    EXPECT_EQ(b.size(), b.default_buffer_size());
    EXPECT_EQ(b.get(), b.get_position());
    EXPECT_EQ(b.length(), 0);
    EXPECT_NE(b.get(), nullptr);
}



TEST(ConstructionTest, CopyCtor)
{
    dus::buffer a;
    dus::buffer b(a);

    EXPECT_EQ(a.size(), b.size());
    EXPECT_NE(a.get(), b.get());
    EXPECT_EQ(a.length(), b.length());
}


void foo(dus::buffer&& a)
{
    dus::buffer b;
    b = std::move(a);
}


TEST(ConstructionTest, MoveCtor)
{
    dus::buffer a;
    const char* p = a.get();

    std::vector<dus::buffer> v;
    v.push_back(std::move(a));
    
    EXPECT_EQ(p, v[0].get());
    EXPECT_EQ(a.get(), nullptr);
    EXPECT_EQ(a.size(), 0);
    EXPECT_EQ(a.length(), 0);
}


TEST(AssignmentTest, Assignment)
{
    dus::buffer a;
    dus::buffer b;

    uint64_t a_int;
    uint64_t b_int;
    std::string a_str;
    std::string b_str;

    a.serialize("test string\n");
    a.serialize<uint64_t>(0xffffffff);

    b = a;
    
    a.reset();
    a.deserialize(a_str);
    a.deserialize<uint64_t>(a_int);
    
    b.reset();
    b.deserialize(b_str);
    b.deserialize<uint64_t>(b_int);

    EXPECT_EQ(a_str, b_str);
    EXPECT_EQ(a_int, b_int);
}


dus::buffer make_buffer(std::string& str, uint64_t val)
{
    dus::buffer a;
    a.serialize(str);
    a.serialize<uint64_t>(val);

    return a;
}


TEST(AssignmentTest, MoveAssignment)
{
    uint64_t a_int;
    std::string a_str;
    uint64_t test_int = 0xffffffff;
    std::string test_string = "test_string\n";


    dus::buffer a;
    const char* p = a.get();

    a = make_buffer(test_string, test_int);

    a.reset();
    a.deserialize(a_str);
    a.deserialize<uint64_t>(a_int);

    EXPECT_NE(a.get(), p);
    EXPECT_EQ(a_str, test_string);
    EXPECT_EQ(a_int, test_int);
}


TEST(Serialization, Pod)
{
    bool     bool_val;
    uint8_t  uint8_val;
    uint16_t uint16_val;
    uint32_t uint32_val;
    uint64_t uint64_val;
    int8_t   int8_val;
    int16_t  int16_val;
    int32_t  int32_val;
    int64_t  int64_val;

    dus::buffer a;
    a.serialize<bool>(false);
    a.serialize<uint8_t>(0xaa);
    a.serialize<uint16_t>(0xbbbb);
    a.serialize<uint32_t>(0xcccc);
    a.serialize<uint64_t>(0xdddd);
    a.serialize<int8_t>(0xa);
    a.serialize<int16_t>(0xbb);
    a.serialize<int32_t>(0xcccc);
    a.serialize<int64_t>(0xdddd);

    a.reset();

    a.deserialize<bool>(bool_val);
    a.deserialize<uint8_t>(uint8_val);
    a.deserialize<uint16_t>(uint16_val);
    a.deserialize<uint32_t>(uint32_val);
    a.deserialize<uint64_t>(uint64_val);
    a.deserialize<int8_t>(int8_val);
    a.deserialize<int16_t>(int16_val);
    a.deserialize<int32_t>(int32_val);
    a.deserialize<int64_t>(int64_val);

    EXPECT_EQ(bool_val, false);
    EXPECT_EQ(uint8_val, 0xaa);
    EXPECT_EQ(uint16_val, 0xbbbb);
    EXPECT_EQ(uint32_val, 0xcccc);
    EXPECT_EQ(uint64_val, 0xdddd);
    EXPECT_EQ(int8_val, 0xa);
    EXPECT_EQ(int16_val, 0xbb);
    EXPECT_EQ(int32_val, 0xcccc);
    EXPECT_EQ(int64_val, 0xdddd);

}


TEST(Serialization, String)
{
    std::string string1;
    std::string string2;

    dus::buffer a;
    a.serialize("test string1");
    a.serialize(std::string("test string2"));

    a.reset();

    a.deserialize(string1);
    a.deserialize(string2);

    EXPECT_EQ(string1, "test string1");
    EXPECT_EQ(string2, "test string2");
}


TEST(Serialization, Binary)
{
    uint32_t length;
    char b[32] = {0};
    const char* p = "test_string";

    dus::buffer a;
    a.serialize<uint32_t>(strlen(p));
    a.serialize(p, strlen(p));

    a.reset();

    a.deserialize<uint32_t>(length);
    a.deserialize(b, length);

    EXPECT_EQ(strlen(p), length);
    EXPECT_EQ(0, strcmp(p, b));
}


TEST(Allocation, Pod)
{
    dus::buffer a;
    uint64_t val = 0;
    uint32_t max_buffer_size = a.default_buffer_size() * a.default_buffer_size(); 

    while(a.size() < max_buffer_size)
    {
        a.serialize<uint64_t>(val++);
    }

    a.reset();

    for(uint64_t i = 0; i < val; i++)
    {
        uint64_t cur;
        a.deserialize<uint64_t>(cur);
        EXPECT_EQ(cur, i);
    }
}


TEST(Allocation, String)
{
    dus::buffer a;
    uint64_t val = 0;
    std::string str = "test string\n";

    uint32_t max_buffer_size = a.default_buffer_size() * a.default_buffer_size(); 

    while(a.size() < max_buffer_size)
    {
        a.serialize(str);
        val++;
    }

    a.reset();

    for(uint64_t i = 0; i < val; i++)
    {
        std::string cur;
        a.deserialize(cur);
        EXPECT_EQ(cur, str);
    }

}


class io
{
private:
    std::vector<char> buffer_;
    char* pos_;
    int reduction_;
    std::default_random_engine rnd_;

public:

    io(int size)
        : buffer_(size)
        , pos_(buffer_.data())
        , reduction_(10)
        , rnd_(std::random_device{}())
    {}

    void reset() { pos_ = buffer_.data(); }

    uint32_t read(void* buffer, int length)
    {
        int nbytes = rnd_() % (buffer_.size() / reduction_);
        if (nbytes > length) { nbytes = length; }

        memcpy(buffer, pos_, nbytes);

        pos_ += nbytes;

        return nbytes;
    }

    uint32_t write(const char* buffer, int length)
    {
        int nbytes = rnd_() % (buffer_.size() / reduction_);
        if (nbytes > length) { nbytes = length; }

        memcpy(pos_, buffer, nbytes);

        pos_ += nbytes;

        return nbytes;
    }
};



TEST(IO, ReadWrite)
{
    dus::buffer a;
    dus::buffer b;
    int size = 4096;

    io socket(size);

    /* fill the buffer with some values */
    for (int i = 0; i < size / (int)(sizeof(int)); i++) { a.serialize<int>(i); }

    a.reset();
    a.expect(size);

    /* write to socket in non-blocking manner */
    while (a.get_expected())
    {
        int count = socket.write(a.get_position(), a.get_expected());
        a.advance(count);
    }


    socket.reset();
    b.expect(size);

    /* read from socket in non-blocking manner */
    while (b.get_expected())
    {
        int count = socket.read(b.get_position(), b.get_expected());
        b.advance(count);
    }

    b.reset();

    for (int i = 0; i < size / (int)(sizeof(int)); i++)
    {
        int val;
        b.deserialize<int>(val);

        EXPECT_EQ(i, val);
    }
}



int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    std::cout << "Running tests..." << std::endl;

    int ret{RUN_ALL_TESTS()};
    
    if (!ret) { std::cout << "---===[ Success ]===---" << std::endl; }
    else { std::cout << "---===[ Failure ]===---" << std::endl; }

    return 0;
}
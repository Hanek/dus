#include <vector>
#include <random>
#include <gtest/gtest.h>
#include <list>

#include "../list/list.hpp"


TEST(Construction, DefaultCtor)
{
    dus::list<int> a;

    EXPECT_EQ(a.size(), 0);
    EXPECT_EQ(a.begin(), a.end());
}



// TEST(Construction, CopyCtor)
// {
//     // dus::buffer a;
//     // dus::buffer b(a);

//     // EXPECT_EQ(a.size(), b.size());
//     // EXPECT_NE(a.get(), b.get());
//     // EXPECT_EQ(a.length(), b.length());
// }


TEST(IteratorIncrementPostfix, Pod)
{
    dus::list<int> a;
    dus::list<int>::iterator it;

   

    a.push_back(1);
    a.push_back(2);
    // a.push_back(3);
    // a.push_back(4);
    // a.push_back(5);
    
    a.print();

    // it = a.begin();

    // EXPECT_EQ(it, a.begin());
    // EXPECT_EQ(*(it++), 1);
    // EXPECT_EQ(*(it++), 2);
    // EXPECT_EQ(*(it++), 3);
    // EXPECT_EQ(it, a.end());
}


// TEST(IteratorIncrementPrefix, Pod)
// {
//     dus::list<int> a;
//     dus::list<int>::iterator it;

//     a.push(1);
//     a.push(2);
//     a.push(3);

//     it = a.begin();

//     EXPECT_EQ(it, a.begin());
//     EXPECT_EQ(*(it), 1);
//     EXPECT_EQ(*(++it), 2);
//     EXPECT_EQ(*(++it), 3);
//     EXPECT_EQ(++it, nullptr);
//     EXPECT_EQ(it, a.end());
// }


// TEST(IteratorDecrementPostfix, Pod)
// {
//     dus::list<int> a;
//     dus::list<int>::iterator it;

//     a.push(1);
//     a.push(2);
//     a.push(3);

//     it = a.end();

//     EXPECT_EQ(it, a.end());
//     --it;
//     // EXPECT_EQ(*it, 0);
//     // EXPECT_EQ(*(--it), 3);
//     // EXPECT_EQ(*(--it), 2);
//     // EXPECT_EQ(*(it), 1);
//     // EXPECT_EQ(it, a.begin());
// }


// TEST(IteratorDecrementPrefix, Pod)
// {
//     dus::list<int> a;
//     dus::list<int>::iterator it;

//     a.push(1);
//     a.push(2);
//     a.push(3);

//     it = a.begin();

//     EXPECT_EQ(it, a.begin());
//     EXPECT_EQ(*(it), 1);
//     EXPECT_EQ(*(++it), 2);
//     EXPECT_EQ(*(++it), 3);
//     EXPECT_EQ(++it, nullptr);
//     EXPECT_EQ(it, a.end());
// }

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    std::cout << "Running tests..." << std::endl;

    dus::list<int> l;
    int ret{RUN_ALL_TESTS()};
    
    if (!ret) { std::cout << "---===[ Success ]===---" << std::endl; }
    else { std::cout << "---===[ Failure ]===---" << std::endl; }

    return 0;
}

// gcc -std=c++14 -lgtest -o span_map_test span_map_test.cpp

#include "span_map.h"
#include <gtest/gtest.h>
#include <iostream>

using namespace std;

TEST(SpanMap, randomTest)
{
    cout << "Random test..." << endl;

    std::srand(std::time(0));
    span_map<unsigned, char> tested('A');

    constexpr static size_t roundTotal = 15000;

    for (size_t round = 1; round < roundTotal; ++round)
    {
        cout << "Round " << round << " of " << roundTotal << "..." << endl;

        std::map<unsigned, char> origMap;
        for (unsigned idx = 0; idx < 5000; ++idx)
            origMap[idx] = tested[idx];

        unsigned lower = std::rand() % 5000;
        unsigned upper = std::rand() % 5000;
        char val = 'A' + std::rand() % 24;

        tested.assign(lower, upper, val);
        if (lower < upper)
        {
            for (unsigned i = lower; i < upper; ++i)
            {
                if (!(tested[i] == val))
                {
                    EXPECT_EQ(val, tested[i]);
                }
                origMap[i] = val; 
            }
        }

        for (unsigned idx = 0; idx < 5000; ++idx)
            EXPECT_EQ(origMap[idx], tested[idx]);
    }
}

TEST(SpanMap, test0)
{
    span_map<unsigned, char> tested('A');

    EXPECT_EQ('A', tested[0]);
    EXPECT_EQ('A', tested[1]);
    EXPECT_EQ('A', tested[std::numeric_limits<uint>::max()]);

    tested.assign(1, 2, 'B');
    EXPECT_EQ('A', tested[0]);
    EXPECT_EQ('B', tested[1]);
    EXPECT_EQ('A', tested[2]);
    EXPECT_EQ('A', tested[3]);
    EXPECT_EQ('A', tested[std::numeric_limits<uint>::max()-1]);
    EXPECT_EQ('A', tested[std::numeric_limits<uint>::max()]);

    tested.assign(1, 4, 'C');
    EXPECT_EQ('A', tested[0]);
    EXPECT_EQ('C', tested[1]);
    EXPECT_EQ('C', tested[2]);
    EXPECT_EQ('C', tested[3]);
    EXPECT_EQ('A', tested[4]);
    EXPECT_EQ('A', tested[std::numeric_limits<uint>::max()-1]);
    EXPECT_EQ('A', tested[std::numeric_limits<uint>::max()]);

    tested.assign(1, 2, 'D');
    EXPECT_EQ('A', tested[0]);
    EXPECT_EQ('D', tested[1]);
    EXPECT_EQ('C', tested[2]);
    EXPECT_EQ('C', tested[3]);
    EXPECT_EQ('A', tested[4]);
    EXPECT_EQ('A', tested[std::numeric_limits<uint>::max()-1]);
    EXPECT_EQ('A', tested[std::numeric_limits<uint>::max()]);

    tested.assign(2, 5, 'E');
    EXPECT_EQ('A', tested[0]);
    EXPECT_EQ('D', tested[1]);
    EXPECT_EQ('E', tested[2]);
    EXPECT_EQ('E', tested[3]);
    EXPECT_EQ('E', tested[4]);
    EXPECT_EQ('A', tested[5]);
    EXPECT_EQ('A', tested[6]);
    EXPECT_EQ('A', tested[std::numeric_limits<uint>::max()-1]);
    EXPECT_EQ('A', tested[std::numeric_limits<uint>::max()]);

    tested.assign(2, 5, 'F');
    EXPECT_EQ('A', tested[0]);
    EXPECT_EQ('D', tested[1]);
    EXPECT_EQ('F', tested[2]);
    EXPECT_EQ('F', tested[3]);
    EXPECT_EQ('F', tested[4]);
    EXPECT_EQ('A', tested[5]);
    EXPECT_EQ('A', tested[6]);
    EXPECT_EQ('A', tested[std::numeric_limits<uint>::max()-1]);
    EXPECT_EQ('A', tested[std::numeric_limits<uint>::max()]);

    tested.assign(0, 10, 'F');
    EXPECT_EQ('F', tested[0]);
    EXPECT_EQ('F', tested[1]);
    EXPECT_EQ('F', tested[2]);
    EXPECT_EQ('F', tested[3]);
    EXPECT_EQ('F', tested[4]);
    EXPECT_EQ('F', tested[5]);
    EXPECT_EQ('F', tested[6]);
    EXPECT_EQ('F', tested[7]);
    EXPECT_EQ('F', tested[8]);
    EXPECT_EQ('F', tested[9]);
    EXPECT_EQ('A', tested[10]);
    EXPECT_EQ('A', tested[11]);
    EXPECT_EQ('A', tested[std::numeric_limits<uint>::max()-1]);
    EXPECT_EQ('A', tested[std::numeric_limits<uint>::max()]);

    tested.assign(0, std::numeric_limits<uint>::max(), 'G');
    EXPECT_EQ('G', tested[0]);
    EXPECT_EQ('G', tested[1]);
    EXPECT_EQ('G', tested[2]);
    EXPECT_EQ('G', tested[3]);
    EXPECT_EQ('G', tested[4]);
    EXPECT_EQ('G', tested[5]);
    EXPECT_EQ('G', tested[6]);
    EXPECT_EQ('G', tested[7]);
    EXPECT_EQ('G', tested[8]);
    EXPECT_EQ('G', tested[9]);
    EXPECT_EQ('G', tested[10]);
    EXPECT_EQ('G', tested[11]);
    EXPECT_EQ('G', tested[std::numeric_limits<uint>::max()-1]);
    EXPECT_EQ('A', tested[std::numeric_limits<uint>::max()]);

    tested.assign(5, 8, 'H');
    EXPECT_EQ('G', tested[0]);
    EXPECT_EQ('G', tested[1]);
    EXPECT_EQ('G', tested[2]);
    EXPECT_EQ('G', tested[3]);
    EXPECT_EQ('G', tested[4]);
    EXPECT_EQ('H', tested[5]);
    EXPECT_EQ('H', tested[6]);
    EXPECT_EQ('H', tested[7]);
    EXPECT_EQ('G', tested[8]);
    EXPECT_EQ('G', tested[9]);
    EXPECT_EQ('G', tested[10]);
    EXPECT_EQ('G', tested[11]);
    EXPECT_EQ('G', tested[std::numeric_limits<uint>::max()-1]);
    EXPECT_EQ('A', tested[std::numeric_limits<uint>::max()]);
    
    tested.assign(4, 6, 'I');
    EXPECT_EQ('G', tested[0]);
    EXPECT_EQ('G', tested[1]);
    EXPECT_EQ('G', tested[2]);
    EXPECT_EQ('G', tested[3]);
    EXPECT_EQ('I', tested[4]);
    EXPECT_EQ('I', tested[5]);
    EXPECT_EQ('H', tested[6]);
    EXPECT_EQ('H', tested[7]);
    EXPECT_EQ('G', tested[8]);
    EXPECT_EQ('G', tested[9]);
    EXPECT_EQ('G', tested[10]);
    EXPECT_EQ('G', tested[11]);
    EXPECT_EQ('G', tested[std::numeric_limits<uint>::max()-1]);
    EXPECT_EQ('A', tested[std::numeric_limits<uint>::max()]);

    tested.assign(2, 9, 'J');
    EXPECT_EQ('G', tested[0]);
    EXPECT_EQ('G', tested[1]);
    EXPECT_EQ('J', tested[2]);
    EXPECT_EQ('J', tested[3]);
    EXPECT_EQ('J', tested[4]);
    EXPECT_EQ('J', tested[5]);
    EXPECT_EQ('J', tested[6]);
    EXPECT_EQ('J', tested[7]);
    EXPECT_EQ('J', tested[8]);
    EXPECT_EQ('G', tested[9]);
    EXPECT_EQ('G', tested[10]);
    EXPECT_EQ('G', tested[11]);
    EXPECT_EQ('G', tested[std::numeric_limits<uint>::max()-1]);
    EXPECT_EQ('A', tested[std::numeric_limits<uint>::max()]);

    tested.assign(2, 2, 'Z');
    EXPECT_EQ('G', tested[0]);
    EXPECT_EQ('G', tested[1]);
    EXPECT_EQ('J', tested[2]);
    EXPECT_EQ('J', tested[3]);
    EXPECT_EQ('J', tested[4]);
    EXPECT_EQ('J', tested[5]);
    EXPECT_EQ('J', tested[6]);
    EXPECT_EQ('J', tested[7]);
    EXPECT_EQ('J', tested[8]);
    EXPECT_EQ('G', tested[9]);
    EXPECT_EQ('G', tested[10]);
    EXPECT_EQ('G', tested[11]);
    EXPECT_EQ('G', tested[std::numeric_limits<uint>::max()-1]);
    EXPECT_EQ('A', tested[std::numeric_limits<uint>::max()]);

    tested.assign(4, 3, 'Z');
    EXPECT_EQ('G', tested[0]);
    EXPECT_EQ('G', tested[1]);
    EXPECT_EQ('J', tested[2]);
    EXPECT_EQ('J', tested[3]);
    EXPECT_EQ('J', tested[4]);
    EXPECT_EQ('J', tested[5]);
    EXPECT_EQ('J', tested[6]);
    EXPECT_EQ('J', tested[7]);
    EXPECT_EQ('J', tested[8]);
    EXPECT_EQ('G', tested[9]);
    EXPECT_EQ('G', tested[10]);
    EXPECT_EQ('G', tested[11]);
    EXPECT_EQ('G', tested[std::numeric_limits<uint>::max()-1]);
    EXPECT_EQ('A', tested[std::numeric_limits<uint>::max()]);

    tested.assign(5, 6, 'K');
    EXPECT_EQ('G', tested[0]);
    EXPECT_EQ('G', tested[1]);
    EXPECT_EQ('J', tested[2]);
    EXPECT_EQ('J', tested[3]);
    EXPECT_EQ('J', tested[4]);
    EXPECT_EQ('K', tested[5]);
    EXPECT_EQ('J', tested[6]);
    EXPECT_EQ('J', tested[7]);
    EXPECT_EQ('J', tested[8]);
    EXPECT_EQ('G', tested[9]);
    EXPECT_EQ('G', tested[10]);
    EXPECT_EQ('G', tested[11]);
    EXPECT_EQ('G', tested[std::numeric_limits<uint>::max()-1]);
    EXPECT_EQ('A', tested[std::numeric_limits<uint>::max()]);
}

TEST(SpanMap, test1)
{
    span_map<unsigned, char> tested('A');
    tested.assign(23, 947, 'I');
    tested.assign(702, 734, 'A');
    EXPECT_EQ('A', tested[702]);
}

TEST(SpanMap, test2)
{
    span_map<unsigned, char> tested('A');
    tested.assign(2540, 4140, 'N');
    tested.assign(2348, 3288, 'J');
    tested.assign(2865, 3760, 'J');
    EXPECT_EQ('J', tested[3288]);
}

TEST(SpanMap, test3)
{
    span_map<unsigned, char> tested('A');
    tested.assign(1,2, 'A');
    EXPECT_EQ(1, tested.size());
}

TEST(SpanMap, test4)
{
    span_map<unsigned, char> tested('A');
    tested.assign(1041, 4050, 'P');
    tested.assign(0, 3198, 'A');
    EXPECT_EQ('A', tested[0]);
}

TEST(SpanMap, test5)
{
    span_map<unsigned, char> tested('A');
    tested.assign(277, 4627, 'D');
    tested.assign(1271, 3212, 'D');
    EXPECT_EQ(3, tested.size());
}

TEST(SpanMap, test6)
{
    span_map<unsigned, char> tested('A');
    tested.assign(416, 1975, 'X');
    tested.assign(0, 1933, 'X');
    EXPECT_EQ(2, tested.size());
}

TEST(SpanMap, test7)
{
    span_map<unsigned, char> tested('A');
    tested.assign(193, 4332, 'H');
    EXPECT_EQ('H', tested[193]);
}

int main(int argc, char* argv[]) 
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}


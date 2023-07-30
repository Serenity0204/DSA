#include "gtest/gtest.h"
#include <iomanip>
#include <iostream>
#ifdef _WIN32
#include <windows.h>
#endif
#include <stdio.h>  /* printf, NULL */
#include <stdlib.h> /* srand, rand */
#include <time.h>   /* time */
#include <unordered_map>
//------------------------------------------------------------------------------
//@TODO: include all files we are testing:
#include "../../includes/Map/map.h"
#include "../../includes/Map/mmap.h"

//------------------------------------------------------------------------------

using namespace std;
int randNum()
{
    return rand() % 10001;
}

template <class T>
bool is_same_vector(const vector<T>& v1, const vector<T>& v2)
{
    if (v1.size() != v2.size()) return false;
    for (int i = 0; i < v1.size(); ++i)
        if (v1[i] != v2[i]) return false;
    return true;
}
//------------------------------------------------------------------------------
//@TODO: add more test functions here:

bool test_map1(bool debug = false)
{
    unordered_map<int, string> standard_map;
    DSA::Map<int, string> map;
    // Add elements to the map
    int n = 100000;
    for (size_t i = 0; i < n; i++)
    {
        int rand_num = randNum();
        map[rand_num] = to_string(rand_num);
        standard_map[rand_num] = to_string(rand_num);
        if (map[rand_num] != standard_map[rand_num])
        {
            if (debug) cout << map << endl;
            return false;
        }
    }
    int i = 0;
    while (!map.empty())
    {
        map.erase(i);
        i++;
    }
    if (!map.empty()) return false;
    return true;
}

bool test_mmap1(bool debug = false)
{
    unordered_map<int, vector<string>> standard_map;
    DSA::MMap<int, string> mmap;
    int n = 100000;
    for (size_t i = 0; i < n; i++)
    {
        int rand_num = randNum();
        mmap.insert(rand_num, to_string(rand_num));
        standard_map[rand_num].push_back(to_string(rand_num));
        bool same = is_same_vector<string>(mmap[rand_num], standard_map[rand_num]);
        if (!same)
        {
            if (debug) cout << mmap << endl;
            return false;
        }
    }
    return true;
}

bool test_hybrid1(bool debug = false)
{
    DSA::Map<int, DSA::MMap<string, int>> map_with_multimap_val;
    DSA::Map<int, DSA::MMap<string, int>>::Iterator it;
    int size = 50;
    for (int i = 1; i <= size; ++i)
    {
        DSA::MMap<string, int> temp;
        temp.insert(to_string(i), i);
        temp.insert(to_string(i), i * 2);
        temp.insert(to_string(i * 2), i);
        temp.insert(to_string(i * 2), i * 2);
        map_with_multimap_val[i] = temp;
    }
    for (it = map_with_multimap_val.begin(); it != map_with_multimap_val.end(); ++it)
    {
        cout << "key:" << endl
             << (*it).key << endl;
        cout << "value:" << endl
             << (*it).value << endl;
    }
    return true;
}
//--
//------------------------------------------------------------------------------

// Lord help me!
const bool debug = false;

TEST(TEST_MAP, TestMap1)
{
    bool success = test_map1(debug);
    EXPECT_EQ(true, success);
}
TEST(TEST_MMAP, TestMMap1)
{
    bool success = test_mmap1(debug);
    EXPECT_EQ(true, success);
}
TEST(TEST_HYBRID, TestHybrid1)
{
    bool success = test_hybrid1(debug);
    EXPECT_EQ(true, success);
}
//------------------------------------------------------------------------------

int main(int argc, char** argv)
{
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif
    srand(time(NULL));
    ::testing::InitGoogleTest(&argc, argv);
    std::cout << "\n\n----------running testB.cpp---------\n\n"
              << std::endl;
    return RUN_ALL_TESTS();
}

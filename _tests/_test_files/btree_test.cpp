#include "gtest/gtest.h"
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <queue>
//------------------------------------------------------------------------------
//@TODO: include all files we are testing:
#include "../../includes/Btree/btree.h"
#ifdef _WIN32
#include <windows.h>
#endif

//------------------------------------------------------------------------------

using namespace std;

bool is_sorted(int arr[], size_t size)
{
    for (int i = 0; i < size - 1; ++i)
        if (arr[i] > arr[i + 1]) return false;
    return true;
}

int rand_num()
{
    int random_num = std::rand() % 1201;
    return random_num;
}

int rand_num_small()
{
    int random_num = std::rand() % 50;
    return random_num;
}
//------------------------------------------------------------------------------
//@TODO: add more test functions here:

bool test_btree1(bool debug = false)
{
    int a[] = {43, 20, 96, 100, 69, 7, 9, 105, 90};
    size_t n = 9;
    DSA::BTree<int> bt(a, n);
    if (debug)
    {
        cout << "btree:" << endl;
        cout << bt << endl;
    }
    if (!bt.is_valid()) return false;
    for (int i = 0; i < n; ++i)
        if (!bt.contains(a[i])) return false;
    for (int i = 200; i < 210; ++i)
    {
        bt.insert(i);
        if (!bt.is_valid()) return false;
        if (!bt.contains(i)) return false;
    }
    if (debug)
    {
        cout << "btree:" << endl;
        cout << bt << endl;
    }

    EXPECT_EQ(bt.size(), 19);
    if (bt.insert(100)) return false;
    if (!bt.is_valid()) return false;
    bt.clear();
    if (!bt.empty()) return false;
    return true;
}

// test constructors and =
bool test_btree2(bool debug = false)
{
    int a[] = {43, 20, 96, 100, 69, 7, 9, 105, 90};
    size_t n = 9;
    DSA::BTree<int> bt(a, n);
    if (debug)
    {
        cout << "btree:" << endl;
        cout << bt << endl;
    }
    if (!bt.is_valid()) return false;
    for (int i = 0; i < n; ++i)
        if (!bt.contains(a[i])) return false;

    DSA::BTree<int> bt_copy(bt);
    if (debug)
    {
        cout << "btree copy:" << endl;
        cout << bt_copy << endl;
    }
    if (!bt_copy.is_valid()) return false;
    for (int i = 0; i < n; ++i)
        if (!bt_copy.contains(a[i])) return false;

    DSA::BTree<int> bt_assign;
    bt_assign = bt_copy;
    if (debug)
    {
        cout << "btree assign:" << endl;
        cout << bt_assign << endl;
    }
    if (!bt_assign.is_valid()) return false;
    for (int i = 0; i < n; ++i)
        if (!bt_assign.contains(a[i])) return false;

    bt.insert(2000);
    if (bt.contains(2000) && !bt_copy.contains(2000) && !bt_assign.contains(2000)) return true;
    return false;
}

bool test_btree3(bool debug = false)
{
    DSA::BTree<int> bt;
    int size = 0;
    for (int i = 0; i < 1500; ++i)
    {
        int num = rand_num();
        if (!bt.contains(num)) size++;
        bt.insert(num);
    }
    if (debug)
    {
        cout << "btree:" << endl;
        cout << bt << endl;
        cout << "count size:" << size << endl;
        cout << "btree size:" << bt.size() << endl;
    }
    EXPECT_EQ(size, bt.size());
    return true;
}

bool test_btree4(bool debug = false)
{
    DSA::BTree<int> bt;
    int size = 0;
    queue<int> q;
    // insert random values
    for (int i = 0; i < 50; ++i)
    {
        int num = rand_num_small();
        if (!bt.contains(num))
        {
            size++;
            q.push(num);
        }
        bt.insert(num);
        EXPECT_TRUE(bt.is_valid());
    }
    if (debug)
    {
        cout << "btree:" << endl;
        cout << bt << endl;
        cout << "count size:" << size << endl;
        cout << "btree size:" << bt.size() << endl;
    }
    // expect size
    EXPECT_EQ(size, bt.size());

    // for loop remove
    DSA::BTree<int> bt_for(bt);
    for (int i = 0; i < 55; ++i)
    {
        bt_for.erase(i);
        if (!bt_for.is_valid())
        {
            cout << "remove" << i << " but tree is not valid anymore" << endl;
            cout << "btree:" << endl;
            cout << bt_for << endl;
            return false;
        }
        EXPECT_TRUE(bt_for.is_valid());
    }

    // while loop remove
    while (!bt.empty() && !q.empty())
    {
        int top = q.front();
        q.pop();
        int* found = bt.find(top);
        if (found == nullptr) return false;
        EXPECT_EQ(bt.erase(top), true);
        EXPECT_TRUE(bt.is_valid());
        if (!bt.is_valid())
        {
            cout << "remove " << top << " but failed" << endl;
            cout << "btree:" << endl;
            cout << bt << endl;
            return false;
        }
    }
    if (debug)
    {
        cout << "should be an empty btree:" << endl;
        cout << bt << endl;
        cout << "btree size:" << bt.size() << endl;
    }
    EXPECT_EQ(bt.empty(), true);
    EXPECT_EQ(q.empty(), true);
    bool size_check = (bt.size() == q.size()) && (bt.size() == 0);
    EXPECT_EQ(size_check, true);
    EXPECT_TRUE(bt_for.empty());
    return true;
}

bool test_btree5(bool debug = false)
{
    DSA::BTree<int> bt;
    int size = 0;
    queue<int> q;
    // insert random values
    for (int i = 0; i < 1500; ++i)
    {
        int num = rand_num();
        if (!bt.contains(num))
        {
            size++;
            q.push(num);
        }
        bt.insert(num);
        EXPECT_TRUE(bt.is_valid());
    }
    if (debug)
    {
        cout << "btree:" << endl;
        cout << bt << endl;
        cout << "count size:" << size << endl;
        cout << "btree size:" << bt.size() << endl;
    }
    // expect size
    EXPECT_EQ(size, bt.size());

    // for loop remove
    DSA::BTree<int> bt_for(bt);
    for (int i = 0; i < 1550; ++i)
    {
        bt_for.erase(i);
        if (!bt_for.is_valid())
        {
            cout << "remove" << i << " but failed" << endl;
            cout << "btree:" << endl;
            cout << bt_for << endl;
            return false;
        }
        EXPECT_TRUE(bt_for.is_valid());
    }

    // while loop remove
    while (!bt.empty() && !q.empty())
    {
        int top = q.front();
        q.pop();
        int* found = bt.find(top);
        if (found == nullptr) return false;
        EXPECT_EQ(bt.erase(top), true);
        EXPECT_TRUE(bt.is_valid());
        if (!bt.is_valid())
        {
            cout << "remove " << top << " but failed" << endl;
            cout << "btree:" << endl;
            cout << bt << endl;
            return false;
        }
    }
    if (debug)
    {
        cout << "should be an empty btree:" << endl;
        cout << bt << endl;
        cout << "btree size:" << bt.size() << endl;
    }
    EXPECT_EQ(bt.empty(), true);
    EXPECT_EQ(q.empty(), true);
    bool size_check = (bt.size() == q.size()) && (bt.size() == 0);
    EXPECT_EQ(size_check, true);
    EXPECT_TRUE(bt_for.empty());
    return true;
}

bool test_basic_remove(bool debug = false)
{

#ifdef _WINDOWS_
    SetConsoleOutputCP(CP_UTF8);
#endif

    DSA::BTree<int> bt;
    vector<int> v = {6, 0, 9, 5, 2, 1, 8, 7, 4, 3};

    int num = 10;

    for (int i = 0; i < num; i++)
    {
        bt.insert(i);
    }
    if (debug) cout << "inserted " << bt.size() << " numbers" << '\n';

    if (debug) cout << "tree now:\n"
                    << bt;

    for (int i = 0; i < num; i++)
    {
        if (debug) cout << "tree is " << (bt.is_valid() ? "valid" : "not valid") << endl;
        if (debug) cout << "removing " << v[i] << "\n";
        bt.erase(v[i]);
        if (debug) cout << bt;
    }

    if (debug)
    {
        cout << "tree now: \n"
             << bt;

        cout << "tree is " << (bt.is_valid() ? "valid" : "not valid") << endl;
    }
    return bt.empty();
}
//------------------------------------------------------------------------------

// Lord help me!
const bool debug = false;

//------------------------------------------------------------------------------
//@TODO: add more test sets to call test functions here:

TEST(TEST_BTREE, TestBtree1)
{
    bool success = test_btree1(debug);
    EXPECT_EQ(success, true);
}

TEST(TEST_BTREE, TestBtree2)
{
    bool success = test_btree2(debug);
    EXPECT_EQ(success, true);
}

TEST(TEST_BTREE, TestBtree3)
{
    bool success = test_btree3(debug);
    EXPECT_EQ(success, true);
}

TEST(TEST_BTREE, TestBtree4)
{
    bool success = test_btree4(debug);
    EXPECT_EQ(success, true);
}

TEST(TEST_BTREE, TestBtree5)
{
    bool success = test_btree5(debug);
    EXPECT_EQ(success, true);
}
TEST(TEST_BTREE, TestBtreeRemove)
{
    bool success = test_basic_remove(debug);
    EXPECT_EQ(success, true);
}
//------------------------------------------------------------------------------

int main(int argc, char** argv)
{
    std::srand(std::time(nullptr));
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif
    ::testing::InitGoogleTest(&argc, argv);
    std::cout << "\n\n----------running testB.cpp---------\n\n"
              << std::endl;
    return RUN_ALL_TESTS();
}

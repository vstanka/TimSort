#include "TimSort.h"
#include <cstring>
#include <vector>
#include <iostream>
#include <ctime>
#include <algorithm>

#define TEST(x) RunTest(#x, [](){return (x);})
#define START_TIMER _counter=clock()
#define END_TIMER _counter=clock()-_counter

clock_t _counter, _std_counter;


template<class RAI>
bool IsSorted(RAI begin1, RAI end1, RAI begin2, RAI end2)
{
    typedef typename std::iterator_traits<RAI>::value_type VAL;
    std::vector<VAL> a(begin1, end1);
    std::vector<VAL> b(begin2, end2);
    _std_counter = clock();
    std::sort(b.begin(), b.end());
    _std_counter = clock() - _std_counter;
    return a == b;
}

template<class RAI, class Compare>
bool IsSorted(RAI begin1, RAI end1, RAI begin2, RAI end2, Compare comp)
{
    typedef typename std::iterator_traits<RAI>::value_type VAL;
    std::vector<VAL> a(begin1, end1);
    std::vector<VAL> b(begin2, end2);
    _std_counter = clock();
    std::sort(b.begin(), b.end(), comp);
    _std_counter = clock() - _std_counter;
    return a == b;
}


bool TestCArray(size_t sz)
{
    int *arr = new int[sz];
    int *brr = new int[sz];
    for (size_t i = 0; i < sz; ++i)
        arr[i] = brr[i] = rand();
    START_TIMER;
    timSort(arr, arr + sz);
    END_TIMER;
    bool res = IsSorted(arr, arr + sz, brr, brr + sz);
    delete[] arr;
    delete[] brr;
    return res;
}

bool TestRandom(size_t sz)
{
    std::vector<int> arr(sz);
    for (size_t i = 0; i < sz; ++i)
        arr[i] = rand();
    std::vector<int> brr = arr;
    START_TIMER;
    timSort(arr.begin(), arr.end());
    END_TIMER;
    return IsSorted(arr.begin(), arr.end(), brr.begin(), brr.end());
}

bool TestRandomMod(size_t sz, int mod)
{
    std::vector<int> arr(sz);
    for (size_t i = 0; i < sz; ++i)
        arr[i] = rand() % mod;
    std::vector<int> brr = arr;
    START_TIMER;
    timSort(arr.begin(), arr.end());
    END_TIMER;
    return IsSorted(arr.begin(), arr.end(), brr.begin(), brr.end());
}

bool TestAlmostSorted(size_t sz)
{
    std::vector<int> arr(sz);
    for (size_t i = 0; i < sz; ++i)
        arr[i] = rand();
    std::vector<int> brr = arr;
    std::sort(arr.begin(), arr.end());
    for (size_t i = 0; i < arr.size() - 1; i++)
    {
        if (rand() % 2)
        {
            std::swap(arr[i], arr[i + 1]);
        }
    }
    START_TIMER;
    timSort(arr.begin(), arr.end());
    END_TIMER;
    return IsSorted(arr.begin(), arr.end(), brr.begin(), brr.end());
}

bool TestReverse(size_t sz)
{
    std::vector<int> arr(sz);
    for (size_t i = 0; i < sz; ++i)
        arr[i] = rand();
    std::vector<int> brr = arr;
    std::sort(arr.begin(), arr.end(), std::greater<int>());
    START_TIMER;
    timSort(arr.begin(), arr.end());
    END_TIMER;
    return IsSorted(arr.begin(), arr.end(), brr.begin(), brr.end());
}

bool TestPartiallySorted(size_t part_cnt, size_t part_sz)
{
    std::vector<int> arr(part_cnt * part_sz);
    for (size_t i = 0; i < part_cnt * part_sz; ++i)
        arr[i] = rand();
    std::vector<int> brr = arr;
    for (size_t i = 0; i < part_cnt; i++)
        std::sort(arr.begin() + i * part_sz, arr.begin() + (i + 1) * part_sz);
    START_TIMER;
    timSort(arr.begin(), arr.end());
    END_TIMER;
    return IsSorted(arr.begin(), arr.end(), brr.begin(), brr.end());
}

bool TestInplaceMerge()
{
    // This test fails if we do not consider the last element of the block when sorting them in InplaceMerge
    std::vector<int> arr = {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                            1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1,
                            1, 1};
    std::vector<int> brr = arr;
    START_TIMER;
    timSort(arr.begin(), arr.end());
    END_TIMER;
    return IsSorted(arr.begin(), arr.end(), brr.begin(), brr.end());
}

bool TestSmallRun()
{
    std::vector<int> arr(200);
    for(int i=0;i<100;++i)
    {
        arr[i] = i;
    }
    arr[100] = -1;
    std::vector<int> brr = arr;
    START_TIMER;
    timSort(arr.begin(), arr.begin() + 101);
    END_TIMER;
    return IsSorted(arr.begin(), arr.begin() + 101, brr.begin(), brr.begin() + 101);
}

struct Point3D
{
    int x, y, z;

    bool operator==(const Point3D &a) const
    {
        return x == a.x && y == a.y && z == a.z;
    }

    static bool cmp(const Point3D &a, const Point3D &b)
    {
        if (a.x != b.x)
            return a.x < b.x;
        if (a.y != b.y)
            return a.y < b.y;
        return a.z < b.z;
    }
};

bool TestPoints(size_t sz, int mod)
{

    std::vector<Point3D> arr(sz);
    for (Point3D &i : arr)
    {
        i.x = rand() % mod;
        i.y = rand() % mod;
        i.z = rand() % mod;
    }
    std::vector<Point3D> brr = arr;
    START_TIMER;
    timSort(arr.begin(), arr.end(), Point3D::cmp);
    END_TIMER;
    return IsSorted(arr.begin(), arr.end(), brr.begin(), brr.end(), Point3D::cmp);
}

bool TestStrings(size_t str_cnt, size_t str_sz)
{
    std::vector<std::string> arr(str_cnt);
    for (std::string &i : arr)
    {
        i.reserve(str_sz);
        for (size_t j = 0; j < str_sz; j++)
            i += rand() % (128 - 32) + 32;
    }
    std::vector<std::string> brr = arr;
    START_TIMER;
    timSort(arr.begin(), arr.end());
    END_TIMER;
    return IsSorted(arr.begin(), arr.end(), brr.begin(), brr.end());
}

std::string Align(const std::string &x, size_t sz)
{
    return x + std::string(std::max(0, int(sz) - int(x.size())), ' ');
}

const size_t MAX_TEST_NAME = 35;

void RunTest(const std::string &name, bool a())
{
    _counter = -1000;
    std::cout << Align(name + ":", MAX_TEST_NAME);
    std::cout.flush();
    bool res = a();
    if(res)
    {
        std::cout << _counter /1000 << " ms (std: " << _std_counter / 1000 << " ms)\n";
    }
    else
    {
        std::cout << "FAILED!\n";
        exit(1);
    }
}


int main()
{
    //begin:
    srand(time(0));
    TEST(TestInplaceMerge());
    TEST(TestSmallRun());
    TEST(TestCArray(100));
    TEST(TestCArray(10000000));
    TEST(TestRandom(3));
    TEST(TestRandom(42));
    TEST(TestRandom(10000));
    TEST(TestRandom(10000000));
    TEST(TestRandomMod(10000000, 1));
    TEST(TestRandomMod(10000000, 2));
    TEST(TestRandomMod(10000000, 10));
    TEST(TestRandomMod(10000000, 1000));
    TEST(TestAlmostSorted(10000000));
    TEST(TestReverse(10000000));
    TEST(TestPartiallySorted(100000, 100));
    TEST(TestPartiallySorted(1000, 10000));
    TEST(TestPartiallySorted(10, 1000000));
    TEST(TestPartiallySorted(2, 5000000));
    TEST(TestPoints(10000000, 1000));
    TEST(TestPoints(10000000, 10));
    TEST(TestStrings(10, 1000000));
    TEST(TestStrings(100000, 100));
    TEST(TestStrings(1000000, 10));
    //goto begin;
    return 0;
}


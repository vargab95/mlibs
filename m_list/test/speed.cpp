#include <gtest/gtest.h>

#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <list>
#include <string>

extern "C"
{
#include "m_list.h"
}

using namespace std;
using namespace std::chrono;

const int test_count = 10000;

TEST(m_list_speed_tests, faster_than_std_list)
{
    system_clock::time_point start, stop, sub_start, sub_stop;
    microseconds duration, mlib_duration, std_duration;
    int a;
    start = high_resolution_clock::now();
    {
        list<int> std_list;

        sub_start = high_resolution_clock::now();
        for (int i = 0; i < test_count; i++)
        {
            std_list.push_back(rand());
        }
        sub_stop = high_resolution_clock::now();
        duration = duration_cast<microseconds>(sub_stop - sub_start);

        sub_start = high_resolution_clock::now();
        for (int i = 0; i < test_count; i++)
        {
            std::next(std_list.begin(), i);
        }
        sub_stop = high_resolution_clock::now();
        duration = duration_cast<microseconds>(sub_stop - sub_start);
    }
    stop = high_resolution_clock::now();
    std_duration = duration = duration_cast<microseconds>(stop - start);

    start = high_resolution_clock::now();
    {
        const size_t pagesize = getpagesize();
        m_alloc_instance_t *allocator = m_alloc_create((m_alloc_config_t){
            .type = M_ALLOC_TYPE_ARENA,
            .u = {
                .arena = {
                    .minimum_size_per_arena = pagesize * 4
                }
            }
        }).allocator;
        m_list_t *mlib_list = m_list_create(allocator);
        m_com_sized_data_t value;
        m_com_sized_data_t *result;
        int i;

        value.data = &a;
        value.size = sizeof(a);

        sub_start = high_resolution_clock::now();
        for (i = 0; i < test_count; i++)
        {
            a = rand();
            m_list_append_to_beginning_set(mlib_list, &value);
        }
        sub_stop = high_resolution_clock::now();
        duration = duration_cast<microseconds>(sub_stop - sub_start);

        sub_start = high_resolution_clock::now();
        for (int i = 0; i < test_count; i++)
        {
            result = m_list_get_by_id(mlib_list, i);
        }
        sub_stop = high_resolution_clock::now();
        duration = duration_cast<microseconds>(sub_stop - sub_start);

        m_list_destroy(&mlib_list);
        m_alloc_destroy(&allocator);
    }
    stop = high_resolution_clock::now();
    mlib_duration = duration = duration_cast<microseconds>(stop - start);

    cout << "STD: " << std_duration.count() << " MLIB: " << mlib_duration.count()
         << " (" << std_duration * 100.0 / mlib_duration << " %)" << endl;
    EXPECT_GT(std_duration * 100.0 / mlib_duration, 100);
}

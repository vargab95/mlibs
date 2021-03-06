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

const int test_count = 100000;

int main(void)
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
        cout << "std write ms: " << duration.count() << endl;

        sub_start = high_resolution_clock::now();
        for (int i = 0; i < test_count; i++)
        {
            std::next(std_list.begin(), i);
        }
        sub_stop = high_resolution_clock::now();
        duration = duration_cast<microseconds>(sub_stop - sub_start);
        cout << "std read ms: " << duration.count() << endl;
    }
    stop = high_resolution_clock::now();
    std_duration = duration = duration_cast<microseconds>(stop - start);
    cout << "std whole ms: " << duration.count() << endl;

    start = high_resolution_clock::now();
    {
        m_list_t *mlib_list = m_list_create();
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
        cout << "mlib write ms: " << duration.count() << endl;

        sub_start = high_resolution_clock::now();
        for (int i = 0; i < test_count; i++)
        {
            result = m_list_get_by_id(mlib_list, i);
        }
        sub_stop = high_resolution_clock::now();
        duration = duration_cast<microseconds>(sub_stop - sub_start);
        cout << "mlib read ms: " << duration.count() << endl;

        m_list_destroy(mlib_list);
    }
    stop = high_resolution_clock::now();
    mlib_duration = duration = duration_cast<microseconds>(stop - start);
    cout << "mlib whole ms: " << duration.count() << endl;
    printf("whole ratio %3.2f %%\n", std_duration * 100.0 / mlib_duration);
}

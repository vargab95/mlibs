#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <map>
#include <string>

extern "C"
{
#include "m_map.h"
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
        map<int, int> std_map;

        sub_start = high_resolution_clock::now();
        for (int i = 0; i < test_count; i++)
        {
            std_map[i] = rand();
        }
        sub_stop = high_resolution_clock::now();
        duration = duration_cast<microseconds>(sub_stop - sub_start);
        cout << "std write ms: " << duration.count() << endl;

        sub_start = high_resolution_clock::now();
        for (int i = 0; i < test_count; i++)
        {
            a = std_map[i];
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
        m_map_t *mlib_map = m_map_create(4096);
        m_com_sized_data_t key, value;
        m_com_sized_data_t *result;
        int i;

        key.data = &i;
        key.size = sizeof(i);
        value.data = &a;
        value.size = sizeof(a);

        sub_start = high_resolution_clock::now();
        for (i = 0; i < test_count; i++)
        {
            a = rand();
            m_map_store(mlib_map, &key, &value);
        }
        sub_stop = high_resolution_clock::now();
        duration = duration_cast<microseconds>(sub_stop - sub_start);
        cout << "mlib write ms: " << duration.count() << endl;

        sub_start = high_resolution_clock::now();
        for (int i = 0; i < test_count; i++)
        {
            // result = m_map_get(mlib_map, &key);
            result = m_map_read(mlib_map, &key, &value);
        }
        sub_stop = high_resolution_clock::now();
        duration = duration_cast<microseconds>(sub_stop - sub_start);
        cout << "mlib read ms: " << duration.count() << endl;

        m_map_destroy(mlib_map);
    }
    stop = high_resolution_clock::now();
    mlib_duration = duration = duration_cast<microseconds>(stop - start);
    cout << "mlib whole ms: " << duration.count() << endl;
    printf("whole ratio %3.2f %%\n", std_duration * 100.0 / mlib_duration);
}

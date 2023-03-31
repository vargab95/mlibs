#include "gtest/gtest.h"

extern "C"
{
#include "../api/m_map.h"
}

TEST(m_map_iterator_tests, iterate_on_one_sized_table)
{
    m_map_t *map = m_map_create(1);
    m_map_iterator_t *iterator = m_map_iterator_create(map);
    m_com_sized_data_t data;
    m_com_sized_data_t *result;

    int a[100];
    for (int i = 0; i < 100; i++)
    {
        a[i] = i;
        data.data = &a[i];
        data.size = sizeof(i);
        m_map_set(map, &data, &data);
    }

    m_map_iterator_reset(iterator);
    for (int i = 0; i < 100; i++)
    {
        result = m_map_iterator_value(iterator);
        EXPECT_NE(result, (m_com_sized_data_t*)NULL);

        m_map_iterator_next(iterator);
    }

    m_map_iterator_destroy(&iterator);
    m_map_destroy(&map);
}

TEST(m_map_iterator_tests, iterate_on_ten_sized_table)
{
    m_map_t *map = m_map_create(10);
    m_map_iterator_t *iterator = m_map_iterator_create(map);
    m_com_sized_data_t data;
    m_com_sized_data_t *result;
    bool is_success = true;

    int a[100];
    for (int i = 0; i < 100; i++)
    {
        a[i] = i;
        data.data = &a[i];
        data.size = sizeof(i);
        m_map_set(map, &data, &data);
    }

    m_map_iterator_reset(iterator);
    for (int i = 0; i < 100; i++)
    {
        result = m_map_iterator_value(iterator);
        EXPECT_NE(result, (m_com_sized_data_t*)NULL);

        m_map_iterator_next(iterator);
    }

    m_map_iterator_destroy(&iterator);
    m_map_destroy(&map);
}

TEST(m_map_iterator_tests, iterate_on_empty_table)
{
    m_map_t *map = m_map_create(10);
    m_map_iterator_t *iterator = m_map_iterator_create(map);
    m_com_sized_data_t *result;

    result = m_map_iterator_value(iterator);
    EXPECT_EQ(result, (m_com_sized_data_t*)NULL);

    m_map_iterator_next(iterator);
    result = m_map_iterator_value(iterator);
    EXPECT_EQ(result, (m_com_sized_data_t*)NULL);

    m_map_iterator_destroy(&iterator);
    m_map_destroy(&map);
}

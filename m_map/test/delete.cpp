#include "gtest/gtest.h"

extern "C"
{
#include "../api/m_map.h"
}

TEST(m_map_delete_tests, delete_copied)
{
    m_map_t *map = m_map_create(10);
    m_com_sized_data_t data;
    m_com_sized_data_t result;
    result.data = malloc(4);
    result.size = 4;

    int a[20];
    for (int i = 0; i < 20; i++)
    {
        a[i] = i;
        data.data = &a[i];
        data.size = sizeof(i);
        m_map_store(map, &data, &data);
    }

    data.data = &a[3];
    m_map_delete(map, &data);

    EXPECT_EQ(m_map_read(map, &data, &result), (m_com_sized_data_t*)NULL);

    m_map_destroy(&map);
}

TEST(m_map_delete_tests, delete_not_copied)
{
    m_map_t *map = m_map_create(10);
    m_com_sized_data_t data;

    int a[20];
    for (int i = 0; i < 20; i++)
    {
        a[i] = i;
        data.data = &a[i];
        data.size = sizeof(i);
        m_map_set(map, &data, &data);
    }

    data.data = &a[3];
    m_map_delete(map, &data);

    EXPECT_EQ(m_map_get(map, &data), (m_com_sized_data_t*)NULL);

    m_map_destroy(&map);
}

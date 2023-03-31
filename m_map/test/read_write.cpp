#include "gtest/gtest.h"

extern "C"
{
#include "../api/m_map.h"
}

TEST(m_map_rw_tests, get_set)
{
    m_map_t *map = m_map_create(100);
    m_com_sized_data_t data;
    m_com_sized_data_t *result;

    int a[1000];
    for (int i = 0; i < 1000; i++)
    {
        a[i] = i;
        data.data = &a[i];
        data.size = sizeof(i);
        m_map_set(map, &data, &data);
    }

    for (int i = 0; i < 1000; i++)
    {
        data.data = &a[i];
        data.size = sizeof(i);
        result = m_map_get(map, &data);

        EXPECT_EQ(*(int *)result->data, i);
        EXPECT_EQ(result->size, sizeof(i));
    }

    m_map_destroy(&map);
}

TEST(m_map_rw_tests, read_store)
{
    m_map_t *map = m_map_create(100);
    m_com_sized_data_t data;
    m_com_sized_data_t result;
    result.data = malloc(4);
    result.size = 4;

    int a[1000];
    for (int i = 0; i < 1000; i++)
    {
        a[i] = i;
        data.data = &a[i];
        data.size = sizeof(i);
        m_map_store(map, &data, &data);
    }

    for (int i = 0; i < 1000; i++)
    {
        data.data = &a[i];
        data.size = sizeof(i);
        m_map_read(map, &data, &result);

        EXPECT_EQ(*(int *)result.data, i);
        EXPECT_EQ(result.size, sizeof(i));
        EXPECT_NE(result.data, data.data);
    }

    m_map_destroy(&map);
}

TEST(m_map_rw_tests, get_store)
{
    m_map_t *map = m_map_create(100);
    m_com_sized_data_t data;
    m_com_sized_data_t *result;

    int a[1000];
    for (int i = 0; i < 1000; i++)
    {
        a[i] = i;
        data.data = &a[i];
        data.size = sizeof(i);
        m_map_store(map, &data, &data);
    }

    for (int i = 0; i < 1000; i++)
    {
        data.data = &a[i];
        data.size = sizeof(i);
        result = m_map_get(map, &data);

        EXPECT_EQ(*(int *)result->data, i);
        EXPECT_EQ(result->size, sizeof(i));
        EXPECT_NE(result->data, data.data);
    }

    m_map_destroy(&map);
}

TEST(m_map_rw_tests, read_set)
{
    m_map_t *map = m_map_create(100);
    m_com_sized_data_t data;
    m_com_sized_data_t result;
    result.data = malloc(4);
    result.size = 4;

    int a[1000];
    for (int i = 0; i < 1000; i++)
    {
        a[i] = i;
        data.data = &a[i];
        data.size = sizeof(i);
        m_map_set(map, &data, &data);
    }

    for (int i = 0; i < 1000; i++)
    {
        data.data = &a[i];
        data.size = sizeof(i);
        m_map_read(map, &data, &result);

        EXPECT_EQ(*(int *)result.data, i);
        EXPECT_EQ(result.size, sizeof(i));
        EXPECT_NE(result.data, data.data);
    }

    m_map_destroy(&map);
}

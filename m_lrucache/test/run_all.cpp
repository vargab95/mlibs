#include "gtest/gtest.h"

extern "C"
{
#include "../api/m_lrucache.h"
}

void destroy_item(m_com_sized_data_t **key) {}

m_lrucache_callbacks_t callbacks = {.destroy_key = destroy_item, .destroy_value = destroy_item};

TEST(m_lrucache_tests, put_and_get)
{
    m_lrucache_t *cache =
        m_lrucache_create(m_alloc_create((m_alloc_config_t){.type = M_ALLOC_TYPE_SYSTEM}).allocator, 10, &callbacks);
    m_com_sized_data_t data;
    int i = 12;

    data.data = &i;
    data.size = sizeof(i);

    m_lrucache_put(cache, &data, &data);

    EXPECT_EQ(*(int *)(m_lrucache_get(cache, &data)->data), 12);
}

TEST(m_lrucache_tests, fill_above_capacity_and_get_new)
{
    m_lrucache_t *cache =
        m_lrucache_create(m_alloc_create((m_alloc_config_t){.type = M_ALLOC_TYPE_SYSTEM}).allocator, 10, &callbacks);
    int buffer[15];
    m_com_sized_data_t data[15];

    for (int i = 0; i < 15; i++)
    {
        buffer[i] = i;
        data[i].data = &buffer[i];
        data[i].size = sizeof(i);

        m_lrucache_put(cache, &data[i], &data[i]);
    }

    EXPECT_EQ(*(int *)(m_lrucache_get(cache, &data[5])->data), 5);
}

TEST(m_lrucache_tests, fill_above_capacity_and_get_old)
{
    m_lrucache_t *cache =
        m_lrucache_create(m_alloc_create((m_alloc_config_t){.type = M_ALLOC_TYPE_SYSTEM}).allocator, 1, &callbacks);
    int buffer[15];
    m_com_sized_data_t data[15];

    for (int i = 0; i < 15; i++)
    {
        buffer[i] = i;
        data[i].data = &buffer[i];
        data[i].size = sizeof(i);

        m_lrucache_put(cache, &data[i], &data[i]);
    }

    EXPECT_EQ(m_lrucache_get(cache, &data[4]), nullptr);
}

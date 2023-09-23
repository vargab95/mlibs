#include "m_alloc.h"
#include "gtest/gtest.h"
#include <unistd.h>

extern "C"
{
#include "../api/m_list.h"
}

TEST(m_list_append_tests, append_to_beginning_set)
{
    const size_t pagesize = getpagesize();
    m_context_id_t context = allocator_functions.create((m_allocator_config_t){
        .arena = {
            .minimum_size_per_arena = pagesize
        }
    });
    m_list_t *list = m_list_create(&allocator_functions, context);
    m_com_sized_data_t data;
    m_com_sized_data_t *result;

    int a[10];
    for (int i = 0; i < 10; i++)
    {
        a[i] = i;
        data.data = &a[i];
        data.size = sizeof(i);
        m_list_append_to_beginning_set(list, &data);
    }

    for (int i = 0; i < 10; i++)
    {
        int current = 10 - 1 - i;
        result = m_list_get_by_id(list, i);

        EXPECT_EQ(*(int *)result->data, current);
        EXPECT_EQ(result->size, 4);
        EXPECT_EQ(result->data, &a[current]);
    }

    m_list_destroy(&list);
    allocator_functions.destroy(context);
}

TEST(m_list_append_tests, append_to_end_set)
{
    const size_t pagesize = getpagesize();
    m_context_id_t context = allocator_functions.create((m_allocator_config_t){
        .arena = {
            .minimum_size_per_arena = pagesize
        }
    });
    m_list_t *list = m_list_create(&allocator_functions, context);
    m_com_sized_data_t data;
    m_com_sized_data_t *result;

    int a[10];
    for (int i = 0; i < 10; i++)
    {
        a[i] = i;
        data.data = &a[i];
        data.size = sizeof(i);
        m_list_append_to_end_set(list, &data);
    }

    for (int i = 0; i < 10; i++)
    {
        int current = i;
        result = m_list_get_by_id(list, i);

        EXPECT_EQ(*(int *)result->data, current);
        EXPECT_EQ(result->size, 4);
        EXPECT_EQ(result->data, &a[current]);
    }

    m_list_destroy(&list);
    allocator_functions.destroy(context);
}

TEST(m_list_append_tests, append_to_beginning_store)
{
    const size_t pagesize = getpagesize();
    m_context_id_t context = allocator_functions.create((m_allocator_config_t){
        .arena = {
            .minimum_size_per_arena = pagesize
        }
    });
    m_list_t *list = m_list_create(&allocator_functions, context);
    m_com_sized_data_t data;
    m_com_sized_data_t *result;

    int a[10];
    for (int i = 0; i < 10; i++)
    {
        a[i] = i;
        data.data = &a[i];
        data.size = sizeof(i);
        m_list_append_to_beginning_store(list, &data);
    }

    for (int i = 0; i < 10; i++)
    {
        int current = 10 - 1 - i;
        result = m_list_get_by_id(list, i);

        EXPECT_EQ(*(int *)result->data, current);
        EXPECT_EQ(result->size, 4);
        EXPECT_NE(result->data, &a[current]);
    }

    m_list_destroy(&list);
    allocator_functions.destroy(context);
}

TEST(m_list_append_tests, append_to_end_store)
{
    const size_t pagesize = getpagesize();
    m_context_id_t context = allocator_functions.create((m_allocator_config_t){
        .arena = {
            .minimum_size_per_arena = pagesize
        }
    });
    m_list_t *list = m_list_create(&allocator_functions, context);
    m_com_sized_data_t data;
    m_com_sized_data_t *result;

    int a[10];
    for (int i = 0; i < 10; i++)
    {
        a[i] = i;
        data.data = &a[i];
        data.size = sizeof(i);
        m_list_append_to_end_store(list, &data);
    }

    for (int i = 0; i < 10; i++)
    {
        int current = i;
        result = m_list_get_by_id(list, i);

        EXPECT_EQ(*(int *)result->data, current);
        EXPECT_EQ(result->size, 4);
        EXPECT_NE(result->data, &a[current]);
    }

    m_list_destroy(&list);
    allocator_functions.destroy(context);
}

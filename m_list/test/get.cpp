#include "gtest/gtest.h"

extern "C"
{
#include "../api/m_list.h"
}

TEST(m_list_get_tests, get_size)
{
    const size_t pagesize = getpagesize();
    m_context_id_t context = allocator_functions.create((m_allocator_config_t){
        .arena = {
            .minimum_size_per_arena = pagesize
        }
    });
    m_list_t *list = m_list_create(&allocator_functions, context);
    m_com_sized_data_t data;
    int i = 12;

    data.data = &i;
    data.size = sizeof(i);

    m_list_append_to_beginning_set(list, &data);
    m_list_append_to_beginning_set(list, &data);

    EXPECT_EQ(m_list_get_size(list), 2);

    allocator_functions.destroy(context);
}

TEST(m_list_get_tests, get_size_of_null)
{
    EXPECT_EQ(m_list_get_size(NULL), 0);
}

TEST(m_list_get_tests, get_by_id_out_of_bounds)
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
    int i;

    i = 12;
    data.data = &i;
    data.size = sizeof(i);

    m_list_append_to_end_store(list, &data);

    i = 13;
    m_list_append_to_end_store(list, &data);

    EXPECT_EQ(m_list_get_by_id(list, 2), (m_com_sized_data_t*)NULL);

    m_list_destroy(&list);
    allocator_functions.destroy(context);
}

TEST(m_list_get_tests, get_first)
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
    int i;

    i = 12;
    data.data = &i;
    data.size = sizeof(i);

    m_list_append_to_end_store(list, &data);

    i = 13;
    m_list_append_to_end_store(list, &data);

    result = m_list_get_first(list);

    EXPECT_EQ(*(int *)result->data, 12);
    EXPECT_EQ(result->size, sizeof(i));

    m_list_destroy(&list);
    allocator_functions.destroy(context);
}

TEST(m_list_get_tests, get_first_null)
{
    EXPECT_EQ(m_list_get_first(NULL), (m_com_sized_data_t*)NULL);
}

TEST(m_list_get_tests, get_first_empty)
{
    const size_t pagesize = getpagesize();
    m_context_id_t context = allocator_functions.create((m_allocator_config_t){
        .arena = {
            .minimum_size_per_arena = pagesize
        }
    });
    m_list_t *list = m_list_create(&allocator_functions, context);

    EXPECT_EQ(m_list_get_first(list), (m_com_sized_data_t*)NULL);

    m_list_destroy(&list);
    allocator_functions.destroy(context);
}

TEST(m_list_get_tests, get_last)
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
    int i;

    i = 12;
    data.data = &i;
    data.size = sizeof(i);

    m_list_append_to_end_store(list, &data);

    i = 13;
    m_list_append_to_end_store(list, &data);

    result = m_list_get_last(list);

    EXPECT_EQ(*(int *)result->data, 13);
    EXPECT_EQ(result->size, sizeof(i));

    m_list_destroy(&list);
    allocator_functions.destroy(context);
}

TEST(m_list_get_tests, get_last_null)
{
    EXPECT_EQ(m_list_get_last(NULL), (m_com_sized_data_t*)NULL);
}

TEST(m_list_get_tests, get_last_empty)
{
    const size_t pagesize = getpagesize();
    m_context_id_t context = allocator_functions.create((m_allocator_config_t){
        .arena = {
            .minimum_size_per_arena = pagesize
        }
    });
    m_list_t *list = m_list_create(&allocator_functions, context);

    EXPECT_EQ(m_list_get_last(list), (m_com_sized_data_t*)NULL);

    m_list_destroy(&list);
    allocator_functions.destroy(context);
}

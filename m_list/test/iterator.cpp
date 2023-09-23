#include <gtest/gtest.h>

extern "C"
{
#include "../api/m_list.h"
}

static m_list_iterator_t* set_up(m_list_t **list, m_context_id_t *context)
{
    const size_t pagesize = getpagesize();
    *context = allocator_functions.create((m_allocator_config_t){
        .arena = {
            .minimum_size_per_arena = pagesize
        }
    });
    *list = m_list_create(&allocator_functions, *context);
    m_com_sized_data_t data;
    m_com_sized_data_t *result;
    m_list_iterator_t *iterator;

    int a[10];
    for (int i = 0; i < 10; i++)
    {
        a[i] = i;
        data.data = &a[i];
        data.size = sizeof(i);
        m_list_append_to_end_store(*list, &data);
    }

    return m_list_iterator_create(*list);
}

static void tear_down(m_list_t **list, m_list_iterator_t **iterator, m_context_id_t *context)
{
    m_list_iterator_destroy(iterator);
    m_list_destroy(list);
    allocator_functions.destroy(*context);
}

TEST(m_list_iterate_tests, destroy_referenced)
{
    const size_t pagesize = getpagesize();
    m_context_id_t context = allocator_functions.create((m_allocator_config_t){
        .arena = {
            .minimum_size_per_arena = pagesize
        }
    });
    m_list_t *list = m_list_create(&allocator_functions, context);
    m_list_iterator_t *iterator = m_list_iterator_create(list);
    m_list_destroy(&list);
    allocator_functions.destroy(context);
}

TEST(m_list_iterate_tests, previous_null_at_head)
{
    m_context_id_t context;
    m_list_t *list;
    m_list_iterator_t *iterator = set_up(&list, &context);

    m_list_iterator_go_to_head(iterator);
    EXPECT_EQ(m_list_iterator_previous(iterator), (m_com_sized_data_t*)NULL);

    tear_down(&list, &iterator, &context);
}

TEST(m_list_iterate_tests, next)
{
    m_context_id_t context;
    m_list_t *list;
    m_list_iterator_t *iterator = set_up(&list, &context);
    m_com_sized_data_t *result;

    m_list_iterator_next(iterator);
    m_list_iterator_next(iterator);
    result = m_list_iterator_next(iterator);

    EXPECT_NE(result, (void*)NULL);
    if (result)
    {
        EXPECT_NE(result->data, (void*)NULL);
        if (result->data) EXPECT_EQ(*(int *)result->data, 3);
    }

    tear_down(&list, &iterator, &context);
}

TEST(m_list_iterate_tests, next_null_at_tail)
{
    m_context_id_t context;
    m_list_t *list;
    m_list_iterator_t *iterator = set_up(&list, &context);

    m_list_iterator_go_to_tail(iterator);
    EXPECT_EQ(m_list_iterator_next(iterator), (m_com_sized_data_t*)NULL);

    tear_down(&list, &iterator, &context);
}

TEST(m_list_iterate_tests, previous)
{
    m_context_id_t context;
    m_list_t *list;
    m_list_iterator_t *iterator = set_up(&list, &context);
    m_com_sized_data_t *result;

    m_list_iterator_go_to_tail(iterator);
    m_list_iterator_previous(iterator);
    m_list_iterator_previous(iterator);
    result = m_list_iterator_previous(iterator);
    EXPECT_EQ(*(int *)result->data, 6);

    tear_down(&list, &iterator, &context);
}

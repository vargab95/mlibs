#include <gtest/gtest.h>

extern "C"
{
#include "../api/m_list.h"
}

static m_list_iterator_t* set_up(m_list_t **list, m_alloc_instance_t **allocator)
{
    const size_t pagesize = getpagesize();
    *allocator = m_alloc_create((m_alloc_config_t){
        .type = M_ALLOC_TYPE_ARENA,
        .u = {
            .arena = {
                .minimum_size_per_arena = pagesize
            }
        }
    }).allocator;
    *list = m_list_create(*allocator);
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

static void tear_down(m_list_t **list, m_list_iterator_t **iterator, m_alloc_instance_t **allocator)
{
    m_list_iterator_destroy(iterator);
    m_list_destroy(list);
    m_alloc_destroy(allocator);
}

TEST(m_list_iterate_tests, destroy_referenced)
{
    const size_t pagesize = getpagesize();
    m_alloc_instance_t *allocator = m_alloc_create((m_alloc_config_t){
        .type = M_ALLOC_TYPE_ARENA,
        .u = {
            .arena = {
                .minimum_size_per_arena = pagesize
            }
        }
    }).allocator;
    m_list_t *list = m_list_create(allocator);
    m_list_iterator_t *iterator = m_list_iterator_create(list);
    m_list_destroy(&list);
    m_alloc_destroy(&allocator);
}

TEST(m_list_iterate_tests, previous_null_at_head)
{
    m_alloc_instance_t *allocator;
    m_list_t *list;
    m_list_iterator_t *iterator = set_up(&list, &allocator);

    m_list_iterator_go_to_head(iterator);
    EXPECT_EQ(m_list_iterator_previous(iterator), (m_com_sized_data_t*)NULL);

    tear_down(&list, &iterator, &allocator);
}

TEST(m_list_iterate_tests, next)
{
    m_alloc_instance_t *allocator;
    m_list_t *list;
    m_list_iterator_t *iterator = set_up(&list, &allocator);
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

    tear_down(&list, &iterator, &allocator);
}

TEST(m_list_iterate_tests, next_null_at_tail)
{
    m_alloc_instance_t *allocator;
    m_list_t *list;
    m_list_iterator_t *iterator = set_up(&list, &allocator);

    m_list_iterator_go_to_tail(iterator);
    EXPECT_EQ(m_list_iterator_next(iterator), (m_com_sized_data_t*)NULL);

    tear_down(&list, &iterator, &allocator);
}

TEST(m_list_iterate_tests, previous)
{
    m_alloc_instance_t *allocator;
    m_list_t *list;
    m_list_iterator_t *iterator = set_up(&list, &allocator);
    m_com_sized_data_t *result;

    m_list_iterator_go_to_tail(iterator);
    m_list_iterator_previous(iterator);
    m_list_iterator_previous(iterator);
    result = m_list_iterator_previous(iterator);
    EXPECT_EQ(*(int *)result->data, 6);

    tear_down(&list, &iterator, &allocator);
}

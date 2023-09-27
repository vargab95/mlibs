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

TEST(m_list_delete_tests, delete_on_null)
{
    m_com_sized_data_t data;
    int i = 3;

    data.data = &i;
    data.size = sizeof(i);

    m_list_delete_by_value(NULL, &data);
}

TEST(m_list_delete_tests, delete_from_empty)
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
    m_com_sized_data_t data;
    int i = 3;

    data.data = &i;
    data.size = sizeof(i);

    m_list_delete_by_value(list, &data);

    m_list_destroy(&list);
    m_alloc_destroy(&allocator);
}

TEST(m_list_delete_tests, delete_one)
{
    m_alloc_instance_t *allocator;
    m_list_t *list;
    m_list_iterator_t *iterator = set_up(&list, &allocator);
    m_com_sized_data_t data;
    m_com_sized_data_t *result;

    int c = 2;
    data.data = &c;
    data.size = sizeof(c);

    m_list_delete_by_value(list, &data);

    m_list_iterator_next(iterator);
    m_list_iterator_next(iterator);
    result = m_list_iterator_next(iterator);
    EXPECT_EQ(*(int *)result->data, 4);

    tear_down(&list, &iterator, &allocator);
}

TEST(m_list_delete_tests, delete_first)
{
    m_alloc_instance_t *allocator;
    m_list_t *list;
    m_list_iterator_t *iterator = set_up(&list, &allocator);
    m_com_sized_data_t data;
    m_com_sized_data_t *result;

    int c = 0;
    data.data = &c;
    data.size = sizeof(c);

    m_list_delete_by_value(list, &data);

    m_list_iterator_go_to_head(iterator);
    result = m_list_iterator_current(iterator);
    EXPECT_EQ(*(int *)result->data, 1);

    tear_down(&list, &iterator, &allocator);
}

TEST(m_list_delete_tests, delete_last)
{
    m_alloc_instance_t *allocator;
    m_list_t *list;
    m_list_iterator_t *iterator = set_up(&list, &allocator);
    m_com_sized_data_t data;
    m_com_sized_data_t *result;

    int c = 9;
    data.data = &c;
    data.size = sizeof(c);

    m_list_delete_by_value(list, &data);

    m_list_iterator_go_to_tail(iterator);
    result = m_list_iterator_current(iterator);
    EXPECT_EQ(*(int *)result->data, 8);

    tear_down(&list, &iterator, &allocator);
}

TEST(m_list_delete_tests, delete_multiple)
{
    m_alloc_instance_t *allocator;
    m_list_t *list;
    m_list_iterator_t *iterator = set_up(&list, &allocator);
    m_com_sized_data_t data;
    m_com_sized_data_t *result;

    result = m_list_get_by_id(list, 2);
    *(int*)result->data = 1;

    int c = 1;
    data.data = &c;
    data.size = sizeof(c);

    m_list_delete_all_by_value(list, &data);

    m_list_iterator_next(iterator);
    m_list_iterator_next(iterator);
    result = m_list_iterator_next(iterator);
    EXPECT_EQ(*(int *)result->data, 5);

    tear_down(&list, &iterator, &allocator);
}

#include <gtest/gtest.h>

extern "C"
{
#include "../api/m_list.h"
}

static m_list_iterator_t* set_up(m_list_t **list)
{
    *list = m_list_create();
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

static void tear_down(m_list_t **list, m_list_iterator_t **iterator)
{
    m_list_iterator_destroy(iterator);
    m_list_destroy(list);
}

TEST(m_list_delete_tests, delete_one)
{
    m_list_t *list;
    m_list_iterator_t *iterator = set_up(&list);
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

    tear_down(&list, &iterator);
}

TEST(m_list_delete_tests, delete_multiple)
{
    m_list_t *list;
    m_list_iterator_t *iterator = set_up(&list);
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

    tear_down(&list, &iterator);
}

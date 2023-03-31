#include "gtest/gtest.h"

extern "C"
{
#include "../api/m_list.h"
}

TEST(m_list_append_tests, append_to_beginning_set)
{
    m_list_t *list = m_list_create();
    m_com_sized_data_t data;
    m_com_sized_data_t *result;
    bool is_success = true;

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
}

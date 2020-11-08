#include <stdio.h>

#include "m_list.h"

int test_append_set_beginning();
int test_append_set_end();
int test_append_store_beginning();
int test_append_store_end();
int test_prev_next();
int test_delete_one();
int test_delete_multiple();

int main(void)
{
    test_append_set_beginning();
    test_append_set_end();
    test_append_store_beginning();
    test_append_store_end();
    test_prev_next();
    test_delete_one();
    test_delete_multiple();
}

int test_append_set_beginning()
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
        if (*(int *)result->data != current || result->size != 4 || result->data != &a[current])
        {
            printf("%d != %d\n", current, *(int*)result->data);
            is_success = false;
        }
    }

    m_list_destroy(list);

    printf("%s\t%d\n", __FUNCTION__, is_success);
    return is_success;
}

int test_append_set_end()
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
        m_list_append_to_end_set(list, &data);
    }

    for (int i = 0; i < 10; i++)
    {
        result = m_list_get_by_id(list, i);
        if (*(int *)result->data != i || result->size != 4 || result->data != &a[i])
        {
            printf("%d != %d\n", i, *(int*)result->data);
            is_success = false;
        }
    }

    m_list_destroy(list);

    printf("%s\t%d\n", __FUNCTION__, is_success);
    return is_success;
}

int test_append_store_beginning()
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
        m_list_append_to_beginning_store(list, &data);
    }

    for (int i = 0; i < 10; i++)
    {
        int current = 10 - 1 - i;
        result = m_list_get_by_id(list, i);
        if (*(int *)result->data != current || result->size != 4 || result->data == &a[current])
        {
            printf("%d != %d\n", current, *(int*)result->data);
            is_success = false;
        }
    }

    m_list_destroy(list);

    printf("%s\t%d\n", __FUNCTION__, is_success);
    return is_success;
}

int test_append_store_end()
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
        m_list_append_to_end_store(list, &data);
    }

    for (int i = 0; i < 10; i++)
    {
        result = m_list_get_by_id(list, i);
        if (*(int *)result->data != i || result->size != 4 || result->data == &a[i])
        {
            printf("%d != %d\n", i, *(int*)result->data);
            is_success = false;
        }
    }

    m_list_destroy(list);

    printf("%s\t%d\n", __FUNCTION__, is_success);
    return is_success;
}

int test_prev_next()
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
        m_list_append_to_end_store(list, &data);
    }

    m_list_set_curr_to_head(list);
    if (NULL != m_list_prev(list))
    {
        puts("Head prev is not NULL");
        is_success = false;
    }

    m_list_next(list);
    m_list_next(list);
    result = m_list_next(list);
    if (3 != *(int*)result->data)
    {
        printf("3 != %d\n", *(int*)result->data);
        is_success = false;
    }

    m_list_set_curr_to_tail(list);
    if (NULL != m_list_next(list))
    {
        puts("Tail next is not NULL");
        is_success = false;
    }

    m_list_prev(list);
    m_list_prev(list);
    result = m_list_prev(list);
    if (6 != *(int*)result->data)
    {
        printf("6 != %d\n", *(int*)result->data);
        is_success = false;
    }

    m_list_destroy(list);

    printf("%s\t%d\n", __FUNCTION__, is_success);
    return is_success;
}

int test_delete_one()
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
        m_list_append_to_end_store(list, &data);
    }

    int c = 2;
    data.data = &c;
    data.size = sizeof(c);

    m_list_delete_by_value(list, &data);
    m_list_set_curr_to_head(list);
    m_list_next(list);
    m_list_next(list);
    result = m_list_next(list);
    if (4 != *(int*)result->data)
    {
        printf("4 != %d\n", *(int*)result->data);
        is_success = false;
    }

    m_list_destroy(list);

    printf("%s\t%d\n", __FUNCTION__, is_success);
    return is_success;
}

int test_delete_multiple()
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
        m_list_append_to_end_store(list, &data);
        m_list_append_to_end_store(list, &data);
    }

    int c = 1;
    data.data = &c;
    data.size = sizeof(c);

    m_list_delete_all_by_value(list, &data);
    m_list_set_curr_to_head(list);
    m_list_next(list);
    m_list_next(list);
    result = m_list_next(list);
    if (2 != *(int*)result->data)
    {
        printf("2 != %d\n", *(int*)result->data);
        is_success = false;
    }

    m_list_destroy(list);

    printf("%s\t%d\n", __FUNCTION__, is_success);
    return is_success;
}

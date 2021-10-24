#include <stdio.h>

#include "m_map.h"

int test_get_set();
int test_read_store();
int test_delete_copied();
int test_delete_not_copied();
int test_get_store();
int test_read_set();
int test_iterator_one_sized();
int test_iterator_ten_sized();
int test_iterator_empty();

int main(void)
{
    test_get_set();
    test_read_store();
    test_delete_copied();
    test_delete_not_copied();
    test_get_store();
    test_read_set();
    test_iterator_one_sized();
    test_iterator_ten_sized();
    test_iterator_empty();
}

int test_get_set()
{
    m_map_t *map = m_map_create(100);
    m_com_sized_data_t data;
    m_com_sized_data_t *result;
    bool is_success = true;

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
        if (*(int *)result->data != i || result->size != 4)
        {
            is_success = false;
        }
    }

    m_map_destroy(map);

    printf("%s\t%d\n", __FUNCTION__, is_success);
    return is_success;
}

int test_read_store()
{
    m_map_t *map = m_map_create(100);
    m_com_sized_data_t data;
    m_com_sized_data_t result;
    result.data = malloc(4);
    result.size = 4;
    bool is_success = true;

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
        if (*(int *)result.data != i || result.size != 4)
        {
            is_success = false;
        }
        if (result.data == data.data)
        {
            is_success = false;
        }
    }

    m_map_destroy(map);

    printf("%s\t%d\n", __FUNCTION__, is_success);
    return is_success;
}

int test_delete_copied()
{
    m_map_t *map = m_map_create(10);
    m_com_sized_data_t data;
    m_com_sized_data_t result;
    result.data = malloc(4);
    result.size = 4;
    bool is_success = true;

    int a[20];
    for (int i = 0; i < 20; i++)
    {
        a[i] = i;
        data.data = &a[i];
        data.size = sizeof(i);
        m_map_store(map, &data, &data);
    }

    data.data = &a[3];
    m_map_delete(map, &data);

    if (m_map_read(map, &data, &result) != NULL)
    {
        is_success = false;
    }

    m_map_destroy(map);

    printf("%s\t%d\n", __FUNCTION__, is_success);
    return is_success;
}

int test_delete_not_copied()
{
    m_map_t *map = m_map_create(10);
    m_com_sized_data_t data;
    m_com_sized_data_t *result;
    bool is_success = true;

    int a[20];
    for (int i = 0; i < 20; i++)
    {
        a[i] = i;
        data.data = &a[i];
        data.size = sizeof(i);
        m_map_set(map, &data, &data);
    }

    data.data = &a[3];
    m_map_delete(map, &data);

    if (m_map_get(map, &data) != NULL)
    {
        is_success = false;
    }

    m_map_destroy(map);

    printf("%s\t%d\n", __FUNCTION__, is_success);
    return is_success;
}

int test_get_store()
{
    m_map_t *map = m_map_create(100);
    m_com_sized_data_t data;
    m_com_sized_data_t *result;
    bool is_success = true;

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
        if (*(int *)result->data != i || result->size != 4)
        {
            is_success = false;
        }
        if (result->data == data.data)
        {
            is_success = false;
        }
    }

    m_map_destroy(map);

    printf("%s\t%d\n", __FUNCTION__, is_success);
    return is_success;
}

int test_read_set()
{
    m_map_t *map = m_map_create(100);
    m_com_sized_data_t data;
    m_com_sized_data_t result;
    result.data = malloc(4);
    result.size = 4;
    bool is_success = true;

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
        if (*(int *)result.data != i || result.size != 4)
        {
            is_success = false;
        }
        if (result.data == data.data)
        {
            is_success = false;
        }
    }

    m_map_destroy(map);

    printf("%s\t%d\n", __FUNCTION__, is_success);
    return is_success;
}

int test_iterator_one_sized()
{
    m_map_t *map = m_map_create(1);
    m_map_iterator_t *iterator = m_map_iterator_create(map);
    m_com_sized_data_t data;
    m_com_sized_data_t *result;
    bool is_success = true;

    int a[100];
    for (int i = 0; i < 100; i++)
    {
        a[i] = i;
        data.data = &a[i];
        data.size = sizeof(i);
        m_map_set(map, &data, &data);
    }

    m_map_iterator_reset(iterator);
    for (int i = 0; i < 100; i++)
    {
        result = m_map_iterator_value(iterator);
        if (result == NULL)
        {
            is_success = false;
        }
        m_map_iterator_next(iterator);
    }

    m_map_iterator_destroy(&iterator);
    m_map_destroy(map);

    printf("%s\t%d\n", __FUNCTION__, is_success);
    return is_success;
}

int test_iterator_ten_sized()
{
    m_map_t *map = m_map_create(10);
    m_map_iterator_t *iterator = m_map_iterator_create(map);
    m_com_sized_data_t data;
    m_com_sized_data_t *result;
    bool is_success = true;

    int a[100];
    for (int i = 0; i < 100; i++)
    {
        a[i] = i;
        data.data = &a[i];
        data.size = sizeof(i);
        m_map_set(map, &data, &data);
    }

    m_map_iterator_reset(iterator);
    for (int i = 0; i < 100; i++)
    {
        result = m_map_iterator_value(iterator);
        if (result == NULL)
        {
            is_success = false;
        }
#if 0
        m_com_sized_data_t *key, *value;
        key = m_map_iterator_key(iterator);
        value = m_map_iterator_value(iterator);

        printf("Key: %d, value: %d\n", *(int *)key->data, *(int *)value->data);
#endif
        m_map_iterator_next(iterator);
    }

    m_map_iterator_destroy(&iterator);
    m_map_destroy(map);

    printf("%s\t%d\n", __FUNCTION__, is_success);
    return is_success;
}

int test_iterator_empty()
{
    m_map_t *map = m_map_create(10);
    m_map_iterator_t *iterator = m_map_iterator_create(map);
    m_com_sized_data_t data;
    m_com_sized_data_t *result;
    bool is_success = true;

    result = m_map_iterator_value(iterator);
    if (result != NULL)
    {
        puts("Iterator value should be empty");
        is_success = false;
    }

    m_map_iterator_next(iterator);
    result = m_map_iterator_value(iterator);
    if (result != NULL)
    {
        puts("Iterator value should be empty after next");
        is_success = false;
    }

    m_map_iterator_destroy(&iterator);
    m_map_destroy(map);

    printf("%s\t%d\n", __FUNCTION__, is_success);
    return is_success;
}

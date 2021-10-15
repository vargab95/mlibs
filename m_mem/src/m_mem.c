#include "m_mem.h"

#include <string.h>

void *m_mem_malloc(size_t size)
{
    void *data = malloc(size);
    if (NULL == data)
    {
        abort();
    }
    return data;
}

void *m_mem_calloc(uint32_t number, size_t size)
{
    void *data = calloc(number, size);
    if (NULL == data)
    {
        abort();
    }
    return data;
}

m_com_sized_data_t *m_mem_sized_malloc(size_t size)
{
    m_com_sized_data_t *data = (m_com_sized_data_t *)m_mem_malloc(sizeof(m_com_sized_data_t));
    data->data = m_mem_malloc(size);
    data->size = size;
    return data;
}

m_com_sized_data_t *m_mem_sized_calloc(uint32_t number, size_t size)
{
    m_com_sized_data_t *data = (m_com_sized_data_t *)m_mem_calloc(number, sizeof(m_com_sized_data_t));

    for (uint32_t i = 0; i < number; i++)
    {
        data[i].data = m_mem_malloc(size);
        data[i].size = size;
    }
    return data;
}

void m_mem_free(void **address)
{
    if (NULL == *address)
    {
        return;
    }
    free(*address);
    *address = NULL;
}

void m_mem_sized_free(m_com_sized_data_t *data)
{
    free(data->data);
    free(data);
}

bool m_mem_cmp(const m_com_sized_data_t *const ptr1, const m_com_sized_data_t *const ptr2)
{
    if (ptr1->size != ptr2->size)
    {
        return false;
    }

    if (0 == memcmp(ptr1->data, ptr2->data, ptr1->size))
    {
        return true;
    }

    return false;
}

void m_mem_copy(const m_com_sized_data_t *const source, m_com_sized_data_t *const destination)
{
    if (source->size < destination->size)
    {
        return;
    }

    memcpy(destination->data, source->data, source->size);
}

void m_mem_dump(const m_com_sized_data_t *const data, FILE *fp)
{
    for (int i = 0; i < data->size; i++)
    {
        fprintf(fp, "%x", ((uint8_t *)data->data)[i]);
    }
}

void m_mem_text_dump(const m_com_sized_data_t *const data, FILE *fp)
{
    for (int i = 0; i < data->size; i++)
    {
        fprintf(fp, "%c", ((uint8_t *)data->data)[i]);
    }
}

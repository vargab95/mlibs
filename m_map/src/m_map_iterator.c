#include "m_map.h"
#include <m_libs/m_mem.h>

m_map_iterator_t *m_map_iterator_create(m_map_t *map)
{
    m_map_iterator_t *iterator = (m_map_iterator_t *)m_mem_malloc(sizeof(m_map_iterator_t));

    iterator->map = map;
    map->reference_count++;
    m_map_iterator_reset(iterator);

    return iterator;
}

void m_map_iterator_destroy(m_map_iterator_t **iterator)
{
    (*iterator)->map->reference_count--;
    free(*iterator);
}

void m_map_iterator_next(m_map_iterator_t *iterator)
{
    if (iterator->element == NULL)
    {
        return;
    }

    iterator->element = iterator->element->next;
    if (iterator->element != NULL)
    {
        return;
    }

    if (iterator->hash_table_idx >= (iterator->map->size - 1))
    {
        return;
    }

    iterator->hash_table_idx++;
    iterator->element = iterator->map->table[iterator->hash_table_idx].next;
}

m_com_sized_data_t *m_map_iterator_key(const m_map_iterator_t *const iterator)
{
    return (iterator->element) ? &iterator->element->key : NULL;
}

m_com_sized_data_t *m_map_iterator_value(const m_map_iterator_t *const iterator)
{
    return (iterator->element) ? &iterator->element->data : NULL;
}

void m_map_iterator_reset(m_map_iterator_t *iterator)
{
    iterator->element = iterator->map->table[0].next;
    iterator->hash_table_idx = 0;
}

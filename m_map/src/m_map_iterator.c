#include "m_map.h"
#include "types.h"

#if !defined(COMPOSITE_BUILD)
#include <m_libs/m_mem.h>
#else
#include "../../m_mem/api/m_mem.h"
#endif

static void find_and_set_next_table_root(m_map_iterator_t *iterator);

m_map_iterator_t *m_map_iterator_create(m_map_t *map)
{
    m_map_iterator_t *iterator = (m_map_iterator_t *)map->allocator->malloc(map->context, sizeof(m_map_iterator_t));

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

    iterator->hash_table_idx++;
    find_and_set_next_table_root(iterator);
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
    iterator->hash_table_idx = 0;
    find_and_set_next_table_root(iterator);
}

static void find_and_set_next_table_root(m_map_iterator_t *iterator)
{
    iterator->element = NULL;
    while ((iterator->hash_table_idx < iterator->map->size) &&
           (iterator->map->table[iterator->hash_table_idx].next == NULL))
    {
        iterator->hash_table_idx++;
    }

    if (iterator->hash_table_idx < iterator->map->size)
    {
        iterator->element = iterator->map->table[iterator->hash_table_idx].next;
    }
}

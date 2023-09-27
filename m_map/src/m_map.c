#include <stdio.h>
#include <stdlib.h>

#include "config.h"
#include "m_map.h"
#include "types.h"

#if !defined(COMPOSITE_BUILD)
#include <m_libs/m_hash.h>
#include <m_libs/m_mem.h>
#include <m_libs/m_alloc.h>
#else
#include "../../m_hash/api/m_hash.h"
#include "../../m_mem/api/m_mem.h"
#include "../../m_mem/api/m_alloc.h"
#endif

static m_map_element_t *get_root_element(const m_map_t *const map, const m_com_sized_data_t *const key);
static m_map_element_t *find_sub_element(const m_map_element_t *const root, const m_com_sized_data_t *const key);
static m_map_element_t *create_element(const m_map_t *const map, m_map_element_t *const root, const m_com_sized_data_t *const key);
static m_map_element_t *get_or_create(const m_map_t *const map, const m_com_sized_data_t *const key);

m_map_t *m_map_create(m_alloc_instance_t *allocator, const uint32_t size)
{
    m_map_t *map;

    map = (m_map_t *)m_alloc_malloc(allocator, sizeof(m_map_t)).pointer;
    if (map == NULL)
    {
        return NULL;
    }

    map->size = size;
    map->table = (m_map_element_t *)m_alloc_calloc(allocator, map->size, sizeof(m_map_element_t)).pointer;
    if (map->table == NULL)
    {
        m_alloc_free(allocator, (void*)map);
        return NULL;
    }

    map->reference_count = 0;
    map->allocator = allocator;

    return map;
}

void m_map_destroy(m_map_t **map)
{
    m_map_iterator_t *iterator = m_map_iterator_create(*map);
    m_com_sized_data_t *tmp;

    while ((tmp = m_map_iterator_key(iterator)))
    {
        m_map_iterator_next(iterator);
        m_map_delete(*map, tmp);
    }

    m_map_iterator_destroy(&iterator);

    if ((*map)->reference_count)
    {
        puts("ERROR: Map deleted with non-zero reference count");
        printf("Map: %p, reference counter: %d\n", *map, (*map)->reference_count);
    }

    m_alloc_free((*map)->allocator, (*map)->table);
    m_alloc_free((*map)->allocator, *map);
    *map = NULL;
}

m_com_sized_data_t *m_map_get(const m_map_t *const map, const m_com_sized_data_t *const key)
{
    m_map_element_t *element = find_sub_element(get_root_element(map, key), key);
    return (element && element->is_data_node) ? (&element->data) : NULL;
}

void m_map_set(const m_map_t *const map, const m_com_sized_data_t *const key, const m_com_sized_data_t *const value)
{
    m_map_element_t *element = get_or_create(map, key);
    element->copied = SET;

    element->data = *value;
    element->key = *key;
}

m_com_sized_data_t *m_map_read(const m_map_t *const map, const m_com_sized_data_t *const key, m_com_sized_data_t *value)
{
    m_map_element_t *element = find_sub_element(get_root_element(map, key), key);

    if (!element || (value->size < element->data.size))
    {
        return NULL;
    }

    m_mem_copy(&element->data, value);
    return value;
}

boolean m_map_store(const m_map_t *const map, const m_com_sized_data_t *const key, const m_com_sized_data_t *const value)
{
    m_map_element_t *element = get_or_create(map, key);
    element->copied = STORED;

    element->data.size = value->size;
    element->data.data = m_alloc_malloc(map->allocator, value->size).pointer;
    if (element->data.data == NULL)
    {
        return false;
    }

    element->key.size = key->size;
    element->key.data = m_alloc_malloc(map->allocator, key->size).pointer;
    if (element->key.data == NULL)
    {
        m_alloc_free(map->allocator, element->data.data);
        return false;
    }

    m_mem_copy(key, &element->key);
    m_mem_copy(value, &element->data);

    return true;
}

void m_map_delete(const m_map_t *const map, const m_com_sized_data_t *const key)
{
    m_map_element_t *root = get_root_element(map, key);
    m_map_element_t *element = root->next;
    m_map_element_t *previous = root;

    while (element)
    {
        if (m_mem_cmp(&element->key, key))
        {
            if (STORED == element->copied)
            {
                m_alloc_free(map->allocator, element->data.data);
                m_alloc_free(map->allocator, element->key.data);
            }
            previous->next = element->next;
            m_alloc_free(map->allocator, element);
            break;
        }

        previous = element;
        element = element->next;
    }
}

void m_map_print(const m_map_t *const map)
{
    m_map_element_t *element;
    for (int idx = 0; idx < map->size; idx++)
    {
        element = map->table[idx].next;
        printf("[%d]: ", idx);
        while (element)
        {
            if (element->data.data)
            {
                printf("0x%p ", element->data.data);
            }
            element = element->next;
        }
        printf("\n");
    }
}

void m_map_custom_print(const m_map_t *const map,
                        void (*func)(const m_com_sized_data_t* const, const m_com_sized_data_t* const))
{
    m_map_element_t *element;
    for (int idx = 0; idx < map->size; idx++)
    {
        element = map->table[idx].next;
        while (element)
        {
            if (element->data.data)
            {
                func(&element->key, &element->data);
            }
            element = element->next;
        }
    }
}

static m_map_element_t *get_root_element(const m_map_t *const map, const m_com_sized_data_t *const key)
{
    uint32_t hash;

#if M_MAP_HASH_SELECTED == M_MAP_HASH_DBJ
    hash = m_hash_djb(key);
#elif M_MAP_HASH_SELECTED == M_MAP_HASH_FNVA1
    hash = m_hash_fnv1a(key);
#else
#error "Invalid hash type selected"
#endif

    hash %= map->size;
    return &map->table[hash];
}

static m_map_element_t *find_sub_element(const m_map_element_t *const root, const m_com_sized_data_t *const key)
{
    m_map_element_t *element = root->next;

    while (element)
    {
        if (m_mem_cmp(&element->key, key))
        {
            break;
        }

        element = element->next;
    }

    return element;
}

static m_map_element_t *create_element(const m_map_t * const map, m_map_element_t *const root, const m_com_sized_data_t *const key)
{
    m_map_element_t *new_element = (m_map_element_t *)m_alloc_malloc(map->allocator, sizeof(m_map_element_t)).pointer;
    if (new_element == NULL)
    {
        return NULL;
    }

    new_element->copied = NOT_USED;
    new_element->is_data_node = 1;
    new_element->next = NULL;
    new_element->key.data = NULL;
    new_element->key.size = 0;
    new_element->data.data = NULL;
    new_element->data.size = 0;
    new_element->next = root->next;

    root->next = new_element;

    return new_element;
}

static m_map_element_t *get_or_create(const m_map_t *const map, const m_com_sized_data_t *const key)
{
    m_map_element_t *root = get_root_element(map, key);
    m_map_element_t *element = find_sub_element(root, key);

    if (!element)
    {
        element = create_element(map, root, key);
    }

    return element;
}

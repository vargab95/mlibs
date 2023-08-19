#include <stdlib.h>

#include "m_lrucache.h"

#if !defined(COMPOSITE_BUILD)
#include <m_libs/m_mem.h>
#else
#include "../../m_mem/api/m_mem.h"
#endif

#if !defined(COMPOSITE_BUILD)
#include <m_libs/m_hash.h>
#else
#include "../../m_hash/api/m_hash.h"
#endif

/**
 * @brief Type definition for LRU cache item
 */
typedef struct m_lrucache_item_t
{
    const m_com_sized_data_t *key;
    const m_com_sized_data_t *value;

    struct
    {
        struct m_lrucache_item_t *next;
        struct m_lrucache_item_t *prev;
    } slot;

    struct
    {
        struct m_lrucache_item_t *next;
        struct m_lrucache_item_t *prev;
    } usage;
} m_lrucache_item_t;

/**
 * @brief Type definition for LRU cache
 */
typedef struct m_lrucache_t
{
    const m_lrucache_callbacks_t *callbacks;

    size_t length;
    m_lrucache_item_t *head;
    m_lrucache_item_t *tail;

    size_t capacity;
    m_lrucache_item_t *table[];
} m_lrucache_t;

static void put_to_beginning_of_usage_list(m_lrucache_t *cache, m_lrucache_item_t *item);
static void delete_oldest(m_lrucache_t *cache);
static inline uint32_t get_table_index(m_lrucache_t *cache, const m_com_sized_data_t *const key);

m_lrucache_t *m_lrucache_create(size_t capacity, const m_lrucache_callbacks_t * const callbacks)
{
    m_lrucache_t *cache = calloc(1, sizeof(m_lrucache_t) + capacity * sizeof(m_lrucache_item_t));

    cache->capacity = capacity;
    cache->callbacks = callbacks;

    return cache;
}

void m_lrucache_destroy(m_lrucache_t **cache)
{
    m_lrucache_item_t *current, *next;

    for (current = (*cache)->head; current; current = next)
    {
        next = current->usage.next;
        (*cache)->callbacks->destroy_key((m_com_sized_data_t**)&current->key);
        (*cache)->callbacks->destroy_value((m_com_sized_data_t**)&current->value);
    }

    free(*cache);
    *cache = NULL;
}

const m_com_sized_data_t *m_lrucache_get(m_lrucache_t *const cache, const m_com_sized_data_t *const key)
{
    const size_t table_index = get_table_index(cache, key);
    m_lrucache_item_t *slot = cache->table[table_index];
    m_lrucache_item_t *item;

    for (item = slot; item && !m_mem_cmp(item->key, key); item = item->slot.next)
        ;

    if (!item)
    {
        return NULL;
    }

    put_to_beginning_of_usage_list(cache, item);

    return item->value;
}

bool m_lrucache_put(m_lrucache_t *const cache, const m_com_sized_data_t *const key,
                    const m_com_sized_data_t *const value)
{
    const size_t table_index = get_table_index(cache, key);
    m_lrucache_item_t *slot = cache->table[table_index];
    m_lrucache_item_t *item;

    if (!slot)
    {
        slot = calloc(1, sizeof(m_lrucache_item_t));
        if (!slot)
        {
            return false;
        }

        slot->key = key;
        slot->value = value;

        item = cache->table[table_index] = slot;

        cache->length++;
    }
    else
    {
        m_lrucache_item_t *prev = NULL;
        for (item = slot; item && !m_mem_cmp(item->key, key); prev = item, item = item->slot.next)
            ;

        if (item)
        {
            item->value = value;
        }
        else
        {
            item = calloc(1, sizeof(m_lrucache_item_t));
            if (!item)
            {
                abort();
            }

            item->key = key;
            item->value = value;
            item->slot.prev = prev;
            prev->slot.next = item;

            cache->length++;
        }
    }

    put_to_beginning_of_usage_list(cache, item);

    if (cache->length > cache->capacity)
    {
        delete_oldest(cache);
    }

    return true;
}

static void delete_oldest(m_lrucache_t *const cache)
{
    cache->length--;

    if (!cache->tail)
    {
        abort();
    }

    m_lrucache_item_t *item = cache->tail;
    item->usage.prev->usage.next = NULL;
    cache->tail = item->usage.prev;

    if (item->slot.prev)
    {
        item->slot.prev->slot.next = item->slot.next;
    }

    if (item->slot.next)
    {
        item->slot.next->slot.prev = item->slot.prev;
    }

    const size_t table_index = get_table_index(cache, item->key);
    if (item->slot.prev == NULL)
    {
        cache->table[table_index] = item->slot.next;
    }

    cache->callbacks->destroy_key((m_com_sized_data_t**)&item->key);
    cache->callbacks->destroy_value((m_com_sized_data_t**)&item->value);

    if (cache->tail == NULL)
    {
        cache->head = NULL;
    }
}

static inline uint32_t get_table_index(m_lrucache_t *cache, const m_com_sized_data_t *const key)
{
    return m_hash_djb2(key) % cache->capacity;
}

static void put_to_beginning_of_usage_list(m_lrucache_t *cache, m_lrucache_item_t *item)
{
    if (item == cache->head)
    {
        return;
    }

    if (cache->head == NULL)
    {
        cache->head = cache->tail = item;
        return;
    }

    // delete item from it's original place in usage list
    if (item->usage.next)
    {
        item->usage.next->usage.prev = item->usage.prev;
    }

    if (item->usage.prev)
    {
        item->usage.prev->usage.next = item->usage.next;
    }

    // update tail if needed
    if (item == cache->tail && item != cache->head)
    {
        cache->tail = item->usage.prev;
    }

    // insert the item at the head of the list
    cache->head->usage.prev = item;
    item->usage.next = cache->head;

    cache->head = item;
    cache->head->usage.prev = NULL;
}

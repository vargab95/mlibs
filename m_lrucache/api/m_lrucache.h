#ifndef __M_LRUCACHE_H__
#define __M_LRUCACHE_H__

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#if !defined(COMPOSITE_BUILD)
#include <m_libs/m_common.h>
#else
#include "../../m_common/api/m_common.h"
#endif

/** @file m_lrucache.h
 * m_lrucache public definitions.
 */

/** @brief Type definition of LRU cache */
typedef struct m_lrucache_t m_lrucache_t;

/** @brief Type definition of LRU cache callbacks */
typedef struct m_lrucache_callbacks_t 
{
    void (*destroy_key)(m_com_sized_data_t **);
    void (*destroy_value)(m_com_sized_data_t **);
} m_lrucache_callbacks_t;

/**
 * Creates a LRU cache
 *
 * @param[in] size_t Capacity of the LRU cache
 * @return m_lrucache_t* New LRU cache instance
 */
m_lrucache_t* m_lrucache_create(size_t capacity, const m_lrucache_callbacks_t * const callbacks);

/**
 * Destroyes the LRU cache
 *
 * @param[in] m_lrucache_t** The cache
 */
void m_lrucache_destroy(m_lrucache_t **cache);

/**
 * Gets an item from the LRU cache
 *
 * @param[in] m_lrucache_t* The cache
 * @return m_com_sized_data_t* The stored value or NULL
 */
const m_com_sized_data_t* m_lrucache_get(m_lrucache_t * const cache, const m_com_sized_data_t * const key);

/**
 * Puts an item to the LRU cache
 *
 * @param[in] m_lrucache_t* The cache
 * @param[in] m_com_sized_data_t* Key to store
 * @param[in] m_com_sized_data_t* Value to store
 */
bool m_lrucache_put(m_lrucache_t * const cache,
                    const m_com_sized_data_t * const key,
                    const m_com_sized_data_t * const value);

#endif

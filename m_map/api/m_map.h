#ifndef __M_MAP_H__
#define __M_MAP_H__

#include <stdbool.h>
#include <stdint.h>

#if !defined(COMPOSITE_BUILD)
#include <m_libs/m_common.h>
#else
#include "../../m_common/api/m_common.h"
#endif

/** @file m_map.h
 * m_map public definitions.
 */

/**
 * @brief Type definition for map
 */
typedef struct m_map_element_t m_map_element_t;
typedef struct m_map_t m_map_t;
typedef struct m_map_iterator_t m_map_iterator_t;

m_map_iterator_t *m_map_iterator_create(m_map_t *map);
void m_map_iterator_destroy(m_map_iterator_t **iterator);
void m_map_iterator_next(m_map_iterator_t *iterator);
m_com_sized_data_t *m_map_iterator_key(const m_map_iterator_t *const iterator);
m_com_sized_data_t *m_map_iterator_value(const m_map_iterator_t *const iterator);
void m_map_iterator_reset(m_map_iterator_t *iterator);

/**
 * @brief Create a new map with a specified table size.
 *
 * @param[in] size
 * @return m_map_t*
 */
m_map_t *m_map_create(const uint32_t size);

/**
 * @brief
 *
 * @param[in] map
 */
void m_map_destroy(m_map_t **map);

/**
 * @brief
 *
 * @param[in] map
 * @param[in] key
 * @return m_com_sized_data_t*
 */
m_com_sized_data_t *m_map_get(const m_map_t *const map, const m_com_sized_data_t *const key);

/**
 * @brief
 *
 * @param map[in] Pointer to the map.
 * @param key[in] Key of the stored data.
 * @param value[in] Value of the stored data. If NULL then the object is deleted.
 * @param copy Set to true to copy into a temporary buffer.
 */
void m_map_set(const m_map_t *const map, const m_com_sized_data_t *const key, const m_com_sized_data_t *const value);

/**
 * @brief Reads the data from the temporary buffer to the given buffer
 *
 * !!! WARNING !!! The allocated memory shall be at least as big as the stored information's.
 *
 * @param[in] map
 * @param[in] key
 * @param[in] value
 * @return m_com_sized_data_t*
 */
m_com_sized_data_t *m_map_read(const m_map_t *const map, const m_com_sized_data_t *const key,
                               m_com_sized_data_t *value);

/**
 * @brief Creates a temporary buffer and copies the value into it.
 *
 * @param[in] map
 * @param[in] key
 * @param[in] value
 */
void m_map_store(const m_map_t *const map, const m_com_sized_data_t *const key, const m_com_sized_data_t *const value);

/**
 * @brief Deletes an object.
 *
 * Can be used both for set and stored objects.
 *
 * @param[in] map
 * @param[in] key
 */
void m_map_delete(const m_map_t *const map, const m_com_sized_data_t *const key);

/**
 * @brief Pretty print for m_map
 *
 * @param[in] map
 */
void m_map_print(const m_map_t *const map);

#endif

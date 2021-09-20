#ifndef __M_MAP_H__
#define __M_MAP_H__

#include <stdint.h>
#include <stdbool.h>

#include <m_libs/m_common.h>

/**
 * @brief Type definition for map elements
 */
typedef struct m_map_element_t
{
    uint8_t copied : 2;
    uint8_t is_data_node : 1; /**< Signs if the node is not root */
    uint8_t reserved : 5;     /**< reserved for further flags */
    m_com_sized_data_t key;
    m_com_sized_data_t data;
    struct m_map_element_t *next;
    struct m_map_element_t **prev_ptr;
} m_map_element_t;

/**
 * @brief Type definition for map data structure
 */
typedef struct
{
    uint32_t size;
    m_map_element_t *table;
} m_map_t;

/**
 * @brief New map creation
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
void m_map_destroy(m_map_t *map);

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
 * !!! AWARE !!! The allocated memory shall be as big as the stored information's
 *
 * @param[in] map
 * @param[in] key
 * @param[in] value
 * @return m_com_sized_data_t*
 */
m_com_sized_data_t *m_map_read(const m_map_t *const map, const m_com_sized_data_t *const key, m_com_sized_data_t *value);

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

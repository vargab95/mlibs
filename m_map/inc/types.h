#ifndef __TYPES_H__
#define __TYPES_H__

#include <stdint.h>
#include "m_map.h"

/**
 * @brief Type definition for map elements
 */
struct m_map_element_t
{
    uint8_t copied : 2;
    uint8_t is_data_node : 1; /**< Signs if the node is not root */
    uint8_t reserved : 5;     /**< reserved for further flags */
    m_com_sized_data_t key;
    m_com_sized_data_t data;
    struct m_map_element_t *next;
    struct m_map_element_t **prev_ptr;
};

/**
 * @brief Type definition for map data structure
 */
struct m_map_t
{
    uint32_t size;
    uint32_t reference_count;
    m_alloc_instance_t *allocator;

    m_map_element_t *table;
};

struct m_map_iterator_t
{
    m_map_t *map;
    uint32_t hash_table_idx;
    m_map_element_t *element;
};

/** @brief Enumeration to store whether the data of an element was copied or stored */
typedef enum
{
    NOT_USED = 0,
    STORED,
    SET
} copy_state_t;

#endif

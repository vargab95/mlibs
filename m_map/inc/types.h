#ifndef __TYPES_H__
#define __TYPES_H__

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
    m_map_element_t *table;
    uint32_t reference_count;
};

struct m_map_iterator_t
{
    m_map_t *map;
    uint32_t hash_table_idx;
    m_map_element_t *element;
};

#endif

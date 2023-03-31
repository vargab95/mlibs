#ifndef __TYPES_H__
#define __TYPES_H__

#if !defined(COMPOSITE_BUILD)
#include <m_libs/m_common.h>
#else
#include "../../m_common/api/m_common.h"
#endif

/**
 * @brief Type definition for list elements
 */
struct m_list_node_t
{
    uint8_t copied : 2;
    uint8_t reserved : 5; /**< reserved for further flags */
    m_com_sized_data_t data;
    struct m_list_node_t *next;
    struct m_list_node_t *prev;
};

/**
 * @brief Type definition for list data structure
 */
struct m_list_t
{
    size_t size;
    uint32_t reference_counter;
    m_list_node_t *head;
    m_list_node_t *tail;
};


/**
 * @brief Type definition for list iterator
 */
struct m_list_iterator_t
{
    m_list_t *list_reference;
    m_list_node_t *curr;
};

/** @brief Enumeration to store whether the data of an element was copied or stored */
typedef enum
{
    NOT_USED = 0,
    STORED,
    SET
} copy_state_t;

#endif

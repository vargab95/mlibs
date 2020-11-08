#ifndef __M_LIST_H__
#define __M_LIST_H__

#include <stdint.h>
#include <stdbool.h>

#include "m_common.h"

/**
 * @brief Type definition for list elements
 */
typedef struct m_list_node_t
{
    uint8_t copied : 2;
    uint8_t reserved : 5;     /**< reserved for further flags */
    m_com_sized_data_t data;
    struct m_list_node_t *next;
    struct m_list_node_t *prev;
} m_list_node_t;

/**
 * @brief Type definition for map data structure
 */
typedef struct
{
    uint32_t size;
    m_list_node_t *head;
    m_list_node_t *tail;
    m_list_node_t *curr;
} m_list_t;

/**
 * @brief New list creation
 *
 * @param[in] size
 * @return m_list_t*
 */
m_list_t *m_list_create();

/**
 * @brief
 *
 * @param[in] list
 */
void m_list_destroy(m_list_t *list);

/**
 * @brief
 *
 * @param[in] list
 * @param[in] id
 */
m_com_sized_data_t* m_list_get_by_id(const m_list_t *const list, uint32_t id);

/**
 * @brief
 *
 * @param[in] list
 */
m_com_sized_data_t* m_list_set_curr_to_head(m_list_t *list);

/**
 * @brief
 *
 * @param[in] list
 */
m_com_sized_data_t* m_list_set_curr_to_tail(m_list_t *list);

/**
 * @brief
 *
 * @param[in] list
 */
m_com_sized_data_t* m_list_next(m_list_t *list);

/**
 * @brief
 *
 * @param[in] list
 */
m_com_sized_data_t* m_list_prev(m_list_t *list);

/**
 * @brief
 *
 * @param[in] list
 * @param[in] value
 */
void m_list_append_to_end_set(m_list_t *list, const m_com_sized_data_t *const value);

/**
 * @brief
 *
 * @param[in] list
 * @param[in] value
 */
void m_list_append_to_beginning_set(m_list_t *list, const m_com_sized_data_t *const value);

/**
 * @brief
 *
 * @param[in] list
 * @param[in] value
 */
void m_list_append_to_end_store(m_list_t *list, const m_com_sized_data_t *const value);

/**
 * @brief
 *
 * @param[in] list
 * @param[in] value
 */
void m_list_append_to_beginning_store(m_list_t *list, const m_com_sized_data_t *const value);

/**
 * @brief Deletes an object by value.
 *
 * Can be used both for set and stored objects.
 *
 * @param[in] list
 * @param[in] value
 */
void m_list_delete_by_value(m_list_t *list, const m_com_sized_data_t *const value);

/**
 * @brief Deletes all objects with value.
 *
 * Can be used both for set and stored objects.
 *
 * @param[in] list
 * @param[in] value
 */
void m_list_delete_all_by_value(m_list_t *list, const m_com_sized_data_t *const value);

/**
 * @brief Pretty print for m_list
 *
 * @param[in] list
 */
void m_list_print(const m_list_t *const list);

#endif

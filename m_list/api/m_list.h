#ifndef __M_LIST_H__
#define __M_LIST_H__

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#if !defined(COMPOSITE_BUILD)
#include <m_libs/m_common.h>
#else
#include "../../m_common/api/m_common.h"
#endif

/** @file m_list.h
 * m_list public definitions.
 */

/**
 * @brief List related type definitions
 */
typedef struct m_list_node_t m_list_node_t;
typedef struct m_list_t m_list_t;
typedef struct m_list_iterator_t m_list_iterator_t;

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
void m_list_destroy(m_list_t **list);

/**
 * @brief
 *
 * @param[in] list
 * @param[in] id
 */
m_com_sized_data_t *m_list_get_by_id(const m_list_t *const list, uint32_t id);

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

void m_list_dump_binary(const m_list_t *const list, FILE *fp);
m_list_t *m_list_load_binary(FILE *fp);

m_list_iterator_t *m_list_iterator_create(m_list_t *list);
void m_list_iterator_destroy(m_list_iterator_t **iterator);
void m_list_iterator_go_to_head(m_list_iterator_t *iterator);
void m_list_iterator_go_to_tail(m_list_iterator_t *iterator);
m_com_sized_data_t *m_list_iterator_current(const m_list_iterator_t *const iterator);
m_com_sized_data_t *m_list_iterator_next(m_list_iterator_t *iterator);
m_com_sized_data_t *m_list_iterator_previous(m_list_iterator_t *iterator);

#endif

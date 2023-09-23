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


#if !defined(COMPOSITE_BUILD)
#include <m_libs/m_alloc.h>
#else
#include "../../m_mem/api/m_alloc.h"
#endif

/** @file m_list.h
 * m_list public definitions.
 */

/** @brief Type definition of a linked list */
typedef struct m_list_t m_list_t;

/** @brief Type definition of a list node */
typedef struct m_list_node_t m_list_node_t;

/** @brief Type definition of a list iterator */
typedef struct m_list_iterator_t m_list_iterator_t;

/**
 * @brief Create a new list.
 *
 * @param[in] allocator
 * @param[in] context
 * @return m_list_t*
 */
m_list_t *m_list_create(m_allocator_functions_t *allocator, m_context_id_t context);

/**
 * @brief Destroys a list.
 *
 * @param[inout] list The list to destroy.
 */
void m_list_destroy(m_list_t **list);

/**
 * @brief Gets the size of a list.
 *
 * @param[in] list The list to use.
 * @return Size of the list.
 */
size_t m_list_get_size(const m_list_t *const list);

/**
 * @brief Returns the data of the first element.
 *
 * @param[in] list The list to use.
 * @return First data element.
 */
m_com_sized_data_t* m_list_get_first(const m_list_t *const list);

/**
 * @brief Returns the data of the last element.
 *
 * @param[in] list The list to use.
 * @return Last data element.
 */
m_com_sized_data_t* m_list_get_last(const m_list_t *const list);

/**
 * @brief Returns the data of the nth node in the list.
 *
 * @param[in] list The list.
 * @param[in] id Id to be read.
 */
m_com_sized_data_t *m_list_get_by_id(const m_list_t *const list, uint32_t id);

/**
 * @brief Adds the element with a shallow copy to the end of the list.
 *
 * @param[in] list List.
 * @param[in] value Value to be appended.
 */
boolean m_list_append_to_end_set(m_list_t *list, const m_com_sized_data_t *const value);

/**
 * @brief Adds the element with a shallow copy to the beginning of the list.
 *
 * @param[in] list List.
 * @param[in] value Value to be appended.
 */
boolean m_list_append_to_beginning_set(m_list_t *list, const m_com_sized_data_t *const value);

/**
 * @brief Adds the element with a deep copy to the end of the list.
 *
 * @param[in] list List.
 * @param[in] value Value to be appended.
 */
boolean m_list_append_to_end_store(m_list_t *list, const m_com_sized_data_t *const value);

/**
 * @brief Adds the element with a deep copy to the beginning of the list.
 *
 * @param[in] list List.
 * @param[in] value Value to be appended.
 */
boolean m_list_append_to_beginning_store(m_list_t *list, const m_com_sized_data_t *const value);

/**
 * @brief Deletes an object by value.
 *
 * Can be used both for set and stored objects.
 *
 * @param[in] list List.
 * @param[in] value Value to be deleted.
 */
void m_list_delete_by_value(m_list_t *list, const m_com_sized_data_t *const value);

/**
 * @brief Deletes all objects with value.
 *
 * Can be used both for set and stored objects.
 *
 * @param[in] list List.
 * @param[in] value Value to be deleted.
 */
void m_list_delete_all_by_value(m_list_t *list, const m_com_sized_data_t *const value);

/**
 * @brief Pretty print for m_list.
 *
 * @param[in] list List to be printed.
 */
void m_list_print(const m_list_t *const list);

/**
 * @brief Pretty print for m_list using a custom print method.
 *
 * @param[in] list List to be printed.
 * @param[in] func The function to be called.
 */
void m_list_custom_print(const m_list_t *const list, void (*func)(const m_com_sized_data_t* const));

/**
 * @brief Dumps the stored elements to a binary file.
 *
 * @param[in] list List to be dumped.
 * @param[in] fp File pointer to be used.
 */
void m_list_dump_binary(const m_list_t *const list, FILE *fp);

/**
 * @brief Dumps the stored elements to a binary file using a custom method.
 *
 * @param[in] list List to be dumped.
 * @param[in] fp File pointer to be used.
 * @param[in] func The function to be called.
 */
void m_list_custom_dump_binary(const m_list_t *const list,
                               FILE *fp,
                               void (*func)(const m_com_sized_data_t* const, FILE *fp));

/**
 * @brief Loads a list from a binary file.
 *
 * @param[in] fp File pointer to be used.
 * @return The loaded list.
 */
m_list_t *m_list_load_binary(FILE *fp);

/**
 * @brief Create an iterator.
 *
 * @param[in] list The list to be used.
 * @return The new iterator.
 */
m_list_iterator_t *m_list_iterator_create(m_list_t *list);

/**
 * @brief Destroy an iterator.
 *
 * @param[inout] iterator The iterator to be destroyed.
 *                        Double pointer is used to be able to reset the pointer's value
 *                        to NULL to avoid use after free.
 */
void m_list_iterator_destroy(m_list_iterator_t **iterator);

/**
 * @brief Reset the iterator to the beginning of a list.
 *
 * @param[inout] iterator The iterator to be used.
 */
void m_list_iterator_go_to_head(m_list_iterator_t *iterator);

/**
 * @brief Reset the iterator to the end of a list.
 *
 * @param[inout] iterator The iterator to be used.
 */
void m_list_iterator_go_to_tail(m_list_iterator_t *iterator);

/**
 * @brief Reads the data stored in the current node of the list.
 *
 * @param[in] iterator The iterator to be used.
 * @return Data stored in the current list node.
 */
m_com_sized_data_t *m_list_iterator_current(const m_list_iterator_t *const iterator);

/**
 * @brief Steps to the next node of the list and returns the it's data.
 *
 * @param[inout] iterator The iterator to be used.
 * @return Data stored in the next list node.
 */
m_com_sized_data_t *m_list_iterator_next(m_list_iterator_t *iterator);

/**
 * @brief Steps to the previous node of the list and returns the it's data.
 *
 * @param[inout] iterator The iterator to be used.
 * @return Data stored in the previous list node.
 */
m_com_sized_data_t *m_list_iterator_previous(m_list_iterator_t *iterator);

#endif

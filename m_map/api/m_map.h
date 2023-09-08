#ifndef __M_MAP_H__
#define __M_MAP_H__

#include <stdbool.h>
#include <stdint.h>

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

/** @file m_map.h
 * m_map public definitions.
 */

/** @brief Map element type definition */
typedef struct m_map_element_t m_map_element_t;

/** @brief Map type definition */
typedef struct m_map_t m_map_t;

/** @brief Map iterator type definition */
typedef struct m_map_iterator_t m_map_iterator_t;

/**
 * @brief Create a new map with a specified hash table size.
 *
 * @param[in] allocator
 * @param[in] context
 * @param[in] size
 * @return m_map_t*
 */
m_map_t *m_map_create(m_allocator_t *allocator, m_context_id_t context, const uint32_t size);

/**
 * @brief Destroys a map created by m_map_create.
 *
 * @param[inout] map Map to destroy.
 */
void m_map_destroy(m_map_t **map);

/**
 * @brief Gets the data from a map element with the specified key.
 *
 * If the key does not exist NULL is returned.
 * Please note, that getting returns the pointer to the stored data
 * without duplication. The stored data may not be modified using
 * the returned value. Use the m_map_read instead to read a copy, then
 * modify it and store the copy.
 *
 * @param[in] map Map to be used.
 * @param[in] key The key.
 * @return m_com_sized_data_t* Data element stored with the specified key.
 */
m_com_sized_data_t *m_map_get(const m_map_t *const map, const m_com_sized_data_t *const key);

/**
 * @brief Sets the specified value with the specified key.
 *
 * Please note, that m_map does not duplicate the data automatically when
 * the set is used, so the user must free it manually after the map is
 * destroyed. To be able to use the auto free feature, please use
 * m_map_store.
 *
 * @param[in] map Pointer to the map.
 * @param[in] key Key of the stored data.
 * @param[in] value Value of the stored data. If NULL then the object is deleted.
 */
void m_map_set(const m_map_t *const map, const m_com_sized_data_t *const key, const m_com_sized_data_t *const value);

/**
 * @brief Reads the data from the map to the given buffer
 *
 * !!! WARNING !!! The allocated memory shall be at least as big as the stored information's.
 *
 * @param[in] map Map to be used.
 * @param[in] key The key.
 * @param[in] value Destination pointer.
 * @return m_com_sized_data_t* Destination pointer.
 */
m_com_sized_data_t *m_map_read(const m_map_t *const map, const m_com_sized_data_t *const key,
                               m_com_sized_data_t *value);

/**
 * @brief Creates a copy of the value and stores it in the map.
 *
 * @param[in] map Map to be used.
 * @param[in] key The key.
 * @param[in] value The value to be stored.
 */
boolean m_map_store(const m_map_t *const map, const m_com_sized_data_t *const key, const m_com_sized_data_t *const value);

/**
 * @brief Deletes an object.
 *
 * Can be used both for set and stored objects.
 *
 * @param[in] map Map to be used.
 * @param[in] key The key.
 */
void m_map_delete(const m_map_t *const map, const m_com_sized_data_t *const key);

/**
 * @brief Pretty print for m_map
 *
 * @param[in] map Map to be printed.
 */
void m_map_print(const m_map_t *const map);

/**
 * @brief Pretty print for m_map using a custom print method.
 *
 * @param[in] map Map to be printed.
 * @param[in] func The function to be called. The first argument is the key and
 *                 the second argument is the value.
 */
void m_map_custom_print(const m_map_t *const map,
                        void (*func)(const m_com_sized_data_t* const, const m_com_sized_data_t* const));

/**
 * @brief Creates an iterator for the specified map.
 *
 * @param[in] map Map to be used.
 * @return The created iterator.
 */
m_map_iterator_t *m_map_iterator_create(m_map_t *map);

/**
 * @brief Creates an iterator for the specified map.
 *
 * After freeing up, it'll set the pointer to NULL.
 *
 * @param[inout] iterator Iterator to be destroyed.
 */
void m_map_iterator_destroy(m_map_iterator_t **iterator);

/**
 * @brief Steps the iterator to the next map element.
 *
 * @param[inout] iterator Iterator to be used.
 */
void m_map_iterator_next(m_map_iterator_t *iterator);

/**
 * @brief Read the key of the currently pointed map element.
 *
 * @param[in] iterator Iterator to be used.
 * @return The key.
 */
m_com_sized_data_t *m_map_iterator_key(const m_map_iterator_t *const iterator);

/**
 * @brief Read the value of the currently pointed map element.
 *
 * @param[in] iterator Iterator to be used.
 * @return The value.
 */
m_com_sized_data_t *m_map_iterator_value(const m_map_iterator_t *const iterator);

/**
 * @brief Resets the iterator to the first map element.
 *
 * @param[inout] iterator Iterator to be used.
 */
void m_map_iterator_reset(m_map_iterator_t *iterator);

#endif

#ifndef __M_MEM_H__
#define __M_MEM_H__

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#if !defined(COMPOSITE_BUILD)
#include <m_libs/m_common.h>
#else
#include "../../m_common/api/m_common.h"
#endif

/** @file m_mem.h
 * m_mem public definitions.
 */

/**
 * @brief malloc wrapper.
 *
 * If the memory allocation is not successful, it aborts the process.
 *
 * @param[in] size Size of the data to be allocated.
 * @return void* Pointer to the allocated memory.
 */
void *m_mem_malloc(size_t size);

/**
 * @brief calloc wrapper.
 *
 * If the memory allocation is not successful, it aborts the process.
 *
 * @param[in] size Size of the data to be allocated.
 * @param[in] number Number of the data segments to be allocated.
 * @return void* Pointer to the allocated memory.
 */
void *m_mem_calloc(uint32_t number, size_t size);

/**
 * @brief malloc wrapper using the common sized memory structure.
 *
 * @param[in] size Size of the data to be allocated.
 * @return void* Pointer to the allocated memory.
 */
m_com_sized_data_t *m_mem_sized_malloc(size_t size);

/**
 * @brief calloc wrapper using the common sized memory structure.
 *
 * @param[in] size Size of the data to be allocated.
 * @param[in] number Number of the data segments to be allocated.
 * @return void* Pointer to the allocated memory.
 */
m_com_sized_data_t *m_mem_sized_calloc(uint32_t number, size_t size);

/**
 * @brief free wrapper.
 *
 * The pointer to pointer is necessary, because this function sets
 * the pointer to NULL after freeing it to avoid use after free
 * problems.
 *
 * @param[inout] address Address to be freed.
 */
void m_mem_free(void **address);

/**
 * @brief free wrapper using the common sized memory structure.
 *
 * @param[inout] data m_com data to be freed.
 */
void m_mem_sized_free(m_com_sized_data_t *data);

/**
 * @brief memcmp wrapper using the common sized memory structure.
 *
 * @param[in] ptr1 First data to be compared.
 * @param[in] ptr2 Second data to be compared.
 */
bool m_mem_cmp(const m_com_sized_data_t *const ptr1, const m_com_sized_data_t *const ptr2);

/**
 * @brief memcpy wrapper using the common sized memory structure.
 *
 * @param[in] source Source pointer.
 * @param[in] destination Destination pointer.
 */
void m_mem_copy(const m_com_sized_data_t *const source, m_com_sized_data_t *const destination);

/**
 * @brief Dumps data in binary format.
 *
 * @param[in] data Data to be dumped.
 * @param[in] fp File pointer to be used.
 */
void m_mem_dump(const m_com_sized_data_t *const data, FILE *fp);

/**
 * @brief Dumps data in text (ASCII) format.
 *
 * @param[in] data Data to be dumped.
 * @param[in] fp File pointer to be used.
 */
void m_mem_text_dump(const m_com_sized_data_t *const data, FILE *fp);

#endif

#ifndef __M_MEM_H__
#define __M_MEM_H__

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "m_common.h"

/**
 * @brief 
 * 
 * @param size 
 * @return void* 
 */
void *m_mem_malloc(size_t size);

/**
 * @brief 
 * 
 * @param number 
 * @param size 
 * @return void* 
 */
void *m_mem_calloc(uint32_t number, size_t size);

/**
 * @brief 
 * 
 * @param size 
 * @return void* 
 */
m_com_sized_data_t *m_mem_sized_malloc(size_t size);

/**
 * @brief 
 * 
 * @param number 
 * @param size 
 * @return void* 
 */
m_com_sized_data_t *m_mem_sized_calloc(uint32_t number, size_t size);

/**
 * @brief 
 * 
 * @param[in|out] address 
 * @return void
 */
void m_mem_free(void **address);

void m_mem_sized_free(m_com_sized_data_t *data);

/**
 * @brief 
 * 
 */
bool m_mem_cmp(const m_com_sized_data_t *const ptr1, const m_com_sized_data_t *const ptr2);

/**
 * @brief 
 * 
 */
void m_mem_copy(const m_com_sized_data_t *const source, m_com_sized_data_t *const destination);

#endif
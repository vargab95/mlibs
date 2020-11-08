#ifndef __M_HASH_H__
#define __M_HASH_H__

#include <stdint.h>
#include <stdlib.h>

#include "m_common.h"

/**
 * @brief D. J. Bernstein hash function
 * 
 * @param[in] input 
 * @param[in] size
 * @return uint32_t 
 */
extern uint32_t m_hash_djb(const m_com_sized_data_t *input);

/**
 * @brief FNV hash function, variant 1a
 * 
 * @param[in] input 
 * @param[in] size
 * @return uint32_t 
 */
extern uint32_t m_hash_fnv1a(const m_com_sized_data_t *input);

#endif
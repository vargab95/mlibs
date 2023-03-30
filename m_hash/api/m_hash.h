#ifndef __M_HASH_H__
#define __M_HASH_H__

#include <stdint.h>
#include <stdlib.h>

#if !defined(COMPOSITE_BUILD)
#include <m_libs/m_common.h>
#else
#include "../../m_common/api/m_common.h"
#endif

/** @file m_hash.h
 * m_hash public definitions.
 */

/**
 * @brief D. J. Bernstein hash function
 *
 * @param[in] input Input in the commonly used m_common format.
 * @return uint32_t The 32 bit hash.
 */
extern uint32_t m_hash_djb(const m_com_sized_data_t *input);

/**
 * @brief FNV hash function, variant 1a
 *
 * @param[in] input Input in the commonly used m_common format.
 * @return uint32_t The 32 bit hash.
 */
extern uint32_t m_hash_fnv1a(const m_com_sized_data_t *input);

#endif

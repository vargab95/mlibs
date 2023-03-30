#ifndef __M_COMMON_H__
#define __M_COMMON_H__

#include <stdlib.h>

/** @file m_common.h
 * m_common public definitions.
 */

/**
 * @brief Boolean value definitions
 */
#define TRUE 1
#define FALSE 0

/**
 * @brief Boolean type definition
 */
typedef unsigned char boolean;

/**
 * @brief Common structure for sized objects
 */
typedef struct
{
    void *data;
    size_t size;
} m_com_sized_data_t;

#endif

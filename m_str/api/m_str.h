#ifndef __M_STR_H__
#define __M_STR_H__

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#if !defined(COMPOSITE_BUILD)
#include <m_libs/m_common.h>
#else
#include "../../m_common/api/m_common.h"
#endif

/** @file m_str.h
 * m_str public definitions.
 */

/**
 * @brief Structure which returns the results of a string split operation.
 *
 * Must be destroyed by using m_str_split_result_destroy. It's not recommended
 * to manually fee up the resource.
 */
typedef struct
{
    int token_count;
    char **tokens;
    char *string;
} m_str_split_result_t;

/**
 * Converts the C string to the m_com format.
 *
 * @param[in] string
 * @param[out] String in the common format
 */
void m_str_convert_to_com(const char *string, m_com_sized_data_t *output);

/**
 * Splits a string into tokens.
 *
 * @param[in] input The input string
 * @param[in] separator Separator
 * @return structure to describe splitted values
 */
m_str_split_result_t *m_str_split(const m_com_sized_data_t *input, const m_com_sized_data_t *separator);

/**
 * Destroys a result structure returned by split.
 *
 * @param[in] result Reference of the returned pointer
 */
void m_str_split_result_destroy(m_str_split_result_t **result);

/**
 * Joins strings on a specified separator.
 *
 * @param[in] strings Strings to join
 * @param[in] no_strings Number of strings
 * @param[in] separator Separator to use
 * @return Joined string
 */
m_com_sized_data_t *m_str_join(m_com_sized_data_t **strings, unsigned int no_strings, m_com_sized_data_t *separator);

#endif

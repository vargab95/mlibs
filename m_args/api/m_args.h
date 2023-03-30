#ifndef __M_TREE_H__
#define __M_TREE_H__

#include <stdbool.h>
#include <stdint.h>

#if !defined(COMPOSITE_BUILD)
#include <m_libs/m_common.h>
#include <m_libs/m_list.h>
#else
#include "../../m_common/api/m_common.h"
#include "../../m_list/api/m_list.h"
#endif

/** @file m_args.h
 * m_args public definitions.
 */

/**
 * @brief Describes a possible command line argument.
 */
typedef struct
{
    uint32_t id;

    char *description;

    char *short_switch;
    char *long_switch;
    char *environment_variable;

    struct
    {
        uint8_t required : 1;
        uint8_t convert : 1;
        uint8_t present : 1;
        uint8_t no_value : 1;
        uint8_t __reserved : 4;
    } flags;

    enum
    {
        ARG_TYPE_STRING = 0,
        ARG_TYPE_INT = 1,
        ARG_TYPE_FLOAT = 2
    } expected_type;
    union
    {
        char *string_val;
        int int_val;
        float float_val;
    } value;

    enum
    {
        ARG_PREFER_SHORT = 0,
        ARG_PREFER_LONG = 1,
        ARG_PREFER_ENV = 2
    } preference;
} m_args_entry_t;

/**
 * @brief Describes all command line arguments.
 */
typedef struct
{
    char *executable;
    char *description;
    m_list_t *arg_list;
} m_args_t;

/**
 * @brief Allocate and initialize a new argument structure.
 *
 * @param[in] description Description of the tool where m_args is used.
 * @return m_args_t* The created argument structure.
 */
m_args_t *m_args_create(const char *description);

/**
 * @brief Destroys the m_args_t command line arguments structure.
 *
 * @param[in] args Arguments structure.
 */
void m_args_destroy(m_args_t **args);

/**
 * @brief Parses command line arguments.
 *
 * Parses command line arguments specified by the usual argc and argv inputs
 * and fills the m_args_t structure based on that.
 *
 * @param[in] args Arguments structure.
 * @param[in] argc Number of command line parameters.
 * @param[in] argv Command line arguments.
 * @return bool Shows whether the parsing was successful and it matches the definition.
 */
bool m_args_parse(m_args_t *args, int argc, char **argv);

/**
 * @brief Reads an argument entry by id.
 *
 * @param[in] args Arguments structure.
 * @param[in] id Id of the argument definition.
 * @return m_args_entry_t Returns the argument entry.
 */
m_args_entry_t *m_args_get(m_args_t *args, uint32_t id);

/**
 * @brief Adds a new argument entry.
 *
 * @param[in] args Arguments structure.
 * @param[in] entry New entry to be added to the arguments.
 */
void m_args_add_entry(m_args_t *args, m_args_entry_t entry);

/**
 * @brief Prints a help menu based on the specification.
 *
 * @param[in] args Arguments structure.
 */
void m_args_print_help(m_args_t *args);

#endif

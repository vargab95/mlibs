#ifndef __M_TREE_H__
#define __M_TREE_H__

#include <stdint.h>
#include <stdbool.h>

#include <m_libs/m_common.h>
#include <m_libs/m_list.h>

typedef struct {
    uint32_t id;

    char *short_switch;
    char *long_switch;
    char *environment_variable;

    struct {
        uint8_t required : 1;
        uint8_t convert : 1;
        uint8_t present : 1;
        uint8_t no_value : 1;
        uint8_t __reserved : 4;
    } flags;

    enum {
        ARG_TYPE_STRING = 0,
        ARG_TYPE_INT = 1,
        ARG_TYPE_FLOAT = 2
    } expected_type;
    union {
        char *string_val;
        int int_val;
        float float_val;
    } value;

    enum {
        ARG_PREFER_SHORT = 0,
        ARG_PREFER_LONG = 1,
        ARG_PREFER_ENV = 2
    } preference;
} m_args_entry_t;

typedef struct {
    m_list_t *arg_list;
} m_args_t;

m_args_t* m_args_create();
void m_args_destroy(m_args_t **args);

bool m_args_parse(m_args_t *args, int argc, char **argv);
m_args_entry_t* m_args_get(m_args_t *args, uint32_t id);
void m_args_add_entry(m_args_t *args, m_args_entry_t entry);

#endif

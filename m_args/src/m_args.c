#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"
#include "m_libs/m_args.h"
#include <m_libs/m_list.h>
#include <m_libs/m_mem.h>

static bool fill_arg_entry(m_args_entry_t *entry, int argc, char **argv);

m_args_t *m_args_create()
{
    m_args_t *args = (m_args_t *)m_mem_malloc(sizeof(m_args_t));

    args->arg_list = m_list_create();

    return args;
}

void m_args_destroy(m_args_t **args)
{
    m_list_destroy(&(*args)->arg_list);
    free(*args);
}

bool m_args_parse(m_args_t *args, int argc, char **argv)
{
    m_com_sized_data_t *tmp;
    m_list_iterator_t *iterator = m_list_iterator_create(args->arg_list);
    bool successful = true;

    for (m_list_iterator_go_to_head(iterator); (tmp = m_list_iterator_current(iterator)) != NULL;
         m_list_iterator_next(iterator))
    {
        m_args_entry_t *entry = tmp->data;
        if (!fill_arg_entry(entry, argc, argv))
        {
            successful = false;
        }
    }

    m_list_iterator_destroy(&iterator);

    return successful;
}

static bool fill_arg_entry(m_args_entry_t *entry, int argc, char **argv)
{
    for (int i = 0; i < argc; i++)
    {
        bool found = FALSE;
        char *env_ptr;
        bool short_switch_found = entry->short_switch && (strcmp(argv[i], entry->short_switch) == 0);
        bool long_switch_found = entry->long_switch && (strcmp(argv[i], entry->long_switch) == 0);

        if (short_switch_found || long_switch_found)
        {
            if (!entry->flags.no_value)
                entry->value.string_val = argv[i + 1];
            found = TRUE;
        }
        else if (entry->environment_variable && ((env_ptr = getenv(entry->environment_variable)) != NULL))
        {
            if (!entry->flags.no_value)
                entry->value.string_val = env_ptr;
            found = TRUE;
        }

        if (found)
        {
            switch (entry->expected_type)
            {
            case ARG_TYPE_INT:
                entry->value.int_val = atoi(entry->value.string_val);
                break;
            case ARG_TYPE_FLOAT:
                entry->value.float_val = atof(entry->value.string_val);
                break;
            case ARG_TYPE_STRING:
                break;
            }

            entry->flags.present = 1;
            break;
        }
    }

    return true;
}

m_args_entry_t *m_args_get(m_args_t *args, uint32_t id)
{
    m_com_sized_data_t *tmp;
    m_args_entry_t *entry = NULL;
    m_list_iterator_t *iterator = m_list_iterator_create(args->arg_list);

    for (m_list_iterator_go_to_head(iterator); (tmp = m_list_iterator_current(iterator)) != NULL;
         m_list_iterator_next(iterator))
    {
        entry = tmp->data;

        if (entry->id == id)
        {
            break;
        }

        entry = NULL;
    }

    m_list_iterator_destroy(&iterator);

    return entry;
}

void m_args_add_entry(m_args_t *args, m_args_entry_t entry)
{
    m_com_sized_data_t tmp;

    tmp.size = sizeof(m_args_entry_t);
    tmp.data = &entry;

    m_list_append_to_end_store(args->arg_list, &tmp);
}

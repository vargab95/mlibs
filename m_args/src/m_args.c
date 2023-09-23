#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "config.h"
#include "m_args.h"

#if !defined(COMPOSITE_BUILD)
#include <m_libs/m_mem.h>
#else
#include "../../m_mem/api/m_mem.h"
#endif

static bool fill_arg_entry(m_args_entry_t *entry, int argc, char **argv);
static bool process_found_entry(m_args_entry_t *entry);
static bool process_arg(m_args_entry_t *entry, int i, char **argv);
static bool process_short_arg(m_args_entry_t *entry, int i, char **argv);
static bool process_long_arg(m_args_entry_t *entry, int i, char **argv);
static bool process_env_arg(m_args_entry_t *entry, int i, char **argv);

m_args_t *m_args_create(const char *description)
{
    int pagesize = getpagesize();
    m_context_id_t context = allocator_functions.create((m_allocator_config_t){
        .arena = {
            .minimum_size_per_arena = pagesize
        }
    });
    m_args_t *args = (m_args_t *)allocator_functions.malloc(context, sizeof(m_args_t));

    args->arg_list = m_list_create(&allocator_functions, context);
    args->description = (char *)description;

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
        if (process_arg(entry, i, argv))
        {
            if (!process_found_entry(entry))
            {
                return FALSE;
            }
            break;
        }
    }

    return TRUE;
}

static bool process_arg(m_args_entry_t *entry, int i, char **argv)
{
    switch (entry->preference)
    {
    case ARG_PREFER_SHORT:
        return process_short_arg(entry, i, argv) ||
               process_long_arg(entry, i, argv) ||
               process_env_arg(entry, i, argv);
    case ARG_PREFER_LONG:
        return process_long_arg(entry, i, argv) ||
               process_short_arg(entry, i, argv) ||
               process_env_arg(entry, i, argv);
    case ARG_PREFER_ENV:
        return process_env_arg(entry, i, argv) ||
               process_short_arg(entry, i, argv) ||
               process_long_arg(entry, i, argv);
    }

    return FALSE;
}

static bool process_short_arg(m_args_entry_t *entry, int i, char **argv)
{
    if (entry->short_switch && (strcmp(argv[i], entry->short_switch) == 0))
    {
        if (!entry->flags.no_value)
            entry->value.string_val = argv[i + 1];
        return TRUE;
    }

    return FALSE;
}

static bool process_long_arg(m_args_entry_t *entry, int i, char **argv)
{
    if(entry->long_switch && (strcmp(argv[i], entry->long_switch) == 0))
    {
        if (!entry->flags.no_value)
            entry->value.string_val = argv[i + 1];
        return TRUE;
    }

    return FALSE;
}

static bool process_env_arg(m_args_entry_t *entry, int i, char **argv)
{
    char *env_ptr;

    if (entry->environment_variable && ((env_ptr = getenv(entry->environment_variable)) != NULL))
    {
        if (!entry->flags.no_value)
            entry->value.string_val = env_ptr;
        return TRUE;
    }

    return FALSE;
}

static bool process_found_entry(m_args_entry_t *entry)
{
    switch (entry->expected_type)
    {
    case ARG_TYPE_INT:
        if (sscanf(entry->value.string_val, "%d", &entry->value.int_val) != 1)
        {
            return FALSE;
        }
        break;
    case ARG_TYPE_FLOAT:
        if (sscanf(entry->value.string_val, "%f", &entry->value.float_val) != 1)
        {
            return FALSE;
        }
        break;
    case ARG_TYPE_STRING:
        break;
    }

    entry->flags.present = 1;

    return TRUE;
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

void m_args_print_help(m_args_t *args)
{
    m_com_sized_data_t *tmp;
    m_args_entry_t *entry = NULL;
    m_list_iterator_t *iterator = m_list_iterator_create(args->arg_list);

    printf("%s", args->executable);
    if (args->description)
    {
        printf(" - %s", args->description);
    }
    puts("\n\nParameters:");

    for (m_list_iterator_go_to_head(iterator); (tmp = m_list_iterator_current(iterator)) != NULL;
         m_list_iterator_next(iterator))
    {
        boolean printed = false;

        entry = tmp->data;

        if (entry->short_switch)
        {
            printf("    %s", entry->short_switch);
            printed = true;
        }

        if (entry->long_switch)
        {
            if (printed)
            {
                printf(" | %s", entry->long_switch);
            }
            else
            {
                printf("    %s", entry->long_switch);
            }
            printed = true;
        }

        if (entry->environment_variable)
        {
            if (printed)
            {
                printf(" | ENV:%s", entry->environment_variable);
            }
            else
            {
                printf("    ENV:%s", entry->environment_variable);
            }
            printed = true;
        }

        if (printed)
        {
            if (entry->description)
            {
                printf(" - %s", entry->description);
            }

            putchar('\n');
        }
    }

    m_list_iterator_destroy(&iterator);
}

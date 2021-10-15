#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"
#include "m_args.h"
#include <m_libs/m_list.h>
#include <m_libs/m_mem.h>

m_args_t *m_args_create() {
  m_args_t *args = (m_args_t *)m_mem_malloc(sizeof(m_args_t));

  args->arg_list = m_list_create();

  return args;
}

void m_args_destroy(m_args_t **args) {
  m_list_destroy((*args)->arg_list);
  free(*args);
}

bool m_args_parse(m_args_t *args, int argc, char **argv) {
  m_com_sized_data_t *tmp;
  m_list_iterator_t *iterator = m_list_iterator_create(args->arg_list);

  for (m_list_iterator_go_to_head(iterator);
       (tmp = m_list_iterator_current(iterator)) != NULL;
       m_list_iterator_next(iterator)) {
    m_args_entry_t *entry = tmp->data;

    for (int i = 0; i < argc; i++) {
      bool found = FALSE;
      char *env_ptr;
      if (strcmp(argv[i], entry->short_switch) == 0 ||
          strcmp(argv[i], entry->long_switch) == 0) {
        if (!entry->flags.no_value)
          entry->value.string_val = argv[i + 1];
        found = TRUE;
      } else if ((env_ptr = getenv(entry->environment_variable)) != NULL) {
        if (!entry->flags.no_value)
          entry->value.string_val = env_ptr;
        found = TRUE;
      }

      if (found) {
        entry->flags.present = 1;
        break;
      }
    }
  }
}

m_args_entry_t *m_args_get(m_args_t *args, uint32_t id) {
  m_com_sized_data_t *tmp;
  m_list_iterator_t *iterator = m_list_iterator_create(args->arg_list);

  for (m_list_iterator_go_to_head(iterator);
       (tmp = m_list_iterator_current(iterator)) != NULL;
       m_list_iterator_next(iterator)) {
    m_args_entry_t *entry = tmp->data;

    if (entry->id == id) {
      return entry;
    }
  }

  return NULL;
}

void m_args_add_entry(m_args_t *args, m_args_entry_t entry) {
  m_com_sized_data_t tmp;

  tmp.size = sizeof(m_args_entry_t);
  tmp.data = &entry;

  m_list_append_to_end_store(args->arg_list, &tmp);
}

#include "m_list.h"
#include "types.h"

#if !defined(COMPOSITE_BUILD)
#include <m_libs/m_mem.h>
#else
#include "../../m_mem/api/m_mem.h"
#endif

m_list_iterator_t *m_list_iterator_create(m_list_t *list)
{
    m_list_iterator_t *iterator;

    iterator = (m_list_iterator_t *)m_alloc_malloc(list->allocator, sizeof(m_list_iterator_t)).pointer;
    if (!iterator)
    {
        return NULL;
    }

    iterator->curr = list->head;
    iterator->list_reference = list;
    list->reference_counter++;

    return iterator;
}

void m_list_iterator_destroy(m_list_iterator_t **iterator)
{
    (*iterator)->list_reference->reference_counter--;
    m_alloc_free((*iterator)->list_reference->allocator, (void*)*iterator);
    *iterator = NULL;
}

void m_list_iterator_go_to_head(m_list_iterator_t *iterator) { iterator->curr = iterator->list_reference->head; }

void m_list_iterator_go_to_tail(m_list_iterator_t *iterator) { iterator->curr = iterator->list_reference->tail; }

m_com_sized_data_t *m_list_iterator_current(const m_list_iterator_t *const iterator)
{
    if (iterator->curr == NULL)
    {
        return NULL;
    }

    return &iterator->curr->data;
}

m_com_sized_data_t *m_list_iterator_next(m_list_iterator_t *iterator)
{
    if (iterator->curr == NULL)
    {
        return NULL;
    }

    iterator->curr = iterator->curr->next;
    if (iterator->curr == NULL)
    {
        return NULL;
    }

    return &iterator->curr->data;
}

m_com_sized_data_t *m_list_iterator_previous(m_list_iterator_t *iterator)
{
    if (iterator->curr == NULL)
    {
        return NULL;
    }

    iterator->curr = iterator->curr->prev;
    if (iterator->curr == NULL)
    {
        return NULL;
    }

    return &iterator->curr->data;
}

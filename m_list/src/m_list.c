#include <stdio.h>
#include <stdlib.h>

#include "config.h"
#include "m_list.h"
#include "m_mem.h"

static void delete_any_by_value(m_list_t * list, const m_com_sized_data_t *const value, boolean multi);
static void append_to_end_any(m_list_t *list, const m_com_sized_data_t *const value, boolean copy);
static void append_to_beginning_any(m_list_t *list, const m_com_sized_data_t *const value, boolean copy);
static void conditional_copy(const m_com_sized_data_t *const src, m_com_sized_data_t *dst, boolean copy);

m_list_t *m_list_create()
{
    m_list_t *list;

    list = (m_list_t *)m_mem_malloc(sizeof(m_list_t));

    list->size = 0;
    list->head = NULL;
    list->tail = NULL;

    return list;
}

void m_list_destroy(m_list_t *list)
{
    free(list);
}

m_com_sized_data_t* m_list_set_curr_to_head(m_list_t *list)
{
    list->curr = list->head;
}

m_com_sized_data_t* m_list_set_curr_to_tail(m_list_t *list)
{
    list->curr = list->tail;
}

m_com_sized_data_t* m_list_next(m_list_t *list)
{
    m_com_sized_data_t *next_data = NULL;

    if (list->curr->next)
    {
        next_data = &list->curr->next->data;
        list->curr = list->curr->next;
    }

    return next_data;
}

m_com_sized_data_t* m_list_prev(m_list_t *list)
{
    m_com_sized_data_t *prev_data = NULL;

    if (list->curr->prev)
    {
        prev_data = &list->curr->prev->data;
        list->curr = list->curr->prev;
    }

    return prev_data;
}

m_com_sized_data_t* m_list_get_by_id(const m_list_t *const list, uint32_t id)
{
    m_list_node_t *tmp = list->head;

    for (int i = 0; i < list->size && i != id; i++, tmp = tmp->next);

    return &tmp->data;
}

void m_list_append_to_end_set(m_list_t *list, const m_com_sized_data_t *const value)
{
    append_to_end_any(list, value, FALSE);
}

void m_list_append_to_beginning_set(m_list_t *list, const m_com_sized_data_t *const value)
{
    append_to_beginning_any(list, value, FALSE);
}

void m_list_append_to_end_store(m_list_t *list, const m_com_sized_data_t *const value)
{
    append_to_end_any(list, value, TRUE);
}

void m_list_append_to_beginning_store(m_list_t *list, const m_com_sized_data_t *const value)
{
    append_to_beginning_any(list, value, TRUE);
}

void m_list_delete_by_value(m_list_t *list, const m_com_sized_data_t *const value)
{
    delete_any_by_value(list, value, FALSE);
}

void m_list_delete_all_by_value(m_list_t *list, const m_com_sized_data_t *const value)
{
    delete_any_by_value(list, value, TRUE);
}

void m_list_print(const m_list_t *const list)
{
    m_list_node_t *tmp = list->head;
    int i = 0;
    while (tmp)
    {
        printf("[%d]\n", i);
        printf("    data: %p\n", tmp->data.data);
        printf("    next: %p\n", tmp->next);
        printf("    prev: %p\n", tmp->prev);
        tmp = tmp->next;
        ++i;
    }
    putchar('\n');
}

static void delete_any_by_value(m_list_t *list, const m_com_sized_data_t *const value, boolean multi)
{
    struct m_list_node_t *tmp;

    if (NULL == list->head)
    {
        return;
    }

    tmp = list->head;
    while (NULL != tmp)
    {
        if (m_mem_cmp(&tmp->data, value))
        {
            if (tmp->prev != NULL)
            {
                tmp->prev->next = tmp->next;
            }
            else
            {
                list->head = tmp->next;
            }

            if (tmp->next != NULL)
            {
                tmp->next->prev = tmp->prev;
            }

            free(tmp);

            if (TRUE != multi)
            {
                return;
            }
        }

        tmp = tmp->next;
    }
}

static void append_to_end_any(m_list_t *list, const m_com_sized_data_t *const value, boolean copy)
{
    struct m_list_node_t *tmp;

    tmp = (struct m_list_node_t *)m_mem_malloc(sizeof(struct m_list_node_t));

    conditional_copy(value, &tmp->data, copy);
    tmp->next = NULL;
    tmp->prev = list->tail;

    if (list->tail)
    {
        list->tail->next = tmp;
    }
    list->tail = tmp;

    if (0 == list->size)
    {
        list->head = list->tail;
    }

    list->size++;
}

static void append_to_beginning_any(m_list_t *list, const m_com_sized_data_t *const value, boolean copy)
{
    struct m_list_node_t *tmp;

    tmp = (struct m_list_node_t *)m_mem_malloc(sizeof(struct m_list_node_t));

    conditional_copy(value, &tmp->data, copy);
    tmp->next = list->head;
    tmp->prev = NULL;

    if (list->head)
    {
        list->head->prev = tmp;
    }
    list->head = tmp;

    if (0 == list->size)
    {
        list->tail = list->head;
    }

    list->size++;
}

static void conditional_copy(const m_com_sized_data_t *const src, m_com_sized_data_t *dst, boolean copy)
{
    if (copy)
    {
        dst->data = m_mem_malloc(src->size);
        m_mem_copy(src, dst);
        dst->size = src->size;
    }
    else
    {
        *dst = *src;
    }
}

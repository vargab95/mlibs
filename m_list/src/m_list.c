#include <stdio.h>
#include <stdlib.h>

#include "m_list.h"
#include "types.h"

#if !defined(COMPOSITE_BUILD)
#include <m_libs/m_mem.h>
#else
#include "../../m_mem/api/m_mem.h"
#endif

static void delete_any_by_value(m_list_t *list, const m_com_sized_data_t *const value, boolean multi);
static void append_to_end_any(m_list_t *list, const m_com_sized_data_t *const value, boolean copy);
static void append_to_beginning_any(m_list_t *list, const m_com_sized_data_t *const value, boolean copy);
static void conditional_copy(const m_com_sized_data_t *const src, m_com_sized_data_t *dst, boolean copy);
static void delete_node(m_list_node_t **node);

m_list_t *m_list_create()
{
    m_list_t *list;

    list = (m_list_t *)m_mem_malloc(sizeof(m_list_t));

    list->size = 0;
    list->reference_counter = 0;
    list->head = NULL;
    list->tail = NULL;

    return list;
}

void m_list_destroy(m_list_t **list)
{
    m_list_node_t *tmp;

    tmp = (*list)->head;
    while (tmp != NULL)
    {
        delete_node(&tmp);
    }

    if ((*list)->reference_counter)
    {
        puts("ERROR: List deleted with non-zero reference count");
        printf("List: %p, reference counter: %d\n", *list, (*list)->reference_counter);
    }

    free(*list);
    *list = NULL;
}

size_t m_list_get_size(const m_list_t *const list)
{
    if (list == NULL)
    {
        return 0;
    }

    return list->size;
}

m_com_sized_data_t* m_list_get_first(const m_list_t *const list)
{
    if (list == NULL || list->head == NULL)
    {
        return NULL;
    }

    return &list->head->data;
}

m_com_sized_data_t* m_list_get_last(const m_list_t *const list)
{
    if (list == NULL || list->tail == NULL)
    {
        return NULL;
    }

    return &list->tail->data;
}

m_com_sized_data_t *m_list_get_by_id(const m_list_t *const list, uint32_t id)
{
    m_list_node_t *tmp = list->head;

    for (size_t i = 0; i < list->size && i != id; i++, tmp = tmp->next)
        ;

    if (!tmp)
    {
        return NULL;
    }

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
        printf("    data: %p with size %ld\n", tmp->data.data, tmp->data.size);
        printf("        dump: ");
        m_mem_dump(&tmp->data, stdout);
        putchar('\n');
        printf("        text dump: ");
        m_mem_text_dump(&tmp->data, stdout);
        putchar('\n');
        printf("    next: %p\n", tmp->next);
        printf("    prev: %p\n", tmp->prev);
        tmp = tmp->next;
        ++i;
    }
    putchar('\n');
}

void m_list_custom_print(const m_list_t *const list, void (*func)(const m_com_sized_data_t* const))
{
    m_list_node_t *tmp = list->head;

    while (tmp)
    {
        func(&tmp->data);
        tmp = tmp->next;
    }
}

static void delete_any_by_value(m_list_t *list, const m_com_sized_data_t *const value, boolean multi)
{
    struct m_list_node_t *tmp;

    if (NULL == list || NULL == list->head)
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
            else
            {
                list->tail = tmp->prev;
            }

            delete_node(&tmp);
            --list->size;

            if (TRUE != multi)
            {
                return;
            }
        }
        else
        {
            tmp = tmp->next;
        }
    }
}

static void append_to_end_any(m_list_t *list, const m_com_sized_data_t *const value, boolean copy)
{
    struct m_list_node_t *tmp;

    tmp = (struct m_list_node_t *)m_mem_malloc(sizeof(struct m_list_node_t));

    conditional_copy(value, &tmp->data, copy);
    tmp->copied = (copy) ? STORED : SET;
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
        dst->size = src->size;
        m_mem_copy(src, dst);
    }
    else
    {
        *dst = *src;
    }
}

void m_list_dump_binary(const m_list_t *const list, FILE *fp)
{
    m_list_node_t *tmp = list->head;
    while (tmp)
    {
        fwrite(&tmp->data.size, sizeof(tmp->data.size), 1, fp);
        fwrite(tmp->data.data, sizeof(uint8_t), tmp->data.size, fp);
        tmp = tmp->next;
    }
}

void m_list_custom_dump_binary(const m_list_t *const list,
                               FILE *fp,
                               void (*func)(const m_com_sized_data_t* const, FILE *fp))
{
    m_list_node_t *tmp = list->head;
    while (tmp)
    {
        func(&tmp->data, fp);
        tmp = tmp->next;
    }
}

m_list_t *m_list_load_binary(FILE *fp)
{
    m_list_t *list = m_list_create();
    size_t data_size;
    long end_of_file, current_position;

    current_position = ftell(fp);
    fseek(fp, 0, SEEK_END);
    end_of_file = ftell(fp);
    fseek(fp, current_position, SEEK_SET);

    while (ftell(fp) < end_of_file)
    {
        m_com_sized_data_t *data;

        if (fread(&data_size, sizeof(data_size), 1, fp) != 1)
        {
            m_list_destroy(&list);
            break;
        }

        data = m_mem_sized_malloc(data_size);
        if (fread(data->data, sizeof(uint8_t), data->size, fp) != data->size)
        {
            m_list_destroy(&list);
            break;
        }

        append_to_end_any(list, data, FALSE);
    }

    return list;
}

static void delete_node(m_list_node_t **node)
{
    m_list_node_t *to_delete;

    if ((*node)->copied == STORED)
    {
        free((*node)->data.data);
    }

    to_delete = (*node);
    (*node) = (*node)->next;
    free(to_delete);
}

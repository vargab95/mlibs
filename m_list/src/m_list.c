#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "m_list.h"
#include "types.h"

#if !defined(COMPOSITE_BUILD)
#include <m_libs/m_mem.h>
#else
#include "../../m_mem/api/m_mem.h"
#endif

#if !defined(COMPOSITE_BUILD)
#include <m_libs/m_alloc.h>
#else
#include "../../m_mem/api/m_alloc.h"
#endif

static void delete_any_by_value(m_list_t *list, const m_com_sized_data_t *const value, boolean multi);
static boolean append_to_end_any(m_list_t *list, const m_com_sized_data_t *const value, boolean copy);
static boolean append_to_beginning_any(m_list_t *list, const m_com_sized_data_t *const value, boolean copy);
static boolean conditional_copy(m_list_t *list, const m_com_sized_data_t *const src, m_com_sized_data_t *dst, boolean copy);
static void delete_node(m_list_t *list, m_list_node_t **node);

m_list_t *m_list_create(m_alloc_instance_t *allocator)
{
    m_list_t *list;

    list = (m_list_t *)m_alloc_malloc(allocator, (sizeof(m_list_t))).pointer;
    if (list == NULL)
    {
        return NULL;
    }

    list->allocator = allocator;
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
        delete_node(*list, &tmp);
    }

    if ((*list)->reference_counter)
    {
        puts("ERROR: List deleted with non-zero reference count");
        printf("List: %p, reference counter: %d\n", *list, (*list)->reference_counter);
    }

    m_alloc_free((*list)->allocator, (void*)*list);
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

boolean m_list_append_to_end_set(m_list_t *list, const m_com_sized_data_t *const value)
{
    return append_to_end_any(list, value, FALSE);
}

boolean m_list_append_to_beginning_set(m_list_t *list, const m_com_sized_data_t *const value)
{
    return append_to_beginning_any(list, value, FALSE);
}

boolean m_list_append_to_end_store(m_list_t *list, const m_com_sized_data_t *const value)
{
    return append_to_end_any(list, value, TRUE);
}

boolean m_list_append_to_beginning_store(m_list_t *list, const m_com_sized_data_t *const value)
{
    return append_to_beginning_any(list, value, TRUE);
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

            delete_node(list, &tmp);
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

static boolean append_to_end_any(m_list_t *list, const m_com_sized_data_t *const value, boolean copy)
{
    struct m_list_node_t *tmp;

    tmp = (struct m_list_node_t *)m_alloc_malloc(list->allocator, sizeof(struct m_list_node_t)).pointer;
    if (tmp == NULL)
    {
        return false;
    }

    if (!conditional_copy(list, value, &tmp->data, copy))
    {
        m_alloc_free(list->allocator, (void*)tmp);
        return false;
    }

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

    return true;
}

static boolean append_to_beginning_any(m_list_t *list, const m_com_sized_data_t *const value, boolean copy)
{
    struct m_list_node_t *tmp;

    tmp = (struct m_list_node_t *)m_alloc_malloc(list->allocator, sizeof(struct m_list_node_t)).pointer;
    if (tmp == NULL)
    {
        return false;
    }

    if (!conditional_copy(list, value, &tmp->data, copy))
    {
        m_alloc_free(list->allocator, (void*)tmp);
        return false;
    }
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

    return true;
}

static boolean conditional_copy(m_list_t *list, const m_com_sized_data_t *const src, m_com_sized_data_t *dst, boolean copy)
{
    if (copy)
    {
        dst->data = m_alloc_malloc(list->allocator, src->size).pointer;
        if (dst->data == NULL)
        {
            return false;
        }
        dst->size = src->size;
        m_mem_copy(src, dst);
    }
    else
    {
        *dst = *src;
    }

    return true;
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
    int pagesize = getpagesize();
    m_alloc_instance_t *allocator = m_alloc_create((m_alloc_config_t){
        .type = M_ALLOC_TYPE_ARENA,
        .u = {
            .arena = {
                .minimum_size_per_arena = pagesize
            }
        }
    }).allocator;
    if (!allocator)
    {
        return NULL;
    }

    m_list_t *list = m_list_create(allocator);
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

        data = m_alloc_sized_malloc(list->allocator, data_size).data;
        if (fread(data->data, sizeof(uint8_t), data->size, fp) != data->size)
        {
            m_list_destroy(&list);
            break;
        }

        if (!append_to_end_any(list, data, FALSE))
        {
            break;
        }
    }

    m_alloc_destroy(&allocator);

    return list;
}

static void delete_node(m_list_t *list, m_list_node_t **node)
{
    m_list_node_t *to_delete;

    if ((*node)->copied == STORED)
    {
        m_alloc_free(list->allocator, (*node)->data.data);
    }

    to_delete = (*node);
    (*node) = (*node)->next;
    m_alloc_free(list->allocator, (void*)to_delete);
}

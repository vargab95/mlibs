#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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

static m_context_id_t create_context(m_allocator_config_t config);
static void destroy_context(m_context_id_t id);
static m_com_sized_data_t *malloc_impl(m_context_id_t id, size_t size);
static m_com_sized_data_t *calloc_impl(m_context_id_t id, uint32_t number, size_t size);
static m_com_sized_data_t *realloc_impl(m_context_id_t id, m_com_sized_data_t *data, size_t size);
static void free_impl(m_context_id_t id, m_com_sized_data_t *data);

m_allocator_t m_fixed_allocator = {
    .create = create_context,
    .destroy = destroy_context,
    .malloc = malloc_impl,
    .calloc = calloc_impl,
    .realloc = realloc_impl,
    .free = free_impl
};

typedef struct {
    void *ptr;
    void *buffer;
} fixed_allocator_context;

static m_context_id_t create_context(m_allocator_config_t config)
{
    size_t allocation_size = config.fixed.minimum_size + sizeof(fixed_allocator_context);
    const int page_size = getpagesize();
    if (allocation_size % page_size)
    {
        allocation_size = ((allocation_size / page_size) + 1) * page_size;
    }

    fixed_allocator_context *context = malloc(allocation_size);
    if (context == NULL)
    {
        return NULL;
    }

    context->buffer = context + sizeof(context->ptr);
    context->ptr = context->buffer + allocation_size;

    return context;
}

static void destroy_context(m_context_id_t context)
{
    free(context);
}

static m_com_sized_data_t *malloc_impl(m_context_id_t context, size_t size)
{
    fixed_allocator_context *_context = context;
    void *result_ptr = _context->ptr - size - sizeof(m_com_sized_data_t);
    m_com_sized_data_t *result;

    if (result_ptr < _context->buffer)
    {
        return NULL;
    }

    result = result_ptr;
    result->size = size;
    result->data = result_ptr + sizeof(m_com_sized_data_t);

    _context->ptr = result_ptr;

    return result;
}

static m_com_sized_data_t *calloc_impl(m_context_id_t context, uint32_t number, size_t size)
{
    const size_t whole_size = size * number;
    m_com_sized_data_t *result = malloc_impl(context, whole_size);

    if (result)
    {
        memset(result->data, 0, whole_size);
    }

    return result;
}

static m_com_sized_data_t *realloc_impl(m_context_id_t context, m_com_sized_data_t *data, size_t size)
{
    m_com_sized_data_t *result = malloc_impl(context, size);

    if (result)
    {
        m_mem_copy(data, result);
    }

    return result;
}

static void free_impl(m_context_id_t context, m_com_sized_data_t *data)
{
}

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
static void destroy_context(m_context_id_t context);

static void *malloc_impl(m_context_id_t context, size_t size);
static void *calloc_impl(m_context_id_t context, uint32_t number, size_t size);
static void *realloc_impl(m_context_id_t context, void *data, size_t size);
static void free_impl(m_context_id_t context, void *data);

static m_com_sized_data_t *sized_malloc_impl(m_context_id_t context, size_t size);
static m_com_sized_data_t *sized_calloc_impl(m_context_id_t context, uint32_t number, size_t size);
static m_com_sized_data_t *sized_realloc_impl(m_context_id_t context, m_com_sized_data_t *data, size_t size);
static void sized_free_impl(m_context_id_t context, m_com_sized_data_t *data);

m_allocator_t m_fixed_allocator = {
    .create = create_context,
    .destroy = destroy_context,

    .malloc = malloc_impl,
    .calloc = calloc_impl,
    .realloc = realloc_impl,
    .free = free_impl,

    .sized_malloc = sized_malloc_impl,
    .sized_calloc = sized_calloc_impl,
    .sized_realloc = sized_realloc_impl,
    .sized_free = sized_free_impl
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

    context->buffer = (void*)context + sizeof(*context);
    context->ptr = context->buffer + allocation_size - sizeof(*context);

    return context;
}

static void destroy_context(m_context_id_t context)
{
    free(context);
}

static void *malloc_impl(m_context_id_t context, size_t size)
{
    fixed_allocator_context *_context = context;
    void *result_ptr = _context->ptr - size;

    if (result_ptr < _context->buffer)
    {
        return NULL;
    }

    _context->ptr = result_ptr;

    return result_ptr;
}

static void *calloc_impl(m_context_id_t context, uint32_t number, size_t size)
{
    const size_t whole_size = size * number;
    void *result = malloc_impl(context, whole_size);

    if (result)
    {
        memset(result, 0, whole_size);
    }

    return result;
}

static void *realloc_impl(m_context_id_t context, void *data, size_t size)
{
    void *result = malloc_impl(context, size);

    if (result)
    {
        memcpy(result, data, size);
    }

    return result;
}

static void free_impl(m_context_id_t context, void *data)
{

}

static m_com_sized_data_t *sized_malloc_impl(m_context_id_t context, size_t size)
{
    m_com_sized_data_t *result = malloc_impl(context, size + sizeof(m_com_sized_data_t));

    if (result)
    {
        result->size = size;
        result->data = (void*)result + sizeof(m_com_sized_data_t);
    }

    return result;
}

static m_com_sized_data_t *sized_calloc_impl(m_context_id_t context, uint32_t number, size_t size)
{
    const size_t whole_size = size * number;
    m_com_sized_data_t *result = sized_malloc_impl(context, whole_size);

    if (result)
    {
        memset(result->data, 0, whole_size);
    }

    return result;
}

static m_com_sized_data_t *sized_realloc_impl(m_context_id_t context, m_com_sized_data_t *data, size_t size)
{
    m_com_sized_data_t *result = sized_malloc_impl(context, size);

    if (result)
    {
        m_mem_copy(data, result);
    }

    return result;
}

static void sized_free_impl(m_context_id_t context, m_com_sized_data_t *data)
{
}

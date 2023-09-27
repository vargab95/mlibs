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

#include "../inc/internal.h"

static m_alloc_context_creation_result_t create_context(void *config);
static m_alloc_rc_t destroy_context(m_context_id_t context);

static m_alloc_alloc_result_t malloc_impl(m_context_id_t context, size_t size);
static m_alloc_alloc_result_t calloc_impl(m_context_id_t context, uint32_t number, size_t size);
static m_alloc_alloc_result_t realloc_impl(m_context_id_t context, void *data, size_t size);
static m_alloc_rc_t free_impl(m_context_id_t context, void *data);

static m_alloc_sized_alloc_result_t sized_malloc_impl(m_context_id_t context, size_t size);
static m_alloc_sized_alloc_result_t sized_calloc_impl(m_context_id_t context, uint32_t number, size_t size);
static m_alloc_sized_alloc_result_t sized_realloc_impl(m_context_id_t context, m_com_sized_data_t *data, size_t size);
static m_alloc_rc_t sized_free_impl(m_context_id_t context, m_com_sized_data_t *data);

m_alloc_functions_t m_fixed_allocator = {
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

m_alloc_creation_result_t m_fixed_allocator_create(m_alloc_fixed_config_t configuration)
{
    m_alloc_context_creation_result_t context_result = create_context(&configuration);
    if (context_result.return_code != M_ALLOC_RC_OK)
    {
        return (m_alloc_creation_result_t) {
            .return_code = context_result.return_code,
            .allocator = NULL
        };
    }

    m_alloc_alloc_result_t alloc_result = malloc_impl(context_result.context, sizeof(m_alloc_instance_t));
    if (alloc_result.return_code != M_ALLOC_RC_OK)
    {
        return (m_alloc_creation_result_t) {
            .return_code = alloc_result.return_code,
            .allocator = NULL
        };
    }

    m_alloc_instance_t *allocator = alloc_result.pointer;

    allocator->context = context_result.context;
    allocator->configuration = (m_alloc_config_t){
        .type = M_ALLOC_TYPE_FIXED,
        .u = {
            .fixed = configuration
        }
    };
    allocator->functions = &m_fixed_allocator;

    return (m_alloc_creation_result_t) {
        .return_code = M_ALLOC_RC_OK,
        .allocator = allocator
    };
}

static m_alloc_context_creation_result_t create_context(void *config)
{
    m_alloc_fixed_config_t *_config = config;
    m_alloc_context_creation_result_t result = {
        .return_code = M_ALLOC_RC_OK,
        .context = NULL
    };

    size_t allocation_size = _config->minimum_size + sizeof(fixed_allocator_context);
    const int page_size = getpagesize();
    if (allocation_size % page_size)
    {
        allocation_size = ((allocation_size / page_size) + 1) * page_size;
    }

    fixed_allocator_context *context = malloc(allocation_size);
    if (context == NULL)
    {
        result.return_code = M_ALLOC_RC_NO_MEMORY;
        return result;
    }

    context->buffer = (void*)context + sizeof(*context);
    context->ptr = context->buffer + allocation_size - sizeof(*context);

    result.context = context;

    return result;
}

static m_alloc_rc_t destroy_context(m_context_id_t context)
{
    free(context);
    return M_ALLOC_RC_OK;
}

static m_alloc_alloc_result_t malloc_impl(m_context_id_t context, size_t size)
{
    fixed_allocator_context *_context = context;
    void *result_ptr = _context->ptr - size;

    if (result_ptr < _context->buffer)
    {
        return (m_alloc_alloc_result_t){
            .return_code = M_ALLOC_RC_SIZE_LIMIT,
            .pointer = NULL
        };
    }

    _context->ptr = result_ptr;

    return (m_alloc_alloc_result_t){
        .return_code = M_ALLOC_RC_OK,
        .pointer = result_ptr
    };
}

static m_alloc_alloc_result_t calloc_impl(m_context_id_t context, uint32_t number, size_t size)
{
    const size_t whole_size = size * number;
    m_alloc_alloc_result_t result = malloc_impl(context, whole_size);

    if (result.return_code == M_ALLOC_RC_OK)
    {
        memset(result.pointer, 0, whole_size);
    }

    return result;
}

static m_alloc_alloc_result_t realloc_impl(m_context_id_t context, void *data, size_t size)
{
    m_alloc_alloc_result_t result = malloc_impl(context, size);

    if (result.return_code == M_ALLOC_RC_OK)
    {
        memcpy(result.pointer, data, size);
    }

    return result;
}

static m_alloc_rc_t free_impl(m_context_id_t context, void *data)
{
    return M_ALLOC_RC_OK;
}

static m_alloc_sized_alloc_result_t sized_malloc_impl(m_context_id_t context, size_t size)
{
    m_alloc_alloc_result_t malloc_result = malloc_impl(context, size + sizeof(m_com_sized_data_t));
    m_alloc_sized_alloc_result_t result;

    if (result.return_code == M_ALLOC_RC_OK)
    {
        result.data = malloc_result.pointer;
        result.data->size = size;
        result.data->data = (void*)result.data + sizeof(m_com_sized_data_t);
    }

    result.return_code = malloc_result.return_code;

    return result;
}

static m_alloc_sized_alloc_result_t sized_calloc_impl(m_context_id_t context, uint32_t number, size_t size)
{
    const size_t whole_size = size * number;
    m_alloc_sized_alloc_result_t result = sized_malloc_impl(context, whole_size);

    if (result.return_code == M_ALLOC_RC_OK)
    {
        memset(result.data, 0, whole_size);
    }

    return result;
}

static m_alloc_sized_alloc_result_t sized_realloc_impl(m_context_id_t context, m_com_sized_data_t *data, size_t size)
{
    m_alloc_sized_alloc_result_t result = sized_malloc_impl(context, size);

    if (result.return_code == M_ALLOC_RC_OK)
    {
        m_mem_copy(data, result.data);
    }

    return result;
}

static m_alloc_rc_t sized_free_impl(m_context_id_t context, m_com_sized_data_t *data)
{
    return M_ALLOC_RC_OK;
}

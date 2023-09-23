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
static m_alloc_rc_t destroy_context(m_context_id_t id);

static m_alloc_alloc_result_t malloc_impl(m_context_id_t id, size_t size);
static m_alloc_alloc_result_t calloc_impl(m_context_id_t id, uint32_t number, size_t size);
static m_alloc_alloc_result_t realloc_impl(m_context_id_t id, void *data, size_t size);
static m_alloc_rc_t free_impl(m_context_id_t id, void *data);

static m_alloc_sized_alloc_result_t sized_malloc_impl(m_context_id_t id, size_t size);
static m_alloc_sized_alloc_result_t sized_calloc_impl(m_context_id_t id, uint32_t number, size_t size);
static m_alloc_sized_alloc_result_t sized_realloc_impl(m_context_id_t id, m_com_sized_data_t *data, size_t size);
static m_alloc_rc_t sized_free_impl(m_context_id_t id, m_com_sized_data_t *data);

m_alloc_functions_t m_system_alloc = {
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

m_alloc_creation_result_t m_system_alloc_create()
{
    m_alloc_creation_result_t result = {
        .return_code = M_ALLOC_RC_OK,
        .allocator = malloc(sizeof(m_alloc_instance_t))
    };

    if (result.allocator == NULL)
    {
        result.return_code = M_ALLOC_RC_NO_MEMORY;
    }

    return result;
}

static m_alloc_context_creation_result_t create_context(void *config)
{
    return (m_alloc_context_creation_result_t){
        .return_code = M_ALLOC_RC_OK,
        .context = (void*)0xABCDEF123
    };
}

static m_alloc_rc_t destroy_context(m_context_id_t context)
{
    return M_ALLOC_RC_OK;
}

static m_alloc_alloc_result_t malloc_impl(m_context_id_t id, size_t size)
{
    m_alloc_alloc_result_t result = {
        .return_code = M_ALLOC_RC_OK,
        .pointer = malloc(size)
    };

    if (result.pointer == NULL)
    {
        result.return_code = M_ALLOC_RC_NO_MEMORY;
    }

    return result;
}

static m_alloc_alloc_result_t calloc_impl(m_context_id_t id, uint32_t number, size_t size)
{
    m_alloc_alloc_result_t result = {
        .return_code = M_ALLOC_RC_OK,
        .pointer = calloc(number, size)
    };

    if (result.pointer == NULL)
    {
        result.return_code = M_ALLOC_RC_NO_MEMORY;
    }

    return result;
}

static m_alloc_alloc_result_t realloc_impl(m_context_id_t id, void *data, size_t size)
{
    m_alloc_alloc_result_t result = {
        .return_code = M_ALLOC_RC_OK,
        .pointer = realloc(data, size)
    };

    if (result.pointer == NULL)
    {
        result.return_code = M_ALLOC_RC_NO_MEMORY;
    }

    return result;
}

static m_alloc_rc_t free_impl(m_context_id_t id, void *data)
{
    free(data);
    return M_ALLOC_RC_OK;
}

static m_alloc_sized_alloc_result_t sized_malloc_impl(m_context_id_t context, size_t size)
{
    m_alloc_sized_alloc_result_t result = {
        .return_code = M_ALLOC_RC_OK,
        .data = malloc(size + sizeof(m_com_sized_data_t))
    };

    if (result.data == NULL)
    {
        result.return_code = M_ALLOC_RC_NO_MEMORY;
    }

    result.data->size = size;
    result.data->data = (void*)result.data + sizeof(m_com_sized_data_t);

    return result;
}

static m_alloc_sized_alloc_result_t sized_calloc_impl(m_context_id_t context, uint32_t number, size_t size)
{
    m_alloc_sized_alloc_result_t result = sized_malloc_impl(context, number * size);
    if (result.return_code != M_ALLOC_RC_OK)
    {
        return result;
    }

    memset(result.data->data, 0, number * size);

    return result;
}

static m_alloc_sized_alloc_result_t sized_realloc_impl(m_context_id_t context, m_com_sized_data_t *data, size_t size)
{
    m_alloc_sized_alloc_result_t result = {
        .return_code = M_ALLOC_RC_OK,
        .data = realloc(data, size)
    };

    if (result.data == NULL)
    {
        result.return_code = M_ALLOC_RC_NO_MEMORY;
    }
    else
    {
        result.data->size = size;
    }

    return result;
}

static m_alloc_rc_t sized_free_impl(m_context_id_t id, m_com_sized_data_t *data)
{
    free(data);
    return M_ALLOC_RC_OK;
}

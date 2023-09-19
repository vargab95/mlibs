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

static void *malloc_impl(m_context_id_t id, size_t size);
static void *calloc_impl(m_context_id_t id, uint32_t number, size_t size);
static void *realloc_impl(m_context_id_t id, void *data, size_t size);
static void free_impl(m_context_id_t id, void *data);

static m_com_sized_data_t *sized_malloc_impl(m_context_id_t id, size_t size);
static m_com_sized_data_t *sized_calloc_impl(m_context_id_t id, uint32_t number, size_t size);
static m_com_sized_data_t *sized_realloc_impl(m_context_id_t id, m_com_sized_data_t *data, size_t size);
static void sized_free_impl(m_context_id_t id, m_com_sized_data_t *data);

m_allocator_t m_system_allocator = {
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

static m_context_id_t create_context(m_allocator_config_t config)
{
    return (void*)0xABCDEF123;
}

static void destroy_context(m_context_id_t context)
{
}

static void *malloc_impl(m_context_id_t id, size_t size)
{
    return malloc(size);
}

static void *calloc_impl(m_context_id_t id, uint32_t number, size_t size)
{
    return calloc(number, size);
}

static void *realloc_impl(m_context_id_t id, void *data, size_t size)
{
    return realloc(data, size);
}

static void free_impl(m_context_id_t id, void *data)
{
    free(data);
}

static m_com_sized_data_t *sized_malloc_impl(m_context_id_t context, size_t size)
{
    m_com_sized_data_t *result = malloc(size + sizeof(m_com_sized_data_t));
    if (result == NULL)
    {
        return NULL;
    }

    result->size = size;
    result->data = (void*)result + sizeof(m_com_sized_data_t);

    return result;
}

static m_com_sized_data_t *sized_calloc_impl(m_context_id_t context, uint32_t number, size_t size)
{
    m_com_sized_data_t *result = sized_malloc_impl(context, number * size);
    if (result == NULL)
    {
        return NULL;
    }

    memset(result->data, 0, number * size);

    return result;
}

static m_com_sized_data_t *sized_realloc_impl(m_context_id_t context, m_com_sized_data_t *data, size_t size)
{
    m_com_sized_data_t *result = realloc(data, size);
    if (result == NULL)
    {
        return NULL;
    }

    result->size = size;

    return result;
}

static void sized_free_impl(m_context_id_t id, m_com_sized_data_t *data)
{
    free(data);
}

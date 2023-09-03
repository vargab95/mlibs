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

m_allocator_t m_slab_allocator = {
    .create = create_context,
    .destroy = destroy_context,
    .malloc = malloc_impl,
    .calloc = calloc_impl,
    .realloc = realloc_impl,
    .free = free_impl
};

typedef struct {
    
} slab_allocator_context;

static m_context_id_t create_context(m_allocator_config_t config)
{

}

static void destroy_context(m_context_id_t id)
{

}

static m_com_sized_data_t *malloc_impl(m_context_id_t id, size_t size)
{

}

static m_com_sized_data_t *calloc_impl(m_context_id_t id, uint32_t number, size_t size)
{

}

static m_com_sized_data_t *realloc_impl(m_context_id_t id, m_com_sized_data_t *data, size_t size)
{

}

static void free_impl(m_context_id_t id, m_com_sized_data_t *data)
{

}

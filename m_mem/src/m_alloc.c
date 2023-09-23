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

m_alloc_config_t m_alloc_get_configuration(m_alloc_instance_t *allocator)
{
    return allocator->configuration;
}

m_alloc_creation_result_t m_alloc_create(m_alloc_config_t config)
{
    m_alloc_creation_result_t result = {
        .return_code = M_ALLOC_RC_INVALID_CONFIG,
        .allocator = NULL
    };

    switch (config.type)
    {
        case M_ALLOC_TYPE_SYSTEM:
            result = m_system_alloc_create();
            break;
        case M_ALLOC_TYPE_FIXED:
            result = m_fixed_allocator_create(config.u.fixed);
            break;
        case M_ALLOC_TYPE_ARENA:
            result = m_arena_allocator_create(config.u.arena);
            break;
        case M_ALLOC_TYPE_SLAB:
            result = m_slab_allocator_create(config.u.slab);
            break;
    }

    return result;
}

m_alloc_rc_t m_alloc_destroy(m_alloc_instance_t **allocator)
{
    m_alloc_rc_t rc = (*allocator)->functions->destroy((*allocator)->context);
    *allocator = NULL;
    return rc;
}

m_alloc_alloc_result_t m_alloc_malloc(m_alloc_instance_t *allocator, size_t size)
{
    return allocator->functions->malloc(allocator->context, size);
}

m_alloc_alloc_result_t m_alloc_calloc(m_alloc_instance_t *allocator, uint32_t number, size_t size)
{
    return allocator->functions->calloc(allocator->context, number, size);
}

m_alloc_alloc_result_t m_alloc_realloc(m_alloc_instance_t *allocator, void *data, size_t size)
{
    return allocator->functions->realloc(allocator->context, data, size);
}

m_alloc_rc_t m_alloc_free(m_alloc_instance_t *allocator, void *data)
{
    return allocator->functions->free(allocator->context, data);
}

m_alloc_sized_alloc_result_t m_alloc_sized_malloc(m_alloc_instance_t *allocator, size_t size)
{
    return allocator->functions->sized_malloc(allocator->context, size);
}

m_alloc_sized_alloc_result_t m_alloc_sized_calloc(m_alloc_instance_t *allocator, uint32_t number, size_t size)
{
    return allocator->functions->sized_calloc(allocator->context, number, size);
}

m_alloc_sized_alloc_result_t m_alloc_sized_realloc(m_alloc_instance_t *allocator, m_com_sized_data_t *data, size_t size)
{
    return allocator->functions->sized_realloc(allocator->context, data, size);
}

m_alloc_rc_t m_alloc_sized_free(m_alloc_instance_t *allocator, m_com_sized_data_t *data)
{
    return allocator->functions->sized_free(allocator->context, data);
}

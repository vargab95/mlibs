#ifndef __M_ALLOC_INTERNAL_H__
#define __M_ALLOC_INTERNAL_H__

#if !defined(COMPOSITE_BUILD)
#include <m_libs/m_alloc.h>
#else
#include "../api/m_alloc.h"
#endif

typedef struct {
    m_alloc_rc_t return_code;
    m_context_id_t context;
} m_alloc_context_creation_result_t;

/**
 * @brief MLibs allocator
 */
typedef struct {
    m_alloc_context_creation_result_t (*create)(void* config);
    m_alloc_rc_t (*destroy)(m_context_id_t id);

    m_alloc_alloc_result_t (*malloc)(m_context_id_t id, size_t size);
    m_alloc_alloc_result_t (*calloc)(m_context_id_t id, uint32_t number, size_t size);
    m_alloc_alloc_result_t (*realloc)(m_context_id_t id, void *data, size_t size);
    m_alloc_rc_t (*free)(m_context_id_t id, void *data);

    m_alloc_sized_alloc_result_t (*sized_malloc)(m_context_id_t id, size_t size);
    m_alloc_sized_alloc_result_t (*sized_calloc)(m_context_id_t id, uint32_t number, size_t size);
    m_alloc_sized_alloc_result_t (*sized_realloc)(m_context_id_t id, m_com_sized_data_t *data, size_t size);
    m_alloc_rc_t (*sized_free)(m_context_id_t id, m_com_sized_data_t *data);
} m_alloc_functions_t;

typedef struct m_alloc_instance_t {
    m_context_id_t context;
    m_alloc_config_t configuration;
    m_alloc_functions_t *functions;
} m_alloc_instance_t;

extern m_alloc_creation_result_t m_system_alloc_create();
extern m_alloc_creation_result_t m_fixed_allocator_create(m_alloc_fixed_config_t configuration);
extern m_alloc_creation_result_t m_arena_allocator_create(m_alloc_arena_config_t configuration);
extern m_alloc_creation_result_t m_slab_allocator_create(m_alloc_slab_config_t configuration);

#endif

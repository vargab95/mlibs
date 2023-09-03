#ifndef __M_ALLOC_H__
#define __M_ALLOC_H__

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#if !defined(COMPOSITE_BUILD)
#include <m_libs/m_mem.h>
#else
#include "../../m_common/api/m_common.h"
#endif

/** @brief MLibs opaque context ID */
typedef void* m_context_id_t;

/** @brief MLibs allocator configuration */
typedef union {
    struct {
        size_t minimum_size;
    } fixed;
    struct {
        size_t minimum_size_per_arena;
    } arena;
    struct {

    } slab;
} m_allocator_config_t;

/**
 * @brief MLibs allocator
 */
typedef struct {
    m_context_id_t (*create)(m_allocator_config_t config);
    void (*destroy)(m_context_id_t id);

    m_com_sized_data_t *(*malloc)(m_context_id_t id, size_t size);
    m_com_sized_data_t *(*calloc)(m_context_id_t id, uint32_t number, size_t size);
    m_com_sized_data_t *(*realloc)(m_context_id_t id, m_com_sized_data_t *data, size_t size);
    void (*free)(m_context_id_t id, m_com_sized_data_t *data);
} m_allocator_t;

extern m_allocator_t m_fixed_allocator;
extern m_allocator_t m_arena_allocator;
extern m_allocator_t m_slab_allocator;

#endif

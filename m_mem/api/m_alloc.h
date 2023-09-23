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

typedef enum {
    M_ALLOC_RC_OK,
    M_ALLOC_RC_NO_MEMORY,
    M_ALLOC_RC_SIZE_LIMIT,
    M_ALLOC_RC_INVALID_CONFIG
} m_alloc_rc_t;

typedef enum {
    M_ALLOC_TYPE_SYSTEM,
    M_ALLOC_TYPE_FIXED,
    M_ALLOC_TYPE_ARENA,
    M_ALLOC_TYPE_SLAB
} m_alloc_type_t;

/** @brief MLibs opaque allocator context ID */
typedef void* m_context_id_t;

/** @brief MLibs fixed allocator configuration */
typedef struct {
    size_t minimum_size;
} m_alloc_fixed_config_t;

/** @brief MLibs arena allocator configuration */
typedef struct {
    size_t minimum_size_per_arena;
} m_alloc_arena_config_t;

/** @brief MLibs slab allocator configuration */
typedef struct {
    size_t minimum_no_cells_per_arena;
    size_t *cell_sizes;
} m_alloc_slab_config_t;

/** @brief MLibs allocator configuration */
typedef struct {
    m_alloc_type_t type;
    union {
        m_alloc_fixed_config_t fixed;
        m_alloc_arena_config_t arena;
        m_alloc_slab_config_t slab;
    } u;
} m_alloc_config_t;

/** @brief MLibs opaque allocator instance */
typedef struct m_alloc_instance_t m_alloc_instance_t;

/** @brief MLibs allocator instance creation result */
typedef struct {
    m_alloc_rc_t return_code;
    m_alloc_instance_t *allocator;
} m_alloc_creation_result_t;

/** @brief MLibs allocator allocation result */
typedef struct {
    m_alloc_rc_t return_code;
    void *pointer;
} m_alloc_alloc_result_t;

/** @brief MLibs allocator sized allocation result */
typedef struct {
    m_alloc_rc_t return_code;
    m_com_sized_data_t *data;
} m_alloc_sized_alloc_result_t;

extern m_alloc_config_t m_alloc_get_configuration(m_alloc_instance_t *allocator);

extern m_alloc_creation_result_t m_alloc_create(m_alloc_config_t config);
extern m_alloc_rc_t m_alloc_destroy(m_alloc_instance_t **allocator);

extern m_alloc_alloc_result_t m_alloc_malloc(m_alloc_instance_t *allocator, size_t size);
extern m_alloc_alloc_result_t m_alloc_calloc(m_alloc_instance_t *allocator, uint32_t number, size_t size);
extern m_alloc_alloc_result_t m_alloc_realloc(m_alloc_instance_t *allocator, void *data, size_t size);
extern m_alloc_rc_t m_alloc_free(m_alloc_instance_t *allocator, void *data);

extern m_alloc_sized_alloc_result_t m_alloc_sized_malloc(m_alloc_instance_t *allocator, size_t size);
extern m_alloc_sized_alloc_result_t m_alloc_sized_calloc(m_alloc_instance_t *allocator, uint32_t number, size_t size);
extern m_alloc_sized_alloc_result_t m_alloc_sized_realloc(m_alloc_instance_t *allocator, m_com_sized_data_t *data, size_t size);
extern m_alloc_rc_t m_alloc_sized_free(m_alloc_instance_t *allocator, m_com_sized_data_t *data);

#endif

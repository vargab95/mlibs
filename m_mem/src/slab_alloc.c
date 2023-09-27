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

typedef struct slab_arena {
    size_t no_allocation_segments;
    uint64_t *allocation_segments;

    struct slab_arena *next;
    void *buffer;
} slab_arena;

typedef struct {
    size_t allocation_size;
    slab_arena *arena;
} slab;

typedef struct {
    uint16_t number_of_cell_types;
    size_t *cell_sizes;

    slab slabs[];
} slab_allocator_context;

static m_alloc_context_creation_result_t create_context(void *config);
static int size_compare_function (const void * a, const void * b);
static m_alloc_rc_t destroy_context(m_context_id_t context);
static void destroy_arena(struct slab_arena *arena);
static slab *find_slab(slab_allocator_context *context, size_t size);

static m_alloc_alloc_result_t malloc_impl(m_context_id_t context, size_t size);
static m_alloc_alloc_result_t calloc_impl(m_context_id_t context, uint32_t number, size_t size);
static m_alloc_alloc_result_t realloc_impl(m_context_id_t context, void *data, size_t size);
static m_alloc_rc_t free_impl(m_context_id_t context, void *data);

static m_alloc_sized_alloc_result_t sized_malloc_impl(m_context_id_t context, size_t size);
static m_alloc_sized_alloc_result_t sized_calloc_impl(m_context_id_t context, uint32_t number, size_t size);
static m_alloc_sized_alloc_result_t sized_realloc_impl(m_context_id_t context, m_com_sized_data_t *data, size_t size);
static m_alloc_rc_t sized_free_impl(m_context_id_t context, m_com_sized_data_t *data);

m_alloc_functions_t m_slab_allocator = {
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

m_alloc_creation_result_t m_slab_allocator_create(m_alloc_slab_config_t configuration)
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
        .type = M_ALLOC_TYPE_SLAB,
        .u = {
            .slab = configuration
        }
    };
    allocator->functions = &m_slab_allocator;

    return (m_alloc_creation_result_t) {
        .return_code = M_ALLOC_RC_OK,
        .allocator = allocator
    };
}

static m_alloc_context_creation_result_t create_context(void *config)
{
    m_alloc_slab_config_t *_config = config;
    slab_allocator_context *context;
    uint16_t no_cell_sizes;
    void *ptr;

    for (no_cell_sizes = 0; _config->cell_sizes[no_cell_sizes]; no_cell_sizes++);

    context = malloc(sizeof(slab_allocator_context) + no_cell_sizes * (sizeof(slab) + sizeof(size_t)));
    if (!context)
    {
        return (m_alloc_context_creation_result_t) {
            .return_code = M_ALLOC_RC_NO_MEMORY,
            .context = NULL
        };
    }

    ptr = context;
    ptr += sizeof(slab_allocator_context);
    ptr += no_cell_sizes * sizeof(slab);
    context->cell_sizes = ptr;
    context->number_of_cell_types = no_cell_sizes;

    memcpy(context->cell_sizes, _config->cell_sizes, no_cell_sizes * sizeof(size_t));
    memset(context->slabs, 0, no_cell_sizes * sizeof(slab));

    qsort(context->cell_sizes, context->number_of_cell_types, sizeof(size_t), size_compare_function);

    for (int i = 0; i < context->number_of_cell_types; i++)
    {
        context->slabs[i].allocation_size = context->cell_sizes[i];

        size_t allocation_size = _config->minimum_no_cells_per_arena * context->cell_sizes[i]
                                 + _config->minimum_no_cells_per_arena * 64
                                 + sizeof(slab_arena);
        const int page_size = getpagesize();
        if (allocation_size % page_size)
        {
            allocation_size = ((allocation_size / page_size) + 1) * page_size;
        }

        slab_arena *arena = context->slabs[i].arena = calloc(1, allocation_size);
        if (!arena)
        {
            return (m_alloc_context_creation_result_t) {
                .return_code = M_ALLOC_RC_NO_MEMORY,
                .context = NULL
            };
        }

        arena->next = NULL;
        arena->no_allocation_segments = _config->minimum_no_cells_per_arena;
        arena->allocation_segments = (void*)arena + sizeof(slab_arena);
        arena->buffer = (void*)arena->allocation_segments + _config->minimum_no_cells_per_arena * 64;
    }

    return (m_alloc_context_creation_result_t) {
        .return_code = M_ALLOC_RC_OK,
        .context = context
    };
}

static int size_compare_function (const void * a, const void * b)
{
    return (*(size_t*)a - *(size_t*)b);
}

static m_alloc_rc_t destroy_context(m_context_id_t context)
{
    slab_allocator_context *_context = context;
    for (int i = 0; i < _context->number_of_cell_types; i++)
    {
        destroy_arena(_context->slabs[i].arena);
    }

    free(_context);

    return M_ALLOC_RC_OK;
}

static void destroy_arena(struct slab_arena *arena)
{
    if (!arena)
    {
        return;
    }

    if (arena->next)
    {
        destroy_arena(arena->next);
    }

    free(arena);
}

static m_alloc_alloc_result_t malloc_impl(m_context_id_t context, size_t size)
{
    slab_allocator_context *_context = context;
    slab *slab = find_slab(_context, size);

    if (!slab)
    {
        // TODO handle big objects
        return (m_alloc_alloc_result_t) {
            .return_code = M_ALLOC_RC_NO_MEMORY,
            .pointer = NULL
        };
    }

    slab_arena *free_arena;
    int64_t free_cell_id = -1;
    uint64_t *allocation_segment;

    for (free_arena = slab->arena; free_cell_id < 0 && free_arena; free_arena = free_arena->next)
    {
        free_cell_id = -1;
        allocation_segment = free_arena->allocation_segments;

        for (int i = 0; i < free_arena->no_allocation_segments; i++, allocation_segment++)
        {
            if (*allocation_segment == 0xFFFFFFFFFFFFFFFF)
            {
                free_cell_id += 64;
                continue;
            }

            for (int j = 0; j <= 64; j++)
            {
                free_cell_id++;
                if ((*allocation_segment & (1 << j)) == 0)
                {
                    goto CELL_FOUND;
                }
            }
        }
    }

CELL_FOUND:
    if (free_cell_id >= 0)
    {
        *allocation_segment |= 1 << (free_cell_id % 64);
    }

    return (m_alloc_alloc_result_t) {
        .return_code = M_ALLOC_RC_OK,
        .pointer = free_arena->buffer + (free_cell_id * slab->allocation_size)
    };
}

static m_alloc_alloc_result_t calloc_impl(m_context_id_t context, uint32_t number, size_t size)
{
    m_alloc_alloc_result_t result = malloc_impl(context, number * size);

    memset(result.pointer, 0, number * size);

    return result;
}

static m_alloc_alloc_result_t realloc_impl(m_context_id_t context, void *data, size_t size)
{
    m_alloc_alloc_result_t result = malloc_impl(context, size);

    memcpy(result.pointer, data, size);
    free_impl(context, data);

    return result;
}

static m_alloc_rc_t free_impl(m_context_id_t context, void *data)
{
    return M_ALLOC_RC_OK;
}

static m_alloc_sized_alloc_result_t sized_malloc_impl(m_context_id_t context, size_t size)
{
    m_alloc_alloc_result_t alloc_result = malloc_impl(context, size + sizeof(m_com_sized_data_t));
    m_alloc_sized_alloc_result_t result = {
        .return_code = alloc_result.return_code,
        .data = alloc_result.pointer
    };

    if (result.return_code == M_ALLOC_RC_OK)
    {
        result.data->size = size;
        result.data->data = (void*)result.data + sizeof(m_com_sized_data_t);
    }

    return result;
}

static m_alloc_sized_alloc_result_t sized_calloc_impl(m_context_id_t context, uint32_t number, size_t size)
{
    m_alloc_sized_alloc_result_t result = sized_malloc_impl(context, number * size);

    if (result.return_code == M_ALLOC_RC_OK)
    {
        memset(result.data->data, 0, result.data->size);
    }

    return result;
}

static m_alloc_sized_alloc_result_t sized_realloc_impl(m_context_id_t context, m_com_sized_data_t *data, size_t size)
{
    m_alloc_sized_alloc_result_t result = sized_malloc_impl(context, size);

    if (result.return_code == M_ALLOC_RC_OK)
    {
        m_mem_copy(data, result.data);
        sized_free_impl(context, data);
    }

    return result;
}

static m_alloc_rc_t sized_free_impl(m_context_id_t context, m_com_sized_data_t *data)
{
    return M_ALLOC_RC_OK;
}

static slab *find_slab(slab_allocator_context *context, size_t size)
{
    for (int i = 0; i < context->number_of_cell_types; i++)
    {
        if (context->slabs[i].allocation_size >= size)
        {
            return &context->slabs[i];
        }
    }

    return NULL;
}

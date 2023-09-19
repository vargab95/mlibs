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

static m_context_id_t create_context(m_allocator_config_t config);
static int size_compare_function (const void * a, const void * b);
static void destroy_context(m_context_id_t context);
static void destroy_arena(struct slab_arena *arena);
static slab *find_slab(slab_allocator_context *context, size_t size);

static void *malloc_impl(m_context_id_t context, size_t size);
static void *calloc_impl(m_context_id_t context, uint32_t number, size_t size);
static void *realloc_impl(m_context_id_t context, void *data, size_t size);
static void free_impl(m_context_id_t context, void *data);

static m_com_sized_data_t *sized_malloc_impl(m_context_id_t context, size_t size);
static m_com_sized_data_t *sized_calloc_impl(m_context_id_t context, uint32_t number, size_t size);
static m_com_sized_data_t *sized_realloc_impl(m_context_id_t context, m_com_sized_data_t *data, size_t size);
static void sized_free_impl(m_context_id_t context, m_com_sized_data_t *data);

m_allocator_t m_slab_allocator = {
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
    slab_allocator_context *context;
    uint16_t no_cell_sizes;
    void *ptr;

    for (no_cell_sizes = 0; config.slab.cell_sizes[no_cell_sizes]; no_cell_sizes++);

    context = malloc(sizeof(slab_allocator_context) + no_cell_sizes * (sizeof(slab) + sizeof(size_t)));
    if (!context)
    {
        return NULL;
    }

    ptr = context;
    ptr += sizeof(slab_allocator_context);
    ptr += no_cell_sizes * sizeof(slab);
    context->cell_sizes = ptr;
    context->number_of_cell_types = no_cell_sizes;

    memcpy(context->cell_sizes, config.slab.cell_sizes, no_cell_sizes * sizeof(size_t));
    memset(context->slabs, 0, no_cell_sizes * sizeof(slab));

    qsort(context->cell_sizes, context->number_of_cell_types, sizeof(size_t), size_compare_function);

    for (int i = 0; i < context->number_of_cell_types; i++)
    {
        context->slabs[i].allocation_size = context->cell_sizes[i];

        size_t allocation_size = config.slab.minimum_no_cells_per_arena * context->cell_sizes[i]
                                 + config.slab.minimum_no_cells_per_arena * 64
                                 + sizeof(slab_arena);
        const int page_size = getpagesize();
        if (allocation_size % page_size)
        {
            allocation_size = ((allocation_size / page_size) + 1) * page_size;
        }

        slab_arena *arena = context->slabs[i].arena = malloc(allocation_size);
        if (!arena)
        {
            // TODO memleak
            return NULL;
        }

        arena->next = NULL;
        arena->no_allocation_segments = config.slab.minimum_no_cells_per_arena;
        arena->allocation_segments = (void*)arena + sizeof(slab_arena);
        arena->buffer = (void*)arena->allocation_segments + config.slab.minimum_no_cells_per_arena * 64;
    }

    return context;
}

static int size_compare_function (const void * a, const void * b)
{
    return (*(size_t*)a - *(size_t*)b);
}

static void destroy_context(m_context_id_t context)
{
    slab_allocator_context *_context = context;
    for (int i = 0; i < _context->number_of_cell_types; i++)
    {
        destroy_arena(_context->slabs[i].arena);
    }

    free(_context);
}

static void destroy_arena(struct slab_arena *arena)
{
    if (arena->next)
    {
        destroy_arena(arena->next);
    }

    free(arena);
}

static void *malloc_impl(m_context_id_t context, size_t size)
{
    slab_allocator_context *_context = context;
    slab *slab = find_slab(_context, size);

    if (!slab)
    {
        // TODO handle big objects
        return NULL;
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
                if (*allocation_segment & (1 << j))
                {
                    free_cell_id++;
                }
                else
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

    return free_arena->buffer + free_cell_id;
}

static void *calloc_impl(m_context_id_t context, uint32_t number, size_t size)
{
    m_com_sized_data_t *result = sized_malloc_impl(context, number * size);

    memset(result, 0, number * size);

    return result;
}

static void *realloc_impl(m_context_id_t context, void *data, size_t size)
{
    m_com_sized_data_t *result = sized_malloc_impl(context, size);

    memcpy(result, data, size);
    free_impl(context, data);

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
    m_com_sized_data_t *result = sized_malloc_impl(context, number * size);

    if (result)
    {
        memset(result->data, 0, result->size);
    }

    return result;
}

static m_com_sized_data_t *sized_realloc_impl(m_context_id_t context, m_com_sized_data_t *data, size_t size)
{
    m_com_sized_data_t *result = sized_malloc_impl(context, size);

    if (result)
    {
        m_mem_copy(data, result);
        sized_free_impl(context, data);
    }

    return result;
}

static void sized_free_impl(m_context_id_t context, m_com_sized_data_t *data)
{
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

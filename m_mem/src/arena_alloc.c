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

struct arena {
    struct arena *next;
    void *ptr;
    void *buffer;
};

static m_alloc_context_creation_result_t create_context(void *config);
static m_alloc_rc_t destroy_context(m_context_id_t context);
static void destroy_arena(struct arena *arena);

static m_alloc_alloc_result_t malloc_impl(m_context_id_t context, size_t size);
static m_alloc_alloc_result_t calloc_impl(m_context_id_t context, uint32_t number, size_t size);
static m_alloc_alloc_result_t realloc_impl(m_context_id_t context, void *data, size_t size);
static m_alloc_rc_t free_impl(m_context_id_t context, void *data);

static m_alloc_sized_alloc_result_t sized_malloc_impl(m_context_id_t context, size_t size);
static m_alloc_sized_alloc_result_t sized_calloc_impl(m_context_id_t context, uint32_t number, size_t size);
static m_alloc_sized_alloc_result_t sized_realloc_impl(m_context_id_t context, m_com_sized_data_t *data, size_t size);
static m_alloc_rc_t sized_free_impl(m_context_id_t context, m_com_sized_data_t *data);

m_alloc_functions_t allocator_functions = {
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
    size_t page_size;
    size_t allocation_size;
    struct arena *arena_in_use;
    struct arena arenas;
} arena_allocator_context;

m_alloc_creation_result_t m_arena_allocator_create(m_alloc_arena_config_t configuration)
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
        .type = M_ALLOC_TYPE_ARENA,
        .u = {
            .arena = configuration
        }
    };
    allocator->functions = &allocator_functions;

    return (m_alloc_creation_result_t) {
        .return_code = M_ALLOC_RC_OK,
        .allocator = allocator
    };
}

static m_alloc_context_creation_result_t create_context(void *config)
{
    size_t allocation_size = ((m_alloc_arena_config_t*)config)->minimum_size_per_arena + sizeof(arena_allocator_context);
    const int page_size = getpagesize();
    if (allocation_size % page_size)
    {
        allocation_size = ((allocation_size / page_size) + 1) * page_size;
    }

    arena_allocator_context *context = aligned_alloc(page_size, allocation_size);
    if (context == NULL)
    {
        return (m_alloc_context_creation_result_t) {
            .return_code = M_ALLOC_RC_NO_MEMORY,
            .context = NULL
        };
    }

    context->arenas.buffer = (void*)context + sizeof(arena_allocator_context);
    context->arenas.ptr = (void*)context + allocation_size;
    context->arenas.next = NULL;
    context->arena_in_use = &context->arenas;
    context->page_size = page_size;
    context->allocation_size = allocation_size;

    return (m_alloc_context_creation_result_t) {
        .return_code = M_ALLOC_RC_OK,
        .context = context
    };
}

static m_alloc_rc_t destroy_context(m_context_id_t context)
{
    arena_allocator_context *_context = context;

    if (_context->arenas.next)
    {
        destroy_arena(_context->arenas.next);
    }

    free(_context);

    return M_ALLOC_RC_OK;
}

static void destroy_arena(struct arena *arena)
{
    if (arena->next)
    {
        destroy_arena(arena->next);
    }

    free(arena);
}

static m_alloc_alloc_result_t malloc_impl(m_context_id_t context, size_t size)
{
    arena_allocator_context *_context = context;

    if (size > _context->allocation_size)
    {
        struct arena *new_arena = malloc(size + sizeof(struct arena));
        if (new_arena == NULL)
        {
            return (m_alloc_alloc_result_t) {
                .return_code = M_ALLOC_RC_NO_MEMORY,
                .pointer = NULL
            };
        }

        new_arena->buffer = (void*)new_arena + sizeof(struct arena);
        new_arena->ptr = new_arena->buffer;
        new_arena->next = _context->arenas.next;
        _context->arenas.next = new_arena;

        return (m_alloc_alloc_result_t) {
            .return_code = M_ALLOC_RC_OK,
            .pointer = new_arena->buffer
        };
    }

    void *result_ptr = _context->arena_in_use->ptr - size;
    if (result_ptr < _context->arena_in_use->buffer)
    {
        struct arena *new_arena = aligned_alloc(_context->page_size, _context->allocation_size);
        if (new_arena == NULL)
        {
            return (m_alloc_alloc_result_t) {
                .return_code = M_ALLOC_RC_NO_MEMORY,
                .pointer = NULL
            };
        }

        new_arena->buffer = (void*)new_arena + sizeof(struct arena);
        new_arena->ptr = (void*)new_arena + _context->allocation_size;
        new_arena->next = _context->arenas.next;

        _context->arenas.next = new_arena;
        _context->arena_in_use = new_arena;

        result_ptr = _context->arena_in_use->ptr - size;
    }

    _context->arena_in_use->ptr = result_ptr;

    return (m_alloc_alloc_result_t) {
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
    const size_t whole_size = size * number;
    m_alloc_sized_alloc_result_t result = sized_malloc_impl(context, whole_size);

    if (result.return_code == M_ALLOC_RC_OK)
    {
        memset(result.data->data, 0, whole_size);
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

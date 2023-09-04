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

struct arena {
    struct arena *next;
    void *ptr;
    void *buffer;
};

static m_context_id_t create_context(m_allocator_config_t config);
static void destroy_context(m_context_id_t id);
static void destroy_arena(struct arena *arena);
static m_com_sized_data_t *malloc_impl(m_context_id_t id, size_t size);
static m_com_sized_data_t *calloc_impl(m_context_id_t id, uint32_t number, size_t size);
static m_com_sized_data_t *realloc_impl(m_context_id_t id, m_com_sized_data_t *data, size_t size);
static void free_impl(m_context_id_t id, m_com_sized_data_t *data);

m_allocator_t m_arena_allocator = {
    .create = create_context,
    .destroy = destroy_context,
    .malloc = malloc_impl,
    .calloc = calloc_impl,
    .realloc = realloc_impl,
    .free = free_impl
};

typedef struct {
    size_t allocation_size;
    struct arena *arena_in_use;
    struct arena arenas;
} arena_allocator_context;

static m_context_id_t create_context(m_allocator_config_t config)
{
    size_t allocation_size = config.arena.minimum_size_per_arena + sizeof(arena_allocator_context);
    const int page_size = getpagesize();
    if (allocation_size % page_size)
    {
        allocation_size = ((allocation_size / page_size) + 1) * page_size;
    }

    arena_allocator_context *context = malloc(allocation_size);
    if (context == NULL)
    {
        return NULL;
    }

    context->arenas.buffer = context + sizeof(arena_allocator_context);
    context->arenas.ptr = context + allocation_size;
    context->arenas.next = NULL;
    context->arena_in_use = &context->arenas;
    context->allocation_size = allocation_size;

    return context;
}

static void destroy_context(m_context_id_t context)
{
    arena_allocator_context *_context = context;

    if (_context->arenas.next)
    {
        destroy_arena(_context->arenas.next);
    }

    free(_context);
}

static void destroy_arena(struct arena *arena)
{
    if (arena->next)
    {
        destroy_arena(arena->next);
    }

    free(arena);
}

static m_com_sized_data_t *malloc_impl(m_context_id_t context, size_t size)
{
    arena_allocator_context *_context = context;

    if (size > _context->allocation_size)
    {
        struct arena *new_arena = malloc(_context->allocation_size);
        if (new_arena == NULL)
        {
            return NULL;
        }

        new_arena->buffer = new_arena + sizeof(struct arena);
        new_arena->ptr = new_arena->buffer;
        new_arena->next = _context->arenas.next;

        m_com_sized_data_t *result = new_arena->buffer;
        result->size = _context->allocation_size - sizeof(struct arena) - sizeof(m_com_sized_data_t);
        result->data = result + sizeof(m_com_sized_data_t);

        return result;
    }

    void *result_ptr = _context->arena_in_use->ptr - size - sizeof(m_com_sized_data_t);
    m_com_sized_data_t *result;

    if (result_ptr < _context->arena_in_use->buffer)
    {
        struct arena *new_arena = malloc(_context->allocation_size);
        if (new_arena == NULL)
        {
            return NULL;
        }

        new_arena->buffer = new_arena + sizeof(struct arena);
        new_arena->ptr = new_arena + _context->allocation_size;
        new_arena->next = _context->arenas.next;
        _context->arena_in_use = new_arena;
        result_ptr = _context->arena_in_use->ptr - size - sizeof(m_com_sized_data_t);
    }

    result = result_ptr;
    result->size = size;
    result->data = result_ptr + sizeof(result->size);

    _context->arena_in_use->ptr = result_ptr;

    return result;
}

static m_com_sized_data_t *calloc_impl(m_context_id_t context, uint32_t number, size_t size)
{
    const size_t whole_size = size * number;
    m_com_sized_data_t *result = malloc_impl(context, whole_size);

    if (result)
    {
        memset(result->data, 0, whole_size);
    }

    return result;
}

static m_com_sized_data_t *realloc_impl(m_context_id_t context, m_com_sized_data_t *data, size_t size)
{
    m_com_sized_data_t *result = malloc_impl(context, size);

    if (result)
    {
        m_mem_copy(data, result);
    }

    return result;
}

static void free_impl(m_context_id_t id, m_com_sized_data_t *data)
{
}

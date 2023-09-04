#include "m_common.h"
#include "gtest/gtest.h"
#include <unistd.h>

extern "C"
{
#include "../api/m_alloc.h"
}

TEST(m_arena_allocator_tests, single_allocation)
{
    const size_t page_size = getpagesize();
    m_context_id_t context = m_arena_allocator.create({ .arena = { .minimum_size_per_arena = page_size } });

    m_com_sized_data_t *data = m_arena_allocator.malloc(context, page_size / 2);

    EXPECT_EQ(data->size, page_size / 2);
    m_arena_allocator.destroy(context);
}

TEST(m_arena_allocator_tests, round_up_to_page_size)
{
    const size_t page_size = getpagesize();
    m_context_id_t context = m_arena_allocator.create({ .arena = { .minimum_size_per_arena = page_size / 4 } });

    m_com_sized_data_t *data = m_arena_allocator.malloc(context, page_size / 2);

    EXPECT_EQ(data->size, page_size / 2);
    m_arena_allocator.destroy(context);
}

TEST(m_arena_allocator_tests, write_to_end)
{
    const size_t page_size = getpagesize();
    m_context_id_t context = m_arena_allocator.create({ .arena = { .minimum_size_per_arena = page_size } });

    m_com_sized_data_t *data = m_arena_allocator.malloc(context, sizeof(int));
    *(int*)data->data = 0xFEDCBA98;

    EXPECT_EQ(*(int*)data->data, 0xFEDCBA98);
    EXPECT_EQ(data->size, sizeof(int));
    m_arena_allocator.destroy(context);
}

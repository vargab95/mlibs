#include "m_common.h"
#include "gtest/gtest.h"
#include <unistd.h>

extern "C"
{
#include "../api/m_alloc.h"
}

TEST(m_slab_allocator_tests, single_allocation)
{
    const size_t page_size = getpagesize();
    size_t cell_sizes[] = { 16, 256, 512, 1024, 4096, 0 };
    m_context_id_t context = m_slab_allocator.create({
        .slab = {
            .minimum_no_cells_per_arena = 1024,
            .cell_sizes = cell_sizes,
        }
    });

    m_com_sized_data_t *data = m_slab_allocator.sized_malloc(context, page_size / 2);

    EXPECT_EQ(data->size, page_size / 2);
    m_slab_allocator.destroy(context);
}

TEST(m_slab_allocator_tests, round_up_to_page_size)
{
    const size_t page_size = getpagesize();
    size_t cell_sizes[] = { 16, 256, 512, 1024, 4096, 0 };
    m_context_id_t context = m_slab_allocator.create({
        .slab = {
            .minimum_no_cells_per_arena = 1024,
            .cell_sizes = cell_sizes,
        }
    });

    m_com_sized_data_t *data = m_slab_allocator.sized_malloc(context, page_size / 2);

    EXPECT_EQ(data->size, page_size / 2);
    m_slab_allocator.destroy(context);
}

TEST(m_slab_allocator_tests, write_to_end)
{
    const size_t page_size = getpagesize();
    size_t cell_sizes[] = { 16, 256, 512, 1024, 4096, 0 };
    m_context_id_t context = m_slab_allocator.create({
        .slab = {
            .minimum_no_cells_per_arena = 1024,
            .cell_sizes = cell_sizes,
        }
    });

    m_com_sized_data_t *data = m_slab_allocator.sized_malloc(context, sizeof(int));
    *(int*)data->data = 0xFEDCBA98;

    EXPECT_EQ(*(int*)data->data, 0xFEDCBA98);
    EXPECT_EQ(data->size, sizeof(int));
    m_slab_allocator.destroy(context);
}

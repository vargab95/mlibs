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
    m_alloc_creation_result_t creation_result = m_alloc_create({
        .type = M_ALLOC_TYPE_SLAB,
        .u = {
            .slab = {
                .minimum_no_cells_per_arena = 1024,
                .cell_sizes = cell_sizes,
            }
        }
    });

    m_alloc_sized_alloc_result_t result = m_alloc_sized_malloc(creation_result.allocator, page_size / 2);

    EXPECT_EQ(result.data->size, page_size / 2);
    m_alloc_destroy(&creation_result.allocator);
}

TEST(m_slab_allocator_tests, round_up_to_page_size)
{
    const size_t page_size = getpagesize();
    size_t cell_sizes[] = { 16, 256, 512, 1024, 4096, 0 };
    m_alloc_creation_result_t creation_result = m_alloc_create({
        .type = M_ALLOC_TYPE_SLAB,
        .u = {
            .slab = {
                .minimum_no_cells_per_arena = 1024,
                .cell_sizes = cell_sizes,
            }
        }
    });

    m_alloc_sized_alloc_result_t result = m_alloc_sized_malloc(creation_result.allocator, page_size / 2);

    EXPECT_EQ(result.data->size, page_size / 2);
    m_alloc_destroy(&creation_result.allocator);
}

TEST(m_slab_allocator_tests, write_to_end)
{
    const size_t page_size = getpagesize();
    size_t cell_sizes[] = { 16, 256, 512, 1024, 4096, 0 };
    m_alloc_creation_result_t creation_result = m_alloc_create({
        .type = M_ALLOC_TYPE_SLAB,
        .u = {
            .slab = {
                .minimum_no_cells_per_arena = 1024,
                .cell_sizes = cell_sizes,
            }
        }
    });

    m_alloc_sized_alloc_result_t result = m_alloc_sized_malloc(creation_result.allocator, sizeof(int));
    // *(int*)result.data->data = 0xFEDCBA98;

    // EXPECT_EQ(*(int*)result.data->data, 0xFEDCBA98);
    // EXPECT_EQ(result.data->size, sizeof(int));
    m_alloc_destroy(&creation_result.allocator);
}

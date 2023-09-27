#include "gtest/gtest.h"
#include <unistd.h>

extern "C"
{
#include "../api/m_alloc.h"
}

TEST(m_fixed_allocator_tests, single_allocation)
{
    const size_t page_size = getpagesize();
    m_alloc_creation_result_t allocator_result = m_alloc_create({
        .type = M_ALLOC_TYPE_FIXED,
        .u = { .fixed = { .minimum_size = page_size } }
    });

    m_alloc_sized_alloc_result_t result = m_alloc_sized_malloc(allocator_result.allocator, page_size / 2);

    EXPECT_EQ(result.return_code, M_ALLOC_RC_OK);
    EXPECT_EQ(result.data->size, page_size / 2);
    m_alloc_destroy(&allocator_result.allocator);
}

TEST(m_fixed_allocator_tests, exhaust_context)
{
    const size_t page_size = getpagesize();
    m_alloc_creation_result_t allocator_result = m_alloc_create({
        .type = M_ALLOC_TYPE_FIXED,
        .u = { .fixed = { .minimum_size = page_size / 2 } }
    });

    m_alloc_sized_alloc_result_t result = m_alloc_sized_malloc(allocator_result.allocator, page_size + 1);

    EXPECT_EQ(result.return_code, M_ALLOC_RC_SIZE_LIMIT);
    EXPECT_EQ(result.data, nullptr);
    m_alloc_destroy(&allocator_result.allocator);
}

TEST(m_fixed_allocator_tests, round_up_to_page_size)
{
    const size_t page_size = getpagesize();
    m_alloc_creation_result_t allocator_result = m_alloc_create({
        .type = M_ALLOC_TYPE_FIXED,
        .u = { .fixed = { .minimum_size = page_size / 4 } }
    });

    m_alloc_sized_alloc_result_t result = m_alloc_sized_malloc(allocator_result.allocator, page_size / 2);

    EXPECT_EQ(result.return_code, M_ALLOC_RC_OK);
    EXPECT_EQ(result.data->size, page_size / 2);
    m_alloc_destroy(&allocator_result.allocator);
}

TEST(m_fixed_allocator_tests, write_to_end)
{
    const size_t page_size = getpagesize();
    m_alloc_creation_result_t allocator_result = m_alloc_create({
        .type = M_ALLOC_TYPE_FIXED,
        .u = { .fixed = { .minimum_size = page_size } }
    });

    m_alloc_sized_alloc_result_t result = m_alloc_sized_malloc(allocator_result.allocator, sizeof(int));
    *(int*)result.data->data = 4;

    EXPECT_EQ(result.return_code, M_ALLOC_RC_OK);
    EXPECT_EQ(*(int*)result.data->data, 4);
    EXPECT_EQ(result.data->size, sizeof(int));
    m_alloc_destroy(&allocator_result.allocator);
}

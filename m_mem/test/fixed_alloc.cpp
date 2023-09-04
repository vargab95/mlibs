#include "m_common.h"
#include "gtest/gtest.h"
#include <unistd.h>

extern "C"
{
#include "../api/m_alloc.h"
}

TEST(m_fixed_allocator_tests, single_allocation)
{
    const size_t page_size = getpagesize();
    m_context_id_t context = m_fixed_allocator.create({ .fixed = { .minimum_size = page_size } });

    m_com_sized_data_t *data = m_fixed_allocator.malloc(context, page_size / 2);

    EXPECT_EQ(data->size, page_size / 2);
    m_fixed_allocator.destroy(context);
}

TEST(m_fixed_allocator_tests, exhaust_context)
{
    const size_t page_size = getpagesize();
    m_context_id_t context = m_fixed_allocator.create({ .fixed = { .minimum_size = page_size / 2 } });

    m_com_sized_data_t *data = m_fixed_allocator.malloc(context, page_size + 1);

    EXPECT_EQ(data, nullptr);
    m_fixed_allocator.destroy(context);
}

TEST(m_fixed_allocator_tests, round_up_to_page_size)
{
    const size_t page_size = getpagesize();
    m_context_id_t context = m_fixed_allocator.create({ .fixed = { .minimum_size = page_size / 4 } });

    m_com_sized_data_t *data = m_fixed_allocator.malloc(context, page_size / 2);

    EXPECT_EQ(data->size, page_size / 2);
    m_fixed_allocator.destroy(context);
}

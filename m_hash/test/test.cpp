#include <string.h>
#include "gtest/gtest.h"

extern "C"
{
#include "../api/m_hash.h"
}

TEST(m_hash_tests, calculate_djb2)
{
    m_com_sized_data_t value;
    const char *string = "test_string";

    value.data = (void*)string;
    value.size = strlen(string);

    const uint32_t result = m_hash_djb2(&value);
    EXPECT_EQ(result, 0xf8e39b9b);
}

TEST(m_hash_tests, calculate_fnv1a32)
{
    m_com_sized_data_t value;
    const char *string = "test_string";

    value.data = (void*)string;
    value.size = strlen(string);

    const uint32_t result = m_hash_fnv1a32(&value);
    EXPECT_EQ(result, 0x8162e7ff);
}

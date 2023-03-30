#include <stdio.h>
#include <string.h>

#include "../api/m_str.h"

#if !defined(COMPOSITE_BUILD)
#include <m_libs/m_mem.h>
#else
#include "../../m_mem/api/m_mem.h"
#endif

int test_normal_split();
int test_normal_join();

int main(void)
{
    test_normal_split();
    test_normal_join();
}

int test_normal_split()
{
    m_com_sized_data_t input, separator;
    m_str_split_result_t *result;

    m_str_convert_to_com("test with this sentence", &input);
    m_str_convert_to_com(" ", &separator);

    result = m_str_split(&input, &separator);
    if (result->token_count != 4)
    {
        m_str_split_result_destroy(&result);
        return 0;
    }

    m_str_split_result_destroy(&result);
    return 1;
}

int test_normal_join()
{
    m_com_sized_data_t **input, separator, *result;

    input = m_mem_calloc(sizeof(m_com_sized_data_t *), 4);
    for (int i = 0; i < 4; i++)
    {
        input[i] = m_mem_malloc(sizeof(m_com_sized_data_t));
    }

    m_str_convert_to_com("test", input[0]);
    m_str_convert_to_com("with", input[1]);
    m_str_convert_to_com("this", input[2]);
    m_str_convert_to_com("sentence", input[3]);
    m_str_convert_to_com(" ", &separator);

    result = m_str_join(input, 4, &separator);

    return strcmp(result->data, "test with this sentence") == 0;
}

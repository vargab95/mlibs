#include <m_libs/m_mem.h>
#include <string.h>

#include "../api/m_str.h"

void m_str_convert_to_com(const char *string, m_com_sized_data_t *output)
{
    output->data = (void *)string;
    output->size = strlen(string);
}

m_str_split_result_t *m_str_split(const m_com_sized_data_t *input, const m_com_sized_data_t *separator)
{
    char *tmp;
    m_str_split_result_t *result = m_mem_malloc(sizeof(m_str_split_result_t));

    result->string = strdup(input->data);
    result->token_count = 0;

    tmp = result->string;
    do
    {
        result->token_count++;
        tmp += separator->size;
    } while ((tmp = strstr(tmp, separator->data)));

    result->tokens = m_mem_calloc(sizeof(char *), result->token_count);

    tmp = result->string;
    for (int i = 0; tmp && i < result->token_count; i++)
    {
        result->tokens[i] = tmp;
        if ((tmp = strstr(tmp, separator->data)))
        {
            *tmp = '\0';
            tmp += separator->size;
        }
    }

    return result;
}

void m_str_split_result_destroy(m_str_split_result_t **result)
{
    free((*result)->string);
    free(*result);
    *result = NULL;
}

m_com_sized_data_t *m_str_join(m_com_sized_data_t **strings, unsigned int no_strings, m_com_sized_data_t *separator)
{
    uint64_t total_length = 0;
    m_com_sized_data_t *result;
    char *output_pointer;

    for (unsigned int i = 0; i < no_strings; i++)
    {
        total_length += strings[i]->size + separator->size;
    }

    result = m_mem_sized_malloc(total_length);
    result->size -= separator->size;

    output_pointer = result->data;
    for (unsigned int i = 0; i < no_strings; i++)
    {
        strncpy(output_pointer, strings[i]->data, strings[i]->size);
        output_pointer += strings[i]->size;

        if (i != no_strings)
        {
            strncpy(output_pointer, separator->data, separator->size);
            output_pointer += separator->size;
        }
    }

    ((char *)result->data)[result->size] = '\0';

    return result;
}

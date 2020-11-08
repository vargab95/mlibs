#include <stdlib.h>

#include "config.h"
#include "m_hash.h"

extern uint32_t m_hash_djb(const m_com_sized_data_t *input)
{
    const uint8_t *data = input->data;
    uint32_t hash = 0x1505u;
    for (int i = 0; i < input->size; i++)
    {
        hash = 33u * hash ^ data[i];
    }
    return hash;
}

extern uint32_t m_hash_fnv1a(const m_com_sized_data_t *input)
{
    const uint8_t *data = input->data;
    uint32_t hash = 0x811C9DC5u;
    for (int i = 0; i < input->size; i++)
    {
        hash ^= data[i];
        hash *= 0x01000193u;
    }
    return hash;
}
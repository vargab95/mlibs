#include <stdio.h>

#include "m_hash.h"
#include "m_common.h"

int main(void)
{
    m_com_sized_data_t data = {"asd", 3};

    printf("0x%x\n", m_hash_djb(&data));
    printf("0x%x\n", m_hash_fnv1a(&data));
    return 0;
}